/************************************************************************************
 *																					*
 * Copyright (C) 2020 Truong Bui.													*
 * Website:	https://github.com/truong-bui/AsyncLoadingScreen						*
 * Licensed under the MIT License. See 'LICENSE' file for full license information. *
 *																					*
 ************************************************************************************/


#include "AsyncLoadingScreenLibrary.h"
#include "MoviePlayer.h"

int32 UAsyncLoadingScreenLibrary::DisplayBackgroundIndex = -1;
int32 UAsyncLoadingScreenLibrary::DisplayTipTextIndex = -1;
int32 UAsyncLoadingScreenLibrary::DisplayMovieIndex = -1;

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

void UAsyncLoadingScreenLibrary::StopLoadingScreen()
{
	GetMoviePlayer()->StopMovie();
}
