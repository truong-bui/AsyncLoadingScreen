/************************************************************************************
 *																					*
 * Copyright (C) 2020 Truong Bui.													*
 * Website:	https://github.com/truong-bui/AsyncLoadingScreen						*
 * Licensed under the MIT License. See 'LICENSE' file for full license information. *
 *																					*
 ************************************************************************************/

#include "SLoadingScreenLayout.h"
#include "Engine/UserInterfaceSettings.h"

float SLoadingScreenLayout::PointSizeToSlateUnits(float PointSize)
{
	const float SlateFreeTypeHorizontalResolutionDPI = 96.0f;
	const float FreeTypeNativeDPI = 72.0;
	const float PixelSize = PointSize * (SlateFreeTypeHorizontalResolutionDPI / FreeTypeNativeDPI);
	return PixelSize;
}

float SLoadingScreenLayout::GetDPIScale() const
{
	FVector2D ViewportSize;
	GEngine->GameViewport->GetViewportSize(ViewportSize);	
	int32 X = FGenericPlatformMath::FloorToInt(ViewportSize.X);
	int32 Y = FGenericPlatformMath::FloorToInt(ViewportSize.Y);
	const FIntPoint Size(X, Y);
	return FMath::Clamp(GetDefault<UUserInterfaceSettings>()->GetDPIScaleBasedOnSize(Size), 0.1f, 1.0f);
}

