/************************************************************************************
 *																					*
 * Copyright (C) 2020 Truong Bui.													*
 * Website:	https://github.com/truong-bui/AsyncLoadingScreen						*
 * Licensed under the MIT License. See 'LICENSE' file for full license information. *
 *																					*
 ************************************************************************************/

#include "SBackgroundWidget.h"
#include "LoadingScreenSettings.h"
#include "Slate/DeferredCleanupSlateBrush.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Layout/SBorder.h"
#include "Engine/Texture2D.h"
#include "AsyncLoadingScreenLibrary.h"
#include "AsyncLoadingScreen.h"

void SBackgroundWidget::Construct(const FArguments& InArgs, const FBackgroundSettings& Settings)
{
	Interval = Settings.UpdateInterval;

	ImageBrushList.Empty();
	for (auto& Image : Settings.Images)
	{
		if (Image != nullptr)
		{
			ImageBrushList.Add(FDeferredCleanupSlateBrush::CreateBrush(Image));
		}
	}

	// If there's an image defined
	if (ImageBrushList.Num() > 0)
	{
		int32 ImageIndex = FMath::RandRange(0, ImageBrushList.Num() - 1);

		if (Settings.bSetDisplayBackgroundManually == true)
		{
			if (ImageBrushList.IsValidIndex(UAsyncLoadingScreenLibrary::GetDisplayBackgroundIndex()))
			{
				ImageIndex = UAsyncLoadingScreenLibrary::GetDisplayBackgroundIndex();
			}
		}				

		// Load background from settings
		ChildSlot
			[
				SNew(SBorder)
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					.Padding(Settings.Padding)
					.BorderBackgroundColor(Settings.BackgroundColor)
					.BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
					[
						SNew(SScaleBox)
							.Stretch(Settings.ImageStretch)
							[
								BackgroundWidget = SNew(SImage)
									.Image(ImageBrushList[ImageIndex].IsValid() ? ImageBrushList[ImageIndex]->GetSlateBrush() : nullptr)
							]
					]
			];
	}
}

int32 SBackgroundWidget::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	// Update the images if Interval > 0 and Images has more than one element
	if (Interval > 0.0f && ImageBrushList.Num() > 1)
	{
		TotalDeltaTime += Args.GetDeltaTime();

		if (TotalDeltaTime >= Interval)
		{
			int32 ImageIndex = FMath::RandRange(0, ImageBrushList.Num() - 1);

			// Load background from settings
			StaticCastSharedRef<SImage>(BackgroundWidget)->SetImage(ImageBrushList[ImageIndex].IsValid() ? ImageBrushList[ImageIndex]->GetSlateBrush() : nullptr);

			TotalDeltaTime = 0.0f;
		}
	}

	return SCompoundWidget::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
}
