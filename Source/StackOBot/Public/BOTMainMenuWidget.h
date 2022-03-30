

#pragma once

#include "CoreMinimal.h"
#include "BOTHUDInterface.h"
#include "Blueprint/UserWidget.h"

#include "BOTMainMenuWidget.generated.h"

class UBOTButtonBaseWidget;

UCLASS()
class STACKOBOT_API UBOTMainMenuWidget : public UUserWidget, public IBOTHUDInterface
{
	GENERATED_BODY()
	
public:
	UBOTMainMenuWidget(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;
	UFUNCTION()
	void OnStartButtonClicked();
	UFUNCTION()
	void OnQuitButtonClicked();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UBOTButtonBaseWidget> UMGStartButton;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UBOTButtonBaseWidget> UMGQuitButton;
};
