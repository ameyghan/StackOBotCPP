

#include "BOTAnimInstance.h"

#include "BOTCharacter.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

void UBOTAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	Owner = TryGetPawnOwner();
}

void UBOTAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (Owner == nullptr)
	{
		return;
	}
	if (Owner != nullptr)
	{
		bIsInAir = Owner->GetMovementComponent()->IsFalling();
		const FVector2D GroundVelocity(Owner->GetMovementComponent()->Velocity.X, Owner->GetMovementComponent()->Velocity.Y);
		GroundSpeed = GroundVelocity.Size();
		CalculateLean(DeltaSeconds);
		bMovementInput = (Owner->GetLastMovementInputVector().Size() > 0);
	}
	const TObjectPtr<ABOTCharacter> Character = Cast<ABOTCharacter>(Owner);
	if (Character != nullptr)
	{
		bIsHovering = Character->IsJetpackActive();
		bIsInactive = Character->IsBOTInactive();
		InteractWorldLocation = Character->GetInteractionWorldLocation();
	}
}

void UBOTAnimInstance::CalculateLean(const float DeltaSeconds)
{
	CurrentRotation = GetOwningActor()->GetActorRotation();
	const FRotator DeltaRotation = UKismetMathLibrary::NormalizedDeltaRotator(PreviousRotation, CurrentRotation);
	LeanAmount = UKismetMathLibrary::FInterpTo(LeanAmount, DeltaRotation.Yaw, DeltaSeconds, 2.0);
	PreviousRotation = GetOwningActor()->GetActorRotation();
}

void UBOTAnimInstance::ShakeCamera()
{
	if (bShouldShakeCamera)
	{
		if (IsValid(CameraShake))
		{
			const TObjectPtr<APlayerCameraManager> CameraManager =  UGameplayStatics::GetPlayerCameraManager(this, 0);
			const FVector CameraLocation = CameraManager->GetCameraLocation();
			if (CameraManager != nullptr)
			{
				CameraManager->PlayWorldCameraShake(GetWorld(), CameraShake, CameraLocation, 500.f, 900.f, 1.0f);
			}
		}
	}
}
