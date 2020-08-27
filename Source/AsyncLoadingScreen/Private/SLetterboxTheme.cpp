/************************************************************************************
 *																					*
 * Copyright (C) 2020 Truong Bui.													*
 * Website:	https://github.com/truong-bui/AsyncLoadingScreen						*
 * Licensed under the MIT License. See 'LICENSE' file for full license information. *
 *																					*
 ************************************************************************************/

#include "SLetterboxTheme.h"
#include "LoadingScreenSettings.h"
#include "Widgets/Layout/SSafeZone.h"
#include "Widgets/Layout/SDPIScaler.h"
#include "SHorizontalLoadingWidget.h"
#include "SVerticalLoadingWidget.h"
#include "SBackgroundWidget.h"
#include "STipWidget.h"

void SLetterboxTheme::Construct(const FArguments& InArgs, const FALoadingScreenSettings& Settings, const FLetterboxThemeSettings& ThemeSettings)
{
	// Root widget and background
	TSharedRef<SOverlay> Root = SNew(SOverlay)
		+ SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SNew(SBackgroundWidget, Settings.Background)
		];

	// Placeholder for loading widget
	TSharedRef<SWidget> LoadingWidget = SNullWidget::NullWidget;
	if (Settings.LoadingWidget.LoadingWidgetType == ELoadingWidgetType::LWT_Horizontal)
	{
		LoadingWidget = SNew(SHorizontalLoadingWidget, Settings.LoadingWidget);
	}
	else
	{
		LoadingWidget = SNew(SVerticalLoadingWidget, Settings.LoadingWidget);
	}
	

	if (ThemeSettings.bIsLoadingWidgetAtTop)
	{
		// Add a border widget at top, then add Loading widget
		Root->AddSlot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Top)	
			[
				SNew(SBorder)
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)		
				.BorderImage(&ThemeSettings.TopBorderBackground)
				.BorderBackgroundColor(FLinearColor::White)
				[
					SNew(SSafeZone)
					.HAlign(ThemeSettings.LoadingWidgetAlignment.HorizontalAlignment)
					.VAlign(ThemeSettings.LoadingWidgetAlignment.VerticalAlignment)
					.IsTitleSafe(true)
					.Padding(ThemeSettings.TopBorderPadding)
					[
						SNew(SDPIScaler)
						.DPIScale(this, &SLetterboxTheme::GetDPIScale)
						[
							LoadingWidget
						]
					]
				]
			];

		// Add a border widget at bottom, then add Tip widget
		Root->AddSlot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Bottom)	
			[
				SNew(SBorder)
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				.BorderImage(&ThemeSettings.BottomBorderBackground)
				.BorderBackgroundColor(FLinearColor::White)
				[
					SNew(SSafeZone)
					.HAlign(ThemeSettings.TipAlignment.HorizontalAlignment)
					.VAlign(ThemeSettings.TipAlignment.VerticalAlignment)
					.IsTitleSafe(true)
					.Padding(ThemeSettings.BottomBorderPadding)
					[
						SNew(SDPIScaler)
						.DPIScale(this, &SLetterboxTheme::GetDPIScale)
						[
							SNew(STipWidget, Settings.TipWidget)
						]						
					]
				]
			];
	}
	else
	{
		// Add a border widget at top, then add Tip widget
		Root->AddSlot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Top)	
			[
				SNew(SBorder)
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				.BorderImage(&ThemeSettings.TopBorderBackground)
				.BorderBackgroundColor(FLinearColor::White)
				[
					SNew(SSafeZone)
					.HAlign(ThemeSettings.TipAlignment.HorizontalAlignment)
					.VAlign(ThemeSettings.TipAlignment.VerticalAlignment)
					.IsTitleSafe(true)
					.Padding(ThemeSettings.TopBorderPadding)
					[
						SNew(SDPIScaler)
						.DPIScale(this, &SLetterboxTheme::GetDPIScale)		
						[					
							SNew(STipWidget, Settings.TipWidget)
						]						
					]
				]
			];

		// Add a border widget at bottom, then add Loading widget
		Root->AddSlot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Bottom)	
			[
				SNew(SBorder)
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				.BorderImage(&ThemeSettings.BottomBorderBackground)
				.BorderBackgroundColor(FLinearColor::White)
				[
					SNew(SSafeZone)
					.HAlign(ThemeSettings.LoadingWidgetAlignment.HorizontalAlignment)
					.VAlign(ThemeSettings.LoadingWidgetAlignment.VerticalAlignment)
					.IsTitleSafe(true)
					.Padding(ThemeSettings.BottomBorderPadding)
					[
						SNew(SDPIScaler)
						.DPIScale(this, &SLetterboxTheme::GetDPIScale)
						[
							LoadingWidget
						]						
					]
				]
			];
	}

	// Add Root to this widget
	ChildSlot
		[
			Root
		];
}
