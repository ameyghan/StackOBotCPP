

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "BOTHUDInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UBOTHUDInterface : public UInterface  
{
	GENERATED_BODY()
};

class STACKOBOT_API IBOTHUDInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Game)
	void LoadGame();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Game)
	void SaveGame();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Game)
	void LoadMenu();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Game)
	void QuitGame();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Game)
	void SetPaused(bool bIsPaused);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Game)
	void ResetMap();
	
};
