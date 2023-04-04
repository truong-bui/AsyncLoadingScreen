/************************************************************************************
 *																					*
 * Copyright (C) 2020 Truong Bui.													*
 * Website:	https://github.com/truong-bui/AsyncLoadingScreen						*
 * Licensed under the MIT License. See 'LICENSE' file for full license information. *
 *																					*
 ************************************************************************************/

#pragma once

#include "Widgets/SCompoundWidget.h"
#include "Widgets/Images/SThrobber.h"
#include "LoadingScreenSettings.h"

class FDeferredCleanupSlateBrush;
struct FLoadingWidgetSettings;

/**
 * Loading Widget base class
 */
class SLoadingWidget : public SCompoundWidget
{
public:

	// SWidgetOverrides
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

	/** Gets the combined value of the animation properties as a single SThrobber::EAnimation value. */
	SThrobber::EAnimation GetThrobberAnimation(const FThrobberSettings& ThrobberSettings) const;

	/** Construct loading icon*/
	void ConstructLoadingIcon(const FLoadingWidgetSettings& Settings);

protected:
	// Placeholder widgets
	TSharedRef<SWidget> LoadingIcon = SNullWidget::NullWidget;
	// Image slate brush list
	TArray<TSharedPtr<FDeferredCleanupSlateBrush>> CleanupBrushList;	

	// Play image sequence in reverse
	bool bPlayReverse = false;

	// Current image sequence index
	mutable int32 ImageIndex = 0;

	// Current total delta time
	mutable float TotalDeltaTime = 0.0f;

	//Time in second to update the images, the smaller value the faster of the animation. A zero value will update the images every frame.
	float Interval = 0.05f;	
	
	// Getter for text visibility
	EVisibility GetLoadingWidgetVisibility() const;
};
