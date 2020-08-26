/**********************************************************************************
 *
 * Copyright (C) 2020 Truong Bui.
 * Website:	https://github.com/truong-bui/AsyncLoadingScreen
 * Licensed under the MIT License. See 'LICENSE' file for full license information.
 *
 **********************************************************************************/

#include "ClassicLoadingTheme.h"
#include "Engine/UserInterfaceSettings.h"
#include "Widgets/Layout/SSafeZone.h"
#include "Widgets/Layout/SDPIScaler.h"
#include "Widgets/Images/SThrobber.h"
#include "Widgets/Layout/SSpacer.h"
#include "Widgets/Images/SImage.h"
#include "Slate/DeferredCleanupSlateBrush.h"
#include "Styling/SlateBrush.h"
#include "SHorizontalLoadingWidget.h"
#include "SVerticalLoadingWidget.h"
#include "SBackgroundWidget.h"
#include "STipWidget.h"

#define LOCTEXT_NAMESPACE "ClassicLoadingTheme"

void SClassicLoadingTheme::Construct(const FArguments& InArgs, const FALoadingScreenSettings& ScreenDescription)
{		

	InScreenDescription = ScreenDescription;

	// Root widget
	TSharedRef<SOverlay> Root = SNew(SOverlay);

	// Add Background image
	Root->AddSlot()
		[
			SNew(SBackgroundWidget, ScreenDescription.Background)
		];


	// If there's an image defined
	/*
	if (ScreenDescription.Background.Images.Num() > 0)
	{
		const int32 ImageIndex = FMath::RandRange(0, ScreenDescription.Background.Images.Num() - 1);
		const FSoftObjectPath& ImageAsset = ScreenDescription.Background.Images[ImageIndex];
		UObject* ImageObject = ImageAsset.TryLoad();
		if (UTexture2D* LoadingImage = Cast<UTexture2D>(ImageObject))
		{
			LoadingThemeBrush = FDeferredCleanupSlateBrush::CreateBrush(LoadingImage);

			Root->AddSlot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SNew(SBorder)
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				.BorderBackgroundColor(ScreenDescription.Background.BackgroundColor)
				.BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
				[
					SNew(SScaleBox)
					.Stretch(ScreenDescription.Background.ImageStretch)
					[
						SNew(SImage)
						.Image(LoadingThemeBrush.IsValid() ? LoadingThemeBrush->GetSlateBrush() : nullptr)
					]
				]
			];
		}
	}
	*/

	/*
	if (ScreenDescription.LoadingWidget.Images.Num() > 0)
	{
		IconIndex = 0;
		const FSoftObjectPath& ImageAsset = ScreenDescription.LoadingWidget.Images[IconIndex];
		UObject* ImageObject = ImageAsset.TryLoad();
		if (UTexture2D* LoadingImage = Cast<UTexture2D>(ImageObject))
		{
			LoadingThemeBrush = FDeferredCleanupSlateBrush::CreateBrush(LoadingImage);
			
			Root->AddSlot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)			
			[
				SAssignNew(IconImage, SImage)
				.Image(LoadingThemeBrush->GetSlateBrush())
				.RenderTransformPivot(FVector2D(0.5f, 0.5f))
			];
		}
	}
	*/

/*
UE_LOG(LogTemp, Warning, TEXT("SClassicLoadingTheme::Construct"));
	if (ScreenDescription.IconMaterial.IsValid())
	{
		const FSoftObjectPath& IconMaterialAsset = ScreenDescription.IconMaterial;
		UObject* IconMaterialObject = IconMaterialAsset.TryLoad();
		UE_LOG(LogTemp, Warning, TEXT("ScreenDescription.IconMaterial.IsValid()"));
		if (UMaterialInstance* LoadingIcon = Cast<UMaterialInstance>(IconMaterialObject))
		{
			IconBrush = MakeShareable(new FSlateBrush());
			IconBrush->SetImageSize(FVector2D(64.0f, 64.0f));
			IconBrush->SetResourceObject(LoadingIcon);


			UE_LOG(LogTemp, Warning, TEXT("CREATE LOADING ICON HERE"));

			Root->AddSlot()
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				[
					SNew(SThrobber)
					.PieceImage(IconBrush.Get())
				.Animate(SThrobber::EAnimation::None)
				];
		}
	}
*/
	// Placeholder widget
	TSharedRef<SWidget> LoadingWidget = SNullWidget::NullWidget;

	if (ScreenDescription.LoadingWidget.LoadingWidgetType == ELoadingWidgetType::LWT_Horizontal)
	{
		LoadingWidget = SNew(SHorizontalLoadingWidget, ScreenDescription.LoadingWidget);
	}
	else
	{
		LoadingWidget = SNew(SVerticalLoadingWidget, ScreenDescription.LoadingWidget);
	}
	

	// Creating loading theme
	Root->AddSlot()
	.HAlign(HAlign_Fill)
	.VAlign(VAlign_Bottom)
	[
		SNew(SBorder)
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.BorderBackgroundColor(FColor::Black)
		.BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
		[
			SNew(SSafeZone)
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Bottom)
			.IsTitleSafe(true)
			[
				SNew(SDPIScaler)
				.DPIScale(this, &SClassicLoadingTheme::GetDPIScale)
				[
					
					SNew(SHorizontalBox)

					// Loading bar
					+ SHorizontalBox::Slot()
					.VAlign(VAlign_Center)
					.HAlign(HAlign_Center)
					.AutoWidth()
					[
						LoadingWidget
					]
					/*
					[						
						SNew(SCircularThrobber)
						.Radius(PointSizeToSlateUnits(LoadingFont.Size) / 2.0f)						
					]

					// Loading text
					+ SHorizontalBox::Slot()
					.Padding(FMargin(40, 0, 0, 0))
					.AutoWidth()
					.VAlign(VAlign_Center)
					[
						SNew(STextBlock)
						.Text(ScreenDescription.LoadingWidget.LoadingText)
						.Font(LoadingFont)
						
					]
					*/
					
					+ SHorizontalBox::Slot()
					.FillWidth(1)
					.HAlign(HAlign_Fill)
					[
						SNew(SSpacer)
						.Size(FVector2D(1.0f, 1.0f))
					]

					// Hint/Tip Text
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.HAlign(HAlign_Right)
					.VAlign(VAlign_Center)
					.Padding(FMargin(10.0f))
					[
						// Add tip text
						SNew(STipWidget, ScreenDescription.Tip)
						//SNew(STextBlock)
						//.WrapTextAt(ScreenDescription.Tip.TipWrapAt)
						//.Font(ScreenDescription.Tip.Font)
						//.Text(ScreenDescription.Tip.TipText[0])
					]
				]
			]
		]
	];
	
	// Add Root to this widget
	ChildSlot
	[
		Root
	];	

	//IconImage.Get()->SetRenderTransformPivot(FVector2D(0.5f, 0.5f));
	//IconImage.Get()->SetRenderTransform(FSlateRenderTransform(FScale2D(-1.0f, 1.0f), FVector2D(0.0f, 0.0f)));
	
}

float SClassicLoadingTheme::GetDPIScale() const
{
	const FVector2D& DrawSize = GetTickSpaceGeometry().ToPaintGeometry().GetLocalSize();
	const FIntPoint Size((int32)DrawSize.X, (int32)DrawSize.Y);

	return GetDefault<UUserInterfaceSettings>()->GetDPIScaleBasedOnSize(Size);
}

#undef LOCTEXT_NAMESPACE