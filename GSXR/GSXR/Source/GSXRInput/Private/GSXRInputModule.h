#pragma once
#include "IGSXRInputPlugin.h"

class FGSXRHMD;
class FGSXRInputModule : public IGSXRInputPlugin
{
public:
	FGSXRInputModule();
	virtual ~FGSXRInputModule();

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	virtual TSharedPtr< class IInputDevice > CreateInputDevice(const TSharedRef< FGenericApplicationMessageHandler >& InMessageHandler) override;

private:	
	TSharedPtr<class FGSXRInput> InputDevice;
};
