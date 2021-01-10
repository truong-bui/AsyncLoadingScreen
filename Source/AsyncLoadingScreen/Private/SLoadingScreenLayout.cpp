/************************************************************************************
 *																					*
 * Copyright (C) 2020 Truong Bui.													*
 * Website:	https://github.com/truong-bui/AsyncLoadingScreen						*
 * Licensed under the MIT License. See 'LICENSE' file for full license information. *
 *																					*
 ************************************************************************************/

#include "SLoadingScreenLayout.h"
#include "Engine/UserInterfaceSettings.h"
#include "MoviePlayer.h"
#include "LoadingScreenSettings.h"

float SLoadingScreenLayout::PointSizeToSlateUnits(float PointSize)
{
	const float SlateFreeTypeHorizontalResolutionDPI = 96.0f;
	const float FreeTypeNativeDPI = 72.0;
	const float PixelSize = PointSize * (SlateFreeTypeHorizontalResolutionDPI / FreeTypeNativeDPI);
	return PixelSize;
}

float SLoadingScreenLayout::GetDPIScale() const
{
	const FVector2D& DrawSize = GetTickSpaceGeometry().ToPaintGeometry().GetLocalSize();
	const FIntPoint Size((int32)DrawSize.X, (int32)DrawSize.Y);
	
	return GetDefault<UUserInterfaceSettings>()->GetDPIScaleBasedOnSize(Size);
}

void SLoadingScreenLayout::ConstructLoadingCompleteText(TSharedRef<SOverlay> Root, const FLoadingCompleteTextSettings& CompleteTextSettings)
{
	Root->AddSlot()
		.VAlign(CompleteTextSettings.Alignment.VerticalAlignment)
		.HAlign(CompleteTextSettings.Alignment.HorizontalAlignment)
		.Padding(CompleteTextSettings.Padding)
		[
			SNew(STextBlock)
			//.ColorAndOpacity(CompleteTextSettings.Appearance.ColorAndOpacity)			
			.Font(CompleteTextSettings.Appearance.Font)
			.ShadowOffset(CompleteTextSettings.Appearance.ShadowOffset)
			.ShadowColorAndOpacity(CompleteTextSettings.Appearance.ShadowColorAndOpacity)
			.Justification(CompleteTextSettings.Appearance.Justification)		
			.Text(CompleteTextSettings.LoadingCompleteText)			
			.ColorAndOpacity(this, &SLoadingScreenLayout::GetLoadingCompleteTextColor)
			.Visibility(this, &SLoadingScreenLayout::GetLoadingCompleteTextVisibility)
		];

	CompleteTextColor = CompleteTextSettings.Appearance.ColorAndOpacity.GetSpecifiedColor();
	CompleteTextAnimationSpeed = CompleteTextSettings.AnimationSpeed;

	// Register animated image sequence active timer event
	if (CompleteTextSettings.bFadeInFadeOutAnim && !bIsActiveTimerRegistered)
	{
		bIsActiveTimerRegistered = true;
		RegisterActiveTimer(0.f, FWidgetActiveTimerDelegate::CreateSP(this, &SLoadingScreenLayout::AnimateTextOpacity));
	}
}

EVisibility SLoadingScreenLayout::GetLoadingCompleteTextVisibility() const
{
	return GetMoviePlayer()->IsLoadingFinished() ? EVisibility::Visible : EVisibility::Hidden;
}

FSlateColor SLoadingScreenLayout::GetLoadingCompleteTextColor() const
{
	return CompleteTextColor;
}

EActiveTimerReturnType SLoadingScreenLayout::AnimateTextOpacity(double InCurrentTime, float InDeltaTime)
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
