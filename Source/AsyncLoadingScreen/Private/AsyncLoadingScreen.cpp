/************************************************************************************
 *																					*
 * Copyright (C) 2020 Truong Bui.													*
 * Website:	https://github.com/truong-bui/AsyncLoadingScreen						*
 * Licensed under the MIT License. See 'LICENSE' file for full license information. *
 *																					*
 ************************************************************************************/

#include "AsyncLoadingScreen.h"
#include "MoviePlayer.h"
#include "LoadingScreenSettings.h"
#include "SCenterLayout.h"
#include "SClassicLayout.h"
#include "SLetterboxLayout.h"
#include "SSidebarLayout.h"
#include "SDualSidebarLayout.h"
#include "Framework/Application/SlateApplication.h"
#include "AsyncLoadingScreenLibrary.h"
#include "Engine/Texture2D.h"
#include "ShaderPipelineCache.h"
#include "PipelineStateCache.h"
#include "Misc/CoreDelegates.h"
#include "Containers/Ticker.h"
#include "Widgets/SWidget.h"
#include "UObject/UObjectGlobals.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"
#include "Engine/GameViewportClient.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/GameStateBase.h"
#include <atomic>

DEFINE_LOG_CATEGORY(LogAsyncLoadingScreen);

#define LOCTEXT_NAMESPACE "FAsyncLoadingScreenModule"

namespace AsyncLoadingScreen
{
	// Written on the game thread by the module, read on the Slate loading thread by SNetworkStatusWidget
	static std::atomic<bool> GIsWaitingForNetwork(false);

	bool IsWaitingForNetwork()
	{
		return GIsWaitingForNetwork.load(std::memory_order_relaxed);
	}

	static void SetIsWaitingForNetwork(bool bWaiting)
	{
		GIsWaitingForNetwork.store(bWaiting, std::memory_order_relaxed);
	}

	// True while the current loading screen is a widget in the game viewport rather than one the movie player hosts
	static std::atomic<bool> GIsViewportHostedScreen(false);

	bool IsLoadingFinished()
	{
		// A viewport hosted screen is only ever up because the plugin is still waiting for something, so it is by
		// definition not finished. Asking the movie player there would return a stale result from the previous screen.
		if (GIsViewportHostedScreen.load(std::memory_order_relaxed))
		{
			return false;
		}

		return GetMoviePlayer()->IsLoadingFinished();
	}

	static void SetIsViewportHostedScreen(bool bViewportHosted)
	{
		GIsViewportHostedScreen.store(bViewportHosted, std::memory_order_relaxed);
	}
}

void FAsyncLoadingScreenModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	if (!IsRunningDedicatedServer() && FSlateApplication::IsInitialized())
	{
		const ULoadingScreenSettings* Settings = GetDefault<ULoadingScreenSettings>();
				
		if (IsMoviePlayerEnabled())
		{
			GetMoviePlayer()->OnPrepareLoadingScreen().AddRaw(this, &FAsyncLoadingScreenModule::PreSetupLoadingScreen);
			GetMoviePlayer()->OnMoviePlaybackStarted().AddRaw(this, &FAsyncLoadingScreenModule::HandleMoviePlaybackStarted);
			GetMoviePlayer()->OnMoviePlaybackFinished().AddRaw(this, &FAsyncLoadingScreenModule::HandleMoviePlaybackFinished);

			// Seamless travel doesn't load a new map, so the engine never broadcasts PreLoadMap and the movie player is
			// never started for it. PostLoadMapWithWorld is broadcast at the end of a level load and at the end of a
			// seamless travel. Both are static delegates, unlike GEngine's failure events, which don't exist yet here.
			SeamlessTravelStartHandle = FWorldDelegates::OnSeamlessTravelStart.AddRaw(this, &FAsyncLoadingScreenModule::HandleSeamlessTravelStart);
			PostLoadMapHandle = FCoreUObjectDelegates::PostLoadMapWithWorld.AddRaw(this, &FAsyncLoadingScreenModule::HandlePostLoadMap);
		}

		// Prepare the startup screen, the PreSetupLoadingScreen callback won't be called
		// if we've already explicitly setup the loading screen
		bIsStartupLoadingScreen = true;
		SetupLoadingScreen(Settings->StartupLoadingScreen);
	}	
}

void FAsyncLoadingScreenModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	if (!IsRunningDedicatedServer() && IsMoviePlayerEnabled())
	{
		GetMoviePlayer()->OnPrepareLoadingScreen().RemoveAll(this);
		GetMoviePlayer()->OnMoviePlaybackStarted().RemoveAll(this);
		GetMoviePlayer()->OnMoviePlaybackFinished().RemoveAll(this);
	}

	FWorldDelegates::OnSeamlessTravelStart.RemoveAll(this);
	FCoreUObjectDelegates::PostLoadMapWithWorld.RemoveAll(this);
	SeamlessTravelStartHandle.Reset();
	PostLoadMapHandle.Reset();

	if (GEngine)
	{
		GEngine->OnTravelFailure().RemoveAll(this);
		GEngine->OnNetworkFailure().RemoveAll(this);
	}
	TravelFailureHandle.Reset();
	NetworkFailureHandle.Reset();

	if (SamplingInputHandle.IsValid())
	{
		FCoreDelegates::OnSamplingInput.Remove(SamplingInputHandle);
		SamplingInputHandle.Reset();
	}

	if (bPSOBoostActive)
	{
		PipelineStateCache::PrecachePSOsBoostToHighestPriority(false);
		bPSOBoostActive = false;
	}

	if (bWorldRenderingDisabled)
	{
		SetWorldRenderingEnabled(true);
		bWorldRenderingDisabled = false;
	}

	HideHoldWidget();
}

bool FAsyncLoadingScreenModule::IsGameModule() const
{
	return true;
}

void FAsyncLoadingScreenModule::PreSetupLoadingScreen()
{
	const bool bIsEnableLoadingScreen = UAsyncLoadingScreenLibrary::GetIsEnableLoadingScreen();
	if (bIsEnableLoadingScreen)
	{
		const ULoadingScreenSettings* Settings = GetDefault<ULoadingScreenSettings>();
		bIsStartupLoadingScreen = false;
		// This screen is started by the engine for a level load, not by the plugin for a seamless travel
		bSeamlessTravelScreenActive = false;
		SetupLoadingScreen(Settings->DefaultLoadingScreen);
	}
}

void FAsyncLoadingScreenModule::SetupLoadingScreen(const FALoadingScreenSettings& LoadingScreenSettings, bool bManualScreen)
{
	TArray<FString> MoviesList = LoadingScreenSettings.MoviePaths;

	// Shuffle the movies list
	if (LoadingScreenSettings.bShuffle == true)
	{
		ShuffleMovies(MoviesList);
	}
		
	if (LoadingScreenSettings.bSetDisplayMovieIndexManually == true)
	{
		MoviesList.Empty();

		// Show specific movie if valid otherwise show original movies list
		if (LoadingScreenSettings.MoviePaths.IsValidIndex(UAsyncLoadingScreenLibrary::GetDisplayMovieIndex()))
		{
			MoviesList.Add(LoadingScreenSettings.MoviePaths[UAsyncLoadingScreenLibrary::GetDisplayMovieIndex()]);
		}
		else
		{
			MoviesList = LoadingScreenSettings.MoviePaths;
		}
	}

	FLoadingScreenAttributes LoadingScreen;
	LoadingScreen.MinimumLoadingScreenDisplayTime = LoadingScreenSettings.MinimumLoadingScreenDisplayTime;
	LoadingScreen.bAutoCompleteWhenLoadingCompletes = LoadingScreenSettings.bAutoCompleteWhenLoadingCompletes;
	LoadingScreen.bMoviesAreSkippable = LoadingScreenSettings.bMoviesAreSkippable;
	LoadingScreen.bWaitForManualStop = LoadingScreenSettings.bWaitForManualStop;
	LoadingScreen.bAllowInEarlyStartup = LoadingScreenSettings.bAllowInEarlyStartup;
	LoadingScreen.bAllowEngineTick = LoadingScreenSettings.bAllowEngineTick;
	LoadingScreen.MoviePaths = MoviesList;
	LoadingScreen.PlaybackType = LoadingScreenSettings.PlaybackType;

	ArmLoadingScreenHold(LoadingScreenSettings, LoadingScreen, bManualScreen);

	if (LoadingScreenSettings.bShowWidgetOverlay)
	{
		if (LoadingScreenSettings.bAllowInEarlyStartup)
		{
			// Early startup loading screens must not reference UObjects (settings textures/fonts), so the overlay cannot be used with them
			UE_LOG(LogAsyncLoadingScreen, Warning, TEXT("bShowWidgetOverlay is ignored because bAllowInEarlyStartup is enabled; early startup loading screens cannot contain UObjects. Only movies will be displayed."));
		}
		else
		{
			const ULoadingScreenSettings* Settings = GetDefault<ULoadingScreenSettings>();

			switch (LoadingScreenSettings.Layout)
			{
			case EAsyncLoadingScreenLayout::ALSL_Classic:
				LoadingScreen.WidgetLoadingScreen = SNew(SClassicLayout, LoadingScreenSettings, Settings->Classic);
				break;
			case EAsyncLoadingScreenLayout::ALSL_Center:
				LoadingScreen.WidgetLoadingScreen = SNew(SCenterLayout, LoadingScreenSettings, Settings->Center);
				break;
			case EAsyncLoadingScreenLayout::ALSL_Letterbox:
				LoadingScreen.WidgetLoadingScreen = SNew(SLetterboxLayout, LoadingScreenSettings, Settings->Letterbox);
				break;
			case EAsyncLoadingScreenLayout::ALSL_Sidebar:
				LoadingScreen.WidgetLoadingScreen = SNew(SSidebarLayout, LoadingScreenSettings, Settings->Sidebar);
				break;
			case EAsyncLoadingScreenLayout::ALSL_DualSidebar:
				LoadingScreen.WidgetLoadingScreen = SNew(SDualSidebarLayout, LoadingScreenSettings, Settings->DualSidebar);
				break;
			}
		}
	}

	GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
}

void FAsyncLoadingScreenModule::ShuffleMovies(TArray<FString>& MoviesList)
{
	if (MoviesList.Num() > 0)
	{
		int32 LastIndex = MoviesList.Num() - 1;
		for (int32 i = 0; i <= LastIndex; ++i)
		{
			int32 Index = FMath::RandRange(i, LastIndex);
			if (i != Index)
			{
				MoviesList.Swap(i, Index);
			}
		}
	}
}

void FAsyncLoadingScreenModule::ArmLoadingScreenHold(const FALoadingScreenSettings& LoadingScreenSettings, FLoadingScreenAttributes& LoadingScreen, bool bManualScreen)
{
	// Reset the hold state for this loading screen
	bWaitForPSOPrecache = false;
	bBoostPSOPriority = false;
	bWaitForNetworkReady = false;
	bLevelLoadFinished = false;
	PSOMaxWaitTime = LoadingScreenSettings.PSOPrecacheMaxWaitTime;
	CachedMinimumDisplayTime = LoadingScreenSettings.MinimumLoadingScreenDisplayTime;
	AsyncLoadingScreen::SetIsWaitingForNetwork(false);

	const ULoadingScreenSettings* Settings = GetDefault<ULoadingScreenSettings>();
	const FMultiplayerSettings& MultiplayerSettings = Settings->Multiplayer;
	SeamlessTravelMaxWaitTime = MultiplayerSettings.SeamlessTravelMaxWaitTime;
	NetworkMaxWaitTime = MultiplayerSettings.NetworkMaxWaitTime;

	if (bManualScreen)
	{
		// A manually displayed screen isn't tied to a level transition, so none of the automatic hold conditions apply.
		// It is shown as a viewport widget and stays up until the game calls StopLoadingScreen.
		return;
	}

	if (LoadingScreenSettings.bWaitForPSOPrecachingToComplete)
	{
		if (LoadingScreenSettings.bAllowInEarlyStartup)
		{
			UE_LOG(LogAsyncLoadingScreen, Warning, TEXT("bWaitForPSOPrecachingToComplete is ignored because bAllowInEarlyStartup is enabled; the PSO caches are not initialized during early startup."));
		}
		else
		{
			bBoostPSOPriority = LoadingScreenSettings.bBoostPSOPrecachePriority;
			bWaitForPSOPrecache = true;
		}
	}

	// Multiplayer holds only make sense for in-game loading screens
	if (MultiplayerSettings.bHoldUntilNetworkReady && !bIsStartupLoadingScreen)
	{
		if (LoadingScreenSettings.bAllowInEarlyStartup)
		{
			UE_LOG(LogAsyncLoadingScreen, Warning, TEXT("bHoldUntilNetworkReady is ignored because bAllowInEarlyStartup is enabled; there is no game world during early startup."));
		}
		else
		{
			bWaitForNetworkReady = true;
			bWaitForGameState = MultiplayerSettings.bWaitForGameState;
			bWaitForPossessedPawn = MultiplayerSettings.bWaitForPossessedPawn;
		}
	}

	if (bWaitForPSOPrecache)
	{
		// Keep the movie player's wait loop alive past level-load completion; PollLoadingScreenHold calls StopMovie() once
		// PSO precaching is done, so any user-configured bWaitForManualStop is taken over by the plugin.
		// Only the PSO wait may do this: it needs no engine tick, because PSO compilation runs on task threads.
		LoadingScreen.bWaitForManualStop = true;
	}

	// The multiplayer holds deliberately do NOT touch bAllowEngineTick. Ticking the engine from inside the movie player's
	// wait loop renders the scene twice on one render frame (GEngine->Tick -> UGameEngine::Tick -> PostRenderAllViewports),
	// which asserts in FRayTracingGeometryManager::Tick in every DO_CHECK build. The network hold and the seamless travel
	// screen are shown as a widget in the game viewport instead, where the engine ticks normally.
}

bool FAsyncLoadingScreenModule::SetWorldRenderingEnabled(bool bEnabled)
{
	if (GEngine == nullptr || GEngine->GameViewport == nullptr)
	{
		return false;
	}

	GEngine->GameViewport->bDisableWorldRendering = bEnabled ? 0 : 1;
	return true;
}

namespace
{
	/** The loading screen widget currently mounted in the game viewport, if any */
	TSharedPtr<SWidget> GHoldWidget;

	/** Drives the hold. FTSTicker ticks from the engine loop and survives world transitions, unlike anything world bound */
	FTSTicker::FDelegateHandle GHoldTickerHandle;

	/** True while the hold is responsible for restoring the viewport's input setting */
	bool GHoldSuppressedInput = false;

	/** True while the screen was shown by the game rather than by a level transition, so only the game takes it down */
	bool GManualHold = false;

	UGameViewportClient* GetGameViewportClient()
	{
		return GEngine ? GEngine->GameViewport : nullptr;
	}
}

TSharedRef<SWidget> FAsyncLoadingScreenModule::MakeLayoutWidget(const FALoadingScreenSettings& LoadingScreenSettings)
{
	const ULoadingScreenSettings* Settings = GetDefault<ULoadingScreenSettings>();

	switch (LoadingScreenSettings.Layout)
	{
	case EAsyncLoadingScreenLayout::ALSL_Center:
		return SNew(SCenterLayout, LoadingScreenSettings, Settings->Center);
	case EAsyncLoadingScreenLayout::ALSL_Letterbox:
		return SNew(SLetterboxLayout, LoadingScreenSettings, Settings->Letterbox);
	case EAsyncLoadingScreenLayout::ALSL_Sidebar:
		return SNew(SSidebarLayout, LoadingScreenSettings, Settings->Sidebar);
	case EAsyncLoadingScreenLayout::ALSL_DualSidebar:
		return SNew(SDualSidebarLayout, LoadingScreenSettings, Settings->DualSidebar);
	case EAsyncLoadingScreenLayout::ALSL_Classic:
	default:
		return SNew(SClassicLayout, LoadingScreenSettings, Settings->Classic);
	}
}

void FAsyncLoadingScreenModule::ShowHoldWidget()
{
	if (GHoldWidget.IsValid())
	{
		return;
	}

	UGameViewportClient* GameViewport = GetGameViewportClient();
	if (GameViewport == nullptr)
	{
		return;
	}

	const ULoadingScreenSettings* Settings = GetDefault<ULoadingScreenSettings>();
	const FALoadingScreenSettings& ScreenSettings = Settings->DefaultLoadingScreen;
	if (!ScreenSettings.bShowWidgetOverlay)
	{
		// Without the widget overlay there is nothing to display, and holding would just freeze the game behind the game's
		// own view. Movies can't be used here: they only play through the movie player, which blocks the game thread.
		UE_LOG(LogAsyncLoadingScreen, Warning, TEXT("The loading screen cannot be held because \"Show Widget Overlay\" is disabled on the Default Loading Screen."));
		return;
	}

	// A high Z order so the loading screen covers the game's own UI
	GHoldWidget = MakeLayoutWidget(ScreenSettings);
	GameViewport->AddViewportWidgetContent(GHoldWidget.ToSharedRef(), 10000);

	// The world is live underneath, so stop it reacting to the player while the screen is up
	GHoldSuppressedInput = !GameViewport->IgnoreInput();
	if (GHoldSuppressedInput)
	{
		GameViewport->SetIgnoreInput(true);
	}

	// The loading widgets ask the module, not the movie player, whether the wait is over
	AsyncLoadingScreen::SetIsViewportHostedScreen(true);

	if (!GHoldTickerHandle.IsValid())
	{
		GHoldTickerHandle = FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateRaw(this, &FAsyncLoadingScreenModule::TickHold));
	}
}

void FAsyncLoadingScreenModule::HideHoldWidget()
{
	if (GHoldTickerHandle.IsValid())
	{
		FTSTicker::GetCoreTicker().RemoveTicker(GHoldTickerHandle);
		GHoldTickerHandle.Reset();
	}

	UGameViewportClient* GameViewport = GetGameViewportClient();

	if (GHoldWidget.IsValid())
	{
		if (GameViewport)
		{
			GameViewport->RemoveViewportWidgetContent(GHoldWidget.ToSharedRef());
		}
		GHoldWidget.Reset();
	}

	if (GHoldSuppressedInput)
	{
		if (GameViewport)
		{
			GameViewport->SetIgnoreInput(false);
		}
		GHoldSuppressedInput = false;
	}

	bSeamlessTravelScreenActive = false;
	bWaitForNetworkReady = false;
	GManualHold = false;
	AsyncLoadingScreen::SetIsViewportHostedScreen(false);
	AsyncLoadingScreen::SetIsWaitingForNetwork(false);
}

bool FAsyncLoadingScreenModule::TickHold(float DeltaTime)
{
	if (!GHoldWidget.IsValid())
	{
		GHoldTickerHandle.Reset();
		return false;
	}

	// A screen the game asked for stays up until the game takes it down again
	if (GManualHold)
	{
		return true;
	}

	const double CurrentTime = FPlatformTime::Seconds();

	// Releasing from inside the ticker: drop the handle first so HideHoldWidget doesn't remove the ticker we are running in
	auto Release = [this]()
	{
		GHoldTickerHandle.Reset();
		HideHoldWidget();
	};

	if (bSeamlessTravelScreenActive && !bLevelLoadFinished)
	{
		// The travel itself is still running. A cancelled seamless travel never reports completion, so it needs its own way out.
		if (SeamlessTravelMaxWaitTime > 0.0f && (CurrentTime - HoldPhaseStartTime) >= SeamlessTravelMaxWaitTime)
		{
			UE_LOG(LogAsyncLoadingScreen, Warning, TEXT("Seamless travel did not complete within %.1f seconds; releasing the loading screen."), SeamlessTravelMaxWaitTime);
			Release();
			return false;
		}

		AsyncLoadingScreen::SetIsWaitingForNetwork(false);
		return true;
	}

	if (IsNetworkReady())
	{
		Release();
		return false;
	}

	if (NetworkMaxWaitTime > 0.0f && (CurrentTime - HoldPhaseStartTime) >= NetworkMaxWaitTime)
	{
		UE_LOG(LogAsyncLoadingScreen, Warning, TEXT("Loading screen timed out after %.1f seconds waiting for the local player to finish joining the server."), NetworkMaxWaitTime);
		Release();
		return false;
	}

	AsyncLoadingScreen::SetIsWaitingForNetwork(true);
	return true;
}

void FAsyncLoadingScreenModule::ShowDefaultLoadingScreen()
{
	if (!IsMoviePlayerEnabled())
	{
		return;
	}

	// Don't use IsMovieCurrentlyPlaying() here; it reports false while the movie player sits in its wait loop
	if (bScreenActive)
	{
		UE_LOG(LogAsyncLoadingScreen, Log, TEXT("ShowLoadingScreen was called while a loading screen is already displayed."));
		return;
	}

	// Shown as a viewport widget, not through the movie player: the movie player would block the game thread, so the game
	// could never reach its own StopLoadingScreen call, and the level isn't changing here anyway
	bIsStartupLoadingScreen = false;
	bSeamlessTravelScreenActive = false;
	bWaitForNetworkReady = false;
	GManualHold = true;
	HoldPhaseStartTime = FPlatformTime::Seconds();

	ShowHoldWidget();
}

void FAsyncLoadingScreenModule::StopDefaultLoadingScreen()
{
	GManualHold = false;
	HideHoldWidget();
}

void FAsyncLoadingScreenModule::DiscardPreparedLoadingScreen()
{
	// The attributes we just prepared stay on the movie player until a screen actually plays, and a prepared screen stops
	// the movie player from asking for a new one on the next level load. Clear them so the next screen is set up from scratch.
	GetMoviePlayer()->SetupLoadingScreen(FLoadingScreenAttributes());
	bWaitForPSOPrecache = false;
	bBoostPSOPriority = false;
	bWaitForNetworkReady = false;
	bSeamlessTravelScreenActive = false;
}

void FAsyncLoadingScreenModule::HandleSeamlessTravelStart(UWorld* CurrentWorld, const FString& TravelURL)
{
	const ULoadingScreenSettings* Settings = GetDefault<ULoadingScreenSettings>();
	if (!Settings->Multiplayer.bShowLoadingScreenOnSeamlessTravel || !UAsyncLoadingScreenLibrary::GetIsEnableLoadingScreen())
	{
		return;
	}

	// Don't use IsMovieCurrentlyPlaying() here; it reports false while the movie player sits in its wait loop
	if (bScreenActive)
	{
		UE_LOG(LogAsyncLoadingScreen, Log, TEXT("Seamless travel to '%s' started while a loading screen is already displayed; keeping the current one."), *TravelURL);
		return;
	}

	// The movie player cannot be used here. Seamless travel is driven by the engine tick, but a movie player screen blocks
	// the game thread inside WaitForMovieToFinish, so the travel would never progress; and letting the wait loop tick the
	// engine (bAllowEngineTick) renders the scene twice on one render frame and asserts in FRayTracingGeometryManager::Tick.
	// A widget in the game viewport keeps the engine ticking normally.
	const FMultiplayerSettings& MultiplayerSettings = Settings->Multiplayer;

	bIsStartupLoadingScreen = false;
	bSeamlessTravelScreenActive = true;
	bLevelLoadFinished = false;
	bWaitForNetworkReady = MultiplayerSettings.bHoldUntilNetworkReady;
	bWaitForGameState = MultiplayerSettings.bWaitForGameState;
	bWaitForPossessedPawn = MultiplayerSettings.bWaitForPossessedPawn;
	SeamlessTravelMaxWaitTime = MultiplayerSettings.SeamlessTravelMaxWaitTime;
	NetworkMaxWaitTime = MultiplayerSettings.NetworkMaxWaitTime;
	HoldPhaseStartTime = FPlatformTime::Seconds();

	UE_LOG(LogAsyncLoadingScreen, Log, TEXT("Showing the loading screen for the seamless travel to '%s'."), *TravelURL);
	ShowHoldWidget();
}

void FAsyncLoadingScreenModule::HandlePostLoadMap(UWorld* LoadedWorld)
{
	// Broadcast at the end of a level load and at the end of a seamless travel. The hold timeouts are counted from here,
	// so that they measure the wait itself and not the loading time before it.
	bLevelLoadFinished = true;
	HoldPhaseStartTime = FPlatformTime::Seconds();

	// Mount the hold before the movie player tears its own screen down. This module's PostLoadMapWithWorld handler is
	// registered at startup, while the movie player only binds its own from inside OnPreLoadMap, so this runs first and
	// no frame is presented in between.
	if (bWaitForNetworkReady && !GHoldWidget.IsValid() && !IsNetworkReady())
	{
		ShowHoldWidget();
	}
}

void FAsyncLoadingScreenModule::HandleTravelFailure(UWorld* World, ETravelFailure::Type FailureType, const FString& ErrorString)
{
	if (bScreenActive)
	{
		UE_LOG(LogAsyncLoadingScreen, Warning, TEXT("Stopping the loading screen because travel failed (%s): %s"), ETravelFailure::ToString(FailureType), *ErrorString);
		ReleaseLoadingScreenHold();
	}
}

void FAsyncLoadingScreenModule::HandleNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString)
{
	if (bScreenActive)
	{
		UE_LOG(LogAsyncLoadingScreen, Warning, TEXT("Stopping the loading screen because of a network failure (%s): %s"), ENetworkFailure::ToString(FailureType), *ErrorString);
		ReleaseLoadingScreenHold();
	}
}

bool FAsyncLoadingScreenModule::IsNetworkReady() const
{
	// The game can hold the loading screen for its own reasons on top of the built-in conditions
	if (UAsyncLoadingScreenLibrary::GetIsLoadingScreenHold())
	{
		return false;
	}

	if (GEngine == nullptr)
	{
		return true;
	}

	// Never cache any of these pointers between polls: garbage collection runs while the engine keeps ticking
	for (const FWorldContext& WorldContext : GEngine->GetWorldContexts())
	{
		if (WorldContext.WorldType != EWorldType::Game || WorldContext.OwningGameInstance == nullptr)
		{
			continue;
		}

		UWorld* World = WorldContext.World();
		if (World == nullptr)
		{
			continue;
		}

		if (bWaitForGameState && World->GetGameState() == nullptr)
		{
			return false;
		}

		// On a client, UEngine::LoadMap spawns a local placeholder PlayerController with ROLE_Authority, which is only replaced
		// by the replicated one from the server in UNetConnection::HandleClientPlayer, so its mere existence proves nothing
		const bool bIsNetClient = World->GetNetMode() == NM_Client;

		for (auto It = WorldContext.OwningGameInstance->GetLocalPlayerIterator(); It; ++It)
		{
			const ULocalPlayer* LocalPlayer = *It;
			if (LocalPlayer == nullptr)
			{
				continue;
			}

			const APlayerController* PlayerController = LocalPlayer->PlayerController;
			if (PlayerController == nullptr)
			{
				return false;
			}

			if (bIsNetClient && PlayerController->GetLocalRole() != ROLE_AutonomousProxy)
			{
				return false;
			}

			if (bWaitForPossessedPawn && PlayerController->GetPawn() == nullptr)
			{
				return false;
			}
		}

		return true;
	}

	// No game world at all (the level failed to load, for instance): there is nothing left to wait for
	return true;
}

void FAsyncLoadingScreenModule::HandleMoviePlaybackStarted()
{
	bScreenActive = true;
	MoviePlaybackStartTime = FPlatformTime::Seconds();
	HoldPhaseStartTime = MoviePlaybackStartTime;

	if (bEngineTickEnabled && !bWorldRenderingDisabled)
	{
		// With bAllowEngineTick the engine ticks from inside the movie player's wait loop, which renders the scene again
		// on a render thread frame counter the engine loop has already used. That trips the engine's once-per-frame
		// checks (FRayTracingGeometryManager::Tick asserts on it). Nothing of the world is visible behind the loading
		// screen anyway, so the world render is switched off for as long as the screen is up.
		bWorldRenderingDisabled = SetWorldRenderingEnabled(false);
	}

	if (bBoostPSOPriority && PipelineStateCache::IsPSOPrecachingEnabled())
	{
		PipelineStateCache::PrecachePSOsBoostToHighestPriority(true);
		bPSOBoostActive = true;
	}

	if ((bWaitForPSOPrecache || bWaitForNetworkReady || bSeamlessTravelScreenActive) && !SamplingInputHandle.IsValid())
	{
		// OnMoviePlaybackTick is not broadcast from the movie player's wait loop, but OnSamplingInput is, every iteration on the game thread
		SamplingInputHandle = FCoreDelegates::OnSamplingInput.AddRaw(this, &FAsyncLoadingScreenModule::PollLoadingScreenHold);
	}

	// GEngine doesn't exist yet when this module starts up, so these are bound here instead. Without them a failed travel
	// or a lost connection would leave a held loading screen up until it times out.
	if (GEngine && (bWaitForNetworkReady || bSeamlessTravelScreenActive) && !TravelFailureHandle.IsValid())
	{
		TravelFailureHandle = GEngine->OnTravelFailure().AddRaw(this, &FAsyncLoadingScreenModule::HandleTravelFailure);
		NetworkFailureHandle = GEngine->OnNetworkFailure().AddRaw(this, &FAsyncLoadingScreenModule::HandleNetworkFailure);
	}
}

void FAsyncLoadingScreenModule::PollLoadingScreenHold()
{
	// Note: don't check IsMovieCurrentlyPlaying() here; it returns false once the movie player enters its wait loop
	// (the sync mechanism is destroyed on entry), which is exactly when this poll runs. The OnSamplingInput binding
	// is already scoped to the loading screen's lifetime by HandleMoviePlaybackStarted/Finished.
	if (!bWaitForPSOPrecache && !bWaitForNetworkReady && !bSeamlessTravelScreenActive)
	{
		return;
	}

	const double CurrentTime = FPlatformTime::Seconds();

	if (!bLevelLoadFinished && !bSeamlessTravelScreenActive)
	{
		// Without a seamless travel this poll runs in the movie player's wait loop, which the movie player enters from its
		// own PostLoadMapWithWorld handler at the end of the level load. That handler runs before this module's: delegates
		// are called in reverse order of binding, and the movie player binds its own in OnPreLoadMap, after this module
		// bound its at startup. So the level has loaded, and HandlePostLoadMap won't run until the movie stops: do here
		// what it does, or a hold that waits for it never ends.
		HandlePostLoadMap(nullptr);
	}

	if (!bLevelLoadFinished)
	{
		// Still travelling. A cancelled seamless travel never reports completion, so a screen displayed for one needs its
		// own way out.
		if (bSeamlessTravelScreenActive && SeamlessTravelMaxWaitTime > 0.0f && (CurrentTime - MoviePlaybackStartTime) >= SeamlessTravelMaxWaitTime)
		{
			UE_LOG(LogAsyncLoadingScreen, Warning, TEXT("Seamless travel did not complete within %.1f seconds; stopping the loading screen."), SeamlessTravelMaxWaitTime);
			ReleaseLoadingScreenHold();
		}
		return;
	}

	// StopMovie() bypasses the movie player's own MinimumLoadingScreenDisplayTime handling, so enforce it here
	const bool bMinTimeSatisfied = CachedMinimumDisplayTime < 0.0f || (CurrentTime - MoviePlaybackStartTime) >= CachedMinimumDisplayTime;
	if (!bMinTimeSatisfied)
	{
		return;
	}

	bool bReadyToStop = true;
	bool bWaitingForNetwork = false;

	if (bWaitForPSOPrecache)
	{
		const uint32 NumPrecompilesRemaining = FShaderPipelineCache::NumPrecompilesRemaining();
		if (NumPrecompilesRemaining > 0)
		{
			if (PSOMaxWaitTime > 0.0f && (CurrentTime - HoldPhaseStartTime) >= PSOMaxWaitTime)
			{
				UE_LOG(LogAsyncLoadingScreen, Warning, TEXT("Loading screen timed out after %.1f seconds with %u PSO precompiles remaining."), PSOMaxWaitTime, NumPrecompilesRemaining);
			}
			else
			{
				bReadyToStop = false;
			}
		}
	}

	if (bWaitForNetworkReady && !IsNetworkReady())
	{
		if (NetworkMaxWaitTime > 0.0f && (CurrentTime - HoldPhaseStartTime) >= NetworkMaxWaitTime)
		{
			UE_LOG(LogAsyncLoadingScreen, Warning, TEXT("Loading screen timed out after %.1f seconds waiting for the local player to finish joining the server."), NetworkMaxWaitTime);
		}
		else
		{
			bReadyToStop = false;
			bWaitingForNetwork = true;
		}
	}

	AsyncLoadingScreen::SetIsWaitingForNetwork(bWaitingForNetwork);

	if (bReadyToStop)
	{
		ReleaseLoadingScreenHold();
	}
}

void FAsyncLoadingScreenModule::ReleaseLoadingScreenHold()
{
	// Travel and network failures come through here, so the viewport widget has to go with everything else; a leaked one
	// would leave the player looking at a loading screen with no way out
	HideHoldWidget();

	bWaitForPSOPrecache = false;
	bWaitForNetworkReady = false;
	AsyncLoadingScreen::SetIsWaitingForNetwork(false);

	if (SamplingInputHandle.IsValid())
	{
		FCoreDelegates::OnSamplingInput.Remove(SamplingInputHandle);
		SamplingInputHandle.Reset();
	}

	GetMoviePlayer()->StopMovie();
}

void FAsyncLoadingScreenModule::HandleMoviePlaybackFinished()
{
	// The loading screen can also be closed by a key press or a manual StopLoadingScreen call, so always clean up here
	if (SamplingInputHandle.IsValid())
	{
		FCoreDelegates::OnSamplingInput.Remove(SamplingInputHandle);
		SamplingInputHandle.Reset();
	}

	if (GEngine)
	{
		GEngine->OnTravelFailure().RemoveAll(this);
		GEngine->OnNetworkFailure().RemoveAll(this);
	}
	TravelFailureHandle.Reset();
	NetworkFailureHandle.Reset();

	if (bPSOBoostActive)
	{
		PipelineStateCache::PrecachePSOsBoostToHighestPriority(false);
		bPSOBoostActive = false;
	}

	if (bWorldRenderingDisabled)
	{
		SetWorldRenderingEnabled(true);
		bWorldRenderingDisabled = false;
	}

	bScreenActive = false;
	bEngineTickEnabled = false;
	bWaitForPSOPrecache = false;
	bBoostPSOPriority = false;
	bWaitForNetworkReady = false;
	bSeamlessTravelScreenActive = false;
	bLevelLoadFinished = false;
	HoldPhaseStartTime = 0.0;
	AsyncLoadingScreen::SetIsWaitingForNetwork(false);

	// A hold the game asked for is scoped to the loading screen it was requested for
	UAsyncLoadingScreenLibrary::SetLoadingScreenHold(false);
}


#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FAsyncLoadingScreenModule, AsyncLoadingScreen)
