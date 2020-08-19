/********************************************************************************** 
 * 
 * Copyright (C) 2020 Truong Bui. 
 * Website:	https://github.com/truongbui1689/AsyncLoadingScreen 
 * Licensed under the MIT License. See 'LICENSE' file for full license information. 
 *  
 **********************************************************************************/

#pragma once

#include "Widgets/SCompoundWidget.h"
#include "LoadingScreenSettings.h"
class FDeferredCleanupSlateBrush;

/**
 * Classic loading screen theme
 */
class SClassicLoadingTheme : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SClassicLoadingTheme) {}

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, const FLoadingScreenDescription& ScreenDescription);

	static float PointSizeToSlateUnits(float PointSize)
	{
		const float SlateFreeTypeHorizontalResolutionDPI = 96.0f;
		const float FreeTypeNativeDPI = 72.0;
		const float PixelSize = PointSize * (SlateFreeTypeHorizontalResolutionDPI / FreeTypeNativeDPI);
		return PixelSize;
	}

	void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;
private:
	float GetDPIScale() const;

private:
	TSharedPtr<FDeferredCleanupSlateBrush> LoadingThemeBrush;	
	float CurrentDeltaTime = 0.0f;
	TSharedPtr<SImage> IconImage;
	FLoadingScreenDescription InScreenDescription;
	int32 IconIndex;
};
