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
		}		
		
		// If PreloadBackgroundImages option is check, load all background images into memory
		if (Settings->bPreloadBackgroundImages)
		{
			LoadBackgroundImages();
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
	if (!IsRunningDedicatedServer())
	{
		// TODO: Unregister later
		GetMoviePlayer()->OnPrepareLoadingScreen().RemoveAll(this);
	}
}

bool FAsyncLoadingScreenModule::IsGameModule() const
{
	return true;
}

TArray<UTexture2D*> FAsyncLoadingScreenModule::GetBackgroundImages()
{
	return bIsStartupLoadingScreen ? StartupBackgroundImages : DefaultBackgroundImages;
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

	if (LoadingScreenSettings.bShowWidgetOverlay)
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

void FAsyncLoadingScreenModule::LoadBackgroundImages()
{
	// Empty all background images array
	RemoveAllBackgroundImages();

	const ULoadingScreenSettings* Settings = GetDefault<ULoadingScreenSettings>();
	
	// Preload startup background images
	for (auto& Image : Settings->StartupLoadingScreen.Background.Images)
	{
		UTexture2D* LoadedImage = Cast<UTexture2D>(Image.TryLoad());
		if (LoadedImage)
		{
			StartupBackgroundImages.Add(LoadedImage);
		}
	}

	// Preload default background images
	for (auto& Image : Settings->DefaultLoadingScreen.Background.Images)
	{
		UTexture2D* LoadedImage = Cast<UTexture2D> (Image.TryLoad());
		if (LoadedImage)
		{
			DefaultBackgroundImages.Add(LoadedImage);
		}		
	}
}

void FAsyncLoadingScreenModule::RemoveAllBackgroundImages()
{
	StartupBackgroundImages.Empty();
	DefaultBackgroundImages.Empty();
}

bool FAsyncLoadingScreenModule::IsPreloadBackgroundImagesEnabled()
{	
	return GetDefault<ULoadingScreenSettings>()->bPreloadBackgroundImages;
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FAsyncLoadingScreenModule, AsyncLoadingScreen)
