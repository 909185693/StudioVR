// Fill out your copyright notice in the Description page of Project Settings.

#include "PicoVRAdapterPrivate.h"
#include "StudioVRAdapter_Pico.h"
#include "Modules/ModuleManager.h"

class FPicoVRAdapterModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override
	{
		IStudioVRAdapterInterface::RegisterAdapter(new FStudioVRAdapter_Pico());
	}

	virtual void ShutdownModule() override
	{

	}
};

IMPLEMENT_MODULE(FPicoVRAdapterModule, PicoVRAdapter);
