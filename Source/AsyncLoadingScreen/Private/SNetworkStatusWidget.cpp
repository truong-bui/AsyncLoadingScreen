/************************************************************************************
 *																					*
 * Copyright (C) 2020 Truong Bui.													*
 * Website:	https://github.com/truong-bui/AsyncLoadingScreen						*
 * Licensed under the MIT License. See 'LICENSE' file for full license information. *
 *																					*
 ************************************************************************************/

#include "SNetworkStatusWidget.h"
#include "AsyncLoadingScreen.h"
#include "LoadingScreenSettings.h"
#include "Widgets/Text/STextBlock.h"

void SNetworkStatusWidget::Construct(const FArguments& InArgs, const FNetworkStatusSettings& Settings)
{
	ChildSlot
		[
			SNew(STextBlock)
				.Text(Settings.StatusText)
				.ColorAndOpacity(Settings.Appearance.ColorAndOpacity)
				.Font(Settings.Appearance.Font)
				.ShadowOffset(Settings.Appearance.ShadowOffset)
				.ShadowColorAndOpacity(Settings.Appearance.ShadowColorAndOpacity)
				.Justification(Settings.Appearance.Justification)
		];

	// The attribute is also evaluated while collapsed, unlike OnPaint, so the widget can show up once the wait starts
	SetVisibility(TAttribute<EVisibility>::CreateSP(this, &SNetworkStatusWidget::GetStatusVisibility));
}

EVisibility SNetworkStatusWidget::GetStatusVisibility() const
{
	// Written by the module on the game thread, read here on the Slate loading thread
	return AsyncLoadingScreen::IsWaitingForNetwork() ? EVisibility::SelfHitTestInvisible : EVisibility::Collapsed;
}
