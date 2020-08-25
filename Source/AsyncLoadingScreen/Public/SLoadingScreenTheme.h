/**********************************************************************************
 *
 * Copyright (C) 2020 Truong Bui.
 * Website:	https://github.com/truong-bui/AsyncLoadingScreen
 * Licensed under the MIT License. See 'LICENSE' file for full license information.
 *
 **********************************************************************************/

#pragma once

#include "Widgets/SCompoundWidget.h"

/**
 * Loading screen base theme
 */
class SLoadingScreenTheme : public SCompoundWidget
{
public:
	static float PointSizeToSlateUnits(float PointSize)
	{
		const float SlateFreeTypeHorizontalResolutionDPI = 96.0f;
		const float FreeTypeNativeDPI = 72.0;
		const float PixelSize = PointSize * (SlateFreeTypeHorizontalResolutionDPI / FreeTypeNativeDPI);
		return PixelSize;
	}

protected:
	float GetDPIScale() const;
};
