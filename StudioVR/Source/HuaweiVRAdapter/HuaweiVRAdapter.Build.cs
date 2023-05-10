// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class HuaweiVRAdapter : ModuleRules
{
	public HuaweiVRAdapter(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {

			}
		);
	
		
		PrivateIncludePaths.AddRange(
			new string[] {

			}
		);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
                "Core",
                "CoreUObject",
				"Engine",
				"StudioVR",
                "HuaweiVRSDK",
                "HuaweiVRController"
            }
		);

		PublicIncludePaths.AddRange(
			new string[] {
				PluginDirectory + "../HuaweiVR/HuaweiVRController/Source/HuaweiVRController/Public",
				PluginDirectory + "../HuaweiVR/HuaweiVRController/Source/HuaweiVRController/Private",
			}
		);

        PrivateDependencyModuleNames.AddRange(
			new string[]
            {

			}
		);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{

			}
		);
	}
}
