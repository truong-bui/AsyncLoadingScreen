/************************************************************************************
 *																					*
 * Copyright (C) 2020 Truong Bui.													*
 * Website:	https://github.com/truong-bui/AsyncLoadingScreen						*
 * Licensed under the MIT License. See 'LICENSE' file for full license information. *
 *																					*
 ************************************************************************************/

#include "SClassicLayout.h"
#include "LoadingScreenSettings.h"
#include "Widgets/SOverlay.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SSafeZone.h"
#include "Widgets/Layout/SDPIScaler.h"
#include "Widgets/Layout/SSpacer.h"
#include "Widgets/SBoxPanel.h"
#include "SBackgroundWidget.h"
#include "STipWidget.h"

void SClassicLayout::Construct(const FArguments& InArgs, const FALoadingScreenSettings& Settings, const FClassicLayoutSettings& LayoutSettings)
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

	TSharedRef<SHorizontalBox> HorizontalBox = SNew(SHorizontalBox);

	if (LayoutSettings.bIsLoadingWidgetAtLeft)
	{
		// Add Loading widget on left first
		HorizontalBox.Get().AddSlot()
			.VAlign(VAlign_Center)
			.HAlign(HAlign_Center)
			.AutoWidth()
			[
				LoadingWidget
			];

		// Add spacer in middle
		HorizontalBox.Get().AddSlot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.AutoWidth()
			[
				SNew(SSpacer)
				.Size(FVector2D(LayoutSettings.Space, 0.0f))
			];

		// Tip Text on the right
		HorizontalBox.Get().AddSlot()
			.FillWidth(1.0f)
			.HAlign(LayoutSettings.TipAlignment.HorizontalAlignment)
			.VAlign(LayoutSettings.TipAlignment.VerticalAlignment)
			[
				TipWidget
			];
	}
	else
	{
		// Tip Text on the left
		HorizontalBox.Get().AddSlot()
			.FillWidth(1.0f)
			.HAlign(LayoutSettings.TipAlignment.HorizontalAlignment)
			.VAlign(LayoutSettings.TipAlignment.VerticalAlignment)
			[
				TipWidget
			];

		// Add spacer in middle
		HorizontalBox.Get().AddSlot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.AutoWidth()
			[
				SNew(SSpacer)
				.Size(FVector2D(LayoutSettings.Space, 0.0f))
			];

		// Add Loading widget on right
		HorizontalBox.Get().AddSlot()
			.VAlign(VAlign_Center)
			.HAlign(HAlign_Center)
			.AutoWidth()
			[
				LoadingWidget
			];
	}

	// The border that contains the widgets is anchored to the bottom or top edge of the screen
	const EVerticalAlignment BorderVerticalAlignment = LayoutSettings.bIsWidgetAtBottom ? VAlign_Bottom : VAlign_Top;

	// Creating loading theme
	Root->AddSlot()
	.HAlign(LayoutSettings.BorderHorizontalAlignment)
	.VAlign(BorderVerticalAlignment)
	[
		SNew(SBorder)
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.BorderImage(&LayoutSettings.BorderBackground)
		.BorderBackgroundColor(FLinearColor::White)
		[
			SNew(SSafeZone)
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.IsTitleSafe(true)
			.Padding(LayoutSettings.BorderPadding)
			[
				SNew(SDPIScaler)
				.DPIScale(this, &SClassicLayout::GetDPIScale)
				[
					HorizontalBox
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
