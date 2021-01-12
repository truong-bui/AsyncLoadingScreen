/************************************************************************************
 *																					*
 * Copyright (C) 2020 Truong Bui.													*
 * Website:	https://github.com/truong-bui/AsyncLoadingScreen						*
 * Licensed under the MIT License. See 'LICENSE' file for full license information. *
 *																					*
 ************************************************************************************/

#pragma once

#include "Widgets/SCompoundWidget.h"

struct FLoadingCompleteTextSettings;
/**
 * 
 */
class SLoadingCompleteText : public SCompoundWidget
{
private:
	// Complete text color
	FLinearColor CompleteTextColor = FLinearColor::White;

	// Complete text fade in or fade out animation
	bool bCompleteTextReverseAnim = false;

	// Complete text animation speed
	float CompleteTextAnimationSpeed = 1.0f;

	// Active timer registered flag
	bool bIsActiveTimerRegistered = false;

public:
	SLATE_BEGIN_ARGS(SLoadingCompleteText) {}

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, const FLoadingCompleteTextSettings& CompleteTextSettings);

	// Getter for text visibility
	EVisibility GetLoadingCompleteTextVisibility() const;

	// Getter for complete text color and opacity
	FSlateColor GetLoadingCompleteTextColor() const;

	/** Active timer event for animating the image sequence */
	EActiveTimerReturnType AnimateText(double InCurrentTime, float InDeltaTime);
};
