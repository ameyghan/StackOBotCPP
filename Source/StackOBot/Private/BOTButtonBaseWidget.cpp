

#include "BOTButtonBaseWidget.h"

#include "Components/TextBlock.h"


UBOTButtonBaseWidget::UBOTButtonBaseWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	LabelText = FText::FromString("Button Label");
}

void UBOTButtonBaseWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	Label->SetText(LabelText);
}
