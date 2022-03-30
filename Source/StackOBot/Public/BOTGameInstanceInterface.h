

#pragma once

#include "CoreMinimal.h"
#include "CollectibleObject.h"
#include "UObject/Interface.h"

#include "BOTGameInstanceInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UBOTGameInstanceInterface : public UInterface
{
	GENERATED_BODY()
};

class STACKOBOT_API IBOTGameInstanceInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Music)
	void PlayMainMenuMusic(float FadeInDuration, float FadeVolumeLevel);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Music)
	void PlayInGameMusic(float FadeInDuration, float FadeVolumeLevel);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Music)
	void StopInGameMusic(float FadeOutDuration, float FadeVolumeLevel);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Music)
	void StopMainMenuMusic(float FadeOutDuration, float FadeVolumeLevel);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SaveGame();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void InitSaveGame();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool ResetSaveGame(const FString& LevelName);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	int32 GetOrbAmount();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UpdateOrbs(int32 Change);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void RemoveCollectibleFromSaveGame(FCollectibleObject CollectibleData);
};
