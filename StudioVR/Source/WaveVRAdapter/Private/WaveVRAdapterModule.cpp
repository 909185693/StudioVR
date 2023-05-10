// Fill out your copyright notice in the Description page of Project Settings.

#include "WaveVRAdapterPrivate.h"
#include "Modules/ModuleManager.h"

class FWaveVRAdapterModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override
	{

	}

	virtual void ShutdownModule() override
	{

	}
};

IMPLEMENT_MODULE(FWaveVRAdapterModule, WaveVRAdapter);
