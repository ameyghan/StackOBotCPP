

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"

#include "BOTAnimInstance.generated.h"

UCLASS()
class STACKOBOT_API UBOTAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	virtual void CalculateLean(const float DeltaSeconds);
	UFUNCTION(BlueprintCallable)
	void ShakeCamera();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = true))
	float GroundSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = true))
	bool bIsInAir;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = true))
	FRotator PreviousRotation{0.0f,0.0f,0.0f};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = true))
	float LeanAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = true))
	bool bMovementInput;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = true))
	bool bIsHovering;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = true))
	float WorldOrientation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = true))
	FRotator CurrentRotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = true))
	bool bIsInactive;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = true))
	FVector InteractWorldLocation;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = true))
	TObjectPtr<APawn> Owner;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = true))
	TSubclassOf<class UCameraShakeBase> CameraShake;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = true))
	bool bShouldShakeCamera;
};
