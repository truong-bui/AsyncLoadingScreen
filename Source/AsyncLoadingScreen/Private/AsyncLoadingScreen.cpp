#include "AsyncLoadingScreen.h"
#include "MoviePlayer.h"
#include "ClassicLoadingTheme.h"

#define LOCTEXT_NAMESPACE "FAsyncLoadingScreenModule"

void FAsyncLoadingScreenModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	//ILoadingScreenInterface::StartupModule();

	if (!IsRunningDedicatedServer() && FSlateApplication::IsInitialized())
	{
		if (IsMoviePlayerEnabled())
		{
			GetMoviePlayer()->OnPrepareLoadingScreen().AddRaw(this, &FAsyncLoadingScreenModule::HandlePrepareLoadingScreen);
		}

		// Prepare the startup screen, the PrepareLoadingScreen callback won't be called
		// if we've already explicitly setup the loading screen
		BeginLoadingScreen();
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
	BeginLoadingScreen();
}

void FAsyncLoadingScreenModule::BeginLoadingScreen()
{
	FLoadingScreenAttributes LoadingScreen;
	LoadingScreen.MinimumLoadingScreenDisplayTime = 10.0f;
	LoadingScreen.bAutoCompleteWhenLoadingCompletes = false;
	LoadingScreen.bMoviesAreSkippable = false;
	LoadingScreen.bWaitForManualStop = false;	
	LoadingScreen.PlaybackType = EMoviePlaybackType::MT_Normal;

	LoadingScreen.WidgetLoadingScreen = SNew(SClassicLoadingTheme);

	GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FAsyncLoadingScreenModule, AsyncLoadingScreen)