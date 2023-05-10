#include "GSXRInputModule.h"
#include "GSXRInput.h"
FGSXRInputModule::FGSXRInputModule()
:InputDevice(nullptr)
{
}

FGSXRInputModule::~FGSXRInputModule()
{
}

void FGSXRInputModule::StartupModule()
{
	IInputDeviceModule::StartupModule();
}

void FGSXRInputModule::ShutdownModule()
{
	IInputDeviceModule::ShutdownModule();
}

TSharedPtr<IInputDevice> FGSXRInputModule::CreateInputDevice(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler)
{
	InputDevice = MakeShared<FGSXRInput>(InMessageHandler);
	if (InputDevice)
	{
		InputDevice->SetMessageHandler(InMessageHandler);
	}	
	return InputDevice;
}
IMPLEMENT_MODULE(FGSXRInputModule, GSXRInput)