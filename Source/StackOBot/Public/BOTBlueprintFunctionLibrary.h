

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "BOTBlueprintFunctionLibrary.generated.h"

UCLASS()
class STACKOBOT_API UBOTBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category = Bots)
	static int32 GetMaxBots();
};
