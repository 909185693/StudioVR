// Fill out your copyright notice in the Description page of Project Settings.

#include "UnLuaHexNavSystemPrivate.h"
#include "Modules/ModuleManager.h"

class FUnLuaHexNavSystemModule : public IModuleInterface
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

IMPLEMENT_MODULE(FUnLuaHexNavSystemModule, UnLuaHexNavSystem);
