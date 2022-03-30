

#include "BOTHUDInGame.h"

#include "BOTGameInstance.h"
#include "BOTLoadingScreenWidget.h"
#include "BOTPauseMenuWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DelegateWrapper.h"

ABOTHUDInGame::ABOTHUDInGame(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UBOTLoadingScreenWidget> LoadingScreenWidgetClassFinder(TEXT("WidgetBlueprint'/Game/_New/UI/BPWBOTLoadingScreenWidget.BPWBOTLoadingScreenWidget_C'"));
	if (LoadingScreenWidgetClassFinder.Class != nullptr)
	{
		LoadingScreenWidgetClass = LoadingScreenWidgetClassFinder.Class;
	}
	static ConstructorHelpers::FClassFinder<UBOTPauseMenuWidget> PauseMenuWidgetClassFinder(TEXT("WidgetBlueprint'/Game/_New/UI/BPWBOTPauseMenuWidget.BPWBOTPauseMenuWidget_C'"));
	if (PauseMenuWidgetClassFinder.Class != nullptr)
	{
		PauseMenuWidgetClass = PauseMenuWidgetClassFinder.Class;
	}
	MainMenuLevelName = "MainMenuLevel";
}

void ABOTHUDInGame::BeginPlay()
{
	Super::BeginPlay();
	
	if (LoadingScreenWidgetClass != nullptr)
	{
		APlayerController* OwningPlayerController = GetOwningPlayerController();
		if (OwningPlayerController != nullptr)
		{
			LoadingScreenWidget = Cast<UBOTLoadingScreenWidget>(CreateWidget(OwningPlayerController, LoadingScreenWidgetClass));
			LoadingScreenWidget->SetFade(0.0f);
			LoadingScreenWidget->AddToViewport(900.0f);
		}
	}
	
	if (PauseMenuWidgetClass != nullptr)
	{
		APlayerController* OwningPlayerController = GetOwningPlayerController();
		if (OwningPlayerController != nullptr)
		{
			PauseMenuWidget = Cast<UBOTPauseMenuWidget>(CreateWidget(OwningPlayerController, PauseMenuWidgetClass));
			if (PauseMenuWidget != nullptr)
			{
				PauseMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
				PauseMenuWidget->AddToViewport(100);
			}
		}
	}
	
	const TObjectPtr<UGameInstance> BotGameInstance = Cast<UBOTGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (BotGameInstance != nullptr)
	{
		if (BotGameInstance->GetClass()->ImplementsInterface(UBOTGameInstanceInterface::StaticClass()))
		{
			IBOTGameInstanceInterface::Execute_PlayInGameMusic(BotGameInstance, 0.5f, 0.5f);
		}
	}
}

void ABOTHUDInGame::SetPaused_Implementation(bool bIsPaused)
{
	if (bIsPaused)
	{
		if (PauseMenuWidget != nullptr)
		{
			PauseMenuWidget->SetVisibility(ESlateVisibility::Visible);
			UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(GetOwningPlayerController(), PauseMenuWidget, EMouseLockMode::DoNotLock);
			UGameplayStatics::SetGamePaused(this, true);
			GetOwningPlayerController()->SetShowMouseCursor(true);
		}
	}
	else
	{
		UGameplayStatics::SetGamePaused(this, false);
		GetOwningPlayerController()->SetShowMouseCursor(false);
		const FInputModeGameOnly GameOnly;
		GetOwningPlayerController()->SetInputMode(GameOnly);
		if (PauseMenuWidget != nullptr)
		{
			PauseMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void ABOTHUDInGame::LoadMenu_Implementation()
{
	const FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(this);
	if (const TObjectPtr<UGameInstance> BOTGameInstance = Cast<UBOTGameInstance>(GetGameInstance()))
	{
		if (BOTGameInstance->GetClass()->ImplementsInterface(UBOTGameInstanceInterface::StaticClass()))
		{
			IBOTGameInstanceInterface::Execute_StopInGameMusic(BOTGameInstance, 0.5f, 0.0f);
			LoadALevel(MainMenuLevelName);
		}
	}
}

void ABOTHUDInGame::LoadGame_Implementation()
{
}

void ABOTHUDInGame::ResetMap_Implementation()
{
	const FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(this);
	if (const TObjectPtr<UGameInstance> BOTGameInstance = Cast<UBOTGameInstance>(GetGameInstance()))
	{
		if (BOTGameInstance->GetClass()->ImplementsInterface(UBOTGameInstanceInterface::StaticClass()))
		{
			IBOTGameInstanceInterface::Execute_StopInGameMusic(BOTGameInstance, 0.5f, 0.0f);
			IBOTGameInstanceInterface::Execute_ResetSaveGame(BOTGameInstance, CurrentLevelName);
		}
		LoadALevel(FName(*CurrentLevelName));
	}
}

void ABOTHUDInGame::QuitGame_Implementation()
{
	APlayerController* OwningPlayerController = GetOwningPlayerController();
	if (OwningPlayerController != nullptr)
	{
		UKismetSystemLibrary::QuitGame(this, OwningPlayerController, EQuitPreference::Quit, false);
	}
}

void ABOTHUDInGame::LoadALevel(const FName& InLevelName)
{
	if (InLevelName != "")
	{
		if (LoadingScreenWidget != nullptr)
		{
			LoadingScreenWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			LoadingScreenWidget->SetFade(1.0f);
			UDelegateWrapper* DoneFadingWrapper = NewObject<UDelegateWrapper>();
			DoneFadingWrapper->SetCallback([this, InLevelName]()
			{
				UGameplayStatics::OpenLevel(this, InLevelName);
			}); 
			LoadingScreenWidget->OnDoneFadingEvent().AddDynamic(DoneFadingWrapper, &UDelegateWrapper::DelegateDispatch);
		}
	}
	else
	{
		UE_LOG(LogUObjectGlobals, Fatal, TEXT("Empty level name. Please add the correct level name and try again."));
	}
}




