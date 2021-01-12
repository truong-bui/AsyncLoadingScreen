/************************************************************************************
 *																					*
 * Copyright (C) 2020 Truong Bui.													*
 * Website:	https://github.com/truong-bui/AsyncLoadingScreen						*
 * Licensed under the MIT License. See 'LICENSE' file for full license information. *
 *																					*
 ************************************************************************************/


#include "SDualSidebarLayout.h"
#include "LoadingScreenSettings.h"
#include "Widgets/Layout/SSafeZone.h"
#include "Widgets/Layout/SDPIScaler.h"
#include "Widgets/Layout/SSpacer.h"
#include "SHorizontalLoadingWidget.h"
#include "SVerticalLoadingWidget.h"
#include "SBackgroundWidget.h"
#include "STipWidget.h"
#include "SLoadingCompleteText.h"

void SDualSidebarLayout::Construct(const FArguments& InArgs, const FALoadingScreenSettings& Settings, const FDualSidebarLayoutSettings& LayoutSettings)
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



	if (LayoutSettings.bIsLoadingWidgetAtRight)
	{
		// Add loading widget at right
		Root.Get().AddSlot()
		.HAlign(HAlign_Right)
		.VAlign(LayoutSettings.RightBorderVerticalAlignment)		
		[
			SNew(SBorder)
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.BorderImage(&LayoutSettings.RightBorderBackground)
			.BorderBackgroundColor(FLinearColor::White)
			[
				SNew(SSafeZone)
				.HAlign(HAlign_Fill)
				.VAlign(LayoutSettings.RightVerticalAlignment)
				.IsTitleSafe(true)
				.Padding(LayoutSettings.RightBorderPadding)
				[
					SNew(SDPIScaler)
					.DPIScale(this, &SDualSidebarLayout::GetDPIScale)
					[
						LoadingWidget
					]
				]
			]
		];

		// Add tip widget at left
		Root.Get().AddSlot()
		.HAlign(HAlign_Left)
		.VAlign(LayoutSettings.LeftBorderVerticalAlignment)		
		[
			SNew(SBorder)
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.BorderImage(&LayoutSettings.LeftBorderBackground)
			.BorderBackgroundColor(FLinearColor::White)
			[
				SNew(SSafeZone)
				.HAlign(HAlign_Fill)
				.VAlign(LayoutSettings.LeftVerticalAlignment)
				.IsTitleSafe(true)
				.Padding(LayoutSettings.LeftBorderPadding)
				[
					SNew(SDPIScaler)
					.DPIScale(this, &SDualSidebarLayout::GetDPIScale)
					[
						SNew(STipWidget, Settings.TipWidget)
					]
				]
			]
		];
	}

	else
	{
		// Add Tip widget at right
		Root.Get().AddSlot()
		.HAlign(HAlign_Right)
		.VAlign(LayoutSettings.RightBorderVerticalAlignment)		
		[
			SNew(SBorder)
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.BorderImage(&LayoutSettings.RightBorderBackground)
			.BorderBackgroundColor(FLinearColor::White)
			[
				SNew(SSafeZone)
				.HAlign(HAlign_Fill)
				.VAlign(LayoutSettings.RightVerticalAlignment)
				.IsTitleSafe(true)
				.Padding(LayoutSettings.RightBorderPadding)
				[
					SNew(SDPIScaler)
					.DPIScale(this, &SDualSidebarLayout::GetDPIScale)
					[
						SNew(STipWidget, Settings.TipWidget)						
					]
				]
			]
		];

		// Add Loading widget at left
		Root.Get().AddSlot()
		.HAlign(HAlign_Left)
		.VAlign(LayoutSettings.LeftBorderVerticalAlignment)		
		[
			SNew(SBorder)
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.BorderImage(&LayoutSettings.LeftBorderBackground)
			.BorderBackgroundColor(FLinearColor::White)
			[
				SNew(SSafeZone)
				.HAlign(HAlign_Fill)
				.VAlign(LayoutSettings.LeftVerticalAlignment)
				.IsTitleSafe(true)
				.Padding(LayoutSettings.LeftBorderPadding)
				[
					SNew(SDPIScaler)
					.DPIScale(this, &SDualSidebarLayout::GetDPIScale)
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

	// Add root to this widget
	ChildSlot
	[
		Root
	];
}
