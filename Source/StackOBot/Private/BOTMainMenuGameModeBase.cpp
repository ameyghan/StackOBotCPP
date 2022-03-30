

#include "BOTMainMenuGameModeBase.h"

#include "BOTCharacter.h"
#include "BOTHUDMainMenu.h"

ABOTMainMenuGameModeBase::ABOTMainMenuGameModeBase()
{
	HUDClass = ABOTHUDMainMenu::StaticClass();
	
	// static ConstructorHelpers::FClassFinder<APawn> PlayerCharacterObject(TEXT("Blueprint'/Game/_New/Blueprints/BP_BotChar.BP_BotChar'"));
	// if (PlayerCharacterObject.Class != nullptr)
	// {
	// 	DefaultPawnClass = PlayerCharacterObject.Class;
	// }
	
}
