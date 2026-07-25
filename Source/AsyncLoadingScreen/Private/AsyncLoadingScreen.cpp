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

DEFINE_LOG_CATEGORY(LogAsyncLoadingScreen);

#define LOCTEXT_NAMESPACE "FAsyncLoadingScreenModule"

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
		SetupLoadingScreen(Settings->DefaultLoadingScreen);
	}	
}

void FAsyncLoadingScreenModule::SetupLoadingScreen(const FALoadingScreenSettings& LoadingScreenSettings)
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

	// Reset the PSO precache wait state for this loading screen
	bWaitForPSOPrecache = false;
	bBoostPSOPriority = false;
	PSOMaxWaitTime = LoadingScreenSettings.PSOPrecacheMaxWaitTime;
	CachedMinimumDisplayTime = LoadingScreenSettings.MinimumLoadingScreenDisplayTime;

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
			// Keep the movie player's wait loop alive past level-load completion; PollPSOPrecaching calls StopMovie() once
			// PSO precaching is done, so any user-configured bWaitForManualStop is taken over by the plugin
			LoadingScreen.bWaitForManualStop = true;
		}
	}

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

void FAsyncLoadingScreenModule::HandleMoviePlaybackStarted()
{
	MoviePlaybackStartTime = FPlatformTime::Seconds();
	PSOWaitPhaseStartTime = 0.0;

	if (bBoostPSOPriority && PipelineStateCache::IsPSOPrecachingEnabled())
	{
		PipelineStateCache::PrecachePSOsBoostToHighestPriority(true);
		bPSOBoostActive = true;
	}

	if (bWaitForPSOPrecache && !SamplingInputHandle.IsValid())
	{
		// OnMoviePlaybackTick is not broadcast from the movie player's post-load wait loop, but OnSamplingInput is, every iteration on the game thread
		SamplingInputHandle = FCoreDelegates::OnSamplingInput.AddRaw(this, &FAsyncLoadingScreenModule::PollPSOPrecaching);
	}
}

void FAsyncLoadingScreenModule::PollPSOPrecaching()
{
	// Note: don't check IsMovieCurrentlyPlaying() here; it returns false once the movie player enters its wait loop
	// (the sync mechanism is destroyed on entry), which is exactly when this poll runs. The OnSamplingInput binding
	// is already scoped to the loading screen's lifetime by HandleMoviePlaybackStarted/Finished.
	if (!bWaitForPSOPrecache)
	{
		return;
	}

	const double CurrentTime = FPlatformTime::Seconds();

	// While a loading screen is up, OnSamplingInput only fires from the wait loop entered after level loading completes,
	// so the first callback marks the start of the PSO wait phase for the timeout
	if (PSOWaitPhaseStartTime == 0.0)
	{
		PSOWaitPhaseStartTime = CurrentTime;
	}

	// StopMovie() bypasses the movie player's own MinimumLoadingScreenDisplayTime handling, so enforce it here
	const bool bMinTimeSatisfied = CachedMinimumDisplayTime < 0.0f || (CurrentTime - MoviePlaybackStartTime) >= CachedMinimumDisplayTime;
	const uint32 NumPrecompilesRemaining = FShaderPipelineCache::NumPrecompilesRemaining();
	const bool bTimedOut = PSOMaxWaitTime > 0.0f && (CurrentTime - PSOWaitPhaseStartTime) >= PSOMaxWaitTime;

	if (bMinTimeSatisfied && (NumPrecompilesRemaining == 0 || bTimedOut))
	{
		if (NumPrecompilesRemaining > 0)
		{
			UE_LOG(LogAsyncLoadingScreen, Warning, TEXT("Loading screen timed out after %.1f seconds with %u PSO precompiles remaining."), PSOMaxWaitTime, NumPrecompilesRemaining);
		}

		bWaitForPSOPrecache = false;
		FCoreDelegates::OnSamplingInput.Remove(SamplingInputHandle);
		SamplingInputHandle.Reset();

		GetMoviePlayer()->StopMovie();
	}
}

void FAsyncLoadingScreenModule::HandleMoviePlaybackFinished()
{
	// The loading screen can also be closed by a key press or a manual StopLoadingScreen call, so always clean up here
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

	bWaitForPSOPrecache = false;
	bBoostPSOPriority = false;
	PSOWaitPhaseStartTime = 0.0;
}


#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FAsyncLoadingScreenModule, AsyncLoadingScreen)
