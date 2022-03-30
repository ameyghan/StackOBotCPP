

#pragma once

#include "CoreMinimal.h"
#include "BOTHUDInterface.h"
#include "GameFramework/HUD.h"

#include "BOTHUDMainMenu.generated.h"

class UBOTMainMenuWidget;
class UBOTLoadingScreenWidget;

UCLASS()
class STACKOBOT_API ABOTHUDMainMenu : public AHUD, public IBOTHUDInterface
{
	GENERATED_BODY()
	
public:
	ABOTHUDMainMenu(const FObjectInitializer& ObjectInitializer);
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Game)
	void LoadGame();
	virtual void LoadGame_Implementation() override;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Game)
	void QuitGame();
	virtual void QuitGame_Implementation() override;
	UFUNCTION()
	void LoadingDoneFading();
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName StartLevelName;
	UPROPERTY()
	TObjectPtr<UBOTMainMenuWidget> MainMenuWidget;
	UPROPERTY()
	TObjectPtr<UBOTLoadingScreenWidget> LoadingScreenWidget;
	UPROPERTY()
	TSubclassOf<UBOTMainMenuWidget> MainMenuWidgetClass;
	UPROPERTY()
	TSubclassOf<UBOTLoadingScreenWidget> LoadingScreenWidgetClass;
};
