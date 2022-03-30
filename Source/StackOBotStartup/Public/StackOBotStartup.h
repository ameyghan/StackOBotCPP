

#pragma once

#include "Modules/ModuleInterface.h"

class IStackOBotStartupModule : public IModuleInterface
{
public:
	
	virtual void StartupLoadingScreen() = 0;
};

