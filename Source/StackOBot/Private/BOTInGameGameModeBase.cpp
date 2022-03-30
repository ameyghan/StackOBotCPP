


#include "BOTInGameGameModeBase.h"

#include "BOTGameInstance.h"
#include "BOTHUDInGame.h"
#include "BOTPlayerControllerInGame.h"
#include "Kismet/GameplayStatics.h"

ABOTInGameGameModeBase::ABOTInGameGameModeBase()
{
	HUDClass = ABOTHUDInGame::StaticClass();
	PlayerControllerClass = ABOTPlayerControllerInGame::StaticClass();
}

void ABOTInGameGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	if (const TObjectPtr<UGameInstance> BOTGameInstance = Cast<UBOTGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		if (BOTGameInstance->GetClass()->ImplementsInterface(UBOTGameInstanceInterface::StaticClass()))
		{
			IBOTGameInstanceInterface::Execute_InitSaveGame(BOTGameInstance);
		}
	}
}
