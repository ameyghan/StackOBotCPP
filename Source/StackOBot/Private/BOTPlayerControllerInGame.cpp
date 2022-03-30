

#include "BOTPlayerControllerInGame.h"

#include "BOTHeadsupDisplayWidget.h"
#include "BOTHUDInGame.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "Kismet/GameplayStatics.h"

ABOTPlayerControllerInGame::ABOTPlayerControllerInGame(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bIsUsingGamepad = false;
	static ConstructorHelpers::FClassFinder<UBOTHeadsupDisplayWidget> HeadsupDisplayWidgetClassFinder(TEXT("WidgetBlueprint'/Game/_New/UI/BPWBOTHeadupDisplayWidget.BPWBOTHeadupDisplayWidget_C'"));
	if (HeadsupDisplayWidgetClassFinder.Class != nullptr)
	{
		HeadsupDisplayWidgetClass = HeadsupDisplayWidgetClassFinder.Class;
	}
}

void ABOTPlayerControllerInGame::BeginPlay()
{
	Super::BeginPlay();
	PlayerCameraManager->ViewPitchMin = -65.0f;
	PlayerCameraManager->ViewPitchMax = 0.0f;
	const FInputModeGameOnly GameOnly;
	SetInputMode(GameOnly);
	SetShowMouseCursor(false);
	if (HeadsupDisplayWidgetClass != nullptr)
	{
		HeadsupDisplayWidget = Cast<UBOTHeadsupDisplayWidget>(CreateWidget(this, HeadsupDisplayWidgetClass));
		HeadsupDisplayWidget->AddToViewport();
	}
}

//	Note that this is a really messy way of setting up input as we have input being driven in 2 different places (one on the character and one here) 
// This is done just so the project is identical to the Blueprint veresion. 
// Notmally the controlls have to be setup all in one place, preferably inside a controller and then added to the character as a component. 
void ABOTPlayerControllerInGame::SetupInputComponent()
{
	Super::SetupInputComponent();
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (EnhancedInputComponent == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnhancedInput Must be enabled. For information on how to enable enhanced input, please visit https://docs.unrealengine.com/5.0/en-US/GameplayFeatures/EnhancedInput/"))
	}
	else 
	{
		const TObjectPtr<UInputAction> PauseGameAction = NewObject<UInputAction>(this);
		if (PauseGameAction != nullptr)
		{
			PauseGameAction->bConsumeInput = true;
			PauseGameAction->bTriggerWhenPaused = true;
			PauseGameAction->bReserveAllMappings = false;
			PauseGameAction->ValueType = EInputActionValueType::Boolean;
			PCMappingContext = NewObject<UInputMappingContext>(this);
			const TObjectPtr<UInputTriggerReleased> InputTriggerReleased = NewObject<UInputTriggerReleased>(this);
			InputTriggerReleased->ActuationThreshold = 0.5f;
			PauseGameAction->Triggers.Add(InputTriggerReleased);
			check(PCMappingContext);
			PCMappingContext->MapKey(PauseGameAction, EKeys::Escape);
			PCMappingContext->MapKey(PauseGameAction, EKeys::Gamepad_Special_Right);
			EnhancedInputComponent->BindAction(PauseGameAction, ETriggerEvent::Triggered, this, &ABOTPlayerControllerInGame::EnhancePauseGame);
		}
	}
}

void ABOTPlayerControllerInGame::EnhancePauseGame(const FInputActionValue& ActionValue)
{
	const TObjectPtr<AHUD> InGameHUD = Cast<ABOTHUDInGame>(GetHUD());
	if (InGameHUD != nullptr)
	{
		if (InGameHUD->GetClass()->ImplementsInterface(UBOTHUDInterface::StaticClass()))
		{
			if (GetWorld() != nullptr)
			{
				IBOTHUDInterface::Execute_SetPaused(InGameHUD, !(UGameplayStatics::IsGamePaused(this)));
			}
		}
	}
}

