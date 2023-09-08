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
#include "Framework/Text/TextLayout.h"
#include "LoadingScreenSettings.generated.h"


/** 
 * Asynce Loading Screen Layouts
 */
UENUM(BlueprintType)
enum class EAsyncLoadingScreenLayout : uint8
{
	/**
	 * The Classic is a simple, generic layout and fits well with many designs.
	 * Loading and tip widgets can be at the bottom or top.
	 */
	ALSL_Classic UMETA(DisplayName = "Classic"),
	/** 
	 * The loading widget is at the center of the screen, tip widget can be at the bottom or top.
	 * The Center layout is a good choice if your loading icon is the main design.
	 */
	ALSL_Center UMETA(DisplayName = "Center"),
	/**
	 * The Letterbox layout has two borders on top and bottom of the screen. Loading widget 
	 * can be on the top and the tip is at the bottom of the screen, or vice versa.
	 */
	 ALSL_Letterbox UMETA(DisplayName = "Letterbox"),
	/**
	 * The Sidebar layout has a vertical border on the left or right of the screen. The Sidebar 
	 * is suitable for storytelling, long paragraphs due to the height of the tip widget.
	 */
	 ALSL_Sidebar UMETA(DisplayName = "Sidebar"),

	/**
	 * Similar to Sidebar layout but Dual Sidebar layout has two vertical borders on both left and right of the screen.
	 * The Dual Sidebar layout is suitable for storytelling, long paragraphs due to the height of the tip widget.
	 */
	 ALSL_DualSidebar UMETA(DisplayName = "Dual Sidebar")
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
	FVector2D ShadowOffset = FVector2D::ZeroVector;

	/** Shadow color and opacity */
	UPROPERTY(BlueprintReadWrite, Config, EditAnywhere, Category = "Text Appearance")
	FLinearColor ShadowColorAndOpacity = FLinearColor::White;

	/** How the text should be aligned with the margin. */
	UPROPERTY(BlueprintReadWrite, Config, EditAnywhere, Category = "Text Appearance")
	TEnumAsByte <ETextJustify::Type> Justification = ETextJustify::Left;
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

USTRUCT(BlueprintType)
struct FImageSequenceSettings
{
	GENERATED_BODY()

	/** An array of images for animating the loading icon.*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Loading Widget Setting", meta = (AllowedClasses = "/Script/Engine.Texture2D"))
	TArray<UTexture2D*> Images;

	/** Scale of the images.*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Loading Widget Setting")
	FVector2D Scale = FVector2D(1.0f, 1.0f);

	/**
	 * Time in second to update the images, the smaller value the faster of the animation. A zero value will update the images every frame.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Loading Widget Setting", meta = (UIMax = 1.00, UIMin = 0.00, ClampMin = "0", ClampMax = "1"))
	float Interval = 0.05f;

	/** Play the image sequence in reverse.*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Loading Widget Setting")
	bool bPlayReverse = false;
};

/**
 * Background widget for the widget loading screen
 */
USTRUCT(BlueprintType)
struct ASYNCLOADINGSCREEN_API FBackgroundSettings
{
	GENERATED_BODY()

	// The images randomly display while in the loading screen on top of the movie 
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Background", meta = (AllowedClasses = "/Script/Engine.Texture2D"))
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

	/**
	 * If true, you will have to manually set which background index you want to display on the loading screen by calling "SetDisplayBackgroundIndex" function
	 * in your Blueprint before opening a new level. If the index you set is not valid, then it will display random background in the "Images" array.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Background")
	bool bSetDisplayBackgroundManually = false;
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
	
	/** Loading Widget type*/
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
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tip Settings")
	FTextAppearance Appearance;

	/** Throbber settings. Ignore this if you don't choose the 'Throbber' icon type*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Loading Widget Setting")
	FThrobberSettings ThrobberSettings;

	/** Circular Throbber settings. Ignore this if you don't choose the 'Circular Throbber' icon type*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Loading Widget Setting")
	FCircularThrobberSettings CircularThrobberSettings;

	/** Image Sequence settings. Ignore this if you don't choose the 'Image Sequence' icon type*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Loading Widget Setting")
	FImageSequenceSettings ImageSequenceSettings;

	/** The alignment of the loading text.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loading Widget Setting")
	FWidgetAlignment TextAlignment;

	/** The alignment of the loading icon. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loading Widget Setting")
	FWidgetAlignment LoadingIconAlignment;

	/** Empty space between the loading text and the loading icon */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loading Widget Setting")
	float Space = 1.0f;

	/** Hide the loading widget when the level loading is complete*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loading Widget Setting")
	bool bHideLoadingWidgetWhenCompletes = false;
};


/**
 * Tips text settings
 */
USTRUCT(BlueprintType)
struct ASYNCLOADINGSCREEN_API FTipSettings
{
	GENERATED_BODY()

	// The tip text randomly display in the loading screen.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tip Settings", meta = (MultiLine = true))
	TArray<FText> TipText;

	// Tip text appearance settings
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tip Settings")
	FTextAppearance Appearance;

	// The size of the tip before it's wrapped to the next line
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tip Settings")
	float TipWrapAt = 0.0f;

	/**
	 * If true, you will have to manually set which TipText index you want to display on the loading screen by calling "SetDisplayTipTextIndex" function
	 * in your Blueprint before opening a new level. If the index you set is not valid, then it will display random Tip in the "TipText" array.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Background")
	bool bSetDisplayTipTextManually = false;
};

/**
 * The text that displayed when loading is complete. Ignore this if you don't set "bShowLoadingCompletedText" = true
 */
USTRUCT(BlueprintType)
struct ASYNCLOADINGSCREEN_API FLoadingCompleteTextSettings
{
	GENERATED_BODY()

//	FLoadingCompleteTextSettings();

	// The text that shows up when level loading is done.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Loading Complete Text Settings")
	FText LoadingCompleteText;

	// Text appearance settings
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Loading Complete Text Settings")
	FTextAppearance Appearance;
	
	/** The alignment of the text.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loading Widget Setting")
	FWidgetAlignment Alignment;

	/** Text padding. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Loading Complete Text Settings")
	FMargin Padding;

	// Animate the text?
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Loading Complete Text Settings")
	bool bFadeInOutAnim = true;

	/**
	 * Animation speed
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Loading Complete Text Settings", meta = (UIMax = 10.00, UIMin = 0.00, ClampMin = "0", ClampMax = "10"))
	float AnimationSpeed = 1.0f;
};

/**
 * Loading Screen Settings
 */
USTRUCT(BlueprintType)
struct ASYNCLOADINGSCREEN_API FALoadingScreenSettings
{
	GENERATED_BODY()	

	// The minimum time that a loading screen should be opened for, -1 if there is no minimum time. I recommend set it to -1.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Movies Settings")
	float MinimumLoadingScreenDisplayTime = -1;
	
	// If true, the loading screen will disappear as soon as loading is done.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Movies Settings")
	bool bAutoCompleteWhenLoadingCompletes = true;

	// If true, movies can be skipped by clicking the loading screen as long as loading is done.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Movies Settings")
	bool bMoviesAreSkippable = true;

	/** 
	 * If true, movie playback continue until Stop is called.
	 * 
	 * NOTE: If set "Minimum Loading Screen Display Time" = -1, it will allow players to press any key to stop the loading screen.
	 * If "Minimum Loading Screen Display Time" >= 0, you have to call "StopLoadingScreen" in the BeginPlay event 
	 * of your GameInstance, GameMode, or PlayerController blueprint to stop the loading screen ("bAllowEngineTick" must be true)
	 **/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Movies Settings")
	bool bWaitForManualStop = false;

	/** If true loading screens here cannot have any uobjects of any kind or use any engine features at all. This will start the movies very early as a result on platforms that support it */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Movies Settings")
	bool bAllowInEarlyStartup = false;

	/** If true, this will call the engine tick while the game thread is stalled waiting for a loading movie to finish. This only works for post-startup load screens and is potentially unsafe */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Movies Settings")
	bool bAllowEngineTick = false;

	/** Should we just play back, loop, etc.  NOTE: if the playback type is MT_LoopLast, then bAutoCompleteWhenLoadingCompletes will be togged on when the last movie is hit*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Movies Settings")
	TEnumAsByte<EMoviePlaybackType> PlaybackType = EMoviePlaybackType::MT_Normal;

	/**
	 * All movie files must be locate at Content/Movies/ directory. Suggested format: MPEG-4 Movie (mp4). Enter file path/name without the extension.
	 * E.g., if you have a movie name my_movie.mp4 in the 'Content/Movies' folder, then enter my_movie in the input field.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Movies Settings")
	TArray<FString> MoviePaths;
	
	/**
	 * If true, shuffle the movies list before playing.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Movies Settings")
	bool bShuffle = false;

	/**
	 * If true, the "Shuffle" option will be ignored, and you will have to manually set which Movie index you want to display on the loading screen 
	 * by calling "SetDisplayMovieIndex" function in your Blueprint before opening a new level.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Movies Settings")
	bool bSetDisplayMovieIndexManually = false;


	/** 
	 * Should we show the loading screen widgets (background/tips/loading widget)? Generally you'll want to set this to false if you just want to show a movie.
	 */ 
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Loading Screen Settings")
	bool bShowWidgetOverlay = true;		

	/**
	 * If true show a text when level loading is completed. Ignore this if you choose "Show Widget Overlay" = false
	 *
	 * NOTE: To enable this option properly, you need to set "Wait For Manual Stop" = true, and "Minimum Loading Screen Display Time" = -1.
	 * This also allows players press any button to stop the Loading Screen.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Loading Screen Settings")
	bool bShowLoadingCompleteText = false;

	/**
	 * The text that displayed when loading is complete. Ignore this if you set "Show Loading Complete Text" = false.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Loading Screen Settings")
	FLoadingCompleteTextSettings LoadingCompleteTextSettings;

	/** Background widget for the loading screen. Ignore this if you choose "Show Widget Overlay = false" */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Loading Screen Settings")
	FBackgroundSettings Background;	
	
	/** Tip widget for the loading screen. Ignore this if you choose "Show Widget Overlay = false" */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Loading Screen Settings")
	FTipSettings TipWidget;

	/** Loading widget for the loading screen. Ignore this if you choose "Show Widget Overlay = false" */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Loading Screen Settings")
	FLoadingWidgetSettings LoadingWidget;

	/**
	 * Select async loading screen Layout. Ignore this if you choose "Show Widget Overlay = false"
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Loading Screen Settings")
	EAsyncLoadingScreenLayout Layout = EAsyncLoadingScreenLayout::ALSL_Classic;
};

/** Classic Layout settings*/
USTRUCT(BlueprintType)
struct FClassicLayoutSettings
{
	GENERATED_BODY()

	/** Is the border that contains loading and tip widget located at the bottom or top? */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Classic Layout")
	bool bIsWidgetAtBottom = true;

	/** Is loading widget on the left of the tip? */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Classic Layout")
	bool bIsLoadingWidgetAtLeft = true;	

	/** The empty space between loading widget and the tip.*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Classic Layout")
	float Space = 1.0f;

	/** The alignment of the tips. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Classic Layout")
	FWidgetAlignment TipAlignment;

	/** The horizontal alignment of the border background.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Center Layout")
	TEnumAsByte<EHorizontalAlignment> BorderHorizontalAlignment = EHorizontalAlignment::HAlign_Fill;

	/** The padding area between the border and the widget it contains.*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Classic Layout")
	FMargin BorderPadding;

	/** Background appearance settings for the border widget */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Classic Layout")
	FSlateBrush BorderBackground;	
};

/** Center Layout settings*/
USTRUCT(BlueprintType)
struct FCenterLayoutSettings
{
	GENERATED_BODY()

	/** Is tip located at the bottom or top? */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Center Layout")
	bool bIsTipAtBottom = true;	

	/** The alignment of the tips. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Center Layout")
	FWidgetAlignment TipAlignment;

	/** The horizontal alignment of the border.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Center Layout")
	TEnumAsByte<EHorizontalAlignment> BorderHorizontalAlignment = EHorizontalAlignment::HAlign_Fill;

	/** Offset to bottom or top of the screen depending on the tip located at the bottom or top position.*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Center Layout")
	float BorderVerticalOffset = 0.0f;

	/** The padding area between the border and the tips it contains.*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Center Layout")
	FMargin BorderPadding;

	/** Background appearance settings for tip area */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Center Layout")
	FSlateBrush BorderBackground;
};

/** Letterbox Layout settings*/
USTRUCT(BlueprintType)
struct FLetterboxLayoutSettings
{
	GENERATED_BODY()

	/** Is loading widget located at the bottom or top? */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Letterbox Layout")
	bool bIsLoadingWidgetAtTop = true;

	/** The alignment of the tips. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Letterbox Layout")
	FWidgetAlignment TipAlignment;

	/** The alignment of the loading widget. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Letterbox Layout")
	FWidgetAlignment LoadingWidgetAlignment;

	/** The horizontal alignment of the top border.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Letterbox Layout")
	TEnumAsByte<EHorizontalAlignment> TopBorderHorizontalAlignment = EHorizontalAlignment::HAlign_Fill;

	/** The horizontal alignment of the bottom border.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Letterbox Layout")
	TEnumAsByte<EHorizontalAlignment> BottomBorderHorizontalAlignment = EHorizontalAlignment::HAlign_Fill;

	/** The top padding area between the border and the widget it contains.*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Letterbox Layout")
	FMargin TopBorderPadding;

	/** The bottom padding area between the border and the widget it contains.*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Letterbox Layout")
	FMargin BottomBorderPadding;

	/** Background appearance settings for top border */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Letterbox Layout")
	FSlateBrush TopBorderBackground;

	/** Background appearance settings for bottom border */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Letterbox Layout")
	FSlateBrush BottomBorderBackground;
};

/** Sidebar Layout settings*/
USTRUCT(BlueprintType)
struct FSidebarLayoutSettings
{
	GENERATED_BODY()

	/** Is the border that contains loading and tip widgets located at the right or left? */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sidebar Layout")
	bool bIsWidgetAtRight = true;

	/** Is loading widget on the top of the tip? */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sidebar Layout")
	bool bIsLoadingWidgetAtTop = true;

	/** The empty space between loading widget and the tip.*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sidebar Layout")
	float Space = 1.0f;	

	/** The vertical alignment of the vertical box that contains loading/tip widgets. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sidebar Layout")
	TEnumAsByte<EVerticalAlignment> VerticalAlignment = EVerticalAlignment::VAlign_Center;

	/** The alignment of the loading widget. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sidebar Layout")
	FWidgetAlignment LoadingWidgetAlignment;

	/** The alignment of the tips. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sidebar Layout")
	FWidgetAlignment TipAlignment;

	/** The vertical alignment of the border background that contains all widgets. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sidebar Layout")
	TEnumAsByte<EVerticalAlignment> BorderVerticalAlignment = EVerticalAlignment::VAlign_Fill;

	/** Offset to left or right of the screen depending on the border that contains loading and tip widgets located at the left or right position.*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sidebar Layout")
	float BorderHorizontalOffset = 0.0f;

	/** The padding area between the border and the widget it contains.*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sidebar Layout")
	FMargin BorderPadding;

	/** Background appearance settings for the border widget */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sidebar Layout")
	FSlateBrush BorderBackground;
};

/** Dual Sidebar Layout settings*/
USTRUCT(BlueprintType)
struct FDualSidebarLayoutSettings
{
	GENERATED_BODY()

	/** Is loading widget on the right or left border? */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Dual Sidebar Layout")
	bool bIsLoadingWidgetAtRight = true;

	/** The vertical alignment of the left widget. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Dual Sidebar Layout")
	TEnumAsByte<EVerticalAlignment> LeftVerticalAlignment = EVerticalAlignment::VAlign_Center;

	/** The vertical alignment of the right widget. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Dual Sidebar Layout")
	TEnumAsByte<EVerticalAlignment> RightVerticalAlignment = EVerticalAlignment::VAlign_Center;

	/** The vertical alignment of the left border background that contains all widgets. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Dual Sidebar Layout")
	TEnumAsByte<EVerticalAlignment> LeftBorderVerticalAlignment = EVerticalAlignment::VAlign_Fill;

	/** The vertical alignment of the right border background that contains all widgets. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Dual Sidebar Layout")
	TEnumAsByte<EVerticalAlignment> RightBorderVerticalAlignment = EVerticalAlignment::VAlign_Fill;

	/** The padding area between the left border and the widget it contains.*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Dual Sidebar Layout")
	FMargin LeftBorderPadding;

	/** The padding area between the right border and the widget it contains.*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Dual Sidebar Layout")
	FMargin RightBorderPadding;

	/** Background appearance settings for the left border widget */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Dual Sidebar Layout")
	FSlateBrush LeftBorderBackground;

	/** Background appearance settings for the right border widget */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Dual Sidebar Layout")
	FSlateBrush RightBorderBackground;
};

/**
 * Async Loading Screen Settings 
 */
UCLASS(Config = "Game", defaultconfig, meta = (DisplayName = "Async Loading Screen"))
class ASYNCLOADINGSCREEN_API ULoadingScreenSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	ULoadingScreenSettings(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	/**
	 * If true, load all background images at the start of the game.
	 * 
	 * This is a workaround for the issue when the background image 
	 * is loaded too late with the wrong image scaling. 
	 * 
	 * This issue only happens in the Standalone or Launch mode.
	 * The packaged game should work fine.
	 * 
	 * If you don't encounter this issue when developing, don't enable 
	 * this option, since it will keep the background images in the 
	 * memory all the time, therefore consumes memory resources.
	 * 
	 * However, you can manually remove all the preloaded background
	 * images by calling the Blueprint function 
	 * "RemovePreloadedBackgroundImages"
	 * 
	 * You will need to re-load all background images by calling 
	 * the Blueprint function "PreloadBackgroundImages"
	 * 
	 * Note: Call "PreloadBackgroundImages" before the "OpenLevel"
	 * 
	 */
	UPROPERTY(Config, EditAnywhere, Category = "General")
	bool bPreloadBackgroundImages = false;

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
	 * Classic Layout settings.
	 * The Classic is a simple, generic layout and fits well with many designs.
	 * A border that contains loading and tip widgets can be at the bottom or top.
	 */
	UPROPERTY(Config, EditAnywhere, Category = "Layout")
	FClassicLayoutSettings Classic;
	
	/**
	 * Center Layout settings.
	 * The loading widget is at the center of the screen, tip widget can be at the bottom or top.
	 * The Center layout is a good choice if your loading icon is the main design.
	 */
	UPROPERTY(Config, EditAnywhere, Category = "Layout")
	FCenterLayoutSettings Center;

	/**
	 * Letterbox Layout settings.
	 * The Letterbox layout has two borders on top and bottom of the screen. Loading widget
	 * can be on the top and the tip is at the bottom of the screen, or vice versa.
	 */
	UPROPERTY(Config, EditAnywhere, Category = "Layout")
	FLetterboxLayoutSettings Letterbox;

	/**
	 * Sidebar Layout settings.
	 * The Sidebar layout has a vertical border on the left or right of the screen. The Sidebar 
	 * is suitable for storytelling, long paragraphs due to the height of the tip widget.
	 */
	UPROPERTY(Config, EditAnywhere, Category = "Layout")
	FSidebarLayoutSettings Sidebar;

	/**
	 * Dual Sidebar Layout settings
	 * Similar to Sidebar layout but Dual Sidebar layout has two vertical borders on both left and right of the screen.
	 * The Dual Sidebar layout is suitable for storytelling, long paragraphs due to the height of the tip widget.
	 */
	UPROPERTY(Config, EditAnywhere, Category = "Layout")
	FDualSidebarLayoutSettings DualSidebar;

};
