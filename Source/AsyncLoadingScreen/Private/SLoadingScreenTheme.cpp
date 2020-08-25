/**********************************************************************************
 *
 * Copyright (C) 2020 Truong Bui.
 * Website:	https://github.com/truong-bui/AsyncLoadingScreen
 * Licensed under the MIT License. See 'LICENSE' file for full license information.
 *
 **********************************************************************************/

#include "SLoadingScreenTheme.h"

float SLoadingScreenTheme::GetDPIScale() const
{
	const FVector2D& DrawSize = GetTickSpaceGeometry().ToPaintGeometry().GetLocalSize();
	const FIntPoint Size((int32)DrawSize.X, (int32)DrawSize.Y);

	return GetDefault<UUserInterfaceSettings>()->GetDPIScaleBasedOnSize(Size);
}
