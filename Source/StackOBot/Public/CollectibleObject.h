 

#pragma once

#include "CollectibleObject.generated.h"

USTRUCT(BlueprintType)
struct FCollectibleObject
{
	GENERATED_BODY()

	FCollectibleObject()
		: ActorClass(nullptr),
		  Transform(FTransform())
	{}

	bool operator==(const FCollectibleObject& Other) const
	{
		return (ActorClass == Other.ActorClass && Transform.Equals(Other.Transform));
	}

	bool operator!=(const FCollectibleObject& Other) const
	{
		return !(*this == Other);
	}
	
	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<AActor> ActorClass;
	UPROPERTY(BlueprintReadWrite)
	FTransform Transform;
};