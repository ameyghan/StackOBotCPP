

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"

#include "BOTPlayerControllerInGame.generated.h"

class UInputAction;
class UInputMappingContext;
class UBOTHeadsupDisplayWidget;

UCLASS()
class STACKOBOT_API ABOTPlayerControllerInGame : public APlayerController
{
	GENERATED_BODY()
	
public:
	ABOTPlayerControllerInGame(const FObjectInitializer& ObjectInitializer);
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	UFUNCTION(BlueprintCallable)
	void EnhancePauseGame(const FInputActionValue& ActionValue);
	UPROPERTY()
	TObjectPtr<UInputMappingContext> PCMappingContext;
	UPROPERTY()
	bool bIsUsingGamepad;
	UPROPERTY()
	TObjectPtr<UBOTHeadsupDisplayWidget> HeadsupDisplayWidget;
	UPROPERTY()
	TSubclassOf<UBOTHeadsupDisplayWidget> HeadsupDisplayWidgetClass;
};
