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
struct FLetterboxThemeSettings;

/**
 * Letterbox Theme
 */
class SLetterboxTheme : public SLoadingScreenTheme
{
public:
	SLATE_BEGIN_ARGS(SLetterboxTheme) {}

	SLATE_END_ARGS()

	/**
	 * Construct this widget
	 */
	void Construct(const FArguments& InArgs, const FALoadingScreenSettings& Settings, const FLetterboxThemeSettings& ThemeSettings);
};
