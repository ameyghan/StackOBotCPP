

#include "StackOBotStartup.h"

#include "SlateBasics.h"
#include "SlateExtras.h"
#include "MoviePlayer.h"

struct FStackOBotStartupBrush : public FSlateDynamicImageBrush, public FGCObject
{
	FStackOBotStartupBrush( const FName InTextureName, const FVector2D& InImageSize)
		: FSlateDynamicImageBrush(InTextureName, InImageSize)
	{
		SetResourceObject(LoadObject<UObject>( nullptr, *InTextureName.ToString()));
	}

	virtual void AddReferencedObjects(FReferenceCollector& Collector) override
	{
		FSlateBrush::AddReferencedObjects(Collector);
	}
};

class SStackOBotStartupScreen : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SStackOBotStartupScreen) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs)
	{
		static const FName LoadingScreenName(TEXT("/Game/_New/UI/Startup/StartupImage.StartupImage"));
		const FMargin PaddingMargin = FMargin(10.f);
		LoadingScreenBrush = MakeShareable(new FStackOBotStartupBrush(LoadingScreenName, FVector2D(1920, 1080)));
		ChildSlot
		[
			SNew(SOverlay)
			+SOverlay::Slot()
			.HAlign( HAlign_Fill )
			.VAlign( VAlign_Fill )
			[
				SNew( SImage)
				.Image(LoadingScreenBrush.Get())
			]
			+SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SNew(SVerticalBox)
				+SVerticalBox::Slot()
				.VAlign(VAlign_Bottom)
				.HAlign(HAlign_Right)
				.Padding(PaddingMargin)
				[
					SNew(SCircularThrobber)
					.Visibility(this, &SStackOBotStartupScreen::GetLoadIndicatorVisibility)
				]
			]
		];
	}

private:
	EVisibility GetLoadIndicatorVisibility() const
	{
		const bool bIsLoadingFinished = GetMoviePlayer()->IsLoadingFinished();
		return bIsLoadingFinished ? EVisibility::Collapsed : EVisibility::Visible;
	}

	TSharedPtr<FSlateDynamicImageBrush> LoadingScreenBrush;
};



class FStackOBotStartupModule : public IStackOBotStartupModule
{
public:
	virtual void StartupModule() override
	{
		LoadObject<UObject>(nullptr, TEXT("/Game/_New/UI/Startup/StartupImage.StartupImage"));
		if (IsMoviePlayerEnabled())
		{
			CreateScreen();
		}
	}

	virtual bool IsGameModule() const override
	{
		return true;
	}
	
	virtual void StartupLoadingScreen() override
	{
		CreateScreen();
	}

	virtual void CreateScreen()
	{
		FLoadingScreenAttributes LoadingScreen;
		LoadingScreen.bAutoCompleteWhenLoadingCompletes = true;
		LoadingScreen.MinimumLoadingScreenDisplayTime = 0.0f;
		LoadingScreen.WidgetLoadingScreen = SNew(SStackOBotStartupScreen);
		GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
	}
};

IMPLEMENT_GAME_MODULE(FStackOBotStartupModule, StackOBotStartup)