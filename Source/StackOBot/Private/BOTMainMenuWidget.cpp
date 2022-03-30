

#include "BOTMainMenuWidget.h"

#include "BOTButtonBaseWidget.h"
#include "BOTHUDMainMenu.h"
#include "Components/Button.h"
#include "GameFramework/HUD.h"
#include "Kismet/GameplayStatics.h"
#include "BOTLog.h"



UBOTMainMenuWidget::UBOTMainMenuWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{}

void UBOTMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (UMGStartButton != nullptr)
	{
		UMGStartButton->GetBaseButton()->OnClicked.AddUniqueDynamic(this, &UBOTMainMenuWidget::OnStartButtonClicked);
	}
	
	if (UMGQuitButton != nullptr)
	{
		UMGQuitButton->GetBaseButton()->OnClicked.AddUniqueDynamic(this, &UBOTMainMenuWidget::OnQuitButtonClicked);
	}
}

void UBOTMainMenuWidget::OnStartButtonClicked()
{
	const TObjectPtr<APlayerController> PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerController != nullptr)
	{
		const TObjectPtr<AHUD> MainMenuHUD = Cast<ABOTHUDMainMenu>(PlayerController->GetHUD());
		if (MainMenuHUD != nullptr)
		{
			if (MainMenuHUD->GetClass()->ImplementsInterface(UBOTHUDInterface::StaticClass()))
			{
				IBOTHUDInterface::Execute_LoadGame(MainMenuHUD);
			}
		}
	}
}

void UBOTMainMenuWidget::OnQuitButtonClicked()
{
	const TObjectPtr<APlayerController> PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerController != nullptr)
	{
		const TObjectPtr<AHUD> MainMenuHUD = Cast<ABOTHUDMainMenu>(PlayerController->GetHUD());
		if (MainMenuHUD != nullptr)
		{
			if (MainMenuHUD->GetClass()->ImplementsInterface(UBOTHUDInterface::StaticClass()))
			{
				IBOTHUDInterface::Execute_QuitGame(MainMenuHUD);
			}
		}
	}
}

