

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "CollectibleObject.h"

#include "BOTSaveLevel.generated.h"

UCLASS()
class STACKOBOT_API UBOTSaveLevel : public USaveGame
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void AddCollectibles(FCollectibleObject Collectibles);
	UFUNCTION()
	void RemoveCollectibles(const FCollectibleObject& Collectibles);
	UFUNCTION()
	FORCEINLINE TArray<FCollectibleObject> GetCollectiblesOnMap() const { return CollectablePickups; };
	
private:
	UPROPERTY()
	TArray<FCollectibleObject> CollectablePickups;
};
