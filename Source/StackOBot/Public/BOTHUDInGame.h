

#pragma once

#include "CoreMinimal.h"
#include "BOTHUDInterface.h"
#include "GameFramework/HUD.h"

#include "BOTHUDInGame.generated.h"

class UBOTPauseMenuWidget;
class UBOTLoadingScreenWidget;

UCLASS()
class STACKOBOT_API ABOTHUDInGame : public AHUD, public IBOTHUDInterface
{
	GENERATED_BODY()
	
public:
	ABOTHUDInGame(const FObjectInitializer& ObjectInitializer);
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Game)
	void SetPaused(bool bIsPaused);
	virtual void SetPaused_Implementation(bool bIsPaused) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Game)
	void LoadMenu();
	virtual void LoadMenu_Implementation() override;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Game)
	void LoadGame();
	virtual void LoadGame_Implementation() override;

	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Game)
	void ResetMap();
	virtual void ResetMap_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Game)
	void QuitGame();
	virtual void QuitGame_Implementation() override;

private:
	UPROPERTY()
	TObjectPtr<UBOTPauseMenuWidget> PauseMenuWidget;
	UPROPERTY()
	TObjectPtr<UBOTLoadingScreenWidget> LoadingScreenWidget;
	UPROPERTY()
	TSubclassOf<UBOTPauseMenuWidget> PauseMenuWidgetClass;
	UPROPERTY()
	TSubclassOf<UBOTLoadingScreenWidget> LoadingScreenWidgetClass;
	UPROPERTY()
	FName MainMenuLevelName;
	UPROPERTY()
	FName NameOfLevelToLoad;
	UFUNCTION()
	void LoadALevel(const FName& InLevelName);
};
