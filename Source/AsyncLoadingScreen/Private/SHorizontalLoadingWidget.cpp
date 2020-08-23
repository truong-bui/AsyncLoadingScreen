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
	ConstructLoadingIcon(Settings);

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
