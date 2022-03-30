

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "BOTButtonBaseWidget.generated.h"

class UTextBlock;
class UButton;

UCLASS()
class STACKOBOT_API UBOTButtonBaseWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UBOTButtonBaseWidget(const FObjectInitializer& ObjectInitializer);
	virtual void NativePreConstruct() override;
	FORCEINLINE TObjectPtr<UButton> GetBaseButton() const
	{
		return ButtonWidget;
	}
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Button, meta = (BindWidget))
	TObjectPtr<UTextBlock> Label;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Button, meta = (BindWidget))
	TObjectPtr<UButton> ButtonWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Button, meta = (BindWidget))
	FText LabelText;
};
