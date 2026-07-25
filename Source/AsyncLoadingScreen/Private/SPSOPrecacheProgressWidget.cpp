/************************************************************************************
 *																					*
 * Copyright (C) 2020 Truong Bui.													*
 * Website:	https://github.com/truong-bui/AsyncLoadingScreen						*
 * Licensed under the MIT License. See 'LICENSE' file for full license information. *
 *																					*
 ************************************************************************************/

#include "SPSOPrecacheProgressWidget.h"
#include "LoadingScreenSettings.h"
#include "ShaderPipelineCache.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Notifications/SProgressBar.h"
#include "Widgets/Text/STextBlock.h"

void SPSOPrecacheProgressWidget::Construct(const FArguments& InArgs, const FPSOPrecacheProgressSettings& Settings)
{
	Interval = Settings.UpdateInterval;
	Style = Settings.Style;
	ProgressTextFormat = Settings.ProgressText;

	ChildSlot
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
				.AutoHeight()
				.HAlign(HAlign_Center)
				[
					SNew(SBox)
						.WidthOverride(Settings.BarSize.X)
						.HeightOverride(Settings.BarSize.Y)
						[
							SAssignNew(ProgressBar, SProgressBar)
								.Style(&Style)
								.Percent(0.0f)
						]
				]
			+ SVerticalBox::Slot()
				.AutoHeight()
				.HAlign(HAlign_Fill)
				[
					SAssignNew(ProgressText, STextBlock)
						// Collapse the text so it takes no space when it is empty
						.Visibility(Settings.ProgressText.IsEmpty() ? EVisibility::Collapsed : EVisibility::SelfHitTestInvisible)
						.ColorAndOpacity(Settings.Appearance.ColorAndOpacity)
						.Font(Settings.Appearance.Font)
						.ShadowOffset(Settings.Appearance.ShadowOffset)
						.ShadowColorAndOpacity(Settings.Appearance.ShadowColorAndOpacity)
						.Justification(Settings.Appearance.Justification)
				]
		];

	// The attribute is also evaluated while collapsed, unlike OnPaint, so the widget can show up once precache work appears
	SetVisibility(TAttribute<EVisibility>::CreateSP(this, &SPSOPrecacheProgressWidget::GetProgressVisibility));
}

EVisibility SPSOPrecacheProgressWidget::GetProgressVisibility() const
{
	return FShaderPipelineCache::NumPrecompilesRemaining() > 0 ? EVisibility::SelfHitTestInvisible : EVisibility::Collapsed;
}

int32 SPSOPrecacheProgressWidget::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	TotalDeltaTime += Args.GetDeltaTime();

	if (TotalDeltaTime >= Interval)
	{
		// Keep the remainder so updates track the configured interval, without a catch-up burst after a long frame
		TotalDeltaTime = (Interval > 0.0f) ? FMath::Fmod(TotalDeltaTime, Interval) : 0.0f;

		const uint32 NumPrecompilesRemaining = FShaderPipelineCache::NumPrecompilesRemaining();
		PeakRemaining = FMath::Max(PeakRemaining, NumPrecompilesRemaining);

		const float Percent = PeakRemaining > 0 ? (float)(PeakRemaining - NumPrecompilesRemaining) / (float)PeakRemaining : 0.0f;
		ProgressBar->SetPercent(Percent);

		if (!ProgressTextFormat.IsEmpty())
		{
			FFormatNamedArguments FormatArgs;
			FormatArgs.Add(TEXT("Percent"), FText::AsNumber(FMath::RoundToInt(Percent * 100.0f)));
			FormatArgs.Add(TEXT("Remaining"), FText::AsNumber(NumPrecompilesRemaining));
			ProgressText->SetText(FText::Format(ProgressTextFormat, FormatArgs));
		}
	}

	return SCompoundWidget::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
}
