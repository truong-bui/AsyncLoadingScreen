/************************************************************************************
 *																					*
 * Copyright (C) 2020 Truong Bui.													*
 * Website:	https://github.com/truong-bui/AsyncLoadingScreen						*
 * Licensed under the MIT License. See 'LICENSE' file for full license information. *
 *																					*
 ************************************************************************************/


#include "SDualSidebarLayout.h"
#include "LoadingScreenSettings.h"
#include "Widgets/SOverlay.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SSafeZone.h"
#include "Widgets/Layout/SDPIScaler.h"
#include "SBackgroundWidget.h"
#include "STipWidget.h"

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

	TSharedRef<SWidget> LoadingWidget = MakeLoadingWidget(Settings.LoadingWidget);
	TSharedRef<SWidget> TipWidget = SNew(STipWidget, Settings.TipWidget);

	// The loading widget goes in the right or left border depending on settings; the tip widget takes the other one
	TSharedRef<SWidget> RightContent = LayoutSettings.bIsLoadingWidgetAtRight ? LoadingWidget : TipWidget;
	TSharedRef<SWidget> LeftContent = LayoutSettings.bIsLoadingWidgetAtRight ? TipWidget : LoadingWidget;

	// Add right border widget
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
					RightContent
				]
			]
		]
	];

	// Add left border widget
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
					LeftContent
				]
			]
		]
	];

	// Construct loading complete text if enable
	AddLoadingCompleteTextSlot(Root, Settings);

	// Add root to this widget
	ChildSlot
	[
		Root
	];
}
