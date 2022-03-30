

#include "BOTGameInstance.h"

#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BOTLog.h"
#include "BOTSaveLevel.h"
#include "BOTSavePlayer.h"
#include "BOTEnergyOrb.h"

void UBOTGameInstance::Init()
{
	Super::Init();
	CollectedOrbs = 0;
	SaveGameSlotName = TEXT("PlayerInventory");
}

void UBOTGameInstance::PlayMainMenuMusic_Implementation(float FadeInDuration, float FadeVolumeLevel)
{
	IBOTGameInstanceInterface::PlayMainMenuMusic_Implementation(FadeInDuration, FadeVolumeLevel);
	if (MainMenuMusicFile != nullptr)
	{
		MainMenuMusic = UGameplayStatics::CreateSound2D(this, MainMenuMusicFile, FadeVolumeLevel, 1.f, 0.f, nullptr, true, true);
		if (MainMenuMusic)
		{
			if (!(MainMenuMusic->IsPlaying()))
			{
				MainMenuMusic->FadeIn(FadeInDuration, FadeVolumeLevel);
			}
		}
	}
	else
	{
		FBOTScreenLogger::AddOnScreenDebugMessage(30, ELogVerbosity::Error, *FString::Printf(TEXT("Error: Main menu music failed to play. Main menu music file was not found.")));
	}
}

void UBOTGameInstance::PlayInGameMusic_Implementation(float FadeInDuration, float FadeVolumeLevel)
{
	IBOTGameInstanceInterface::PlayInGameMusic_Implementation(FadeInDuration, FadeVolumeLevel);
	if (InGameMusicFile != nullptr)
	{
		InGameMusic = UGameplayStatics::CreateSound2D(this, InGameMusicFile, FadeVolumeLevel, 1.f, 0.f, nullptr, true, true);
		if (InGameMusic != nullptr)
		{
			InGameMusic->FadeIn(FadeInDuration, FadeVolumeLevel);
		}
	}
	else
	{
		FBOTScreenLogger::AddOnScreenDebugMessage(31, ELogVerbosity::Error, *FString::Printf(TEXT("Error: Level music failed to play. In-Game music file was not found.")));
	}
}

void UBOTGameInstance::StopMainMenuMusic_Implementation(float FadeOutDuration, float FadeVolumeLevel)
{
	if (MainMenuMusicFile != nullptr)
	{
		MainMenuMusic->FadeOut(FadeOutDuration, FadeVolumeLevel);
	}
}

void UBOTGameInstance::StopInGameMusic_Implementation(float FadeOutDuration, float FadeVolumeLevel)
{
	if (InGameMusicFile != nullptr)
	{
		InGameMusic->FadeOut(FadeOutDuration, FadeVolumeLevel);
	}
}

void UBOTGameInstance::InitSaveGame_Implementation()
{
	InitPlayerSaveData();
	LoadPlayerSaveData();
	InitLevelSaveData();
}

int32 UBOTGameInstance::GetOrbAmount_Implementation()
{
	return CollectedOrbs;
}

void UBOTGameInstance::UpdateOrbs_Implementation(int32 Change)
{
	CollectedOrbs = CollectedOrbs + Change;
	OnOrbsUpdated.Broadcast(CollectedOrbs);
}

void UBOTGameInstance::RemoveCollectibleFromSaveGame_Implementation(FCollectibleObject CollectibleData)
{
	LevelSave->RemoveCollectibles(CollectibleData);
}

void UBOTGameInstance::SaveGame_Implementation()
{
	SaveGameToSlots();
}

bool UBOTGameInstance::ResetSaveGame_Implementation(const FString& LevelName)
{
	if (UGameplayStatics::DoesSaveGameExist(LevelName, 0))
	{
		const bool bDeleteLevelSave = (UGameplayStatics::DeleteGameInSlot(LevelName, 0));
		const bool bDeletePlayerInventorySave = (UGameplayStatics::DeleteGameInSlot(SaveGameSlotName, 0)); 
		return (bDeleteLevelSave && bDeletePlayerInventorySave);
	}
	return false;
}

void UBOTGameInstance::InitPlayerSaveData()
{
	if (UGameplayStatics::DoesSaveGameExist(SaveGameSlotName, 0))
	{
		PlayerInventorySaveGame = Cast<UBOTSavePlayer>(UGameplayStatics::LoadGameFromSlot(SaveGameSlotName, 0));
	}
	else
	{
		PlayerInventorySaveGame = Cast<UBOTSavePlayer>(UGameplayStatics::CreateSaveGameObject(UBOTSavePlayer::StaticClass()));
	}
}

void UBOTGameInstance::LoadPlayerSaveData()
{
	CollectedOrbs = PlayerInventorySaveGame->GetOrbs();
}

void UBOTGameInstance::SaveGameToSlots() const
{
	PlayerInventorySaveGame->SetOrbs(CollectedOrbs);
	UGameplayStatics::SaveGameToSlot(PlayerInventorySaveGame, SaveGameSlotName, 0);
	const FString CurrentLeveLName = UGameplayStatics::GetCurrentLevelName(this);
	UGameplayStatics::SaveGameToSlot(LevelSave, CurrentLeveLName, 0);
}

void UBOTGameInstance::InitLevelSaveData()
{
	const FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(this);
	if (UGameplayStatics::DoesSaveGameExist(CurrentLevelName, 0))
	{
		LevelSave = Cast<UBOTSaveLevel>(UGameplayStatics::LoadGameFromSlot(CurrentLevelName, 0));
		if (LevelSave != nullptr)
		{
			TArray<TObjectPtr<AActor>> OutActors;
			UGameplayStatics::GetAllActorsOfClass(this, ABOTEnergyOrb::StaticClass(), OutActors);
			for (TObjectPtr<AActor> Actor : OutActors)
			{
				TObjectPtr<ABOTEnergyOrb> EnergyOrb = Cast<ABOTEnergyOrb>(Actor);
				if (EnergyOrb != nullptr)
				{
					EnergyOrb->Destroy();
				}
			}
			TArray<FCollectibleObject> CollectibleObjects = LevelSave->GetCollectiblesOnMap();
			for (FCollectibleObject Collectibles : CollectibleObjects)
			{
				FActorSpawnParameters Parameters;
				Parameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				GetWorld()->SpawnActor<AActor>(Collectibles.ActorClass, Collectibles.Transform.GetLocation(), Collectibles.Transform.GetRotation().Rotator(), Parameters);
				
			}
		}
	}
	else
	{
		LevelSave = Cast<UBOTSaveLevel>(UGameplayStatics::CreateSaveGameObject(UBOTSaveLevel::StaticClass()));
		if (LevelSave != nullptr)
		{
			TArray<TObjectPtr<AActor>> OutActors;
			UGameplayStatics::GetAllActorsOfClass(this, ABOTEnergyOrb::StaticClass(), OutActors);
			for (TObjectPtr<AActor> Actor : OutActors)
			{
				TObjectPtr<ABOTEnergyOrb> EnergyOrb = Cast<ABOTEnergyOrb>(Actor);
				if (EnergyOrb != nullptr)
				{
					FCollectibleObject Collectible;
					Collectible.ActorClass = EnergyOrb->GetClass();
					Collectible.Transform = EnergyOrb->GetTransform();
					LevelSave->AddCollectibles(Collectible);
				}
			}
		}
	}
}