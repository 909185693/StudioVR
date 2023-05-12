// Fill out your copyright notice in the Description page of Project Settings.

#include "StudioVRAdapter_Nolo.h"
#include "Modules/ModuleManager.h"

class FNoloVRAdapterModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override
	{
		IStudioVRAdapterInterface::RegisterAdapter(new FStudioVRAdapter_Nolo());
	}

	virtual void ShutdownModule() override
	{

	}
};

IMPLEMENT_MODULE(FNoloVRAdapterModule, NoloVRAdapter);
