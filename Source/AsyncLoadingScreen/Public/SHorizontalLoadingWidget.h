/**********************************************************************************
 *
 * Copyright (C) 2020 Truong Bui.
 * Website:	https://github.com/truong-bui/AsyncLoadingScreen
 * Licensed under the MIT License. See 'LICENSE' file for full license information.
 *
 **********************************************************************************/

#pragma once

#include "Widgets/SCompoundWidget.h"

struct FLoadingWidgetSettings;

/**
 * 
 */
class SHorizontalLoadingWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SClassicLoadingTheme) {}

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, const FLoadingWidgetSettings& Settings);
private:

	/** Active timer to animate the image sequence */
	EActiveTimerReturnType AnimatingImageSequence(double InCurrentTime, float InDeltaTime);

	/** Gets the combined value of the animation properties as a single SThrobber::EAnimation value. */
	SThrobber::EAnimation GetThrobberAnimation(FThrobberSettings ThrobberSettings) const;

	// Placeholder widgets
	TSharedRef<SWidget> LoadingIcon = SNullWidget::NullWidget;
	TArray<TSharedPtr<FDeferredCleanupSlateBrush>> CleanupBrushList;
	int32 ImageIndex = 0;
private:
	bool bIsActiveTimerRegistered = false;
};
