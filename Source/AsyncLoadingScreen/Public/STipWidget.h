/************************************************************************************
 *																					*
 * Copyright (C) 2020 Truong Bui.													*
 * Website:	https://github.com/truong-bui/AsyncLoadingScreen						*
 * Licensed under the MIT License. See 'LICENSE' file for full license information. *
 *																					*
 ************************************************************************************/

#pragma once

#include "Widgets/SCompoundWidget.h"

struct FTipSettings;

/**
 * Tip widget
 */
class STipWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(STipWidget) {}

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, const FTipSettings& Settings);
};
