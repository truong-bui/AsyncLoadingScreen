#include "AsyncLoadingScreen.h"
#include "MoviePlayer.h"
#include "ClassicLoadingTheme.h"
#include "LoadingScreenSettings.h"

#define LOCTEXT_NAMESPACE "FAsyncLoadingScreenModule"

void FAsyncLoadingScreenModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	//ILoadingScreenInterface::StartupModule();

	if (!IsRunningDedicatedServer() && FSlateApplication::IsInitialized())
	{
		// Load for cooker reference
		const ULoadingScreenSettings* Settings = GetDefault<ULoadingScreenSettings>();
		for (const FSoftObjectPath& Ref : Settings->StartupScreen.Images)
		{
			Ref.TryLoad();
		}

		for (const FSoftObjectPath& Ref : Settings->DefaultScreen.Images)
		{
			Ref.TryLoad();
		}

		for (const FSoftObjectPath& Ref : Settings->StartupScreen.IconImages)
		{
			Ref.TryLoad();
		}

		for (const FSoftObjectPath& Ref : Settings->DefaultScreen.IconImages)
		{
			Ref.TryLoad();
		}

		if (IsMoviePlayerEnabled())
		{
			GetMoviePlayer()->OnPrepareLoadingScreen().AddRaw(this, &FAsyncLoadingScreenModule::HandlePrepareLoadingScreen);
		}

		// Prepare the startup screen, the PrepareLoadingScreen callback won't be called
		// if we've already explicitly setup the loading screen
		BeginLoadingScreen(Settings->StartupScreen);
	}
	
}

void FAsyncLoadingScreenModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	if (!IsRunningDedicatedServer())
	{
		// TODO: Unregister later
		GetMoviePlayer()->OnPrepareLoadingScreen().RemoveAll(this);
	}


	//ILoadingScreenInterface::ShutdownModule();
}

/*
void FAsyncLoadingScreenModule::AddModuleListeners()
{
	// Add new modules here
}
*/

bool FAsyncLoadingScreenModule::IsGameModule() const
{
	return true;
}

void FAsyncLoadingScreenModule::HandlePrepareLoadingScreen()
{
	const ULoadingScreenSettings* Settings = GetDefault<ULoadingScreenSettings>();
	BeginLoadingScreen(Settings->DefaultScreen);
}

void FAsyncLoadingScreenModule::BeginLoadingScreen(const FLoadingScreenDescription& ScreenDescription)
{
	FLoadingScreenAttributes LoadingScreen;
	LoadingScreen.MinimumLoadingScreenDisplayTime = ScreenDescription.MinimumLoadingScreenDisplayTime;
	LoadingScreen.bAutoCompleteWhenLoadingCompletes = ScreenDescription.bAutoCompleteWhenLoadingCompletes;
	LoadingScreen.bMoviesAreSkippable = ScreenDescription.bMoviesAreSkippable;
	LoadingScreen.bWaitForManualStop = ScreenDescription.bWaitForManualStop;
	LoadingScreen.MoviePaths = ScreenDescription.MoviePaths;
	LoadingScreen.PlaybackType = ScreenDescription.PlaybackType;	

	if (ScreenDescription.bShowUIOverlay)
	{
		LoadingScreen.WidgetLoadingScreen = SNew(SClassicLoadingTheme, ScreenDescription);
	}
	

	GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FAsyncLoadingScreenModule, AsyncLoadingScreen)