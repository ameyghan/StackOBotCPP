

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"

#include "BOTCharacter.generated.h"

class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class USpringArmComponent;
class UInputAction;
class UInputMappingContext;
class UAudioComponent;
class UNiagaraComponent;
class UCurveFloat;
class UMaterialInstanceDynamic;
class UMaterialParameterCollection;
class APlayerCameraManager;
class UCurveFloat;
class UTimelineComponent;

UENUM(BlueprintType)
enum class EJetpackState : uint8
{
	EJS_Inactive		UMETA(DisplayName = "Inactive"),
	EJS_Ready			UMETA(DisplayName = "Ready"),
	EJS_Activated		UMETA(DisplayName = "Active"),
	EJS_InAir			UMETA(DisplayName = "InAir"),
	EJS_Deactivated		UMETA(DisplayName = "Deactivated"),
	EJS_CoolingOff		UMETA(DisplayName = "CollingOff"),
	EJS_Recharging		UMETA(DisplayName = "Recharging"),

	UES_MAX				UMETA(DisplayName = "DefaultMAX")
};

UCLASS(config=Game)
class STACKOBOT_API ABOTCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABOTCharacter();
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void PawnClientRestart() override;
	void UpdateCamera(const float DeltaSeconds) const;
	void UpdateJetpack(float DeltaTime);
	void ToggleJetpack(bool Reset = false, bool Activate = false);

	void LaunchBOT();
	void ToggleLaunch();

	UFUNCTION()
	void ReactToFoliage();
	float GetHueByIndex() const;

	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE bool IsJetpackActive() const { return bIsJetpackActive; }
	FORCEINLINE bool IsBOTInactive() const { return bIsInactive; }
	FORCEINLINE FVector GetInteractionWorldLocation() const { return InteractionWorldLocation; }


protected:
	virtual void BeginPlay() override;

#pragma region INPUT

	UPROPERTY(EditDefaultsOnly, Category = "BOTBOT | Inputs | Movement")
	TObjectPtr<UInputAction> MoveForwardAction;
	UPROPERTY(EditDefaultsOnly, Category = "BOTBOT | Inputs | Movement")
	TObjectPtr<UInputAction> MoveRightAction;
	UPROPERTY(EditDefaultsOnly, Category = "BOTBOT | Inputs | Movement")
	TObjectPtr<UInputAction> LookAction;
	UPROPERTY(EditDefaultsOnly, Category = "BOTBOT | Inputs | Movement")
	TObjectPtr<UInputAction> TurnAction;
	UPROPERTY(EditDefaultsOnly, Category = "BOTBOT | Inputs | Interactions")
	TObjectPtr<UInputAction> JumpAction;
	UPROPERTY(EditDefaultsOnly, Category = "BOTBOT | Inputs | Interactions")
	TObjectPtr<UInputAction> InteractAction;
	UPROPERTY(EditDefaultsOnly, Category = "BOTBOT | Inputs | Interactions")
	TObjectPtr<UInputAction> PauseAction;
	UPROPERTY(EditDefaultsOnly, Category = "BOTBOT | Inputs | Menus")
	TObjectPtr<UInputAction> ToggleHUDAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BOTBOT | Location", meta = (AllowPrivateAccess = true))
	FVector InteractionWorldLocation;
	

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BOTBOT | Input Mappings")
	UInputMappingContext* BaseInputMappingContext;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BOTBOT | Input Mappings")
	int32 BaseInputMappingPriority = 0;

	void EnhanceMoveForward(const FInputActionValue& Value);
	void EnhanceMoveRight(const FInputActionValue& Value);
	void EnhanceLook(const FInputActionValue& Value);
	void EnhanceTurn(const FInputActionValue& Value);
	void EnhanceJumpStarted(const FInputActionValue& Value);
	void EnhanceJumpCanceled(const FInputActionValue& Value);
	void EnhanceJumpCompleted(const FInputActionValue& Value);
	void EnhanceInteract(const FInputActionValue& Value);
	virtual void Landed(const FHitResult& Hit) override;

	UFUNCTION()
	void EnableSpawnEffect(float EnablePrinting, float SliceZ);
#pragma endregion 
	
#pragma region JETPACK
public:

	float JetpackFuelCurvePlaybackPosition;
	float JetpackFuelCurveFloatValue;
	float JetpackFuelPercentage;

	UPROPERTY(EditAnywhere, Category = "BOTBOT | Jetpack | Stats")
	bool bShowLaunchCapsule;
	UPROPERTY(EditAnywhere, Category = "BOTBOT | Jetpack | Stats")
	bool bShowJetpackStats;
	UPROPERTY(VisibleAnywhere, Category = "BOTBOT | Jetpack")
	bool bIsJetpackDepleted;

	UPROPERTY(VisibleAnywhere, Category = "BOTBOT | Jetpack")
	EJetpackState JetpackState;

	UPROPERTY(EditDefaultsOnly, Category = "BOTBOT | Jetpack", meta = (AllowPrivateAccess = true))
	TObjectPtr<UTimelineComponent> JetpackFuelTimeline;
	
	FORCEINLINE float GetCurrentThrusterTime() const { return CurrentThrusterTime; }
	FORCEINLINE float GetMaxThrusterTime() const { return MaxThrusterTime; }
	UFUNCTION(BlueprintPure, Category = "BOTBOT | Jetpack")
	float GetJetpackFuelPercentage();
	UFUNCTION(Category = "BOTBOT | Jetpack")
	void ShowJetpackDebugStats();
	UFUNCTION(Exec, Category = "BOTBOT | Jetpack")
	void ToggleJetpackDebugStats();
	UFUNCTION()
	void OnJetpackFuelTimelineCallbackStarted();
	UFUNCTION()
	void OnJetpackFuelTimelineCallbackFinished();
	UFUNCTION()
	void ResetJetpackFuel();
	void ShowDistanceFromFloor(const float Distance);
	
	float ThrusterPercentage;
	float FloorDistance;
	FTimerHandle JetpackFuelTimerHandle;

	UPROPERTY(EditDefaultsOnly, Category = "BOTBOT | Jetpack")
	float FloorHeight;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BOTBOT | Jetpack", meta = (AllowPrivateAccess = true))
	float CurrentThrusterTime;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BOTBOT | Jetpack", meta = (AllowPrivateAccess = true))
	float MaxThrusterTime;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BOTBOT | Jetpack", meta = (AllowPrivateAccess = true))
	float JetpackFlightHeight;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BOTBOT | Jetpack", meta = (AllowPrivateAccess = true))
	float JetpackLaunchUpStrength;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BOTBOT | Jetpack", meta = (AllowPrivateAccess = true))
	float JetpackLaunchForwardStrength;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BOTBOT | Jetpack", meta = (AllowPrivateAccess = true))
	bool bIsLaunched;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BOTBOT | Jetpack", meta = (AllowPrivateAccess = true))
	float LaunchAngle;
	UFUNCTION(BlueprintPure)
	FText GetJetpackFuelText();
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BOTBOT | Jetpack", meta = (AllowPrivateAccess = true))
	bool bVerticalHUD;
	UFUNCTION(BlueprintCallable)
	void ToggleVerticalHUD();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BOTBOT | Jetpack", meta = (AllowPrivateAccess = true))
	TObjectPtr<USkeletalMeshComponent> Jetpack;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BOTBOT | Jetpack", meta = (AllowPrivateAccess = true))
	TObjectPtr<UAudioComponent> JetpackSoundFX;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BOTBOT | Jetpack", meta = (AllowPrivateAccess = true))
	TObjectPtr<UNiagaraComponent> FX_JetpackThruster;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BOTBOT | Jetpack", meta = (AllowPrivateAccess = true))
	bool bIsJetpackActive;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BOTBOT | Jetpack", meta = (AllowPrivateAccess = true))
	TObjectPtr<UCurveFloat> JetpackBoostCurve;
	UPROPERTY(EditDefaultsOnly, Category = "BOTBOT | Jetpack")
	float SecondsToResetJetpackFuel;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BOTBOT | Jetpack | States", meta = (AllowPrivateAccess = true))
	bool bIsInactive;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BOTBOT | Jetpack", meta = (AllowPrivateAccess = true))
	bool bInfiniteJetpack;
	UPROPERTY(EditDefaultsOnly, Category = "BOTBOT | Jetpack", meta = (AllowPrivateAccess = true))
	TObjectPtr<UCurveFloat> JetpackFuelCurve;
	


#pragma endregion

#pragma region CHARACTER


private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BOTBOT | Camera", meta = (AllowPrivateAccess = true))
	UCameraComponent* FollowCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BOTBOT | Camera", meta = (AllowPrivateAccess = true))
	USpringArmComponent* CameraBoom;
	UPROPERTY(BlueprintReadOnly, Category = "BOTBOT | Camera", meta = (AllowPrivateAccess = true))
	TObjectPtr<APlayerCameraManager> CameraManager;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BOTBOT | Camera", meta = (AllowPrivateAccess = true))
	float GroundCameraLength;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BOTBOT | Camera", meta = (AllowPrivateAccess = true))
	float AirCameraLength;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BOTBOT | Movement", meta = (AllowPrivateAccess = true))
	float BaseTurnRate;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BOTBOT | Movement", meta = (AllowPrivateAccess = true))
	float BaseLookUpRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BOTBOT | Materials", meta = (AllowPrivateAccess = true))
	TArray<TObjectPtr<class UMaterialInstanceDynamic>> Materials;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BOTBOT | Materials", meta = (AllowPrivateAccess = true))
	TObjectPtr<UMaterialParameterCollection> MaterialCollection;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BOTBOT | Index", meta = (AllowPrivateAccess = true))
	int32 BotIdx;
	
#pragma endregion
 
};
