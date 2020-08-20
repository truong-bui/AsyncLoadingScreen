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

/** Loading Widget Type*/
UENUM(BlueprintType)
enum class ELoadingWidgetType : uint8
{		
	/** SThrobber widget */
	LWT_Throbber UMETA(DisplayName = "Throbber"),
	/** SCircularThrobber widget */
	LWT_CircularThrobber UMETA(DisplayName = "Circular Throbber"),
	/** Animated images */
	LWT_ImageSequence UMETA(DisplayName = "Image Sequence")
};

/**
 * Background content for the widget loading screen
 */
USTRUCT(BlueprintType)
struct ASYNCLOADINGSCREEN_API FBackgroundSettings
{
	GENERATED_BODY()

	//FBackgroundSettings();

	// The images display while in the loading screen on top of the movie.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Images", meta = (AllowedClasses = "Texture2D"))
		TArray<FSoftObjectPath> Images;

	// The scaling type to apply to images.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Images")
		TEnumAsByte<EStretch::Type> ImageStretch = EStretch::ScaleToFit;

	// The background color to use
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Images")
		FLinearColor BackgroundColor = FLinearColor::Black;

};

/**
 * Loading widget settings
 */
USTRUCT(BlueprintType)
struct ASYNCLOADINGSCREEN_API FLoadingWidgetSettings
{
	GENERATED_BODY()

	FLoadingWidgetSettings();

	/** Loading widget type*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Loading Widget Setting")
	ELoadingWidgetType LoadingWidgetType;

	// Text displayed beside the animated icon
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Loading Widget Setting")
		FText LoadingText;

	/** A sequence of images to animate the loading icon, don't use the sprite sheet here. Ignore this if you don't choose the 'Image Sequence' widget type*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Loading Widget Setting", meta = (AllowedClasses = "Texture2D"))
		TArray<FSoftObjectPath> Images;

	/** Time in second to change the images, the smaller numbers the faster of the animation. Ignore this if you don't choose the 'Image Sequence' widget type */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Loading Widget Setting", meta = (UIMax = 1.00, UIMin = 0.00))
		float Interval = 0.1f;

	// The font of the loading text
	UPROPERTY(BlueprintReadWrite, Config, EditAnywhere, Category = "Loading Screen")
		FSlateFontInfo Font;
};


/**
 * Tips text settings
 */
USTRUCT(BlueprintType)
struct ASYNCLOADINGSCREEN_API FTipSettings
{
	GENERATED_BODY()

	//	FTipSettings();
	// The tips to display on the load screen.
	UPROPERTY(BlueprintReadWrite, Config, EditAnywhere, Category = "Tip Settings", meta = (MultiLine = true))
		TArray<FText> TipsText;

	// The background color to use for the tips
	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tip Settings")
	//FLinearColor TipBackgroundColor = FLinearColor(0, 0, 0, 0.75f);

	// The font of Tip text.
	UPROPERTY(BlueprintReadWrite, Config, EditAnywhere, Category = "Tip Settings")
		FSlateFontInfo Font;

	// The size of the tip before it's wrapped to the next line
	UPROPERTY(BlueprintReadWrite, Config, EditAnywhere, Category = "Tip Settings")
		float TipWrapAt;
};

USTRUCT(BlueprintType)
struct ASYNCLOADINGSCREEN_API FALoadingScreenSettings
{
	GENERATED_BODY()	

	// The minimum time that a loading screen should be opened for, -1 if there is nor minimum time.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Movies Settings")
	float MinimumLoadingScreenDisplayTime = -1;
	
	// If true, the loading screen will disappear as soon as loading is done.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Movies Settings")
	bool bAutoCompleteWhenLoadingCompletes = true;

	// If true, movies can be skipped by clicking the loading screen as long as loading is done.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Movies Settings")
	bool bMoviesAreSkippable = true;

	// If true, movie playback continue until Stop is called.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Movies Settings")
	bool bWaitForManualStop = false;

	/** Should we just play back, loop, etc.  NOTE: if the playback type is MT_LoopLast, then bAutoCompleteWhenLoadingCompletes will be togged on when the last movie is hit*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Movies Settings")
	TEnumAsByte<EMoviePlaybackType> PlaybackType;

	/**
	* All movie files must be locate at Content/Movies/ directory. Suggested format: MPEG-4 Movie (mp4). Enter file path without the extension.
	* E.g., if you have a movie name my_movie.mp4 in the 'Content/Movies' folder, then enter my_movie in the input field.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Movies Settings")
	TArray<FString> MoviePaths;
	
	/** 
	 * Should we show the loading screen widget (background/tips/loading widget)? Generally you'll want to set this to false if you just want to show a movie.
	 */ 
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Movies Settings")
	bool bShowWidgetOverlay = true;

	/** Background content for the loading screen. Ignore this if you choose "Show Widget Overlay = false" */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI Settings")
	FBackgroundSettings Background;
	
	/** Loading contents for the loading screen. Ignore this if you choose "Show Widget Overlay = false" */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI Settings")
	FLoadingWidgetSettings LoadingWidget;
	
	/** Tips text for the loading screen. Ignore this if you choose "Show Widget Overlay = false" */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI Settings")
	FTipSettings Tips;
};



/**
 * Async Loading Screen plugin Settings 
 */
UCLASS(Config = "Game", defaultconfig, meta = (DisplayName = "Async Loading Screen"))
class ASYNCLOADINGSCREEN_API ULoadingScreenSettings : public UDeveloperSettings
{
	GENERATED_BODY()
public:

	ULoadingScreenSettings(const FObjectInitializer& Initializer);
	
	/**
	 * The startup loading screen when the users first open the game. Setup any studio logo movies here.
	 */
	UPROPERTY(Config, EditAnywhere, Category = "General")
	FALoadingScreenSettings StartupLoadingScreen;

	/**
	 * The default loading screen that shows up whenever you open a new level.
	 */
	UPROPERTY(Config, EditAnywhere, Category = "General")
	FALoadingScreenSettings DefaultLoadingScreen;
	
};
