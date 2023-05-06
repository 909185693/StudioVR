// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "IInputDeviceModule.h"

class IGSXRInputPlugin : public IInputDeviceModule
{
public:

	static IGSXRInputPlugin& Get()
	{
		return FModuleManager::LoadModuleChecked< IGSXRInputPlugin >("GSXRInput");
	}

	
	static bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("GSXRInput");
	}

};
