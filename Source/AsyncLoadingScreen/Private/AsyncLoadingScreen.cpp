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
#include "SCentralityTheme.h"
#include "SClassicTheme.h"
#include "SLetterboxTheme.h"

#define LOCTEXT_NAMESPACE "FAsyncLoadingScreenModule"

void FAsyncLoadingScreenModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	//ILoadingScreenInterface::StartupModule();

	if (!IsRunningDedicatedServer() && FSlateApplication::IsInitialized())
	{
		const ULoadingScreenSettings* Settings = GetDefault<ULoadingScreenSettings>();
		/**
		for (const FSoftObjectPath& Ref : Settings->StartupLoadingScreen.Background.Images)
		{
			Ref.TryLoad();
		}

		for (const FSoftObjectPath& Ref : Settings->DefaultLoadingScreen.Background.Images)
		{
			Ref.TryLoad();
		}

		for (const FSoftObjectPath& Ref : Settings->StartupLoadingScreen.LoadingWidget.Images)
		{
			Ref.TryLoad();
		}

		for (const FSoftObjectPath& Ref : Settings->DefaultLoadingScreen.LoadingWidget.Images)
		{
			Ref.TryLoad();
		}
		*/
		

		if (IsMoviePlayerEnabled())
		{
			GetMoviePlayer()->OnPrepareLoadingScreen().AddRaw(this, &FAsyncLoadingScreenModule::PreSetupLoadingScreen);
		}

		// Prepare the startup screen, the PreSetupLoadingScreen callback won't be called
		// if we've already explicitly setup the loading screen
		SetupLoadingScreen(Settings->StartupLoadingScreen);
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

void FAsyncLoadingScreenModule::PreSetupLoadingScreen()
{
	const ULoadingScreenSettings* Settings = GetDefault<ULoadingScreenSettings>();
	SetupLoadingScreen(Settings->DefaultLoadingScreen);
}

void FAsyncLoadingScreenModule::SetupLoadingScreen(const FALoadingScreenSettings& LoadingScreenSettings)
{
	FLoadingScreenAttributes LoadingScreen;
	LoadingScreen.MinimumLoadingScreenDisplayTime = LoadingScreenSettings.MinimumLoadingScreenDisplayTime;
	LoadingScreen.bAutoCompleteWhenLoadingCompletes = LoadingScreenSettings.bAutoCompleteWhenLoadingCompletes;
	LoadingScreen.bMoviesAreSkippable = LoadingScreenSettings.bMoviesAreSkippable;
	LoadingScreen.bWaitForManualStop = LoadingScreenSettings.bWaitForManualStop;
	LoadingScreen.MoviePaths = LoadingScreenSettings.MoviePaths;
	LoadingScreen.PlaybackType = LoadingScreenSettings.PlaybackType;

	if (LoadingScreenSettings.bShowWidgetOverlay)
	{
		const ULoadingScreenSettings* Settings = GetDefault<ULoadingScreenSettings>();

		switch (LoadingScreenSettings.Theme)
		{
		case EAsyncLoadingScreenTheme::ALST_Classic:
			LoadingScreen.WidgetLoadingScreen = SNew(SClassicTheme, LoadingScreenSettings, Settings->Classic);
			break;
		case EAsyncLoadingScreenTheme::ALST_Centrality:
			LoadingScreen.WidgetLoadingScreen = SNew(SCentralityTheme, LoadingScreenSettings, Settings->Centrality);
			break;
		case EAsyncLoadingScreenTheme::ALST_Letterbox:
			LoadingScreen.WidgetLoadingScreen = SNew(SLetterboxTheme, LoadingScreenSettings, Settings->Letterbox);
			break;
		}
		
	}
	
	GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FAsyncLoadingScreenModule, AsyncLoadingScreen)