

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "BOTInGameGameModeBase.generated.h"

UCLASS()
class STACKOBOT_API ABOTInGameGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABOTInGameGameModeBase();
	virtual void BeginPlay() override;
};
