// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "IHeadMountedDisplayModule.h"

class GSXRHMD_API IGSXRHMDPlugin : public IHeadMountedDisplayModule
{
public:
	static inline IGSXRHMDPlugin& Get()
	{
		return FModuleManager::LoadModuleChecked<IGSXRHMDPlugin>("GSXRHMD");
	}
	
	static inline  bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("GSXRHMD");
	}
};
