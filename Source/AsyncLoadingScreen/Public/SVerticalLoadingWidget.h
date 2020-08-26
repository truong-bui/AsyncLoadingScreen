/************************************************************************************
 *																					*
 * Copyright (C) 2020 Truong Bui.													*
 * Website:	https://github.com/truong-bui/AsyncLoadingScreen						*
 * Licensed under the MIT License. See 'LICENSE' file for full license information. *
 *																					*
 ************************************************************************************/

#pragma once

#include "SLoadingWidget.h"

/**
 * 
 */
class SVerticalLoadingWidget : public SLoadingWidget
{
public:
	SLATE_BEGIN_ARGS(SVerticalLoadingWidget) {}

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, const FLoadingWidgetSettings& Settings);
};
