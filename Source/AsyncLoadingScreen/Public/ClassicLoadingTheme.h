/********************************************************************************** 
 * 
 * Copyright (C) 2020 Truong Bui. 
 * Website:	https://github.com/truongbui1689/AsyncLoadingScreen 
 * Licensed under the MIT License. See 'LICENSE' file for full license information. 
 *  
 **********************************************************************************/

#pragma once

#include "Widgets/SCompoundWidget.h"

class FDeferredCleanupSlateBrush;

/**
 * Classic loading screen theme
 */
class SClassicLoadingTheme : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SClassicLoadingTheme) {}

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	float GetDPIScale() const;

private:
	TSharedPtr<FDeferredCleanupSlateBrush> LoadingThemeBrush;
};
