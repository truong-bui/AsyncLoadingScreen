/**********************************************************************************
 *
 * Copyright (C) 2020 Truong Bui.
 * Website:	https://github.com/truong-bui/AsyncLoadingScreen
 * Licensed under the MIT License. See 'LICENSE' file for full license information.
 *
 **********************************************************************************/

#include "SLoadingScreenTheme.h"
#include "Widgets/Layout/SSafeZone.h"
#include "Widgets/Layout/SDPIScaler.h"
#include "Widgets/Layout/SSpacer.h"
#include "SHorizontalLoadingWidget.h"
#include "SVerticalLoadingWidget.h"
#include "SBackgroundWidget.h"
#include "STipWidget.h"

void SLoadingScreenTheme::Construct(const FArguments& InArgs, const FALoadingScreenSettings& Settings)
{
	const ULoadingScreenSettings* LoadingScreenSettings = GetDefault<ULoadingScreenSettings>();
	
	switch (Settings.Theme)
	{	
	case EAsynceLoadingScreenTheme::ALST_Centrality:
		ConstructCentralityTheme(Settings, LoadingScreenSettings->Centrality);
		break;
	}
}

void SLoadingScreenTheme::ConstructCentralityTheme(const FALoadingScreenSettings& Settings, const FCentralityThemeSettings& ThemeSettings)
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

	ChildSlot
	[
		Root
	];
}

float SLoadingScreenTheme::PointSizeToSlateUnits(float PointSize)
{
	const float SlateFreeTypeHorizontalResolutionDPI = 96.0f;
	const float FreeTypeNativeDPI = 72.0;
	const float PixelSize = PointSize * (SlateFreeTypeHorizontalResolutionDPI / FreeTypeNativeDPI);
	return PixelSize;
}

float SLoadingScreenTheme::GetDPIScale() const
{
	const FVector2D& DrawSize = GetTickSpaceGeometry().ToPaintGeometry().GetLocalSize();
	const FIntPoint Size((int32)DrawSize.X, (int32)DrawSize.Y);

	return GetDefault<UUserInterfaceSettings>()->GetDPIScaleBasedOnSize(Size);
}
