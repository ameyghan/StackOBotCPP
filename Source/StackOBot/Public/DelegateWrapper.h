

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

#include "DelegateWrapper.generated.h"

UCLASS()
class STACKOBOT_API UDelegateWrapper final : public UObject
{
	GENERATED_BODY()
	
public:
	void SetCallback(TUniqueFunction<void()> InCallback);
	UFUNCTION()
	void DelegateDispatch();

private:
	TUniqueFunction<void()> FuncWrapper;
};
