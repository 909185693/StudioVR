// Fill out your copyright notice in the Description page of Project Settings.

#include "GSXRAdapterPrivate.h"
#include "Modules/ModuleManager.h"

class FGSXRAdapterModule : public IModuleInterface
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

IMPLEMENT_MODULE(FGSXRAdapterModule, GSXRAdapter);
