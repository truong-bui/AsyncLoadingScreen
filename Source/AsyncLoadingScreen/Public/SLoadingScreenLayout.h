/************************************************************************************
 *																					*
 * Copyright (C) 2020 Truong Bui.													*
 * Website:	https://github.com/truong-bui/AsyncLoadingScreen						*
 * Licensed under the MIT License. See 'LICENSE' file for full license information. *
 *																					*
 ************************************************************************************/

#pragma once

#include "Widgets/SCompoundWidget.h"

/**
 * Loading screen base theme
 */
class SLoadingScreenLayout : public SCompoundWidget
{
public:	
	static float PointSizeToSlateUnits(float PointSize);
protected:
	float GetDPIScale() const;	
};
