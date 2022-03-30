

#include "BOTSaveLevel.h"

void UBOTSaveLevel::AddCollectibles(FCollectibleObject Collectibles)
{
	CollectablePickups.AddUnique(Collectibles);
}

void UBOTSaveLevel::RemoveCollectibles(const FCollectibleObject& Collectibles)
{
	CollectablePickups.Remove(Collectibles);
}