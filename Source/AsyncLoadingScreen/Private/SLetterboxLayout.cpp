/************************************************************************************
 *																					*
 * Copyright (C) 2020 Truong Bui.													*
 * Website:	https://github.com/truong-bui/AsyncLoadingScreen						*
 * Licensed under the MIT License. See 'LICENSE' file for full license information. *
 *																					*
 ************************************************************************************/

#include "SLetterboxLayout.h"
#include "LoadingScreenSettings.h"
#include "Widgets/Layout/SSafeZone.h"
#include "Widgets/Layout/SDPIScaler.h"
#include "SHorizontalLoadingWidget.h"
#include "SVerticalLoadingWidget.h"
#include "SBackgroundWidget.h"
#include "STipWidget.h"
#include "SLoadingCompleteText.h"

void SLetterboxLayout::Construct(const FArguments& InArgs, const FALoadingScreenSettings& Settings, const FLetterboxLayoutSettings& LayoutSettings)
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
	

	if (LayoutSettings.bIsLoadingWidgetAtTop)
	{
		// Add a border widget at top, then add Loading widget
		Root->AddSlot()
			.HAlign(LayoutSettings.TopBorderHorizontalAlignment)
			.VAlign(VAlign_Top)	
			[
				SNew(SBorder)
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)		
				.BorderImage(&LayoutSettings.TopBorderBackground)
				.BorderBackgroundColor(FLinearColor::White)
				[
					SNew(SSafeZone)
					.HAlign(LayoutSettings.LoadingWidgetAlignment.HorizontalAlignment)
					.VAlign(LayoutSettings.LoadingWidgetAlignment.VerticalAlignment)
					.IsTitleSafe(true)
					.Padding(LayoutSettings.TopBorderPadding)
					[
						SNew(SDPIScaler)
						.DPIScale(this, &SLetterboxLayout::GetDPIScale)
						[
							LoadingWidget
						]
					]
				]
			];

		// Add a border widget at bottom, then add Tip widget
		Root->AddSlot()
			.HAlign(LayoutSettings.BottomBorderHorizontalAlignment)
			.VAlign(VAlign_Bottom)	
			[
				SNew(SBorder)
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				.BorderImage(&LayoutSettings.BottomBorderBackground)
				.BorderBackgroundColor(FLinearColor::White)
				[
					SNew(SSafeZone)
					.HAlign(LayoutSettings.TipAlignment.HorizontalAlignment)
					.VAlign(LayoutSettings.TipAlignment.VerticalAlignment)
					.IsTitleSafe(true)
					.Padding(LayoutSettings.BottomBorderPadding)
					[
						SNew(SDPIScaler)
						.DPIScale(this, &SLetterboxLayout::GetDPIScale)
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
			.HAlign(LayoutSettings.TopBorderHorizontalAlignment)
			.VAlign(VAlign_Top)	
			[
				SNew(SBorder)
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				.BorderImage(&LayoutSettings.TopBorderBackground)
				.BorderBackgroundColor(FLinearColor::White)
				[
					SNew(SSafeZone)
					.HAlign(LayoutSettings.TipAlignment.HorizontalAlignment)
					.VAlign(LayoutSettings.TipAlignment.VerticalAlignment)
					.IsTitleSafe(true)
					.Padding(LayoutSettings.TopBorderPadding)
					[
						SNew(SDPIScaler)
						.DPIScale(this, &SLetterboxLayout::GetDPIScale)
						[					
							SNew(STipWidget, Settings.TipWidget)
						]						
					]
				]
			];

		// Add a border widget at bottom, then add Loading widget
		Root->AddSlot()
			.HAlign(LayoutSettings.BottomBorderHorizontalAlignment)
			.VAlign(VAlign_Bottom)	
			[
				SNew(SBorder)
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				.BorderImage(&LayoutSettings.BottomBorderBackground)
				.BorderBackgroundColor(FLinearColor::White)
				[
					SNew(SSafeZone)
					.HAlign(LayoutSettings.LoadingWidgetAlignment.HorizontalAlignment)
					.VAlign(LayoutSettings.LoadingWidgetAlignment.VerticalAlignment)
					.IsTitleSafe(true)
					.Padding(LayoutSettings.BottomBorderPadding)
					[
						SNew(SDPIScaler)
						.DPIScale(this, &SLetterboxLayout::GetDPIScale)
						[
							LoadingWidget
						]						
					]
				]
			];
	}

	// Construct loading complete text if enable
	if (Settings.bShowLoadingCompleteText)
	{
		Root->AddSlot()
			.VAlign(Settings.LoadingCompleteTextSettings.Alignment.VerticalAlignment)
			.HAlign(Settings.LoadingCompleteTextSettings.Alignment.HorizontalAlignment)
			.Padding(Settings.LoadingCompleteTextSettings.Padding)
			[
				SNew(SLoadingCompleteText, Settings.LoadingCompleteTextSettings)
			];
	}

	// Add Root to this widget
	ChildSlot
		[
			Root
		];
}
