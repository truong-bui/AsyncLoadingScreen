/************************************************************************************
 *																					*
 * Copyright (C) 2020 Truong Bui.													*
 * Website:	https://github.com/truong-bui/AsyncLoadingScreen						*
 * Licensed under the MIT License. See 'LICENSE' file for full license information. *
 *																					*
 ************************************************************************************/

#include "SLoadingScreenLayout.h"
#include "Engine/UserInterfaceSettings.h"
#include "Engine/Engine.h"

//DEFINE_LOG_CATEGORY(LogLoadingScreen);

float SLoadingScreenLayout::PointSizeToSlateUnits(float PointSize)
{
	const float SlateFreeTypeHorizontalResolutionDPI = 96.0f;
	const float FreeTypeNativeDPI = 72.0;
	const float PixelSize = PointSize * (SlateFreeTypeHorizontalResolutionDPI / FreeTypeNativeDPI);
	return PixelSize;
}

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
			_cachedViewportSize = FIntPoint(X, Y);
		}		
	}
	else 
	{
		const FVector2D DrawSize = GetTickSpaceGeometry().ToPaintGeometry().GetLocalSize();
		if (!DrawSize.Equals(FVector2D::ZeroVector))
		{
			int32 X = FGenericPlatformMath::FloorToInt(DrawSize.X);
			int32 Y = FGenericPlatformMath::FloorToInt(DrawSize.Y);
			_cachedViewportSize = FIntPoint(X, Y);
		}						
	}
	/*auto DPIOnSize = GetDefault<UUserInterfaceSettings>()->GetDPIScaleBasedOnSize(_cachedViewportSize);
	auto DPIScale = FMath::Clamp(DPIOnSize, 0.1f, 1.0f);
	UE_LOG(LogLoadingScreen, Log, TEXT("DPIOnSize: %f, Size: %s, DPIScale: %f"), DPIOnSize, *_cachedViewportSize.ToString(), DPIScale);*/
	return FMath::Clamp(GetDefault<UUserInterfaceSettings>()->GetDPIScaleBasedOnSize(_cachedViewportSize), 0.1f, 1.0f);
}

