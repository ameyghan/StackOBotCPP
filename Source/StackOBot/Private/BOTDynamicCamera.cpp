#include "BOTDynamicCamera.h"

#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ABOTDynamicCamera::ABOTDynamicCamera()
{
	CameraRoot = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = CameraRoot;
	
	OverlapComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapComponent"));
	OverlapComponent->SetCollisionProfileName("OverlapOnlyPawn");
	OverlapComponent->SetBoxExtent(FVector(100.f));
	OverlapComponent->SetupAttachment(GetRootComponent());
	OverlapComponent->SetGenerateOverlapEvents(true);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(GetRootComponent());
	Camera->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
	
	BlendTime = 2.0f;
	CameraBlendTimer = 3.0f;
}

void ABOTDynamicCamera::BeginPlay()
{
	Super::BeginPlay();
	/** Called when the character steps on the OverlapComponent (The box) */
	OverlapComponent->OnComponentBeginOverlap.AddDynamic(this, &ABOTDynamicCamera::OnOverlapBegin);
	/** Called when the player leaves the OverlapComponent */
	OverlapComponent->OnComponentEndOverlap.AddDynamic(this, &ABOTDynamicCamera::OnOverlapEnd);
}

void ABOTDynamicCamera::BlendCameraInAndOut(ACharacter* InCharacter, UCharacterMovementComponent* InCharacterMovement)
{
	if (InCharacterMovement->IsWalking())
	{
		InCharacterMovement->DisableMovement();
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
		check(PlayerController);
		PlayerController->SetIgnoreLookInput(true);
		PlayerController->SetViewTargetWithBlend(this, BlendTime);
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([=, &TimerHandle]
		{
			check(InCharacter);
			PlayerController->SetViewTargetWithBlend(InCharacter, CameraBlendTimer);
			GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([=]
			{
				check(InCharacterMovement && PlayerController);
				InCharacterMovement->SetMovementMode(MOVE_Walking);
				PlayerController->SetIgnoreLookInput(false);
			}),CameraBlendTimer, false);
		}),CameraBlendTimer, false);
	}
}

void ABOTDynamicCamera::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacter* OverlappedCharacter = Cast<ACharacter>(OtherActor);
	UCharacterMovementComponent* CharacterMovement = OverlappedCharacter->GetCharacterMovement();
	check(OverlappedCharacter && CharacterMovement);
	BlendCameraInAndOut(OverlappedCharacter, CharacterMovement);
}

void ABOTDynamicCamera::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Destroy();
}





