/************************************************************************************
 *																					*
 * Copyright (C) 2020 Truong Bui.													*
 * Website:	https://github.com/truong-bui/AsyncLoadingScreen						*
 * Licensed under the MIT License. See 'LICENSE' file for full license information. *
 *																					*
 ************************************************************************************/

#pragma once

#include "Widgets/SCompoundWidget.h"

struct FNetworkStatusSettings;

/**
 * Network status widget: a text that is only displayed while the plugin is holding the loading screen
 * waiting for the local player to finish joining the server.
 */
class SNetworkStatusWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SNetworkStatusWidget) {}

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, const FNetworkStatusSettings& Settings);

private:
	/** Visibility attribute getter; also evaluated while collapsed, so the widget can appear once the wait starts */
	EVisibility GetStatusVisibility() const;
};
