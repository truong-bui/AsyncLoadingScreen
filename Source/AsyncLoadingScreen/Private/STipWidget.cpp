/************************************************************************************
 *																					*
 * Copyright (C) 2020 Truong Bui.													*
 * Website:	https://github.com/truong-bui/AsyncLoadingScreen						*
 * Licensed under the MIT License. See 'LICENSE' file for full license information. *
 *																					*
 ************************************************************************************/

#include "STipWidget.h"
#include "LoadingScreenSettings.h"
#include "Widgets/Text/STextBlock.h"

void STipWidget::Construct(const FArguments& InArgs, const FTipSettings& Settings)
{
	if (Settings.TipText.Num() > 0)
	{
		const int32 TipIndex = FMath::RandRange(0, Settings.TipText.Num() - 1);

		ChildSlot
		[
			SNew(STextBlock)		
			.ColorAndOpacity(Settings.Appearance.ColorAndOpacity)
			.Font(Settings.Appearance.Font)
			.ShadowOffset(Settings.Appearance.ShadowOffset)
			.ShadowColorAndOpacity(Settings.Appearance.ShadowColorAndOpacity)
			.Justification(Settings.Appearance.Justification)
			.WrapTextAt(Settings.TipWrapAt)
			.Text(Settings.TipText[TipIndex])			
		];
		
	}
}
