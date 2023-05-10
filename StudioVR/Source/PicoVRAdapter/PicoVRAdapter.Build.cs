// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PicoVRAdapter : ModuleRules
{
	public PicoVRAdapter(ReadOnlyTargetRules Target) : base(Target)
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
                "OnlineSubsystemPico",
                "PicoMobile",
                "PicoMobileController",
                "PicoNeoController"
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
