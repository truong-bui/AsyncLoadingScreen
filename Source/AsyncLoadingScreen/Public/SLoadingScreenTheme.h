/**********************************************************************************
 *
 * Copyright (C) 2020 Truong Bui.
 * Website:	https://github.com/truong-bui/AsyncLoadingScreen
 * Licensed under the MIT License. See 'LICENSE' file for full license information.
 *
 **********************************************************************************/

#pragma once

#include "Widgets/SCompoundWidget.h"
#include "LoadingScreenSettings.h"


/**
 * Loading screen base theme
 */
class SLoadingScreenTheme : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SLoadingScreenTheme) {}

	SLATE_END_ARGS()

	/**
	 * Construct this widget
	 */
	void Construct(const FArguments& InArgs, const FALoadingScreenSettings& Settings);

	/**
	 * Construct Centrality theme loading screen
	 */
	void ConstructCentralityTheme(const FALoadingScreenSettings& Settings, const FCentralityThemeSettings& ThemeSettings);

	static float PointSizeToSlateUnits(float PointSize);
	float GetDPIScale() const;	
};
