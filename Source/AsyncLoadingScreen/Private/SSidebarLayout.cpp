/************************************************************************************
 *																					*
 * Copyright (C) 2020 Truong Bui.													*
 * Website:	https://github.com/truong-bui/AsyncLoadingScreen						*
 * Licensed under the MIT License. See 'LICENSE' file for full license information. *
 *																					*
 ************************************************************************************/

#include "SSidebarLayout.h"
#include "LoadingScreenSettings.h"
#include "Widgets/Layout/SSafeZone.h"
#include "Widgets/Layout/SDPIScaler.h"
#include "Widgets/Layout/SSpacer.h"
#include "SHorizontalLoadingWidget.h"
#include "SVerticalLoadingWidget.h"
#include "SBackgroundWidget.h"
#include "STipWidget.h"
#include "SLoadingCompleteText.h"
#include "Widgets/SBoxPanel.h"

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
				SNew(STipWidget, Settings.TipWidget)
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
				SNew(STipWidget, Settings.TipWidget)
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


 
	if (LayoutSettings.bIsWidgetAtRight)
	{
		// Add widget at right
		Root.Get().AddSlot()
		.HAlign(HAlign_Right)
		.VAlign(LayoutSettings.BorderVerticalAlignment)
		.Padding(0, 0, LayoutSettings.BorderHorizontalOffset, 0)
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
	}
	else
	{
		// Add widget at left
		Root.Get().AddSlot()
		.HAlign(HAlign_Left)
		.VAlign(LayoutSettings.BorderVerticalAlignment)
		.Padding(LayoutSettings.BorderHorizontalOffset, 0, 0, 0)
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
