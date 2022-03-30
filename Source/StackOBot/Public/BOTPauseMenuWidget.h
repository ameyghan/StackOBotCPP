

#pragma once

#include "CoreMinimal.h"
#include "BOTHUDInterface.h"
#include "Blueprint/UserWidget.h"

#include "BOTPauseMenuWidget.generated.h"

class UBOTButtonBaseWidget;
class APlayerController;
class AHUD;

UCLASS()
class STACKOBOT_API UBOTPauseMenuWidget : public UUserWidget, public IBOTHUDInterface
{
	GENERATED_BODY()
	
public:
	UBOTPauseMenuWidget(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;
	UFUNCTION()
	void OnResumeButtonClicked();
	UFUNCTION()
	void OnSaveGameButtonClicked();
	UFUNCTION()
	void OnMainMenuButtonClicked();
	UFUNCTION()
	void OnResetMapButtonClicked();
	UFUNCTION()
	void OnQuitToWindowsButtonClicked();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UBOTButtonBaseWidget> UMGResumeButton;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UBOTButtonBaseWidget> UMGSaveGameButton;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UBOTButtonBaseWidget> UMGMainMenuButton;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UBOTButtonBaseWidget> UMGResetMapButton;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UBOTButtonBaseWidget> UMGQuitToWindowsButton;
	UPROPERTY()
	TObjectPtr<APlayerController> PlayerController;
	UPROPERTY()
	TObjectPtr<AHUD> InGameHUD;
};
