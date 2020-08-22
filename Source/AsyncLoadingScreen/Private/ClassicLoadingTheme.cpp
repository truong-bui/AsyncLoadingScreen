#include "ClassicLoadingTheme.h"
#include "Engine/UserInterfaceSettings.h"
#include "Widgets/Layout/SSafeZone.h"
#include "Widgets/Layout/SDPIScaler.h"
#include "Widgets/Images/SThrobber.h"
#include "Widgets/Layout/SSpacer.h"
#include "Widgets/Images/SImage.h"
#include "Slate/DeferredCleanupSlateBrush.h"
#include "Styling/SlateBrush.h"

#define LOCTEXT_NAMESPACE "ClassicLoadingTheme"

void SClassicLoadingTheme::Construct(const FArguments& InArgs, const FALoadingScreenSettings& ScreenDescription)
{		

	InScreenDescription = ScreenDescription;
	//const ULoadingScreenSettings* Settings = GetDefault<ULoadingScreenSettings>();

	const FSlateFontInfo& TipFont = ScreenDescription.Tips.Font;
	const FSlateFontInfo& LoadingFont = ScreenDescription.LoadingWidget.Font;

	// Root widget
	TSharedRef<SOverlay> Root = SNew(SOverlay);	
	
	// If there's an image defined
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
	TSharedRef<SWidget> TipWidget = SNullWidget::NullWidget;
	if (ScreenDescription.Tips.TipsText.Num() > 0)
	{
		const int32 TipIndex = FMath::RandRange(0, ScreenDescription.Tips.TipsText.Num() - 1);

		TipWidget = SNew(STextBlock)
			.WrapTextAt(ScreenDescription.Tips.TipWrapAt)
			.Font(TipFont)
			.Text(ScreenDescription.Tips.TipsText[TipIndex]);
	}
	else
	{
		// Need to use a spacer when being rendered on another thread, incrementing the SNullWidget will
		// lead to shared ptr crashes.
		TipWidget = SNew(SSpacer);
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
					.Padding(FMargin(25, 0, 0, 0))
					.VAlign(VAlign_Center)
					.AutoWidth()
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
						TipWidget		
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

void SClassicLoadingTheme::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	//UE_LOG(LogTemp, Warning, TEXT("Current Time: %lf, DeltaTime: %f"), InCurrentTime, InDeltaTime);

	CurrentDeltaTime += InDeltaTime;
	if (CurrentDeltaTime >= 0.05f)
	{
		IconIndex++;
		if (IconIndex >= InScreenDescription.LoadingWidget.Images.Num())
		{
			IconIndex = 0;
		}

		const FSoftObjectPath& ImageAsset = InScreenDescription.LoadingWidget.Images[IconIndex];
		UObject* ImageObject = ImageAsset.TryLoad();
		if (UTexture2D* LoadingImage = Cast<UTexture2D>(ImageObject))
		{
			LoadingThemeBrush = FDeferredCleanupSlateBrush::CreateBrush(LoadingImage);
			IconImage.Get()->SetImage(LoadingThemeBrush->GetSlateBrush());			
		}
		CurrentDeltaTime = 0;
	}
	
}

float SClassicLoadingTheme::GetDPIScale() const
{
	const FVector2D& DrawSize = GetTickSpaceGeometry().ToPaintGeometry().GetLocalSize();
	const FIntPoint Size((int32)DrawSize.X, (int32)DrawSize.Y);

	return GetDefault<UUserInterfaceSettings>()->GetDPIScaleBasedOnSize(Size);
}

#undef LOCTEXT_NAMESPACE