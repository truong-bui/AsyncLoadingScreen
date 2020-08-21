#include "LoadingScreenSettings.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Font.h"

#define LOCTEXT_NAMESPACE "AsyncLoadingScreen"

FLoadingWidgetSettings::FLoadingWidgetSettings() : LoadingText(LOCTEXT("Loading", "LOADING")) {}

ULoadingScreenSettings::ULoadingScreenSettings(const FObjectInitializer& Initializer) : Super(Initializer)
{
	StartupLoadingScreen.Tips.TipWrapAt = 1000.0f;
	StartupLoadingScreen.bShowWidgetOverlay = false;
	DefaultLoadingScreen.Tips.TipWrapAt = 1000.0f;
	// Set default font
	if (!IsRunningDedicatedServer())
	{
		static ConstructorHelpers::FObjectFinder<UFont> RobotoFontObj(TEXT("/Engine/EngineFonts/Roboto"));
		StartupLoadingScreen.Tips.Font = FSlateFontInfo(RobotoFontObj.Object, 20, FName("Normal"));
		DefaultLoadingScreen.Tips.Font = FSlateFontInfo(RobotoFontObj.Object, 20, FName("Normal"));
		StartupLoadingScreen.LoadingWidget.Font = FSlateFontInfo(RobotoFontObj.Object, 32, FName("Bold"));
		DefaultLoadingScreen.LoadingWidget.Font = FSlateFontInfo(RobotoFontObj.Object, 32, FName("Bold"));		
	}
}

#undef LOCTEXT_NAMESPACE