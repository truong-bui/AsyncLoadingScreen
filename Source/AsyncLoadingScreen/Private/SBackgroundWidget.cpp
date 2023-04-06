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
	// If there's an image defined
	if (Settings.Images.Num() > 0)
	{
		int32 ImageIndex = FMath::RandRange(0, Settings.Images.Num() - 1);

		if (Settings.bSetDisplayBackgroundManually == true)
		{
			if (Settings.Images.IsValidIndex(UAsyncLoadingScreenLibrary::GetDisplayBackgroundIndex()))
			{
				ImageIndex = UAsyncLoadingScreenLibrary::GetDisplayBackgroundIndex();
			}
		}		
		
		// Load background from settings
		UTexture2D* LoadingImage = nullptr;
		const FSoftObjectPath& ImageAsset = Settings.Images[ImageIndex];
		UObject* ImageObject = ImageAsset.TryLoad();
		LoadingImage = Cast<UTexture2D>(ImageObject);

		// If IsPreloadBackgroundImagesEnabled is enabled, load from images array
		FAsyncLoadingScreenModule& LoadingScreenModule = FAsyncLoadingScreenModule::Get();
		if (LoadingScreenModule.IsPreloadBackgroundImagesEnabled())
		{
			TArray<UTexture2D*> BackgroundImages = LoadingScreenModule.GetBackgroundImages();
			if (!BackgroundImages.IsEmpty() && BackgroundImages.IsValidIndex(ImageIndex))
			{
				LoadingImage = BackgroundImages[ImageIndex];
			}
		}		
		
		if (LoadingImage)
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