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
#include "AsyncLoadingScreenLibrary.h"

int32 STipWidget::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	// Update the tip text if Interval > 0 and TipText has more than one element
	if (Interval > 0.0f && TipText.Num() > 1)
	{
		TotalDeltaTime += Args.GetDeltaTime();

		if (TotalDeltaTime >= Interval)
		{
			int32 TipIndex = FMath::RandRange(0, TipText.Num() - 1);

			StaticCastSharedRef<STextBlock>(TipWidget)->SetText(TipText[TipIndex]);

			TotalDeltaTime = 0.0f;
		}
	}

	return SCompoundWidget::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
}

void STipWidget::Construct(const FArguments& InArgs, const FTipSettings& Settings)
{
	TipText = Settings.TipText;
	Interval = Settings.UpdateInterval;

	if (TipText.Num() > 0)
	{
		int32 TipIndex = FMath::RandRange(0, TipText.Num() - 1);
		
		if (Settings.bSetDisplayTipTextManually == true)
		{			
			if (TipText.IsValidIndex(UAsyncLoadingScreenLibrary::GetDisplayTipTextIndex()))
			{
				TipIndex = UAsyncLoadingScreenLibrary::GetDisplayTipTextIndex();
			}
		}

		ChildSlot
		[
			TipWidget = SNew(STextBlock)		
				.ColorAndOpacity(Settings.Appearance.ColorAndOpacity)
				.Font(Settings.Appearance.Font)
				.ShadowOffset(Settings.Appearance.ShadowOffset)
				.ShadowColorAndOpacity(Settings.Appearance.ShadowColorAndOpacity)
				.Justification(Settings.Appearance.Justification)
				.WrapTextAt(Settings.TipWrapAt)
				.Text(TipText[TipIndex])			
		];
		
	}
}
