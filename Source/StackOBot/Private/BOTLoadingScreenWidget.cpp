

#include "BOTLoadingScreenWidget.h"

UBOTLoadingScreenWidget::UBOTLoadingScreenWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	TargetAlpha = 1.0f;
	bIsDoneFading = false;
}

void UBOTLoadingScreenWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	// The Do-Once is replaced by a bool flag
	if (FMath::IsNearlyEqual(ColorAndOpacity.A, TargetAlpha, 0.0001f))
	{
		SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, TargetAlpha));
		if (bIsDoneFading)
		{
			OnDoneFading.Broadcast();;
		}
	}
	else
	{
		const float Interp = FMath::FInterpConstantTo(ColorAndOpacity.A, TargetAlpha, InDeltaTime, 1.0f);
		SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, Interp));
		bIsDoneFading = true;
	}
}

void UBOTLoadingScreenWidget::SetFade(float InTargetAlpha)
{
	TargetAlpha = InTargetAlpha;
}
