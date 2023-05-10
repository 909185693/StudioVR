// Fill out your copyright notice in the Description page of Project Settings.

#include "HuaweiVRAdapterPrivate.h"
#include "StudioVRAdapter_Huawei.h"
#include "Modules/ModuleManager.h"

class FHuaweiVRAdapterModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override
	{
		IStudioVRAdapterInterface::RegisterAdapter(new FStudioVRAdapter_Huawei());
	}

	virtual void ShutdownModule() override
	{

	}
};

IMPLEMENT_MODULE(FHuaweiVRAdapterModule, HuaweiVRAdapter);
