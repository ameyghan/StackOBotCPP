

#pragma once

#include "CoreMinimal.h"
#include "BOTGameInstanceInterface.h"
#include "Engine/GameInstance.h"

#include "BOTGameInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOrbsUpdated, int32, NewAmount);

class UAudioComponent;
class USoundBase;
class USaveGame;
class UBOTSavePlayer;
class UBOTSaveLevel;

UCLASS()
class STACKOBOT_API UBOTGameInstance : public UGameInstance, public IBOTGameInstanceInterface
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Music)
	void PlayMainMenuMusic(float FadeInDuration, float FadeVolumeLevel);
	virtual void PlayMainMenuMusic_Implementation(float FadeInDuration, float FadeVolumeLevel) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Music)
	void PlayInGameMusic(float FadeInDuration, float FadeVolumeLevel);
	virtual void PlayInGameMusic_Implementation(float FadeInDuration, float FadeVolumeLevel) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Music)
	void StopMainMenuMusic(float FadeOutDuration, float FadeVolumeLevel);
	virtual void StopMainMenuMusic_Implementation(float FadeOutDuration, float FadeVolumeLevel) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Music)
	void StopInGameMusic(float FadeOutDuration, float FadeVolumeLevel);
	virtual void StopInGameMusic_Implementation(float FadeOutDuration, float FadeVolumeLevel) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Game)
	void InitSaveGame();
	virtual void InitSaveGame_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Orbs)
	int32 GetOrbAmount();
	virtual int32 GetOrbAmount_Implementation() override;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Orbs)
	void UpdateOrbs(int32 Change);
	virtual void UpdateOrbs_Implementation(int32 Change) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Collectible)
	void RemoveCollectibleFromSaveGame(FCollectibleObject CollectibleData);
	virtual void RemoveCollectibleFromSaveGame_Implementation(FCollectibleObject CollectibleData) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Save)
	void SaveGame();
	virtual void SaveGame_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Save)
	bool ResetSaveGame(const FString& LevelName);
	virtual bool ResetSaveGame_Implementation(const FString& LevelName) override;
	

	void InitPlayerSaveData();
	void LoadPlayerSaveData();
	void SaveGameToSlots() const;
	void InitLevelSaveData();

	FOnOrbsUpdated& OnOrbsUpdatedEvent()
	{
		return OnOrbsUpdated;
	}
	
	FORCEINLINE TObjectPtr<UAudioComponent> GetInGameMusic() const
	{
		return InGameMusic;
	}
	FORCEINLINE TObjectPtr<UAudioComponent> GetMainMenuMusic() const
	{
		return MainMenuMusic;
	}

	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Music")
	TObjectPtr<USoundBase> MainMenuMusicFile;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Music")
	TObjectPtr<USoundBase> InGameMusicFile;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Music")
	TObjectPtr<UAudioComponent> InGameMusic;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Music")
	TObjectPtr<UAudioComponent> MainMenuMusic;
	
private:
	UPROPERTY()
	int32 CollectedOrbs;
	UPROPERTY()
	FString SaveGameSlotName;
	
	FOnOrbsUpdated OnOrbsUpdated;
	
	UPROPERTY()
	TObjectPtr<UBOTSavePlayer> PlayerInventorySaveGame;
	UPROPERTY()
	TObjectPtr<UBOTSaveLevel> LevelSave; 
};
