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
struct FDualSidebarLayoutSettings;

/**
 * Dual Sidebar Layout
 */
class SDualSidebarLayout : public SLoadingScreenLayout
{
public:
	SLATE_BEGIN_ARGS(SDualSidebarLayout) {}

	SLATE_END_ARGS()

	/**
	 * Construct this widget
	 */
	void Construct(const FArguments& InArgs, const FALoadingScreenSettings& Settings, const FDualSidebarLayoutSettings& LayoutSettings);
};
