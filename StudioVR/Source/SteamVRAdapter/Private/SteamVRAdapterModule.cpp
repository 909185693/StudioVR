// Fill out your copyright notice in the Description page of Project Settings.

#include "Modules/ModuleManager.h"
#include "StudioVRAdapter_Steam.h"


class FSteamVRAdapterModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override
	{
		IStudioVRAdapterInterface::RegisterAdapter(new FStudioVRAdapter_Steam());
	}

	virtual void ShutdownModule() override
	{

	}
};

IMPLEMENT_MODULE(FSteamVRAdapterModule, SteamVRAdapter);
