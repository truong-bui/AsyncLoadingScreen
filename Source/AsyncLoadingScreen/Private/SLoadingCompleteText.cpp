/************************************************************************************
 *																					*
 * Copyright (C) 2020 Truong Bui.													*
 * Website:	https://github.com/truong-bui/AsyncLoadingScreen						*
 * Licensed under the MIT License. See 'LICENSE' file for full license information. *
 *																					*
 ************************************************************************************/


#include "SLoadingCompleteText.h"
#include "LoadingScreenSettings.h"
#include "MoviePlayer.h"
#include "Widgets/Text/STextBlock.h"

void SLoadingCompleteText::Construct(const FArguments& InArgs, const FLoadingCompleteTextSettings& CompleteTextSettings)
{
	CompleteTextColor = CompleteTextSettings.Appearance.ColorAndOpacity.GetSpecifiedColor();
	CompleteTextAnimationSpeed = CompleteTextSettings.AnimationSpeed;

	ChildSlot
	[
		SNew(STextBlock)					
		.Font(CompleteTextSettings.Appearance.Font)
		.ShadowOffset(CompleteTextSettings.Appearance.ShadowOffset)
		.ShadowColorAndOpacity(CompleteTextSettings.Appearance.ShadowColorAndOpacity)
		.Justification(CompleteTextSettings.Appearance.Justification)
		.Text(CompleteTextSettings.LoadingCompleteText)
		.ColorAndOpacity(this, &SLoadingCompleteText::GetLoadingCompleteTextColor)
		.Visibility(this, &SLoadingCompleteText::GetLoadingCompleteTextVisibility)
	];	

	// Register animated image sequence active timer event
	if (CompleteTextSettings.bFadeInOutAnim && !bIsActiveTimerRegistered)
	{
		bIsActiveTimerRegistered = true;
		RegisterActiveTimer(0.f, FWidgetActiveTimerDelegate::CreateSP(this, &SLoadingCompleteText::AnimateText));
	}
}

EVisibility SLoadingCompleteText::GetLoadingCompleteTextVisibility() const
{
	return GetMoviePlayer()->IsLoadingFinished() ? EVisibility::Visible : EVisibility::Hidden;
}

FSlateColor SLoadingCompleteText::GetLoadingCompleteTextColor() const
{
	return CompleteTextColor;
}

EActiveTimerReturnType SLoadingCompleteText::AnimateText(double InCurrentTime, float InDeltaTime)
{
	const float MinAlpha = 0.1f;
	const float MaxAlpha = 1.0f;

	float TextAlpha = CompleteTextColor.A;

	if (TextAlpha >= MaxAlpha)
	{
		bCompleteTextReverseAnim = true;
	}
	else if (TextAlpha <= MinAlpha)
	{
		bCompleteTextReverseAnim = false;
	}

	if (!bCompleteTextReverseAnim)
	{
		TextAlpha += InDeltaTime * CompleteTextAnimationSpeed;
	}
	else
	{
		TextAlpha -= InDeltaTime * CompleteTextAnimationSpeed;
	}

	CompleteTextColor.A = TextAlpha;

	return EActiveTimerReturnType::Continue;
}
