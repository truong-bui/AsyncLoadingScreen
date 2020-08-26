/**********************************************************************************
 *
 * Copyright (C) 2020 Truong Bui.
 * Website:	https://github.com/truong-bui/AsyncLoadingScreen
 * Licensed under the MIT License. See 'LICENSE' file for full license information.
 *
 **********************************************************************************/

#include "SCentralityTheme.h"
#include "LoadingScreenSettings.h"
#include "SHorizontalLoadingWidget.h"
#include "SVerticalLoadingWidget.h"
#include "SBackgroundWidget.h"
#include "STipWidget.h"

void SCentralityTheme::Construct(const FArguments& InArgs, const FALoadingScreenSettings& Settings, const FCentralityThemeSettings& ThemeSettings)
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

	// Add loading widget at center
	Root->AddSlot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			LoadingWidget
		];


	if (ThemeSettings.bTipAtBottom)
	{
		// Add tip widget at bottom
		Root->AddSlot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Bottom)
			.Padding(0, 0, 0, ThemeSettings.Padding)
			[
				SNew(SBorder)
				.HAlign(ThemeSettings.TipAlignment.HorizontalAlignment)
				.VAlign(ThemeSettings.TipAlignment.VerticalAlignment)
				.Padding(ThemeSettings.TipPadding)
				.BorderImage(&ThemeSettings.TipBackground)
				.BorderBackgroundColor(FLinearColor::White)
				[
					SNew(STipWidget, Settings.Tip)
				]
			];
	}
	else
	{
		// Add tip widget at top
		Root->AddSlot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Top)
			.Padding(0, ThemeSettings.Padding, 0, 0)
			[
				SNew(SBorder)
				.HAlign(ThemeSettings.TipAlignment.HorizontalAlignment)
				.VAlign(ThemeSettings.TipAlignment.VerticalAlignment)
				.Padding(ThemeSettings.TipPadding)
				.BorderImage(&ThemeSettings.TipBackground)
				.BorderBackgroundColor(FLinearColor::White)
				[
					SNew(STipWidget, Settings.Tip)
				]
			];
	}

	// Add root to this widget
	ChildSlot
	[
		Root
	];
}
