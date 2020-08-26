/************************************************************************************
 *																					*
 * Copyright (C) 2020 Truong Bui.													*
 * Website:	https://github.com/truong-bui/AsyncLoadingScreen						*
 * Licensed under the MIT License. See 'LICENSE' file for full license information. *
 *																					*
 ************************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "MoviePlayer.h"
#include "Widgets/Layout/SScaleBox.h"
#include "Styling/SlateBrush.h"
#include "LoadingScreenSettings.generated.h"


/** 
 * Asynce Loading Screen Themes
 */
UENUM(BlueprintType)
enum class EAsyncLoadingScreenTheme : uint8
{
	/**
	 * The Classic is a simple, generic theme and fits well with many designs. 
	 * Loading and tip widgets can be at the bottom or top.
	 */
	ALST_Classic UMETA(DisplayName = "Classic"),
	/** 
	 * The loading widget is at the center of the screen, tips can be at the bottom or top. 	
	 * The Centrality theme is a good choice if your loading icon is the main actor. 
	 */
	ALST_Centrality UMETA(DisplayName = "Centrality")
};

/** Loading Icon Type*/
UENUM(BlueprintType)
enum class ELoadingIconType : uint8
{		
	/** SThrobber widget */
	LIT_Throbber UMETA(DisplayName = "Throbber"),
	/** SCircularThrobber widget */
	LIT_CircularThrobber UMETA(DisplayName = "Circular Throbber"),
	/** Animated images */
	LIT_ImageSequence UMETA(DisplayName = "Image Sequence")
};

/** Loading Widget type */
UENUM(BlueprintType)
enum class ELoadingWidgetType : uint8
{
	/** Horizontal alignment */
	LWT_Horizontal UMETA(DisplayName = "Horizontal"),
	/** Vertical alignment */
	LWT_Vertical UMETA(DisplayName = "Vertical"),
};

/** Alignment for widget*/
USTRUCT(BlueprintType)
struct FWidgetAlignment
{
	GENERATED_BODY()
	/** The horizontal alignment of the widget.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Alignment Setting")
	TEnumAsByte<EHorizontalAlignment> HorizontalAlignment = EHorizontalAlignment::HAlign_Center;

	/** The vertical alignment of the widget.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Alignment Setting")
	TEnumAsByte<EVerticalAlignment> VerticalAlignment = EVerticalAlignment::VAlign_Center;
};

// Text appearance settings
USTRUCT(BlueprintType)
struct FTextAppearance
{
	GENERATED_BODY()

	/** Text color and opacity */
	UPROPERTY(BlueprintReadWrite, Config, EditAnywhere, Category = "Text Appearance")
	FSlateColor ColorAndOpacity = FSlateColor(FLinearColor::White);

	// The font to render the text with.
	UPROPERTY(BlueprintReadWrite, Config, EditAnywhere, Category = "Text Appearance")
	FSlateFontInfo Font;

	/** Drop shadow offset in pixels */
	UPROPERTY(BlueprintReadWrite, Config, EditAnywhere, Category = "Text Appearance")
	FVector2D ShadowOffset;

	/** Shadow color and opacity */
	UPROPERTY(BlueprintReadWrite, Config, EditAnywhere, Category = "Text Appearance")
	FLinearColor ShadowColorAndOpacity;

	/** How the text should be aligned with the margin. */
	UPROPERTY(BlueprintReadWrite, Config, EditAnywhere, Category = "Text Appearance")
	TEnumAsByte <ETextJustify::Type> Justification;
};

USTRUCT(BlueprintType)
struct FThrobberSettings
{
	GENERATED_BODY()

	/** How many pieces there are */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Appearance, meta = (ClampMin = "1", ClampMax = "25", UIMin = "1", UIMax = "25"))
	int32 NumberOfPieces = 3;

	/** Should the pieces animate horizontally? */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Appearance)
	bool bAnimateHorizontally = true;

	/** Should the pieces animate vertically? */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Appearance)
	bool bAnimateVertically = true;

	/** Should the pieces animate their opacity? */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Appearance)
	bool bAnimateOpacity = true;

	/** Image to use for each segment of the throbber */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Appearance)
	FSlateBrush Image;
};

USTRUCT(BlueprintType)
struct FCircularThrobberSettings
{
	GENERATED_BODY()

	/** How many pieces there are */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Appearance, meta = (ClampMin = "1", ClampMax = "25", UIMin = "1", UIMax = "25"))
	int32 NumberOfPieces = 6; 

	/** The amount of time for a full circle (in seconds) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Appearance, meta = (ClampMin = "0", UIMin = "0"))
	float Period = 0.75f;

	/** The radius of the circle. If the throbber is a child of Canvas Panel, the 'Size to Content' option must be enabled in order to set Radius. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Appearance)
	float Radius = 64.0f;

	/** Image to use for each segment of the throbber */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Appearance)
	FSlateBrush Image;
};


/**
 * Background content for the widget loading screen
 */
USTRUCT(BlueprintType)
struct ASYNCLOADINGSCREEN_API FBackgroundSettings
{
	GENERATED_BODY()

	// The images randomly display while in the loading screen on top of the movie 
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Background", meta = (AllowedClasses = "Texture2D"))
	TArray<FSoftObjectPath> Images;

	// The scaling type to apply to images.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Background")
	TEnumAsByte<EStretch::Type> ImageStretch = EStretch::ScaleToFit;

	/** The padding area between the border and the image it contains.*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Background")
	FMargin Padding;

	// The border's background color if there is any image defined. If padding = 0 you will not see the border color.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Background")
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

	/** Loading icon type*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Loading Widget Setting")
	ELoadingIconType LoadingIconType = ELoadingIconType::LIT_CircularThrobber;
	
	/** Loading Widget alignment type*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Loading Widget Setting")
	ELoadingWidgetType LoadingWidgetType = ELoadingWidgetType::LWT_Horizontal;

	/** Render transform translation of the loading icon.*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Loading Widget Setting")
	FVector2D TransformTranslation = FVector2D(0.0f, 0.0f);

	/** Render transform scale of the loading icon, a negative value will flip the icon.*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Loading Widget Setting")
	FVector2D TransformScale = FVector2D(1.0f, 1.0f);

	/** Render transform pivot of the loading icon (in normalized local space).*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Loading Widget Setting")
	FVector2D TransformPivot = FVector2D(0.5f, 0.5f);

	// Text displayed beside the animated icon
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Loading Widget Setting")
	FText LoadingText;

	/** Is Loading Text on the right of the loading icon? Ignore this if you don't choose Loading Widget Type = Horizontal.*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Loading Widget Setting")
	bool bLoadingTextRightPosition = true;

	/** Is Loading Text on the top of the loading icon? Ignore this if you don't choose Loading Widget Type = Vertical.*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Loading Widget Setting")
	bool bLoadingTextTopPosition = true;

	// Loading text appearance settings
	UPROPERTY(BlueprintReadWrite, Config, EditAnywhere, Category = "Tip Settings")
	FTextAppearance Appearance;

	/** Throbber settings. Ignore this if you don't choose the 'Throbber' icon type*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Loading Widget Setting")
	FThrobberSettings ThrobberSettings;

	/** Circular Throbber settings. Ignore this if you don't choose the 'Circular Throbber' icon type*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Loading Widget Setting")
	FCircularThrobberSettings CircularThrobberSettings;

	/** An array of images for animating the loading icon. Ignore this if you don't choose the 'Image Sequence' icon type*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Loading Widget Setting", meta = (AllowedClasses = "Texture2D"))
	TArray<FSoftObjectPath> Images;	

	/** 
	 * Time in second to update the images, the smaller value the faster of the animation. A zero value will update the images every frame. 
	 * Ignore this if you don't choose the 'Image Sequence' icon type.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Loading Widget Setting", meta = (UIMax = 1.00, UIMin = 0.00, ClampMin = "0", ClampMax = "1"))
	float Interval = 0.05f;
	
	/** The alignment of the loading text.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loading Widget Setting")
	FWidgetAlignment TextAlignment;

	/** The alignment of the loading icon. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loading Widget Setting")
	FWidgetAlignment LoadingIconAlignment;

	/** Empty space between the loading text and the loading icon */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loading Widget Setting")
	float Space = 1.0f;
};


/**
 * Tips text settings
 */
USTRUCT(BlueprintType)
struct ASYNCLOADINGSCREEN_API FTipSettings
{
	GENERATED_BODY()

	// The tip text randomly display in the loading screen.
	UPROPERTY(BlueprintReadWrite, Config, EditAnywhere, Category = "Tip Settings", meta = (MultiLine = true))
	TArray<FText> TipText;

	// Tip text appearance settings
	UPROPERTY(BlueprintReadWrite, Config, EditAnywhere, Category = "Tip Settings")
	FTextAppearance Appearance;

	// The size of the tip before it's wrapped to the next line
	UPROPERTY(BlueprintReadWrite, Config, EditAnywhere, Category = "Tip Settings")
	float TipWrapAt;
};

/**
 * Loading Screen Settings
 */
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
	* All movie files must be locate at Content/Movies/ directory. Suggested format: MPEG-4 Movie (mp4). Enter file path/name without the extension.
	* E.g., if you have a movie name my_movie.mp4 in the 'Content/Movies' folder, then enter my_movie in the input field.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Movies Settings")
	TArray<FString> MoviePaths;
	
	/** 
	 * Should we show the loading screen widget (background/tips/loading widget)? Generally you'll want to set this to false if you just want to show a movie.
	 */ 
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Loading Screen Settings")
	bool bShowWidgetOverlay = true;

	/**
	 * Select async loading screen theme. Ignore this if you choose "Show Widget Overlay = false"
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Loading Screen Settings")
	EAsyncLoadingScreenTheme Theme = EAsyncLoadingScreenTheme::ALST_Classic;

	/** Background content for the loading screen. Ignore this if you choose "Show Widget Overlay = false" */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Loading Screen Settings")
	FBackgroundSettings Background;
	
	/** Loading content for the loading screen. Ignore this if you choose "Show Widget Overlay = false" */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Loading Screen Settings")
	FLoadingWidgetSettings LoadingWidget;
	
	/** Tips text widget for the loading screen. Ignore this if you choose "Show Widget Overlay = false" */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Loading Screen Settings")
	FTipSettings TipWidget;
};

/** Classic Theme settings*/
USTRUCT(BlueprintType)
struct FClassicThemeSettings
{
	GENERATED_BODY()

	/** Is loading/tip widget located at the bottom or top? */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Classic Theme")
	bool bWidgetAtBottom = true;

	/** Is loading widget on the left of the tip? */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Classic Theme")
	bool bLoadingWidgetLeft = true;

	/** The padding area between the border and the widget it contains.*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Classic Theme")
	FMargin WidgetBorderPadding;

	/** The empty space between loading widget and the tip.*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Classic Theme")
	float Space = 1.0f;

	/** The alignment of the tips. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Classic Theme")
	FWidgetAlignment TipAlignment;

	/** Background appearance settings for the border widget */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Classic Theme")
	FSlateBrush WidgetBorderBackground;
};

/** Centrality Theme settings*/
USTRUCT(BlueprintType)
struct FCentralityThemeSettings
{
	GENERATED_BODY()

	/** Is tip located at the bottom or top? */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Centrality Theme")
	bool bTipAtBottom = true;
	
	/** Padding at bottom or top depend on the tip located at the bottom or top position.*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Centrality Theme")
	float TipWidgetVerticalPadding = 0.0f;

	/** The padding area between the border and the tips it contains.*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Centrality Theme")
	FMargin WidgetBorderPadding;

	/** The alignment of the tips. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Centrality Theme")
	FWidgetAlignment TipAlignment;

	/** Background appearance settings for tip area */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Centrality Theme")
	FSlateBrush WidgetBorderBackground;

	// The color and opacity for the background of the tip area
	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Centrality Theme")
	//FLinearColor TipBackgroundColor = FLinearColor::Black;
};


/**
 * Async Loading Screen Settings 
 */
UCLASS(Config = "Game", defaultconfig, meta = (DisplayName = "Async Loading Screen"))
class ASYNCLOADINGSCREEN_API ULoadingScreenSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	ULoadingScreenSettings(const FObjectInitializer& Initializer);
	
	/**
	 * The startup loading screen when you first open the game. Setup any studio logo movies here.
	 */
	UPROPERTY(Config, EditAnywhere, Category = "General")
	FALoadingScreenSettings StartupLoadingScreen;

	/**
	 * The default loading screen that shows up whenever you open a new level.
	 */
	UPROPERTY(Config, EditAnywhere, Category = "General")
	FALoadingScreenSettings DefaultLoadingScreen;
	
	/**
	 * Classic Theme settings.
	 */
	UPROPERTY(Config, EditAnywhere, Category = "Themes")
	FClassicThemeSettings Classic;
	
	/**
	 * Centrality Theme settings.
	 */
	UPROPERTY(Config, EditAnywhere, Category = "Themes")
	FCentralityThemeSettings Centrality;

	
};
