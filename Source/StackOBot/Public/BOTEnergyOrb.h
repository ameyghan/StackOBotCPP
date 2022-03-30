

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "BOTEnergyOrb.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class UAudioComponent;

UCLASS()
class STACKOBOT_API ABOTEnergyOrb : public AActor
{
	GENERATED_BODY()
	
public:	
	ABOTEnergyOrb(const FObjectInitializer& ObjectInitializer);
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	void BeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(EditAnywhere, Category = Box)
	TObjectPtr<UBoxComponent> Box;
	UPROPERTY(EditAnywhere, Category = OrbMesh)
	TObjectPtr<UStaticMeshComponent> OrbMesh;
	UPROPERTY(EditAnywhere, Category = Ring)
	TObjectPtr<UStaticMeshComponent> Ring;
	// TODO setup sound after object save and load and functionality is tested and confirmed 
	// TObjectPtr<UAudioComponent> CollectOrbSound;
	TObjectPtr<AActor> PlayerCharacter;
	FTransform Transform;
};
