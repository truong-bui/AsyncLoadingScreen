/************************************************************************************
 *																					*
 * Copyright (C) 2020 Truong Bui.													*
 * Website:	https://github.com/truong-bui/AsyncLoadingScreen						*
 * Licensed under the MIT License. See 'LICENSE' file for full license information. *
 *																					*
 ************************************************************************************/

#pragma once

#include "SLoadingScreenLayout.h"

struct FALoadingScreenSettings;
struct FClassicLayoutSettings;

/**
 * Classic layout loading screen
 */
class SClassicLayout : public SLoadingScreenLayout
{
public:
	SLATE_BEGIN_ARGS(SClassicLayout) {}

	SLATE_END_ARGS()

	/**
	 * Construct this widget
	 */
	void Construct(const FArguments& InArgs, const FALoadingScreenSettings& Settings, const FClassicLayoutSettings& LayoutSettings);
};
