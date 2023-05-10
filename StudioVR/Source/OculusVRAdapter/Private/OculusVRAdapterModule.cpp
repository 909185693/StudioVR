// Fill out your copyright notice in the Description page of Project Settings.

#include "OculusVRAdapterPrivate.h"
#include "StudioVRAdapter_Oculus.h"
#include "Modules/ModuleManager.h"


class FOculusVRAdapterModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override
	{
		IStudioVRAdapterInterface::RegisterAdapter(new FStudioVRAdapter_Oculus());
	}

	virtual void ShutdownModule() override
	{

	}
};

IMPLEMENT_MODULE(FOculusVRAdapterModule, OculusVRAdapter);
