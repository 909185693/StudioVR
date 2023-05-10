// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class WaveVRAdapter : ModuleRules
{
	public WaveVRAdapter(ReadOnlyTargetRules Target) : base(Target)
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
                "WVR",
                "WaveVR",
                "WaveVRInput",
                "WaveVRGesture"
            }
		);

		PublicIncludePaths.AddRange(
            new string[] {
                PluginDirectory + "/../WaveVR/WaveVR/Source/WaveVR/Public",
                PluginDirectory + "/../WaveVR/WaveVR/Source/WaveVR/Private",
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
