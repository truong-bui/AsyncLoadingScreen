#include "SHorizontalLoadingWidget.h"
#include "LoadingScreenSettings.h"
#include "Widgets/Layout/SSpacer.h"
#include "Widgets/Images/SImage.h"
#include "Slate/DeferredCleanupSlateBrush.h"

void SHorizontalLoadingWidget::Construct(const FArguments& InArgs, const FLoadingWidgetSettings& Settings)
{
	bIsActiveTimerRegistered = false;

	// Root is a Horizontal Box of course
	TSharedRef<SHorizontalBox> Root = SNew(SHorizontalBox);		
	
	// Construct Loading Icon Widget
	if (Settings.LoadingIconType == ELoadingIconType::LIT_ImageSequence)
	{	
		// Loading Widget is image sequence
		if (Settings.Images.Num() > 0)
		{	
			CleanupBrushList.Empty();
			ImageIndex = 0;
			
			for (auto ImageAsset : Settings.Images)
			{
				UObject* ImageObject = ImageAsset.TryLoad();
				if (UTexture2D* LoadingImage = Cast<UTexture2D>(ImageObject))
				{
					CleanupBrushList.Add(FDeferredCleanupSlateBrush::CreateBrush(LoadingImage));
				}
			}
			
			// Create Image slate widget
			LoadingIcon = SNew(SImage)
				.Image(CleanupBrushList[ImageIndex]->GetSlateBrush());

			// Register animated image sequence active timer event
			if (!bIsActiveTimerRegistered)
			{
				bIsActiveTimerRegistered = true;
				RegisterActiveTimer(Settings.Interval, FWidgetActiveTimerDelegate::CreateSP(this, &SHorizontalLoadingWidget::AnimatingImageSequence));
			}
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

	// If loading text is on the right
	if (Settings.bLoadingTextRightPosition)
	{
		// Add Loading Icon on the left first
		Root.Get().AddSlot()
			.HAlign(Settings.LoadingIconHorizontalAlignment)
			.VAlign(Settings.LoadingIconTextVerticalAlignment)
			.AutoWidth()
			[
				LoadingIcon
			];

		// Add a Spacer in middle
		Root.Get().AddSlot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.AutoWidth()
			[
				SNew(SSpacer)
				.Size(FVector2D(Settings.Space, 0.0f))
			];

		// Add Loading Text on the right
		Root.Get().AddSlot()
			.HAlign(Settings.TextHorizontalAlignment)
			.VAlign(Settings.TextVerticalAlignment)
			.AutoWidth()
			[
				SNew(STextBlock)
				.Text(Settings.LoadingText)
				.Font(Settings.Font)
			];
	}

	// If loading text is on the left
	else
	{
		// Add Loading Text on the left first
		Root.Get().AddSlot()
			.HAlign(Settings.TextHorizontalAlignment)
			.VAlign(Settings.TextVerticalAlignment)
			.AutoWidth()
			[
				SNew(STextBlock)
				.Text(Settings.LoadingText)
				.Font(Settings.Font)
			];


		// Add a Spacer in middle
		Root.Get().AddSlot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.AutoWidth()
			[
				SNew(SSpacer)
				.Size(FVector2D(Settings.Space, 0.0f))
			];

		// Add Loading Icon on the right finally
		Root.Get().AddSlot()
			.HAlign(Settings.LoadingIconHorizontalAlignment)
			.VAlign(Settings.LoadingIconTextVerticalAlignment)
			.AutoWidth()
			[
				LoadingIcon
			];
	}

	// Add root to this widget
	ChildSlot
	[
		Root
	];	
}
