/************************************************************************************
 *																					*
 * Copyright (C) 2020 Truong Bui.													*
 * Website:	https://github.com/truong-bui/AsyncLoadingScreen						*
 * Licensed under the MIT License. See 'LICENSE' file for full license information. *
 *																					*
 ************************************************************************************/

#include "LoadingScreenSettings.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Font.h"
#include "Brushes/SlateColorBrush.h"

#define LOCTEXT_NAMESPACE "AsyncLoadingScreen"

FLoadingWidgetSettings::FLoadingWidgetSettings() : LoadingText(LOCTEXT("Loading", "LOADING")) {}

FPSOPrecacheProgressSettings::FPSOPrecacheProgressSettings()
{
	ProgressText = LOCTEXT("CompilingShaders", "Compiling Shaders... {Percent}%");
	// Solid color brushes so the progress bar is visible out of the box without configuring any images
	Style.BackgroundImage = FSlateColorBrush(FLinearColor(0.02f, 0.02f, 0.02f, 0.8f));
	Style.FillImage = FSlateColorBrush(FLinearColor::White);
	Appearance.Justification = ETextJustify::Center;
	Alignment.VerticalAlignment = EVerticalAlignment::VAlign_Bottom;
}


ULoadingScreenSettings::ULoadingScreenSettings(const FObjectInitializer& Initializer) : Super(Initializer)
{
	StartupLoadingScreen.TipWidget.TipWrapAt = 1000.0f;
	StartupLoadingScreen.bShowWidgetOverlay = false;
	DefaultLoadingScreen.TipWidget.TipWrapAt = 1000.0f;
	// Set default font
	if (!IsRunningDedicatedServer())
	{
		static ConstructorHelpers::FObjectFinder<UFont> RobotoFontObj(TEXT("/Engine/EngineFonts/Roboto"));
		StartupLoadingScreen.TipWidget.Appearance.Font = FSlateFontInfo(RobotoFontObj.Object, 20, FName("Normal"));
		DefaultLoadingScreen.TipWidget.Appearance.Font = FSlateFontInfo(RobotoFontObj.Object, 20, FName("Normal"));
		StartupLoadingScreen.LoadingWidget.Appearance.Font = FSlateFontInfo(RobotoFontObj.Object, 32, FName("Bold"));
		DefaultLoadingScreen.LoadingWidget.Appearance.Font = FSlateFontInfo(RobotoFontObj.Object, 32, FName("Bold"));
		StartupLoadingScreen.LoadingCompleteTextSettings.Appearance.Font = FSlateFontInfo(RobotoFontObj.Object, 24, FName("Normal"));
		DefaultLoadingScreen.LoadingCompleteTextSettings.Appearance.Font = FSlateFontInfo(RobotoFontObj.Object, 24, FName("Normal"));
		StartupLoadingScreen.PSOPrecacheProgressWidget.Appearance.Font = FSlateFontInfo(RobotoFontObj.Object, 18, FName("Normal"));
		DefaultLoadingScreen.PSOPrecacheProgressWidget.Appearance.Font = FSlateFontInfo(RobotoFontObj.Object, 18, FName("Normal"));
	}
}

#undef LOCTEXT_NAMESPACE


