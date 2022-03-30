

#pragma once

#include "CoreMinimal.h"

STACKOBOT_API DECLARE_LOG_CATEGORY_EXTERN(BOTLogOutput, Display, All);
STACKOBOT_API DECLARE_LOG_CATEGORY_EXTERN(BOTLogScreen, Display, All);
DECLARE_LOG_CATEGORY_EXTERN(LogBOT, All, All);

#define BOT_DEBUG_LOG(Verbosity, Format, ...) \
{ \
UE_LOG(BOTLogOutput, Verbosity, Format, ##__VA_ARGS__); \
}

#define BOT_DEBUG_WARN(Format, ...) \
{ \
UE_LOG(BOTLogOutput, Warning, Format, ##__VA_ARGS__); \
}

#define BOT_DEBUG_ERROR(Format, ...) \
{ \
UE_LOG(BOTLogOutput, Error, Format, ##__VA_ARGS__); \
}

class FBOTScreenLogger
{
public:
	static FColor GetOnScreenVerbosityColor(const ELogVerbosity::Type Verbosity)
	{
		return
	   (Verbosity == ELogVerbosity::Fatal	||  Verbosity	== ELogVerbosity::Error)	   ? FColor::Red	:
	   (Verbosity == ELogVerbosity::Warning)											   ? FColor::Yellow	:
	   (Verbosity == ELogVerbosity::Display ||  Verbosity	== ELogVerbosity::Log)		   ? FColor::White	:
	   (Verbosity == ELogVerbosity::Verbose ||  Verbosity	== ELogVerbosity::VeryVerbose) ? FColor::Orange	:
																							 FColor::Cyan;
	}

	static void AddOnScreenDebugMessage(int Index, const ELogVerbosity::Type Verbosity, const FString Message, float TimeToDisplay = 2.0f)
	{
		if (GEngine)
		{
			const FColor Color = GetOnScreenVerbosityColor(Verbosity);
			GEngine->AddOnScreenDebugMessage(Index, TimeToDisplay, Color, Message);
		}
	}
};

#define BOT_ONSCREEN_LOG(Verbosity, Format, ...) \
{ \
FBOTScreenLogger::AddOnScreenDebugMessage(-1, ELogVerbosity::Verbosity, FString::Printf(Format, ##__VA_ARGS__)); \
UE_LOG(BOTLogScreen, Verbosity, Format, ##__VA_ARGS__); \
}

