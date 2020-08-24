#include "SBackgroundWidget.h"
#include "LoadingScreenSettings.h"
#include "Slate/DeferredCleanupSlateBrush.h"
#include "Widgets/Images/SImage.h"

void SBackgroundWidget::Construct(const FArguments& InArgs, const FBackgroundSettings& Settings)
{
	// If there's an image defined
	if (Settings.Images.Num() > 0)
	{
		const int32 ImageIndex = FMath::RandRange(0, Settings.Images.Num() - 1);
		const FSoftObjectPath& ImageAsset = Settings.Images[ImageIndex];
		UObject* ImageObject = ImageAsset.TryLoad();
		if (UTexture2D* LoadingImage = Cast<UTexture2D>(ImageObject))
		{
			ImageBrush = FDeferredCleanupSlateBrush::CreateBrush(LoadingImage);
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
						SNew(SImage)						
						.Image(ImageBrush.IsValid() ? ImageBrush->GetSlateBrush() : nullptr)						
					]
				]
			];			
		}
	}
}
