/************************************************************************************
 *																					*
 * Copyright (C) 2020 Truong Bui.													*
 * Website:	https://github.com/truong-bui/AsyncLoadingScreen						*
 * Licensed under the MIT License. See 'LICENSE' file for full license information. *
 *																					*
 ************************************************************************************/

#pragma once

#include "SLoadingScreenTheme.h"

struct FALoadingScreenSettings;
struct FCentralityThemeSettings;

/**
 * Centrality Theme loading screen
 */
class SCentralityTheme : public SLoadingScreenTheme
{
public:
	SLATE_BEGIN_ARGS(SCentralityTheme) {}

	SLATE_END_ARGS()

	/**
	 * Construct this widget
	 */
	void Construct(const FArguments& InArgs, const FALoadingScreenSettings& Settings, const FCentralityThemeSettings& ThemeSettings);
};
