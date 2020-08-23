#include "SLoadingWidget.h"
#include "LoadingScreenSettings.h"
#include "Widgets/Images/SThrobber.h"
#include "Widgets/Images/SImage.h"
#include "Slate/DeferredCleanupSlateBrush.h"


EActiveTimerReturnType SLoadingWidget::AnimatingImageSequence(double InCurrentTime, float InDeltaTime)
{	
	if (CleanupBrushList.Num() > 1)
	{
		ImageIndex++;
		if (ImageIndex >= CleanupBrushList.Num())
		{
			ImageIndex = 0;
		}

		StaticCastSharedRef<SImage>(LoadingIcon)->SetImage(CleanupBrushList[ImageIndex]->GetSlateBrush());

		return EActiveTimerReturnType::Continue;
	}	
	else
	{
		bIsActiveTimerRegistered = false;
		return EActiveTimerReturnType::Stop;
	}


}

SThrobber::EAnimation SLoadingWidget::GetThrobberAnimation(FThrobberSettings ThrobberSettings) const
{
	const int32 AnimationParams = (ThrobberSettings.bAnimateVertically ? SThrobber::Vertical : 0) |
		(ThrobberSettings.bAnimateHorizontally ? SThrobber::Horizontal : 0) |
		(ThrobberSettings.bAnimateOpacity ? SThrobber::Opacity : 0);

	return static_cast<SThrobber::EAnimation>(AnimationParams);
}
	