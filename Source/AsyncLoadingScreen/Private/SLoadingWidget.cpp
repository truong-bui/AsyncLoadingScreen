/************************************************************************************
 *																					*
 * Copyright (C) 2020 Truong Bui.													*
 * Website:	https://github.com/truong-bui/AsyncLoadingScreen						*
 * Licensed under the MIT License. See 'LICENSE' file for full license information. *
 *																					*
 ************************************************************************************/

#include "SLoadingWidget.h"
#include "Widgets/Images/SImage.h"
#include "Slate/DeferredCleanupSlateBrush.h"
#include "Widgets/Layout/SSpacer.h"
#include "Engine/Texture2D.h"
#include "MoviePlayer.h"
#include "Widgets/SCompoundWidget.h"

int32 SLoadingWidget::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{		
	TotalDeltaTime += Args.GetDeltaTime();

	if (TotalDeltaTime >= Interval)
	{
		if (CleanupBrushList.Num() > 1)
		{
			if (bPlayReverse)
			{
				ImageIndex--;
			}
			else
			{
				ImageIndex++;
			}

			if (ImageIndex >= CleanupBrushList.Num())
			{
				ImageIndex = 0;
			}
			else if (ImageIndex < 0)
			{
				ImageIndex = CleanupBrushList.Num() - 1;
			}

			StaticCastSharedRef<SImage>(LoadingIcon)->SetImage(CleanupBrushList[ImageIndex].IsValid() ? CleanupBrushList[ImageIndex]->GetSlateBrush() : nullptr);			
		}

		TotalDeltaTime = 0.0f;
	}
	

	return SCompoundWidget::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
}

SThrobber::EAnimation SLoadingWidget::GetThrobberAnimation(const FThrobberSettings& ThrobberSettings) const
{
	const int32 AnimationParams = (ThrobberSettings.bAnimateVertically ? SThrobber::Vertical : 0) |
		(ThrobberSettings.bAnimateHorizontally ? SThrobber::Horizontal : 0) |
		(ThrobberSettings.bAnimateOpacity ? SThrobber::Opacity : 0);

	return static_cast<SThrobber::EAnimation>(AnimationParams);
}

void SLoadingWidget::ConstructLoadingIcon(const FLoadingWidgetSettings& Settings)
{
	if (Settings.LoadingIconType == ELoadingIconType::LIT_ImageSequence)
	{
		// Loading Widget is image sequence
		if (Settings.ImageSequenceSettings.Images.Num() > 0)
		{
			CleanupBrushList.Empty();
			ImageIndex = 0;

			FVector2D Scale = Settings.ImageSequenceSettings.Scale;

			for (auto Image: Settings.ImageSequenceSettings.Images)
			{
				if (Image)
				{
					CleanupBrushList.Add(FDeferredCleanupSlateBrush::CreateBrush(Image, FVector2D(Image->GetSurfaceWidth() * Scale.X, Image->GetSurfaceHeight() * Scale.Y)));					
				}				
			}
		
			// Create Image slate widget
			LoadingIcon = SNew(SImage)
				.Image(CleanupBrushList[ImageIndex]->GetSlateBrush());

			// Update play animation interval
			Interval = Settings.ImageSequenceSettings.Interval;
		}
		else
		{
			// If there is no image in the array then create a spacer instead
			LoadingIcon = SNew(SSpacer).Size(FVector2D::ZeroVector);
		}

	}
	else if (Settings.LoadingIconType == ELoadingIconType::LIT_CircularThrobber)
	{
		// Loading Widget is SCircularThrobber
		LoadingIcon = SNew(SCircularThrobber)
			.NumPieces(Settings.CircularThrobberSettings.NumberOfPieces)
			.Period(Settings.CircularThrobberSettings.Period)
			.Radius(Settings.CircularThrobberSettings.Radius)
			.PieceImage(&Settings.CircularThrobberSettings.Image);
	}
	else
	{
		// Loading Widget is SThrobber
		LoadingIcon = SNew(SThrobber)
			.NumPieces(Settings.ThrobberSettings.NumberOfPieces)
			.Animate(GetThrobberAnimation(Settings.ThrobberSettings))
			.PieceImage(&Settings.ThrobberSettings.Image);
	}

	// Set Loading Icon render transform
	LoadingIcon.Get().SetRenderTransform(FSlateRenderTransform(FScale2D(Settings.TransformScale), Settings.TransformTranslation));
	LoadingIcon.Get().SetRenderTransformPivot(Settings.TransformPivot);

	// Hide loading widget when level loading is done if bHideLoadingWidgetWhenCompletes is true 
	if (Settings.bHideLoadingWidgetWhenCompletes)
	{		
		SetVisibility(TAttribute<EVisibility>::Create(TAttribute<EVisibility>::FGetter::CreateRaw(this, &SLoadingWidget::GetLoadingWidgetVisibility)));
	}	
}

EVisibility SLoadingWidget::GetLoadingWidgetVisibility() const
{
	return GetMoviePlayer()->IsLoadingFinished() ? EVisibility::Hidden : EVisibility::Visible;
}