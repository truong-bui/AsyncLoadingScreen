/**********************************************************************************
 *
 * Copyright (C) 2020 Truong Bui.
 * Website:	https://github.com/truong-bui/AsyncLoadingScreen
 * Licensed under the MIT License. See 'LICENSE' file for full license information.
 *
 **********************************************************************************/

#include "SClassicTheme.h"
#include "LoadingScreenSettings.h"
#include "Widgets/Layout/SSafeZone.h"
#include "Widgets/Layout/SDPIScaler.h"
#include "Widgets/Layout/SSpacer.h"
#include "SHorizontalLoadingWidget.h"
#include "SVerticalLoadingWidget.h"
#include "SBackgroundWidget.h"
#include "STipWidget.h"

void SClassicTheme::Construct(const FArguments& InArgs, const FALoadingScreenSettings& Settings, const FClassicThemeSettings& ThemeSettings)
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

	TSharedRef<SHorizontalBox> HorizontalBox = SNew(SHorizontalBox);

	if (ThemeSettings.bLoadingWidgetLeft)
	{
		// Add Loading widget on left first
		HorizontalBox.Get().AddSlot()
			.VAlign(VAlign_Center)
			.HAlign(HAlign_Center)
			.AutoWidth()
			[
				LoadingWidget
			];

		// Add spacer at midder
		HorizontalBox.Get().AddSlot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.AutoWidth()
			[
				SNew(SSpacer)
				.Size(FVector2D(ThemeSettings.Space, 0.0f))
			];

			// Tip Text on the right
		HorizontalBox.Get().AddSlot()
			.FillWidth(1.0f)
			.HAlign(ThemeSettings.TipAlignment.HorizontalAlignment)
			.VAlign(ThemeSettings.TipAlignment.VerticalAlignment)
			[
				SNew(STipWidget, Settings.Tip)
			];
	}
	else
	{
		// Tip Text on the left
		HorizontalBox.Get().AddSlot()
			.FillWidth(1.0f)
			.HAlign(ThemeSettings.TipAlignment.HorizontalAlignment)
			.VAlign(ThemeSettings.TipAlignment.VerticalAlignment)
			[
				// Add tip text
				SNew(STipWidget, Settings.Tip)
			];

		// Add spacer at midder
		HorizontalBox.Get().AddSlot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.AutoWidth()
			[
				SNew(SSpacer)
				.Size(FVector2D(ThemeSettings.Space, 0.0f))
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


	EVerticalAlignment VerticalAlignment;
	// Set vertical alignment for widget
	if (ThemeSettings.bWidgetAtBottom)
	{
		VerticalAlignment = EVerticalAlignment::VAlign_Bottom;
	}
	else
	{
		VerticalAlignment = EVerticalAlignment::VAlign_Top;
	}

	// Creating loading theme
	Root->AddSlot()
	.HAlign(HAlign_Fill)
	.VAlign(VerticalAlignment)		
	[
		SNew(SBorder)
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.BorderImage(&ThemeSettings.WidgetBackground)
		.BorderBackgroundColor(FLinearColor::White)
		[
			SNew(SSafeZone)
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.IsTitleSafe(true)
			.Padding(ThemeSettings.WidgetPadding)
			[
				SNew(SDPIScaler)
				.DPIScale(this, &SClassicTheme::GetDPIScale)		
				[					
					HorizontalBox
				]
			]
		]
	];

	// Add root to this widget
	ChildSlot
		[
			Root
		];
}
