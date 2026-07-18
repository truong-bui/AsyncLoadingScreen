/************************************************************************************
 *																					*
 * Copyright (C) 2020 Truong Bui.													*
 * Website:	https://github.com/truong-bui/AsyncLoadingScreen						*
 * Licensed under the MIT License. See 'LICENSE' file for full license information. *
 *																					*
 ************************************************************************************/

#pragma once

#include "Widgets/SCompoundWidget.h"

class SOverlay;
struct FALoadingScreenSettings;
struct FLoadingWidgetSettings;

/**
 * SLoadingScreenLayout
 *
 * This is the base class for all loading screen layouts.
 * It provides common functionality such as DPI scaling and shared widget construction.
 *
 * Note: This class is not intended to be instantiated directly.
 * Instead, use derived classes that implement specific loading screen layouts.
 *
 */
class SLoadingScreenLayout : public SCompoundWidget
{
protected:
	float GetDPIScale() const;
	void CalculateViewportSize() const;

	/** Creates the horizontal or vertical loading widget selected in the settings */
	static TSharedRef<SWidget> MakeLoadingWidget(const FLoadingWidgetSettings& Settings);

	/** Adds the loading complete text overlay slot if it is enabled in the settings */
	static void AddLoadingCompleteTextSlot(const TSharedRef<SOverlay>& Root, const FALoadingScreenSettings& Settings);

private:
	// Default to 1080p until a real viewport size is available (FIntPoint doesn't zero-initialize)
	mutable FIntPoint CachedViewportSize = FIntPoint(1920, 1080);
};
