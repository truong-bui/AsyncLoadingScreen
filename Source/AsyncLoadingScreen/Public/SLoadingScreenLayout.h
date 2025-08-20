/************************************************************************************
 *																					*
 * Copyright (C) 2020 Truong Bui.													*
 * Website:	https://github.com/truong-bui/AsyncLoadingScreen						*
 * Licensed under the MIT License. See 'LICENSE' file for full license information. *
 *																					*
 ************************************************************************************/

#pragma once

#include "Widgets/SCompoundWidget.h"
#include "Engine/GameViewportClient.h"

//DECLARE_LOG_CATEGORY_EXTERN(LogLoadingScreen, Log, All);
/**
 * SLoadingScreenLayout
 * 
 * This is the base class for all loading screen layouts.
 * It provides common functionality such as DPI scaling and point size conversion.
 * 
 * Note: This class is not intended to be instantiated directly.
 * Instead, use derived classes that implement specific loading screen layouts.
 *
 */
class SLoadingScreenLayout : public SCompoundWidget
{
public:	
	static float PointSizeToSlateUnits(float PointSize);
protected:
	float GetDPIScale() const;	
private:
	mutable FIntPoint _cachedViewportSize;
};
