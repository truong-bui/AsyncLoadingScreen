/************************************************************************************
 *																					*
 * Copyright (C) 2020 Truong Bui.													*
 * Website:	https://github.com/truong-bui/AsyncLoadingScreen						*
 * Licensed under the MIT License. See 'LICENSE' file for full license information. *
 *																					*
 ************************************************************************************/

#include "SSidebarLayout.h"
#include "LoadingScreenSettings.h"
#include "Widgets/SOverlay.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SSafeZone.h"
#include "Widgets/Layout/SDPIScaler.h"
#include "Widgets/Layout/SSpacer.h"
#include "Widgets/SBoxPanel.h"
#include "SBackgroundWidget.h"
#include "STipWidget.h"

void SSidebarLayout::Construct(const FArguments& InArgs, const FALoadingScreenSettings& Settings, const FSidebarLayoutSettings& LayoutSettings)
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

	TSharedRef<SVerticalBox> VerticalBox = SNew(SVerticalBox);
	if (LayoutSettings.bIsLoadingWidgetAtTop)
	{
		// Add loading widget at top
		VerticalBox.Get().AddSlot()
			.AutoHeight()
			.HAlign(LayoutSettings.LoadingWidgetAlignment.HorizontalAlignment)
			.VAlign(LayoutSettings.LoadingWidgetAlignment.VerticalAlignment)
			[
				LoadingWidget
			];

		// Add SSpacer at middle
		VerticalBox.Get().AddSlot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.AutoHeight()
			[
				SNew(SSpacer)
				.Size(FVector2D(0.0f, LayoutSettings.Space))
			];

		// Add tip widget at bottom
		VerticalBox.Get().AddSlot()
			.AutoHeight()
			.HAlign(LayoutSettings.TipAlignment.HorizontalAlignment)
			.VAlign(LayoutSettings.TipAlignment.VerticalAlignment)
			[
				TipWidget
			];
	}
	else
	{
		// Add tip widget at top
		VerticalBox.Get().AddSlot()
			.AutoHeight()
			.HAlign(LayoutSettings.TipAlignment.HorizontalAlignment)
			.VAlign(LayoutSettings.TipAlignment.VerticalAlignment)
			[
				TipWidget
			];

		// Add SSpacer at middle
		VerticalBox.Get().AddSlot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.AutoHeight()
			[
				SNew(SSpacer)
				.Size(FVector2D(0.0f, LayoutSettings.Space))
			];

		// Add loading widget at bottom
		VerticalBox.Get().AddSlot()
			.AutoHeight()
			.HAlign(LayoutSettings.LoadingWidgetAlignment.HorizontalAlignment)
			.VAlign(LayoutSettings.LoadingWidgetAlignment.VerticalAlignment)
			[
				LoadingWidget
			];
	}

	// The sidebar border is anchored to the right or left edge of the screen
	const EHorizontalAlignment SidebarHorizontalAlignment = LayoutSettings.bIsWidgetAtRight ? HAlign_Right : HAlign_Left;
	const FMargin SidebarOffset = LayoutSettings.bIsWidgetAtRight
		? FMargin(0, 0, LayoutSettings.BorderHorizontalOffset, 0)
		: FMargin(LayoutSettings.BorderHorizontalOffset, 0, 0, 0);

	Root.Get().AddSlot()
	.HAlign(SidebarHorizontalAlignment)
	.VAlign(LayoutSettings.BorderVerticalAlignment)
	.Padding(SidebarOffset)
	[
		SNew(SBorder)
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.BorderImage(&LayoutSettings.BorderBackground)
		.BorderBackgroundColor(FLinearColor::White)
		[
			SNew(SSafeZone)
			.HAlign(HAlign_Fill)
			.VAlign(LayoutSettings.VerticalAlignment)
			.IsTitleSafe(true)
			.Padding(LayoutSettings.BorderPadding)
			[
				SNew(SDPIScaler)
				.DPIScale(this, &SSidebarLayout::GetDPIScale)
				[
					VerticalBox
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
