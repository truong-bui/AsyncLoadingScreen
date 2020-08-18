#include "ClassicLoadingTheme.h"
#include "Engine/UserInterfaceSettings.h"
#include "Widgets/Layout/SSafeZone.h"
#include "Widgets/Layout/SDPIScaler.h"
#include "Widgets/Images/SThrobber.h"
#include "Widgets/Layout/SSpacer.h"
#include "Widgets/Images/SImage.h"


#define LOCTEXT_NAMESPACE "ClassicLoadingTheme"

void SClassicLoadingTheme::Construct(const FArguments& InArgs)
{

	// Root widget
	TSharedRef<SOverlay> Root = SNew(SOverlay);
	TSharedRef<STextBlock> LoadingText = SNew(STextBlock)
		.Text(FText::FromString("LOADING..."));
	
	// Creating loading theme
	Root->AddSlot()
	.HAlign(HAlign_Fill)
	.VAlign(VAlign_Bottom)
	[
		SNew(SBorder)
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.BorderBackgroundColor(FColor::Blue)
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
						.Radius(32.0f)						
					]

					// Loading text
					+ SHorizontalBox::Slot()
					.Padding(FMargin(40, 0, 0, 0))
					.AutoWidth()
					.VAlign(VAlign_Center)
					[
						//SNew(STextBlock)
						//.Text(FText::FromString("LOADING..."))
						LoadingText
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
						SNew(STextBlock)
						.Text(FText::FromString("This is a Hint/Tip text"))			
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
}

float SClassicLoadingTheme::GetDPIScale() const
{
	const FVector2D& DrawSize = GetTickSpaceGeometry().ToPaintGeometry().GetLocalSize();
	const FIntPoint Size((int32)DrawSize.X, (int32)DrawSize.Y);

	return GetDefault<UUserInterfaceSettings>()->GetDPIScaleBasedOnSize(Size);
}

#undef LOCTEXT_NAMESPACE