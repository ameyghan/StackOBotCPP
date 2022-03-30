

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "BOTHeadsupDisplayWidget.generated.h"

class UTextBlock;

UCLASS()
class STACKOBOT_API UBOTHeadsupDisplayWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UBOTHeadsupDisplayWidget(const FObjectInitializer& ObjectInitializer);
	virtual void NativePreConstruct() override;
	virtual void NativeOnInitialized() override;
	UFUNCTION()
	void SetOrbsText(const int32 Amount);
	UFUNCTION()
	void OnOrbsUpdated(const int32 NewAmount);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Button, meta = (BindWidget))
	TObjectPtr<UTextBlock> OrbsAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Button, meta = (BindWidget))
	FText OrbsTextBlockText;
};
