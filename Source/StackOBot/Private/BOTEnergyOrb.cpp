


#include "BOTEnergyOrb.h"

#include "BOTGameInstance.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NavAreas/NavArea_Obstacle.h"

ABOTEnergyOrb::	ABOTEnergyOrb(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	SetRootComponent(Box);
	Box->SetBoxExtent(FVector(92.0f, 92.0f, 32.0f));
	Box->AreaClass = UNavArea_Obstacle::StaticClass();
	Box->SetCollisionProfileName("OverlapOnlyPawn");
	
	OrbMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OrbMesh"));
	OrbMesh->SetupAttachment(Box);
	OrbMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -17.50));
	OrbMesh->SetHiddenInGame(false);
	OrbMesh->SetRelativeScale3D(FVector(0.75f));
	OrbMesh->SetGenerateOverlapEvents(false);
	OrbMesh->SetCollisionProfileName("NoCollision");
	
	Ring = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ring"));
	Ring->SetupAttachment(OrbMesh);
	Ring->SetRelativeRotation(FRotator(0.0f, 0.0f, 30.0f));
	Ring->SetRelativeScale3D(FVector(1.33));
}

void ABOTEnergyOrb::BeginPlay()
{
	Super::BeginPlay();
	OnActorBeginOverlap.AddDynamic(this, &ABOTEnergyOrb::BeginOverlap);
	const float NewYaw = (FMath::RandRange(0, 15) * 22.5f);
	OrbMesh->SetWorldRotation(FRotator(0.0f, NewYaw,0.0f));
}

void ABOTEnergyOrb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	const float NewYaw = DeltaTime * 100.f;
	OrbMesh->AddLocalRotation(FRotator(0.0f, NewYaw, 0.0f));
}

void ABOTEnergyOrb::BeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	PlayerCharacter = OtherActor;
	if (const TObjectPtr<UGameInstance> BOTGameInstance = Cast<UBOTGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		if (BOTGameInstance->GetClass()->ImplementsInterface(UBOTGameInstanceInterface::StaticClass()))
		{
			IBOTGameInstanceInterface::Execute_UpdateOrbs(BOTGameInstance, 1);
			FCollectibleObject Collectible;
			Collectible.ActorClass = this->GetClass();
			Collectible.Transform = this->GetActorTransform();
			IBOTGameInstanceInterface::Execute_RemoveCollectibleFromSaveGame(BOTGameInstance, Collectible);
			Box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			//TODO setup sound and visuals once functionality is tested.
			Destroy();
		}
	}
}


