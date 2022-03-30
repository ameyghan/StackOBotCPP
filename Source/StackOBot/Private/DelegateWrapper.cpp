#include "DelegateWrapper.h"

void UDelegateWrapper::SetCallback(TUniqueFunction<void()> InCallback)
{
	FuncWrapper = [Function = MoveTemp(InCallback)]()
	{
		Function();
	};
}

void UDelegateWrapper::DelegateDispatch()
{
	FuncWrapper();
}

