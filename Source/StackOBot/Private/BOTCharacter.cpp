
#include "BOTCharacter.h"

#include "BOTLog.h"
#include "BOTBlueprintFunctionLibrary.h"
#include "BOTPlayerControllerInGame.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "NiagaraComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "TimerManager.h"
#include "Components/AudioComponent.h"


ABOTCharacter::ABOTCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	CurrentThrusterTime = 2.0f;
	MaxThrusterTime = 2.0f;
	CurrentThrusterTime = MaxThrusterTime;
	SecondsToResetJetpackFuel = 1.5f;
	JetpackFlightHeight = 600.f;
	JetpackLaunchForwardStrength = 900.f;
	JetpackLaunchUpStrength = 700.f;
	JetpackState = EJetpackState::EJS_Ready;
	bShowJetpackStats = true;
	GroundCameraLength = 600.f;
	AirCameraLength = 700.f;
	JetpackFuelPercentage = 1.0f;
	FloorHeight = 160.0f;
	FloorDistance = 0.0f;
	bShowLaunchCapsule = false;
	bInfiniteJetpack = false;
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	bFindCameraComponentWhenViewTarget = true;
	LaunchAngle = 35.0f;
	
	bVerticalHUD = true;
	
	GetMesh()->SetLinearDamping(1.0f);
	GetMesh()->SetAngularDamping(3.0f);
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.000856f, -62.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, 270.0f, 0.0f));
	GetMesh()->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_Yes;
	GetMesh()->SetGenerateOverlapEvents(true);
	GetMesh()->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	GetMesh()->SetEnableGravity(true);
	
	GetCapsuleComponent()->SetCapsuleHalfHeight(60.0f);
	GetCapsuleComponent()->SetCapsuleRadius(35.0f);
	GetCapsuleComponent()->SetMassOverrideInKg(NAME_None, 60.0f);
	GetCapsuleComponent()->bIgnoreRadialImpulse = true;
	GetCapsuleComponent()->bIgnoreRadialForce = true;
	GetCapsuleComponent()->SetNotifyRigidBodyCollision(true);
	GetCapsuleComponent()->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_Yes;
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetRelativeLocation(FVector(8.86f, 0.0f, 30.99f));
	CameraBoom->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	CameraBoom->TargetArmLength = 230.0f;
	CameraBoom->ProbeSize = 0.1f;
	CameraBoom->ProbeChannel = ECollisionChannel::ECC_Visibility;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	Jetpack = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Jetpack"));
	Jetpack->SetupAttachment(GetMesh(), FName(TEXT("BackpackSocket")));
	
	if (Jetpack)
	{
		Jetpack->SetEnableGravity(false);
		Jetpack->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
		Jetpack->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Jetpack->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
		Jetpack->SetCollisionResponseToAllChannels(ECR_Block);
		Jetpack->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
		Jetpack->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
		Jetpack->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Overlap);

		FX_JetpackThruster = CreateDefaultSubobject<UNiagaraComponent>(TEXT("JetpackThruster"));
		FX_JetpackThruster->SetupAttachment(Jetpack, FName(TEXT("ThrusterSocketLeft")));
		FX_JetpackThruster->SetAutoActivate(false);
	}
	
	JetpackSoundFX = CreateDefaultSubobject<UAudioComponent>(TEXT("JetpackSoundSFX"));
	JetpackSoundFX->SetAutoActivate(false);
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 700.0f, 0.0f);
	GetCharacterMovement()->GravityScale = 2.5;
	GetCharacterMovement()->MaxAcceleration = 1500.0f;
	GetCharacterMovement()->bUseSeparateBrakingFriction = true;
	GetCharacterMovement()->Mass = 60.0f;
	GetCharacterMovement()->SetWalkableFloorAngle(45.0f);
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	GetCharacterMovement()->JumpZVelocity = 900.0f;
	GetCharacterMovement()->AirControl = 1.0f;
	GetCharacterMovement()->AirControlBoostMultiplier = 5.0f;
	GetCharacterMovement()->StandingDownwardForceScale = 0.0f;
	GetCharacterMovement()->GetNavAgentPropertiesRef().AgentRadius = 42.0f;
	GetCharacterMovement()->GetNavAgentPropertiesRef().AgentHeight = 192.0f;
	GetCharacterMovement()->GetNavAgentPropertiesRef().AgentStepHeight = -1.0f;
	GetCharacterMovement()->GetNavAgentPropertiesRef().NavWalkingSearchHeightScale = 0.5f;
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = false;
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanJump = true;
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanWalk = true;
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanSwim = true;
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanFly = false;
}

void ABOTCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void ABOTCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (Materials.IsEmpty())
    {
    	if (GetMesh()->CreateDynamicMaterialInstance(0) != nullptr && GetMesh()->CreateDynamicMaterialInstance(1))
    	{
    		Materials.Add(GetMesh()->CreateDynamicMaterialInstance(0));
    		GetMesh()->CreateDynamicMaterialInstance(0)->SetScalarParameterValue(FName(TEXT("HueShift")), GetHueByIndex());
    		Materials.Add(GetMesh()->CreateDynamicMaterialInstance(1));
    	}
    	if (Jetpack)
    	{
    		if (Jetpack->CreateDynamicMaterialInstance(0) != nullptr)
    		{
    			Materials.Add(Jetpack->CreateDynamicMaterialInstance(0));
    		}
    	}
    }
	JetpackFuelTimeline = NewObject<UTimelineComponent>(this, FName("JetpackFuelTimeline"));

	if (JetpackFuelTimeline)
	{
		FOnTimelineFloat JetpackFuelTimelineCallback;
		FOnTimelineEvent JetpackFuelTimelineFinished;
		JetpackFuelTimelineCallback.BindUFunction(this, FName("OnJetpackFuelTimelineCallbackStarted"));
		JetpackFuelTimelineFinished.BindUFunction(this, FName("OnJetpackFuelTimelineCallbackFinished"));
		if (JetpackFuelCurve)
		{
			JetpackFuelTimeline->AddInterpFloat(JetpackFuelCurve, JetpackFuelTimelineCallback, FName("Jetpack Fuel Reset Bar"));
			JetpackFuelTimeline->SetTimelineFinishedFunc(JetpackFuelTimelineFinished);
			JetpackFuelTimeline->SetLooping(false);
			JetpackFuelTimeline->SetIgnoreTimeDilation(false);
			JetpackFuelTimeline->RegisterComponent();
		}
	}
}

void ABOTCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bShowJetpackStats)
	{
		ShowJetpackDebugStats();
	}
	if (JetpackFuelTimeline)
	{
		JetpackFuelTimeline->TickComponent(DeltaTime, ELevelTick::LEVELTICK_TimeOnly, nullptr);
	}
	UpdateCamera(DeltaTime);
	ReactToFoliage();
	UpdateJetpack(DeltaTime);
	FBOTScreenLogger::AddOnScreenDebugMessage(32, ELogVerbosity::Display, *FString::Printf(TEXT("Max Speed : %.2f"), GetCharacterMovement()->GetMaxSpeed()));
	ShowDistanceFromFloor(FloorDistance);
}

void ABOTCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* PlayerEnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		
		// There are ways to bind a UInputAction* to a handler function and multiple types of ETriggerEvent that may be of interest.
		// This calls the handler function on the tick when MyInputAction starts, such as when pressing an action button.
		if (MoveForwardAction)
		{
			PlayerEnhancedInputComponent->BindAction(MoveForwardAction, ETriggerEvent::Triggered, this, &ABOTCharacter::EnhanceMoveForward);
		}
		// We can also call the handler function (a UFUNCTION) by name on every tick while the input conditions are met, such as when holding a movement key down.
		// e.g PlayerEnhancedInputComponent->BindAction(MoveRightAction, ETriggerEvent::Triggered, this, TEXT("MyOtherInputHandlerFunction"));
		if (MoveRightAction)
		{
			PlayerEnhancedInputComponent->BindAction(MoveRightAction, ETriggerEvent::Triggered, this, &ABOTCharacter::EnhanceMoveRight);
		}
		if (LookAction)
		{
			PlayerEnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABOTCharacter::EnhanceLook);
		}
		if (TurnAction)
		{
			PlayerEnhancedInputComponent->BindAction(TurnAction, ETriggerEvent::Triggered, this, &ABOTCharacter::EnhanceTurn);
		}
		if (JumpAction)
		{
			PlayerEnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ABOTCharacter::EnhanceJumpStarted);
			PlayerEnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Canceled, this, &ABOTCharacter::EnhanceJumpCanceled);
			PlayerEnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ABOTCharacter::EnhanceJumpCompleted);
		}
		if (InteractAction)
		{
			PlayerEnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ABOTCharacter::EnhanceInteract);
		}
		if (ToggleHUDAction)
		{
			PlayerEnhancedInputComponent->BindAction(ToggleHUDAction, ETriggerEvent::Started, this, &ABOTCharacter::ToggleVerticalHUD);
		}
	}
}

void ABOTCharacter::PawnClientRestart()
{
	Super::PawnClientRestart();
	if (const ABOTPlayerControllerInGame* InGamePlayerController = Cast<ABOTPlayerControllerInGame>(GetController()))
	// if (const APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		// Get the Enhanced Input Local Player Subsystem from the Local Player related to our Player Controller.
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(InGamePlayerController->GetLocalPlayer()))
		{
			// PawnClientRestart can run more than once in an Actor's lifetime, so start by clearing out any leftover mappings.
			Subsystem->ClearAllMappings();

			// Add each mapping context, along with their priority values. Higher values outprioritize lower values.
			Subsystem->AddMappingContext(InGamePlayerController->PCMappingContext/*BaseInputMappingContext*/, BaseInputMappingPriority);
			Subsystem->AddMappingContext(BaseInputMappingContext, 1);
		}
	}
}

float ABOTCharacter::GetJetpackFuelPercentage()
{
	JetpackFuelPercentage = UKismetMathLibrary::NormalizeToRange(CurrentThrusterTime, 0.0f, MaxThrusterTime);
	return FMath::Clamp(JetpackFuelPercentage, 0.0f, 1.0f);
}

void ABOTCharacter::UpdateCamera(const float DeltaSeconds) const
{
	const FVector StartTrace = GetActorLocation();
	const FVector EndTrace = StartTrace + FVector(0.0f, 0.0f, 250.0f);
	TArray<TObjectPtr<AActor>> ActorsToIgnore;
	FHitResult HitResult;
	const bool bTraceHit = UKismetSystemLibrary::LineTraceSingle(this, StartTrace, EndTrace,UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Camera),false, ActorsToIgnore, EDrawDebugTrace::None, HitResult, true,FLinearColor::Red, FLinearColor::Green, 0.1f);
	const float AirCameraOut = UKismetMathLibrary::SelectFloat(AirCameraLength, GroundCameraLength, GetCharacterMovement()->IsFalling());
	const float TargetInterpolationRate = (bTraceHit ? GroundCameraLength : AirCameraOut);
	const float SpringArmLenght = GetCameraBoom()->TargetArmLength;
	const float FinalSpringArmLength = UKismetMathLibrary::FInterpTo(SpringArmLenght, TargetInterpolationRate, DeltaSeconds, 2.0f);
	GetCameraBoom()->TargetArmLength = FinalSpringArmLength;
}

void ABOTCharacter::UpdateJetpack(float DeltaTime)
{
	if (bIsJetpackActive)
	{
		if (bInfiniteJetpack)
		{
			CurrentThrusterTime = 100.f;
			MaxThrusterTime = 100.f;
		}
		CameraBoom->bUsePawnControlRotation = true;
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		CurrentThrusterTime = CurrentThrusterTime - DeltaTime;
		ThrusterPercentage = UKismetMathLibrary::NormalizeToRange((MaxThrusterTime - CurrentThrusterTime), 0.0f, MaxThrusterTime);
		if (JetpackBoostCurve)
		{
			const float JetpackVelocity = ((JetpackBoostCurve->GetFloatValue(ThrusterPercentage) * JetpackFlightHeight));
			LaunchCharacter(FVector(0.0f, 0.0f, JetpackVelocity), false, true);
			if (FX_JetpackThruster)
			{
				FX_JetpackThruster->SetVariableFloat(FName(TEXT("ThrusterStrength")), ThrusterPercentage);
			}
		
			if (CurrentThrusterTime <= 0.0f)
			{
				JetpackState = EJetpackState::EJS_Deactivated;
				ToggleJetpack(false, false);
			}
		}
	}
}

void ABOTCharacter::ToggleJetpack(bool Reset, bool Activate)
{
	FBOTScreenLogger::AddOnScreenDebugMessage(13, ELogVerbosity::Display, *FString::Printf(TEXT("Jetpack Reset : %s"), ((Reset) ? (TEXT("True") ): (TEXT("False")))));
	FBOTScreenLogger::AddOnScreenDebugMessage(14, ELogVerbosity::Display, *FString::Printf(TEXT("Jetpack Active : %s"), ((Activate) ? (TEXT("True") ): (TEXT("False")))));
	if (Reset == true )
	{
		// TODO convert to GameplayTags once testing passes 100%
		if (JetpackState == EJetpackState::EJS_CoolingOff)
		{
			return;
		}
		if (CurrentThrusterTime == MaxThrusterTime)
		{
			JetpackState = EJetpackState::EJS_Ready;
		}
		else if (CurrentThrusterTime < MaxThrusterTime)
		{
			JetpackState = EJetpackState::EJS_CoolingOff;
		}
		if (JetpackState == EJetpackState::EJS_CoolingOff && JetpackState != EJetpackState::EJS_Deactivated ) 
		{
			GetWorldTimerManager().ClearTimer(JetpackFuelTimerHandle);
			GetWorldTimerManager().SetTimer(JetpackFuelTimerHandle, this, &ABOTCharacter::ResetJetpackFuel, SecondsToResetJetpackFuel,false);
		}
	}
	else
	{
		bIsJetpackActive = Activate;
	}
	
	if (bIsJetpackActive)
	{
		if (JetpackFuelTimeline)
		{
			JetpackFuelTimeline->Stop();
		}
		JetpackState = EJetpackState::EJS_Activated;
		GetCharacterMovement()->AirControl = 1.0f;
		if (FX_JetpackThruster)
		{
			FX_JetpackThruster->SetActive(true, false);
			if (JetpackSoundFX)
			{
				JetpackSoundFX->Play();
			}
		}
	}
	else if (!bIsJetpackActive)
	{
		GetCharacterMovement()->AirControl = 0.0f;
		if (FX_JetpackThruster)
		{
			FX_JetpackThruster->SetActive(false, false);
			if (JetpackSoundFX)
			{
				if (JetpackSoundFX->IsPlaying() && !bIsJetpackActive)
				{
					const TScriptInterface<IAudioParameterInterface> AudioInterface = JetpackSoundFX->GetParameterInterface();
					if (AudioInterface)
					{
						AudioInterface->Trigger(FName(TEXT("JetpackOff")));
					}
				}
			}
		}
	}
}

void ABOTCharacter::ShowJetpackDebugStats()
{
	FBOTScreenLogger::AddOnScreenDebugMessage(1, ELogVerbosity::Display, *FString::Printf(TEXT("Current Thrust Timer : %.2f"), CurrentThrusterTime));
	FBOTScreenLogger::AddOnScreenDebugMessage(2, ELogVerbosity::Display, *FString::Printf(TEXT("Speed : %.2f units/cm"), GetMovementComponent()->Velocity.Size()));
	FBOTScreenLogger::AddOnScreenDebugMessage(3, ELogVerbosity::Display, *FString::Printf(TEXT("Max Thruster Time %.2f."), MaxThrusterTime));
	FBOTScreenLogger::AddOnScreenDebugMessage(5, ELogVerbosity::Display, *FString::Printf(TEXT("Jetpack Fuel Percentage %.0f %%."), GetJetpackFuelPercentage() * 100.f));
	FBOTScreenLogger::AddOnScreenDebugMessage(9, ELogVerbosity::Display, FString::Printf(TEXT("Jetpack State %s"), *StaticEnum<EJetpackState>()->GetValueAsString(JetpackState)));
	FBOTScreenLogger::AddOnScreenDebugMessage(10, ELogVerbosity::Display, *FString::Printf(TEXT("JetpackFuelCurvePlaybackPosition %.2f."), JetpackFuelCurvePlaybackPosition));
	FBOTScreenLogger::AddOnScreenDebugMessage(11, ELogVerbosity::Display, *FString::Printf(TEXT("JetpackFuelCurveFloatValue %.2f."), JetpackFuelCurveFloatValue));
}

void ABOTCharacter::ToggleJetpackDebugStats()
{
	bShowJetpackStats = !bShowJetpackStats;
	if (!bShowJetpackStats)
	{
		GEngine->RemoveOnScreenDebugMessage(1);
		GEngine->RemoveOnScreenDebugMessage(2);
		GEngine->RemoveOnScreenDebugMessage(3);
		GEngine->RemoveOnScreenDebugMessage(5);
		GEngine->RemoveOnScreenDebugMessage(9);
		GEngine->RemoveOnScreenDebugMessage(10);
		GEngine->RemoveOnScreenDebugMessage(11);
		GEngine->RemoveOnScreenDebugMessage(11);
	}
}

void ABOTCharacter::OnJetpackFuelTimelineCallbackStarted()
{
	if ( !bIsJetpackActive && ( (GetCharacterMovement()->MovementMode != EMovementMode::MOVE_Flying) || (GetCharacterMovement()->MovementMode != EMovementMode::MOVE_Falling) ) ) /*&& (JetpackState != EJetpackState::EJS_Recharging) && (JetpackState != EJetpackState::EJS_CoolingOff)*/ 
	{
		JetpackFuelCurvePlaybackPosition = JetpackFuelTimeline->GetPlaybackPosition();
		JetpackFuelCurveFloatValue = JetpackFuelCurve->GetFloatValue(JetpackFuelCurvePlaybackPosition);
		JetpackFuelCurveFloatValue = JetpackFuelCurvePlaybackPosition;
		JetpackFuelCurveFloatValue = FMath::Clamp(JetpackFuelCurveFloatValue, 0.0f, 2.0f);
		FBOTScreenLogger::AddOnScreenDebugMessage(12, ELogVerbosity::Warning, *FString::Printf(TEXT("JetpackFuelCurveFloatValue from %s() : %.2f."), *FString(__FUNCTION__), JetpackFuelCurveFloatValue), 1.0f);
		CurrentThrusterTime = UKismetMathLibrary::FInterpTo_Constant(CurrentThrusterTime, MaxThrusterTime, GetWorld()->GetDeltaSeconds(), JetpackFuelCurveFloatValue);
		
		if (CurrentThrusterTime >= MaxThrusterTime)
		{
			if (JetpackFuelTimeline != nullptr)
			{
				JetpackFuelTimeline->Stop();
				JetpackState = EJetpackState::EJS_Ready;
			}
		}
	}
}

void ABOTCharacter::OnJetpackFuelTimelineCallbackFinished()
{
	FBOTScreenLogger::AddOnScreenDebugMessage(18, ELogVerbosity::Error, *FString::Printf(TEXT("Timeline Finished")));
	JetpackState = EJetpackState::EJS_Ready;
}

void ABOTCharacter::ResetJetpackFuel()
{
	if (JetpackState == EJetpackState::EJS_Activated)
	{
		return;
	}
	JetpackState = EJetpackState::EJS_Recharging;
	if (JetpackFuelTimeline)
	{
		JetpackFuelTimeline->PlayFromStart();
	}
}

void ABOTCharacter::ShowDistanceFromFloor(const float Distance)
{
	const FVector BotCapsuleLocation = GetCapsuleComponent()->GetComponentLocation();
	FFindFloorResult BotFloorResult;
	FHitResult HitResult;
	float MaxLineDistance = 1000.f;
	float SweepRadius = 30.f;
	GetCharacterMovement()->ComputeFloorDist(BotCapsuleLocation, MaxLineDistance, MaxLineDistance, BotFloorResult, SweepRadius, &HitResult);
	FloorDistance = BotFloorResult.FloorDist;
	FBOTScreenLogger::AddOnScreenDebugMessage(15, ELogVerbosity::Display, *FString::Printf(TEXT("Distance of capsule from floor %.2f cm"), Distance));
}

FText ABOTCharacter::GetJetpackFuelText()
{
	const int32 JP = FMath::RoundHalfFromZero(GetJetpackFuelPercentage() * 100);
	const FString JPS = FString::FromInt(JP);
	
	const FString FullJPS = FString::FromInt((UKismetMathLibrary::Clamp(MaxThrusterTime, 0.0f, 1.0f)) * 100);
	const FString JetpackFuelHUD = JPS + FString(TEXT(" % "));
	const FText JPText = FText::FromString(JetpackFuelHUD);
	return JPText;
}

void ABOTCharacter::ToggleVerticalHUD()
{
	FBOTScreenLogger::AddOnScreenDebugMessage(16, ELogVerbosity::Display, *FString::Printf(TEXT("%s()"), *FString(__FUNCTION__)));
	bVerticalHUD = !bVerticalHUD;
}

void ABOTCharacter::LaunchBOT()
{
	if (bShowLaunchCapsule)
	{
		GetCapsuleComponent()->SetHiddenInGame(false);
	}
	if ( (JetpackState != EJetpackState::EJS_Deactivated) && (JetpackState != EJetpackState::EJS_Recharging) && (JetpackState != EJetpackState::EJS_CoolingOff) )
	{
		const FVector LaunchVelocity =( ( GetActorForwardVector() * JetpackLaunchForwardStrength ) + ( GetActorUpVector() * JetpackLaunchUpStrength ) + GetMesh()->GetMass() ) ;
		if ( (GetCharacterMovement()->Velocity.Size() >= GetMovementComponent()->GetMaxSpeed()) && !bIsLaunched)
		{
			bIsLaunched = true;
			if (FloorDistance >= FloorHeight)
			{
				FBOTScreenLogger::AddOnScreenDebugMessage(12, ELogVerbosity::Error, *FString::Printf(TEXT(" Using Jetpack Booster ")), 1.0f);
				GetCharacterMovement()->AddImpulse(LaunchVelocity, true);
			}
		}
	}
}

void ABOTCharacter::ToggleLaunch()
{
	bIsLaunched = !bIsLaunched;
}

void ABOTCharacter::ReactToFoliage()
{
	const FLinearColor ParameterValue = FLinearColor(GetActorLocation());
	if (MaterialCollection)
	{
		UKismetMaterialLibrary::SetVectorParameterValue(this, MaterialCollection, FName(TEXT("PlayerPosition")), ParameterValue);
	}
}

float ABOTCharacter::GetHueByIndex() const
{
	const int32 MaxBots = UBOTBlueprintFunctionLibrary::GetMaxBots();
	return static_cast<float>(BotIdx / MaxBots);   
}

void ABOTCharacter::EnhanceMoveForward(const FInputActionValue& Value)
{
	if ((Controller) && (Value.GetMagnitude() != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		// float Input = Value.Get<FInputActionValue::Axis1D>();
		AddMovementInput(Direction, Value[0]);
	}
}

void ABOTCharacter::EnhanceMoveRight(const FInputActionValue& Value)
{
	if ((Controller) && (Value.GetMagnitude() != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value[0]);
	}
}

void ABOTCharacter::EnhanceLook(const FInputActionValue& Value)
{
	AddControllerPitchInput(Value[0]);
}

void ABOTCharacter::EnhanceTurn(const FInputActionValue& Value)
{
	AddControllerYawInput(Value[0]);
}

void ABOTCharacter::EnhanceJumpStarted(const FInputActionValue& Value)
{
	const EMovementMode MovementMode = GetCharacterMovement()->MovementMode;
	switch (MovementMode)
	{
	case MOVE_None:
		break;
	case MOVE_Walking:
		Jump();
		break;
	case MOVE_NavWalking:
		Jump();
		break;
	case MOVE_Falling:
		if (JetpackState != EJetpackState::EJS_Deactivated)
		{
			ToggleJetpack(false, true);
		}
		if ( (JetpackState != EJetpackState::EJS_Deactivated) && (JetpackState != EJetpackState::EJS_Recharging) && (JetpackState != EJetpackState::EJS_CoolingOff) )
		{
			LaunchBOT();
		}
		break;
	case MOVE_Swimming: break;
	case MOVE_Flying:
		if (JetpackState != EJetpackState::EJS_Deactivated)
		{
			ToggleJetpack(false, true);
		}
		if ( (JetpackState != EJetpackState::EJS_Deactivated) && (JetpackState != EJetpackState::EJS_Recharging) && (JetpackState != EJetpackState::EJS_CoolingOff) )
		{
			LaunchBOT();
		}
		break;
	case MOVE_Custom: break;
	case MOVE_MAX: break;
	default:
		break;
	}
}

void ABOTCharacter::EnhanceJumpCanceled(const FInputActionValue& Value)
{
	StopJumping();
	ToggleJetpack(false, false);
}

void ABOTCharacter::EnhanceJumpCompleted(const FInputActionValue& Value)
{
	StopJumping();
	ToggleJetpack(false, false);
}

void ABOTCharacter::EnhanceInteract(const FInputActionValue& Value)
{
	FBOTScreenLogger::AddOnScreenDebugMessage(0, ELogVerbosity::Display, *FString::Printf(TEXT("%s() Works."), *FString(__FUNCTION__)));
}

void ABOTCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCapsuleComponent()->SetHiddenInGame(true);
	ToggleJetpack(true, false);
	ToggleLaunch();
}

void ABOTCharacter::EnableSpawnEffect(float EnablePrinting, float SliceZ)
{
	for (const TObjectPtr<UMaterialInstanceDynamic> Material : Materials)
	{
		Material->SetScalarParameterValue(FName(TEXT("EnablePrinting")), EnablePrinting);
		Material->SetScalarParameterValue(FName(TEXT("SliceZ")), SliceZ);
	}
}


