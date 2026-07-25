/************************************************************************************
 *																					*
 * Copyright (C) 2020 Truong Bui.													*
 * Website:	https://github.com/truong-bui/AsyncLoadingScreen						*
 * Licensed under the MIT License. See 'LICENSE' file for full license information. *
 *																					*
 ************************************************************************************/

#pragma once

#include "Widgets/SCompoundWidget.h"
#include "Styling/SlateTypes.h"

struct FPSOPrecacheProgressSettings;
class SProgressBar;
class STextBlock;

/**
 * PSO precache progress widget: a progress bar with an optional text that displays the
 * outstanding PSO precache compilations. Only visible while there is work remaining.
 */
class SPSOPrecacheProgressWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SPSOPrecacheProgressWidget) {}

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, const FPSOPrecacheProgressSettings& Settings);

	int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

private:
	/** Visibility attribute getter; also evaluated while collapsed so the widget can appear once precache work shows up */
	EVisibility GetProgressVisibility() const;

	// Progress is updated in OnPaint because active timers don't tick on the Slate loading thread during a blocking load
	mutable float TotalDeltaTime = 0.0f;

	// The highest number of remaining precompiles seen so far, used as the denominator of the progress fraction
	mutable uint32 PeakRemaining = 0;

	// Interval time (in seconds) to update the progress bar and text. A zero value will update them every frame.
	float Interval = 0.1f;

	// The settings style is copied so the SProgressBar style pointer stays valid for the widget's lifetime
	FProgressBarStyle Style;

	FText ProgressTextFormat;

	TSharedPtr<SProgressBar> ProgressBar;
	TSharedPtr<STextBlock> ProgressText;
};
