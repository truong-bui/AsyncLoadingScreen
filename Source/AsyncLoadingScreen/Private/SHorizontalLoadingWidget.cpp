/************************************************************************************
 *																					*
 * Copyright (C) 2020 Truong Bui.													*
 * Website:	https://github.com/truong-bui/AsyncLoadingScreen						*
 * Licensed under the MIT License. See 'LICENSE' file for full license information. *
 *																					*
 ************************************************************************************/

#include "SHorizontalLoadingWidget.h"
#include "LoadingScreenSettings.h"
#include "Widgets/Layout/SSpacer.h"
#include "Widgets/SBoxPanel.h"

void SHorizontalLoadingWidget::Construct(const FArguments& InArgs, const FLoadingWidgetSettings& Settings)
{
	bPlayReverse = Settings.ImageSequenceSettings.bPlayReverse;

	// Root is a Horizontal Box of course
	TSharedRef<SHorizontalBox> Root = SNew(SHorizontalBox);

	// Construct Loading Icon Widget
	ConstructLoadingIcon(Settings);

	TSharedRef<SWidget> LoadingText = MakeLoadingTextWidget(Settings);
	TSharedRef<SWidget> Spacer = SNew(SSpacer)
		.Size(FVector2D(Settings.Space, 0.0f));

	// If loading text is on the right
	if (Settings.bLoadingTextRightPosition)
	{
		// Add Loading Icon on the left first
		Root.Get().AddSlot()
			.HAlign(Settings.LoadingIconAlignment.HorizontalAlignment)
			.VAlign(Settings.LoadingIconAlignment.VerticalAlignment)
			.AutoWidth()
			[
				LoadingIcon
			];

		// Add a Spacer in middle
		Root.Get().AddSlot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.AutoWidth()
			[
				Spacer
			];

		// Add Loading Text on the right
		Root.Get().AddSlot()
			.HAlign(Settings.TextAlignment.HorizontalAlignment)
			.VAlign(Settings.TextAlignment.VerticalAlignment)
			.AutoWidth()
			[
				LoadingText
			];
	}

	// If loading text is on the left
	else
	{
		// Add Loading Text on the left first
		Root.Get().AddSlot()
			.HAlign(Settings.TextAlignment.HorizontalAlignment)
			.VAlign(Settings.TextAlignment.VerticalAlignment)
			.AutoWidth()
			[
				LoadingText
			];

		// Add a Spacer in middle
		Root.Get().AddSlot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.AutoWidth()
			[
				Spacer
			];

		// Add Loading Icon on the right finally
		Root.Get().AddSlot()
			.HAlign(Settings.LoadingIconAlignment.HorizontalAlignment)
			.VAlign(Settings.LoadingIconAlignment.VerticalAlignment)
			.AutoWidth()
			[
				LoadingIcon
			];
	}

	// Add root to this widget
	ChildSlot
	[
		Root
	];
}
