#include "SLoadingWidget.h"
#include "LoadingScreenSettings.h"

void SLoadingWidget::Construct(const FArguments& InArgs, const FLoadingWidgetSettings& Settings)
{
}

EActiveTimerReturnType SLoadingWidget::AnimatingImageSequence(double InCurrentTime, float InDeltaTime)
{
	return EActiveTimerReturnType::Continue;
}
	