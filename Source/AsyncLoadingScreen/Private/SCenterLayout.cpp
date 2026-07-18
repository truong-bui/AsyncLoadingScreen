/************************************************************************************
 *																					*
 * Copyright (C) 2020 Truong Bui.													*
 * Website:	https://github.com/truong-bui/AsyncLoadingScreen						*
 * Licensed under the MIT License. See 'LICENSE' file for full license information. *
 *																					*
 ************************************************************************************/

#include "SCenterLayout.h"
#include "LoadingScreenSettings.h"
#include "Widgets/SOverlay.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SSafeZone.h"
#include "Widgets/Layout/SDPIScaler.h"
#include "SBackgroundWidget.h"
#include "STipWidget.h"

void SCenterLayout::Construct(const FArguments& InArgs, const FALoadingScreenSettings& Settings, const FCenterLayoutSettings& LayoutSettings)
{
	// Root widget and background
	TSharedRef<SOverlay> Root = SNew(SOverlay)
		+ SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SNew(SBackgroundWidget, Settings.Background)
		];

	// Add loading widget at center
	Root->AddSlot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			MakeLoadingWidget(Settings.LoadingWidget)
		];

	// The tip border is anchored to the bottom or top edge of the screen
	const EVerticalAlignment TipBorderVerticalAlignment = LayoutSettings.bIsTipAtBottom ? VAlign_Bottom : VAlign_Top;
	const FMargin TipBorderOffset = LayoutSettings.bIsTipAtBottom
		? FMargin(0, 0, 0, LayoutSettings.BorderVerticalOffset)
		: FMargin(0, LayoutSettings.BorderVerticalOffset, 0, 0);

	Root->AddSlot()
		.HAlign(LayoutSettings.BorderHorizontalAlignment)
		.VAlign(TipBorderVerticalAlignment)
		.Padding(TipBorderOffset)
		[
			SNew(SBorder)
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.BorderImage(&LayoutSettings.BorderBackground)
			.BorderBackgroundColor(FLinearColor::White)
			[
				SNew(SSafeZone)
				.HAlign(LayoutSettings.TipAlignment.HorizontalAlignment)
				.VAlign(LayoutSettings.TipAlignment.VerticalAlignment)
				.IsTitleSafe(true)
				.Padding(LayoutSettings.BorderPadding)
				[
					SNew(SDPIScaler)
					.DPIScale(this, &SCenterLayout::GetDPIScale)
					[
						SNew(STipWidget, Settings.TipWidget)
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
