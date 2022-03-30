

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"

#include "BOTSavePlayer.generated.h"

UCLASS()
class STACKOBOT_API UBOTSavePlayer : public USaveGame
{
	GENERATED_BODY()
	
public:
	UBOTSavePlayer();
	UFUNCTION()
	FORCEINLINE int32 GetOrbs() const { return Orbs; };
	void SetOrbs(const int32 InOrbs);
 
private:
	UPROPERTY()
	int32 Orbs;
};
