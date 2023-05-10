// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class GSXRInput : ModuleRules
{
	public GSXRInput(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePathModuleNames.AddRange(
			new [] {
				"Launch",
			});		
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				"GSXRLibrary",
				"InputCore",
				"InputDevice",
				"HeadMountedDisplay",
				"Json",
				"JsonUtilities",
				"GSXRHMD"
				// ... add private dependencies that you statically link with here ...	
			}
		);
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"Projects",
				"AndroidRuntimeSettings",
				"GSXRLibrary",
				"GSXRHMD"
				// ... 	,add other public dependencies that you statically link with here ...
			}
		);


		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
