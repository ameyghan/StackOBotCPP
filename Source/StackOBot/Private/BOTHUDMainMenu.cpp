

#include "BOTHUDMainMenu.h"

#include "BOTGameInstance.h"
#include "BOTLoadingScreenWidget.h"
#include "BOTLog.h"
#include "BOTMainMenuWidget.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"

ABOTHUDMainMenu::ABOTHUDMainMenu(const FObjectInitializer& ObjectInitializer )
	: Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UBOTLoadingScreenWidget> LoadingScreenWidgetClassFinder(TEXT("WidgetBlueprint'/Game/_New/UI/BPWBOTLoadingScreenWidget.BPWBOTLoadingScreenWidget_C'"));
	if (LoadingScreenWidgetClassFinder.Class != nullptr)
	{
		LoadingScreenWidgetClass = LoadingScreenWidgetClassFinder.Class;
	}
	
	static ConstructorHelpers::FClassFinder<UBOTMainMenuWidget> MainMenuWidgetClassFinder(TEXT("WidgetBlueprint'/Game/_New/UI/BPWBOTMainMenuWidget.BPWBOTMainMenuWidget_C'"));
	if (MainMenuWidgetClassFinder.Class != nullptr)
	{
		MainMenuWidgetClass = MainMenuWidgetClassFinder.Class;
	}

	StartLevelName = "Playground";
}

void ABOTHUDMainMenu::BeginPlay()
{
	Super::BeginPlay();
	
	if (LoadingScreenWidgetClass != nullptr)
	{
		APlayerController* OwningPlayerController = GetOwningPlayerController();
		if (OwningPlayerController != nullptr)
		{
			LoadingScreenWidget = Cast<UBOTLoadingScreenWidget>(CreateWidget(OwningPlayerController, LoadingScreenWidgetClass));
			if (LoadingScreenWidget != nullptr)
			{
				LoadingScreenWidget->SetFade(0.0f);
				LoadingScreenWidget->AddToViewport(90000.f);
			}
		}
	}
	
	if (MainMenuWidgetClass != nullptr)
	{
		APlayerController* OwningPlayerController = GetOwningPlayerController();
		if (OwningPlayerController != nullptr)
		{
			MainMenuWidget = Cast<UBOTMainMenuWidget>(CreateWidget(OwningPlayerController, MainMenuWidgetClass));
			if (MainMenuWidget != nullptr)
			{
				MainMenuWidget->AddToViewport();
				UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(OwningPlayerController, MainMenuWidget, EMouseLockMode::DoNotLock, false);
				OwningPlayerController->SetShowMouseCursor(true);
				const TObjectPtr<APawn> PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
				
				if (PlayerPawn != nullptr)
				{
					PlayerPawn->DisableInput(OwningPlayerController);
				}
			}
		}
	}
	
	const TObjectPtr<UGameInstance> BotGameInstance = Cast<UBOTGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (BotGameInstance != nullptr)
	{
		if (BotGameInstance->GetClass()->ImplementsInterface(UBOTGameInstanceInterface::StaticClass()))
		{
			IBOTGameInstanceInterface::Execute_PlayMainMenuMusic(BotGameInstance, 1.0f, 0.5f);
		}
	}
}

void ABOTHUDMainMenu::LoadGame_Implementation()
{
	IBOTHUDInterface::LoadGame_Implementation();
	if (StartLevelName == "Playground")
	{
		LoadingScreenWidget->SetVisibility(ESlateVisibility::Visible);
		LoadingScreenWidget->SetFade(1.0f);
		const TObjectPtr<UGameInstance> BotGameInstance = Cast<UBOTGameInstance>(UGameplayStatics::GetGameInstance(this));
		if (BotGameInstance != nullptr)
		{
			if (BotGameInstance->GetClass()->ImplementsInterface(UBOTGameInstanceInterface::StaticClass()))
			{
				IBOTGameInstanceInterface::Execute_StopMainMenuMusic(BotGameInstance, 0.5f, 0.0f);
			}
		}
		LoadingScreenWidget->OnDoneFadingEvent().AddDynamic(this, &ABOTHUDMainMenu::LoadingDoneFading);
	}
	else
	{
		FBOTScreenLogger::AddOnScreenDebugMessage(37, ELogVerbosity::Error, *FString::Printf(TEXT("Start Level Could not be found")), 5.0f);
	}
}

void ABOTHUDMainMenu::QuitGame_Implementation()
{
	IBOTHUDInterface::QuitGame_Implementation();
	APlayerController* OwningPlayerController = GetOwningPlayerController();
	if (OwningPlayerController != nullptr)
	{
		UKismetSystemLibrary::QuitGame(this, OwningPlayerController, EQuitPreference::Quit, false);
	}
}

void ABOTHUDMainMenu::LoadingDoneFading()
{
	UGameplayStatics::OpenLevel(this, StartLevelName);
}
