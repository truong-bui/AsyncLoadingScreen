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

	// Pick the image to display, if any is defined
	int32 ImageIndex = INDEX_NONE;
	if (ImageBrushList.Num() > 0)
	{
		ImageIndex = FMath::RandRange(0, ImageBrushList.Num() - 1);

		if (Settings.bSetDisplayBackgroundManually == true)
		{
			if (ImageBrushList.IsValidIndex(UAsyncLoadingScreenLibrary::GetDisplayBackgroundIndex()))
			{
				ImageIndex = UAsyncLoadingScreenLibrary::GetDisplayBackgroundIndex();
				// A manually chosen background stays on screen; disable the random interval refresh
				Interval = 0.0f;
			}
		}
	}

	// Always build the border so the background color renders even when no image is defined
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
								.Image(ImageIndex != INDEX_NONE ? ImageBrushList[ImageIndex]->GetSlateBrush() : nullptr)
						]
				]
		];
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
			StaticCastSharedRef<SImage>(BackgroundWidget)->SetImage(ImageBrushList[ImageIndex]->GetSlateBrush());

			TotalDeltaTime = 0.0f;
		}
	}

	return SCompoundWidget::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
}
