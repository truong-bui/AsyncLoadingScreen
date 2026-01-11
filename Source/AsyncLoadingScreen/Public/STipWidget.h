/************************************************************************************
 *																					*
 * Copyright (C) 2020 Truong Bui.													*
 * Website:	https://github.com/truong-bui/AsyncLoadingScreen						*
 * Licensed under the MIT License. See 'LICENSE' file for full license information. *
 *																					*
 ************************************************************************************/

#pragma once

#include "Widgets/SCompoundWidget.h"

struct FTipSettings;

/**
 * Tip widget
 */
class STipWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(STipWidget) {}

	SLATE_END_ARGS()

	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

	void Construct(const FArguments& InArgs, const FTipSettings& Settings);	

protected:
	// Current total delta time
	mutable float TotalDeltaTime = 0.0f;

	// Interval time (in seconds) to update the tip text, a value less than or equal to 0 will not update the tip text.
	float Interval = 0.0f;

private:
	// Placehold widget
	TSharedRef<SWidget> TipWidget = SNullWidget::NullWidget;


	TArray<FText> TipText;
};
