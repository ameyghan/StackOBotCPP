


#include "BOTHeadsupDisplayWidget.h"

#include "BOTGameInstance.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetTextLibrary.h"

UBOTHeadsupDisplayWidget::UBOTHeadsupDisplayWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	OrbsTextBlockText = FText::FromString("999");
}

void UBOTHeadsupDisplayWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	OrbsAmount->SetText(OrbsTextBlockText);
}

void UBOTHeadsupDisplayWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	const TObjectPtr<UGameInstance> GameInstance = UGameplayStatics::GetGameInstance(this);
	if (GameInstance != nullptr)
	{
		if (GameInstance->GetClass()->ImplementsInterface(UBOTGameInstanceInterface::StaticClass()))
		{
			const int32 OrbsText = IBOTGameInstanceInterface::Execute_GetOrbAmount(GameInstance); 
			SetOrbsText(OrbsText);
			const TObjectPtr<UBOTGameInstance> BOTGameInstance = Cast<UBOTGameInstance>(UGameplayStatics::GetGameInstance(this));
			if (BOTGameInstance != nullptr)
			{
				BOTGameInstance->OnOrbsUpdatedEvent().AddDynamic(this, &UBOTHeadsupDisplayWidget::OnOrbsUpdated);
			}
		}
	}
}

void UBOTHeadsupDisplayWidget::SetOrbsText(const int32 Amount)
{
	OrbsTextBlockText = FText::AsNumber(Amount);
	OrbsAmount->SetText(OrbsTextBlockText);
}

void UBOTHeadsupDisplayWidget::OnOrbsUpdated(const int32 NewAmount)
{
	SetOrbsText(NewAmount);
}
