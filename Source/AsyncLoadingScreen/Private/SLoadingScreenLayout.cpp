/************************************************************************************
 *																					*
 * Copyright (C) 2020 Truong Bui.													*
 * Website:	https://github.com/truong-bui/AsyncLoadingScreen						*
 * Licensed under the MIT License. See 'LICENSE' file for full license information. *
 *																					*
 ************************************************************************************/

#include "SLoadingScreenLayout.h"
#include "LoadingScreenSettings.h"
#include "SHorizontalLoadingWidget.h"
#include "SVerticalLoadingWidget.h"
#include "SLoadingCompleteText.h"
#include "Engine/UserInterfaceSettings.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "Widgets/SOverlay.h"

float SLoadingScreenLayout::GetDPIScale() const
{
	if (GEngine && GEngine->GameViewport)
	{
		FVector2D ViewportSize;
		GEngine->GameViewport->GetViewportSize(ViewportSize);
		int32 X = FGenericPlatformMath::FloorToInt(ViewportSize.X);
		int32 Y = FGenericPlatformMath::FloorToInt(ViewportSize.Y);

		if (X != 0 && Y != 0)
		{
			CachedViewportSize = FIntPoint(X, Y);
		}
		else
		{
			CalculateViewportSize();
		}
	}
	else
	{
		CalculateViewportSize();
	}

	// No upper clamp so the loading screen scales up on high-resolution displays like the rest of the game UI
	return FMath::Max(GetDefault<UUserInterfaceSettings>()->GetDPIScaleBasedOnSize(CachedViewportSize), 0.1f);
}

void SLoadingScreenLayout::CalculateViewportSize() const
{
	// Fall back to this widget's own geometry when the game viewport is not available yet
	const FVector2D DrawSize = GetTickSpaceGeometry().ToPaintGeometry().GetLocalSize();
	if (!DrawSize.Equals(FVector2D::ZeroVector))
	{
		CachedViewportSize = FIntPoint(FGenericPlatformMath::FloorToInt(DrawSize.X), FGenericPlatformMath::FloorToInt(DrawSize.Y));
	}
}

TSharedRef<SWidget> SLoadingScreenLayout::MakeLoadingWidget(const FLoadingWidgetSettings& Settings)
{
	if (Settings.LoadingWidgetType == ELoadingWidgetType::LWT_Horizontal)
	{
		return SNew(SHorizontalLoadingWidget, Settings);
	}

	return SNew(SVerticalLoadingWidget, Settings);
}

void SLoadingScreenLayout::AddLoadingCompleteTextSlot(const TSharedRef<SOverlay>& Root, const FALoadingScreenSettings& Settings)
{
	if (Settings.bShowLoadingCompleteText)
	{
		Root->AddSlot()
			.VAlign(Settings.LoadingCompleteTextSettings.Alignment.VerticalAlignment)
			.HAlign(Settings.LoadingCompleteTextSettings.Alignment.HorizontalAlignment)
			.Padding(Settings.LoadingCompleteTextSettings.Padding)
			[
				SNew(SLoadingCompleteText, Settings.LoadingCompleteTextSettings)
			];
	}
}
