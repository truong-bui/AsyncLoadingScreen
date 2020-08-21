/**********************************************************************************
 *
 * Copyright (C) 2020 Truong Bui.
 * Website:	https://github.com/truong-bui/AsyncLoadingScreen
 * Licensed under the MIT License. See 'LICENSE' file for full license information.
 *
 **********************************************************************************/

#pragma once

#include "Widgets/SCompoundWidget.h"

struct FLoadingWidgetSettings;

/**
 * Loading Widget
 */
class SLoadingWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SLoadingWidget) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, const FLoadingWidgetSettings& Settings);
	

private:
	EActiveTimerReturnType AnimatingImageSequence(double InCurrentTime, float InDeltaTime);

private:
	bool bIsActiveTimerRegistered;

};
