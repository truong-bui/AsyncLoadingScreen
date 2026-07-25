/************************************************************************************
 *																					*
 * Copyright (C) 2020 Truong Bui.													*
 * Website:	https://github.com/truong-bui/AsyncLoadingScreen						*
 * Licensed under the MIT License. See 'LICENSE' file for full license information. *
 *																					*
 ************************************************************************************/

#include "SVerticalLoadingWidget.h"
#include "LoadingScreenSettings.h"
#include "Widgets/Layout/SSpacer.h"
#include "Widgets/SBoxPanel.h"

void SVerticalLoadingWidget::Construct(const FArguments& InArgs, const FLoadingWidgetSettings& Settings)
{
	bPlayReverse = Settings.ImageSequenceSettings.bPlayReverse;

	// Root is a Vertical Box
	TSharedRef<SVerticalBox> Root = SNew(SVerticalBox);

	// Construct Loading Icon Widget
	ConstructLoadingIcon(Settings);

	TSharedRef<SWidget> LoadingText = MakeLoadingTextWidget(Settings);
	TSharedRef<SWidget> Spacer = SNew(SSpacer)
		.Size(FVector2D(0.0f, Settings.Space));

	// If loading text is on the top
	if (Settings.bLoadingTextTopPosition)
	{
		// Add Loading Text on the top first
		Root.Get().AddSlot()
			.HAlign(Settings.TextAlignment.HorizontalAlignment)
			.VAlign(Settings.TextAlignment.VerticalAlignment)
			.AutoHeight()
			[
				LoadingText
			];

		// Add a Spacer in middle
		Root.Get().AddSlot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.AutoHeight()
			[
				Spacer
			];

		// Add Loading Icon at the bottom finally
		Root.Get().AddSlot()
			.HAlign(Settings.LoadingIconAlignment.HorizontalAlignment)
			.VAlign(Settings.LoadingIconAlignment.VerticalAlignment)
			.AutoHeight()
			[
				LoadingIcon
			];
	}

	// If loading text is at the bottom
	else
	{
		// Add Loading Icon on the top
		Root.Get().AddSlot()
			.HAlign(Settings.LoadingIconAlignment.HorizontalAlignment)
			.VAlign(Settings.LoadingIconAlignment.VerticalAlignment)
			.AutoHeight()
			[
				LoadingIcon
			];

		// Add a Spacer in middle
		Root.Get().AddSlot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.AutoHeight()
			[
				Spacer
			];

		// Add Loading Text at the bottom
		Root.Get().AddSlot()
			.HAlign(Settings.TextAlignment.HorizontalAlignment)
			.VAlign(Settings.TextAlignment.VerticalAlignment)
			.AutoHeight()
			[
				LoadingText
			];
	}

	// Add root to this widget
	ChildSlot
	[
		Root
	];
}
