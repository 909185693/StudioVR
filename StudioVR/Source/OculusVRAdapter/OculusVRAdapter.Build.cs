// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class OculusVRAdapter : ModuleRules
{
	public OculusVRAdapter(ReadOnlyTargetRules Target) : base(Target)
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
                "HeadMountedDisplay",
                "StudioVR",
                "OculusHMD",
                "OculusInput"
            }
		);

		if (Target.Type == TargetType.Editor)
		{
            PublicDependencyModuleNames.AddRange(
                new string[]
                {
					"OculusEditor"
                }
            );
        }
		
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
