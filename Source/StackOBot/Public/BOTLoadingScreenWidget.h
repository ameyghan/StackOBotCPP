

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "BOTLoadingScreenWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDoneFading);

class UImage;

UCLASS()
class STACKOBOT_API UBOTLoadingScreenWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UBOTLoadingScreenWidget(const FObjectInitializer& ObjectInitializer);
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	FOnDoneFading& OnDoneFadingEvent() { return OnDoneFading; }
	UFUNCTION(BlueprintCallable)
	void SetFade(float TargetAlpha);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> Image_0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> BlackBG;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	float TargetAlpha;
	UPROPERTY()
	FOnDoneFading OnDoneFading;
	UPROPERTY()
	bool bIsDoneFading;
};
