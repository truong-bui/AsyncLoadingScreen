/************************************************************************************
 *																					*
 * Copyright (C) 2020 Truong Bui.													*
 * Website:	https://github.com/truong-bui/AsyncLoadingScreen						*
 * Licensed under the MIT License. See 'LICENSE' file for full license information. *
 *																					*
 ************************************************************************************/

#include "SLetterboxLayout.h"
#include "LoadingScreenSettings.h"
#include "Widgets/SOverlay.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SSafeZone.h"
#include "Widgets/Layout/SDPIScaler.h"
#include "SBackgroundWidget.h"
#include "STipWidget.h"

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

	TSharedRef<SWidget> LoadingWidget = MakeLoadingWidget(Settings.LoadingWidget);
	TSharedRef<SWidget> TipWidget = SNew(STipWidget, Settings.TipWidget);

	// The loading widget goes in the top or bottom border depending on settings; the tip widget takes the other one
	const bool bIsLoadingWidgetAtTop = LayoutSettings.bIsLoadingWidgetAtTop;
	TSharedRef<SWidget> TopContent = bIsLoadingWidgetAtTop ? LoadingWidget : TipWidget;
	TSharedRef<SWidget> BottomContent = bIsLoadingWidgetAtTop ? TipWidget : LoadingWidget;
	const FWidgetAlignment& TopContentAlignment = bIsLoadingWidgetAtTop ? LayoutSettings.LoadingWidgetAlignment : LayoutSettings.TipAlignment;
	const FWidgetAlignment& BottomContentAlignment = bIsLoadingWidgetAtTop ? LayoutSettings.TipAlignment : LayoutSettings.LoadingWidgetAlignment;

	// Add a border widget at top
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
				.HAlign(TopContentAlignment.HorizontalAlignment)
				.VAlign(TopContentAlignment.VerticalAlignment)
				.IsTitleSafe(true)
				.Padding(LayoutSettings.TopBorderPadding)
				[
					SNew(SDPIScaler)
					.DPIScale(this, &SLetterboxLayout::GetDPIScale)
					[
						TopContent
					]
				]
			]
		];

	// Add a border widget at bottom
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
				.HAlign(BottomContentAlignment.HorizontalAlignment)
				.VAlign(BottomContentAlignment.VerticalAlignment)
				.IsTitleSafe(true)
				.Padding(LayoutSettings.BottomBorderPadding)
				[
					SNew(SDPIScaler)
					.DPIScale(this, &SLetterboxLayout::GetDPIScale)
					[
						BottomContent
					]
				]
			]
		];

	// Construct loading complete text if enable
	AddLoadingCompleteTextSlot(Root, Settings);

	// Add Root to this widget
	ChildSlot
		[
			Root
		];
}
