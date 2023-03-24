// Fill out your copyright notice in the Description page of Project Settings.

using System.IO;
using UnrealBuildTool;

public class NOLOLibrary : ModuleRules
{
	public NOLOLibrary(ReadOnlyTargetRules Target): base(Target)
	{
		//PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "include"));
		Type = ModuleType.External;
		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			PublicSystemLibraryPaths.Add(Path.Combine(ModuleDirectory, "x64", "Release"));
			// Add the import library
			PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "x64", "Release","NoLo_USBHID.lib"));
			// Delay-load the DLL, so we can load it from the right place first
			// PublicDelayLoadDLLs.Add(Path.Combine(ModuleDirectory, "x64", "Release","NoLo_USBHID.dll"));
			PublicDelayLoadDLLs.Add("NoLo_USBHID.dll");
			RuntimeDependencies.Add(Path.Combine(ModuleDirectory, "x64", "Release") + "/NoLo_USBHID.dll");
		}

		if (Target.Platform == UnrealTargetPlatform.Android)
		{
			PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "libs", "armeabi-v7a","libNoloVRClient.so"));
			PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "libs", "arm64-v8a","libNoloVRClient.so"));
			//PublicAdditionalLibraries.Add("NoloVRClient"); 
		}
	}
}