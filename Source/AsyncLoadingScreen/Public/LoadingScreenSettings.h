/**********************************************************************************
 *
 * Copyright (C) 2020 Truong Bui.
 * Website:	https://github.com/truong-bui/AsyncLoadingScreen
 * Licensed under the MIT License. See 'LICENSE' file for full license information.
 * 
 **********************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "MoviePlayer.h"
#include "Widgets/Layout/SScaleBox.h"
#include "LoadingScreenSettings.generated.h"

USTRUCT(BlueprintType)
struct ASYNCLOADINGSCREEN_API FLoadingScreenDescription
{
	GENERATED_BODY()

	FLoadingScreenDescription();

	// The minimum time that a loading screen should be opened for, -1 if there is nor minimum time.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Loading")
	float MinimumLoadingScreenDisplayTime = -1;
	
	// If true, the loading screen will disappear as soon as loading is done.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Loading")
	bool bAutoCompleteWhenLoadingCompletes = true;

	// If true, movies can be skipped by clicking the loading screen as long as loading is done.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Loading")
	bool bMoviesAreSkippable = true;

	// If true, movie playback continue until Stop is called.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Loading")
	bool bWaitForManualStop = false;

	// Should we just play back, loop, etc. NOTE: if playback type is MT_LoadingLoop, then MoviePlayer will auto complete in the last movie and load finishes regardless of bAutoCompleteWhenLoadingCompletes
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Loading")
	TEnumAsByte<EMoviePlaybackType> PlaybackType;

	/**
	* All movie files must be locate at Content/Movies/ directory.
	* Supported formart: MPEG-4 Movie (*.mp4).
	* Enter file path/name without extension.
	* For example, if you have a movie name my_movie.mp4 in the 'Content/Movies' folder,
	* then enter my_movie in the input.
	*/ 
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Movies")
	TArray<FString> MoviePaths;

	// Should we show the images/tips/loading text? Generally you'll want to set this to false if you just want to show a movie.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Display")
	bool bShowUIOverlay = true;

	// Text displayed beside the animated icon
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Display")
	FText LoadingText;
	
	// The texture display while in the loading screen on top of the movie.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Images", meta = (AllowedClasses = "Texture2D"))
	TArray<FSoftObjectPath> Images;	

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Images", meta = (AllowedClasses = "Texture2D"))
	TArray<FSoftObjectPath> IconImages;

	// The scaling type to apply to images.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Images")
	TEnumAsByte<EStretch::Type> ImageStretch = EStretch::ScaleToFit;

	// The background color to use
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Images")
	FLinearColor BackgroundColor = FLinearColor::Black;

	// The background color to use for the tips
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Images")
	FLinearColor TipBackgroundColor = FLinearColor(0, 0, 0, 0.75f);
};



/**
 * Setttings for the classic loading screen plugin
 */
UCLASS(Config = "Game", defaultconfig, meta = (DisplayName = "Async Loading Screen"))
class ASYNCLOADINGSCREEN_API ULoadingScreenSettings : public UDeveloperSettings
{
	GENERATED_BODY()
public:

	ULoadingScreenSettings(const FObjectInitializer& Initializer);
	
	// The startup screen for the project.
	UPROPERTY(Config, EditAnywhere, Category = "Loading Screen")
	FLoadingScreenDescription StartupScreen;

	// The default load screen between maps
	UPROPERTY(Config, EditAnywhere, Category = "Loading Screen")
	FLoadingScreenDescription DefaultScreen;

	// The font to display the tips in.
	UPROPERTY(BlueprintReadWrite, Config, EditAnywhere, Category = "Loading Screen")
	FSlateFontInfo TipFont;

	// The font to display on loading
	UPROPERTY(BlueprintReadWrite, Config, EditAnywhere, Category = "Loading Screen")
	FSlateFontInfo LoadingFont;

	// The size of the tip before it's wrapped to the next line
	UPROPERTY(BlueprintReadWrite, Config, EditAnywhere, Category = "Loading Screen")
	float TipWrapAt;

	// The tips to display on the load screen.
	UPROPERTY(BlueprintReadWrite, Config, EditAnywhere, Category = "Loading Screen", meta = (MultiLine = true))
	TArray<FText> Tips;

};
