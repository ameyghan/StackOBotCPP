

#include "BOTPauseMenuWidget.h"

#include "BOTButtonBaseWidget.h"
#include "BOTGameInstance.h"
#include "BOTHUDInGame.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

UBOTPauseMenuWidget::UBOTPauseMenuWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerController != nullptr)
	{
		InGameHUD = Cast<ABOTHUDInGame>(PlayerController->GetHUD());
	}
}

void UBOTPauseMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (UMGResumeButton != nullptr)
	{
		UMGResumeButton->GetBaseButton()->OnClicked.AddUniqueDynamic(this, &UBOTPauseMenuWidget::OnResumeButtonClicked);
	}
	if (UMGSaveGameButton != nullptr)
	{
		UMGSaveGameButton->GetBaseButton()->OnClicked.AddUniqueDynamic(this, &UBOTPauseMenuWidget::OnSaveGameButtonClicked);
	}
	if (UMGMainMenuButton != nullptr)
	{
		UMGMainMenuButton->GetBaseButton()->OnClicked.AddUniqueDynamic(this, &UBOTPauseMenuWidget::OnMainMenuButtonClicked);
	}
	if (UMGResetMapButton != nullptr)
	{
		UMGResetMapButton->GetBaseButton()->OnClicked.AddUniqueDynamic(this, &UBOTPauseMenuWidget::OnResetMapButtonClicked);
	}
	if (UMGQuitToWindowsButton != nullptr)
	{
		UMGQuitToWindowsButton->GetBaseButton()->OnClicked.AddUniqueDynamic(this, &UBOTPauseMenuWidget::OnQuitToWindowsButtonClicked);
	}
}

void UBOTPauseMenuWidget::OnResumeButtonClicked()
{
	if (InGameHUD != nullptr)
	{
		if (InGameHUD->GetClass()->ImplementsInterface(UBOTHUDInterface::StaticClass()))
		{
			IBOTHUDInterface::Execute_SetPaused(InGameHUD, false);
		}
	}
}

void UBOTPauseMenuWidget::OnSaveGameButtonClicked()
{
	const TObjectPtr<UGameInstance> BotGameInstance = Cast<UBOTGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (BotGameInstance != nullptr)
	{
		if (BotGameInstance->GetClass()->ImplementsInterface(UBOTGameInstanceInterface::StaticClass()))
		{
			IBOTGameInstanceInterface::Execute_SaveGame(BotGameInstance);
		}
	}
}

void UBOTPauseMenuWidget::OnMainMenuButtonClicked()
{
	if (InGameHUD != nullptr)
	{
		if (InGameHUD->GetClass()->ImplementsInterface(UBOTHUDInterface::StaticClass()))
		{
			IBOTHUDInterface::Execute_LoadMenu(InGameHUD);
		}
	}
}

void UBOTPauseMenuWidget::OnResetMapButtonClicked()
{
	if (InGameHUD != nullptr)
	{
		if (InGameHUD->GetClass()->ImplementsInterface(UBOTHUDInterface::StaticClass()))
		{
			IBOTHUDInterface::Execute_ResetMap(InGameHUD);
		}
	}
	
		
}

void UBOTPauseMenuWidget::OnQuitToWindowsButtonClicked()
{
	if (InGameHUD != nullptr)
	{
		if (InGameHUD->GetClass()->ImplementsInterface(UBOTHUDInterface::StaticClass()))
		{
			IBOTHUDInterface::Execute_QuitGame(InGameHUD);
		}
	}
}
