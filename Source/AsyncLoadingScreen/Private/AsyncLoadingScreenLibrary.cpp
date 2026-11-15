/************************************************************************************
 *																					*
 * Copyright (C) 2020 Truong Bui.													*
 * Website:	https://github.com/truong-bui/AsyncLoadingScreen						*
 * Licensed under the MIT License. See 'LICENSE' file for full license information. *
 *																					*
 ************************************************************************************/


#include "AsyncLoadingScreenLibrary.h"
#include "AsyncLoadingScreen.h"
#include "MoviePlayer.h"

int32 UAsyncLoadingScreenLibrary::DisplayBackgroundIndex = -1;
int32 UAsyncLoadingScreenLibrary::DisplayTipTextIndex = -1;
int32 UAsyncLoadingScreenLibrary::DisplayMovieIndex = -1;
bool  UAsyncLoadingScreenLibrary::bShowLoadingScreen = true;
bool  UAsyncLoadingScreenLibrary::bHoldLoadingScreen = false;

void UAsyncLoadingScreenLibrary::SetDisplayBackgroundIndex(int32 BackgroundIndex)
{
	UAsyncLoadingScreenLibrary::DisplayBackgroundIndex = BackgroundIndex;
}

void UAsyncLoadingScreenLibrary::SetDisplayTipTextIndex(int32 TipTextIndex)
{
	UAsyncLoadingScreenLibrary::DisplayTipTextIndex = TipTextIndex;
}

void UAsyncLoadingScreenLibrary::SetDisplayMovieIndex(int32 MovieIndex)
{
	UAsyncLoadingScreenLibrary::DisplayMovieIndex = MovieIndex;	
}

void UAsyncLoadingScreenLibrary::SetEnableLoadingScreen(bool bIsEnableLoadingScreen)
{
	bShowLoadingScreen = bIsEnableLoadingScreen;
}

void UAsyncLoadingScreenLibrary::StopLoadingScreen()
{
	// Stopping the loading screen also gives up any hold the game asked for, otherwise the plugin would put it back up
	bHoldLoadingScreen = false;
	GetMoviePlayer()->StopMovie();

	// A screen shown by ShowLoadingScreen lives in the game viewport rather than in the movie player
	if (FAsyncLoadingScreenModule::IsAvailable())
	{
		FAsyncLoadingScreenModule::Get().StopDefaultLoadingScreen();
	}
}

void UAsyncLoadingScreenLibrary::SetLoadingScreenHold(bool bHold)
{
	bHoldLoadingScreen = bHold;
}

void UAsyncLoadingScreenLibrary::ShowLoadingScreen()
{
	if (FAsyncLoadingScreenModule::IsAvailable())
	{
		FAsyncLoadingScreenModule::Get().ShowDefaultLoadingScreen();
	}
}

