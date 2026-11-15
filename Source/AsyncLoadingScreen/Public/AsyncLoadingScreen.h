/************************************************************************************
 *																					*
 * Copyright (C) 2020 Truong Bui.													*
 * Website:	https://github.com/truong-bui/AsyncLoadingScreen						*
 * Licensed under the MIT License. See 'LICENSE' file for full license information. *
 *																					*
 ************************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Engine/EngineBaseTypes.h"

struct FALoadingScreenSettings;
struct FLoadingScreenAttributes;
class UNetDriver;
class UWorld;

DECLARE_LOG_CATEGORY_EXTERN(LogAsyncLoadingScreen, Log, All);

namespace AsyncLoadingScreen
{
	/**
	 * True while the plugin is holding the loading screen waiting for the local player to finish joining the server.
	 * Safe to call from the Slate loading thread.
	 */
	ASYNCLOADINGSCREEN_API bool IsWaitingForNetwork();

	/**
	 * True once whatever the current loading screen is waiting for is done.
	 *
	 * The loading widgets can't ask the movie player directly: a screen hosted in the game viewport (the multiplayer
	 * hold) has no movie player behind it, and IGameMoviePlayer::IsLoadingFinished() would report a stale "finished"
	 * from the previous screen. Safe to call from the Slate loading thread.
	 */
	ASYNCLOADINGSCREEN_API bool IsLoadingFinished();
}

class FAsyncLoadingScreenModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */

	/**
	 * Called right after the module DLL has been loaded and the module object has been created
	 */
	virtual void StartupModule() override;
	
	/**
	 * Called before the module is unloaded, right before the module object is destroyed.
	 */
	virtual void ShutdownModule() override;

	/**
	 * Returns true if this module hosts gameplay code 
	 * 
	 * @return True for "gameplay modules", or false for engine code modules, plugins, etc.
	 */
	virtual bool IsGameModule() const override;

	/**
	 * Singleton-like access to this module's interface. This is just for convenience!
	 * Beware of calling this during the shutdown phase, though.  Your module might have been unloaded already.
	 *
	 * @return Returns singleton instance, loading the module on demand if needed
	 */
	static inline FAsyncLoadingScreenModule& Get()
	{
		return FModuleManager::LoadModuleChecked<FAsyncLoadingScreenModule>("AsyncLoadingScreen");
	}

	/**
	 * Checks to see if this module is loaded and ready. It is only valid to call Get() if IsAvailable() returns true.
	 *
	 * @return True if the module is loaded and ready to use
	 */
	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("AsyncLoadingScreen");
	}

	/**
	 * Is showing Startup Loading Screen?
	 */
	bool IsStartupLoadingScreen() { return bIsStartupLoadingScreen; }

	/**
	 * Displays the Default Loading Screen immediately, outside of a level transition. The screen is kept up until
	 * "StopLoadingScreen" is called. See UAsyncLoadingScreenLibrary::ShowLoadingScreen.
	 */
	void ShowDefaultLoadingScreen();

	/** Takes down a loading screen shown by ShowDefaultLoadingScreen. See UAsyncLoadingScreenLibrary::StopLoadingScreen. */
	void StopDefaultLoadingScreen();

private:
	/**
	 * Loading screen callback, it won't be called if we've already explicitly setup the loading screen
	 */
	void PreSetupLoadingScreen();

	/**
	 * Setup loading screen settings
	 *
	 * @param bManualScreen True for a screen the plugin displays outside of a level transition, which the game has to stop itself
	 */
	void SetupLoadingScreen(const FALoadingScreenSettings& LoadingScreenSettings, bool bManualScreen = false);

	/**
	 * Shuffle the movies list
	 */
	void ShuffleMovies(TArray<FString>& MoviesList);

	/**
	 * Movie player lifecycle callbacks, bound once in StartupModule
	 */
	void HandleMoviePlaybackStarted();
	void HandleMoviePlaybackFinished();

	/**
	 * Seamless travel doesn't load a new map, so the engine never broadcasts PreLoadMap and the movie player is
	 * never started for it; the loading screen has to be displayed manually here
	 */
	void HandleSeamlessTravelStart(UWorld* CurrentWorld, const FString& TravelURL);

	/**
	 * Broadcast at the end of a level load and at the end of a seamless travel; marks the start of the hold phase
	 */
	void HandlePostLoadMap(UWorld* LoadedWorld);

	/**
	 * Travel/network failures would otherwise leave a held loading screen up until it times out
	 */
	void HandleTravelFailure(UWorld* World, ETravelFailure::Type FailureType, const FString& ErrorString);
	void HandleNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString);

	/**
	 * Arms the hold conditions of the loading screen that is about to be displayed, and forces the movie player
	 * attributes those conditions need
	 */
	void ArmLoadingScreenHold(const FALoadingScreenSettings& LoadingScreenSettings, FLoadingScreenAttributes& LoadingScreen, bool bManualScreen);

	/** True once every local player has finished joining the server (see FMultiplayerSettings) */
	bool IsNetworkReady() const;

	/**
	 * Polled from FCoreDelegates::OnSamplingInput while the loading screen is up; stops the loading screen once every
	 * armed hold condition is satisfied (or timed out) and the minimum display time has elapsed
	 */
	void PollLoadingScreenHold();

	/** Stops the loading screen and releases everything the hold has bound */
	void ReleaseLoadingScreenHold();

	/** Throws away loading screen attributes that were prepared for a screen that failed to start */
	void DiscardPreparedLoadingScreen();

	/** Builds the layout widget configured for the given loading screen */
	static TSharedRef<class SWidget> MakeLayoutWidget(const FALoadingScreenSettings& LoadingScreenSettings);

	/**
	 * Mounts / unmounts the loading screen in the game viewport. Used for everything that has to stay on screen while the
	 * engine keeps ticking (the multiplayer holds), which the movie player cannot do.
	 */
	void ShowHoldWidget();
	void HideHoldWidget();

	/** Ticked from FTSTicker while the hold widget is displayed; releases it once the wait is over or times out */
	bool TickHold(float DeltaTime);

	/**
	 * Turns the game viewport's world rendering on or off; used while the engine ticks behind the loading screen
	 *
	 * @return True if a game viewport was there to apply it to
	 */
	bool SetWorldRenderingEnabled(bool bEnabled);
private:

	bool bIsStartupLoadingScreen = false;

	// Hold state, captured per loading screen in SetupLoadingScreen

	/** True while the plugin is responsible for stopping the current loading screen when PSO precaching completes */
	bool bWaitForPSOPrecache = false;
	/** True if the current loading screen wants the PSO precache priority boost */
	bool bBoostPSOPriority = false;
	/** True while the priority boost is applied and needs to be restored */
	bool bPSOBoostActive = false;
	float PSOMaxWaitTime = 0.0f;
	/** The engine's minimum display time is bypassed by StopMovie(), so it is enforced manually in PollLoadingScreenHold */
	float CachedMinimumDisplayTime = -1.0f;
	double MoviePlaybackStartTime = 0.0;
	FDelegateHandle SamplingInputHandle;

	// Multiplayer state

	/**
	 * True between OnMoviePlaybackStarted and OnMoviePlaybackFinished. IGameMoviePlayer::IsMovieCurrentlyPlaying()
	 * cannot be used for this: it reports false for the whole time the movie player sits in its wait loop
	 */
	bool bScreenActive = false;
	/** True while the current loading screen was started by the plugin for a seamless travel */
	bool bSeamlessTravelScreenActive = false;
	/** True once the level load (or the seamless travel) behind the current loading screen is finished */
	bool bLevelLoadFinished = false;
	/** True while the plugin is responsible for stopping the current loading screen when the local player has joined */
	bool bWaitForNetworkReady = false;
	bool bWaitForGameState = false;
	bool bWaitForPossessedPawn = false;
	float NetworkMaxWaitTime = 0.0f;
	float SeamlessTravelMaxWaitTime = 0.0f;
	/** Set when the level load finishes; the hold timeouts are counted from there */
	double HoldPhaseStartTime = 0.0;
	FDelegateHandle SeamlessTravelStartHandle;
	FDelegateHandle PostLoadMapHandle;
	FDelegateHandle TravelFailureHandle;
	FDelegateHandle NetworkFailureHandle;

	/** True if the current loading screen lets the engine keep ticking while it is displayed */
	bool bEngineTickEnabled = false;
	/** True while this module has switched the game viewport's world rendering off for the current loading screen */
	bool bWorldRenderingDisabled = false;
};
