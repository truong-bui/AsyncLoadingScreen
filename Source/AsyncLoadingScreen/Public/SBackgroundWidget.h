/************************************************************************************
 *																					*
 * Copyright (C) 2020 Truong Bui.													*
 * Website:	https://github.com/truong-bui/AsyncLoadingScreen						*
 * Licensed under the MIT License. See 'LICENSE' file for full license information. *
 *																					*
 ************************************************************************************/

#pragma once

#include "Widgets/SCompoundWidget.h"

struct FBackgroundSettings;
class FDeferredCleanupSlateBrush;

/**
 * Background widget
 */
class SBackgroundWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SBackgroundWidget) {}

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, const FBackgroundSettings& Settings);

private:
	TSharedPtr<FDeferredCleanupSlateBrush> ImageBrush;
};
