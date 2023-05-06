// Fill out your copyright notice in the Description page of Project Settings.

using System.IO;
using UnrealBuildTool;

public class GSXRLibrary : ModuleRules
{
	public GSXRLibrary(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External; 
		PrivateDependencyModuleNames.AddRange(
			new []
			{
				"Core",
				"CoreUObject",
				"Engine",
				"InputCore",
				"RHI",
				"RenderCore",
				"Renderer",
				"OpenGLDrv",
				"OpenGL",
				"InputDevice"
				// ... add private dependencies that you statically link with here ...	
			}
		);
		
		PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "include"));
		if (Target.Platform == UnrealTargetPlatform.Android)
		{
			PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "armeabi-v7a", "libgsxr_loader.so"));
			PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "arm64-v8a", "libgsxr_loader.so"));
		}
	}
}
