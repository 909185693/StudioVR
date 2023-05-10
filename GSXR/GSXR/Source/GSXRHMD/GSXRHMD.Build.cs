// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class GSXRHMD : ModuleRules
{
	public GSXRHMD(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		string EngineDir = Path.GetFullPath(Target.RelativeEnginePath);
		string RendererPrivatePath = Path.Combine(EngineDir, "Source/Runtime/Renderer/Private");
		string OpenGlDrvPrivatePath = Path.Combine(EngineDir, "Source/Runtime/OpenGLDrv/Private");
		string VulkanRhiPrivatePath = Path.Combine(EngineDir, "Source/Runtime/VulkanRHI/Private");
		string VulkanRhiPrivateAndroidPath = Path.Combine(EngineDir, "Source/Runtime/VulkanRHI/Private/Android");

		PrivateIncludePaths.AddRange(
			new string[] {
				"GSXRHMD/Public",
				"GSXRHMD/Private",
				RendererPrivatePath,
				OpenGlDrvPrivatePath,
				VulkanRhiPrivatePath,
				VulkanRhiPrivateAndroidPath,
				// ... add other private include paths required here ...
			}
			);
			
		PublicIncludePathModuleNames.AddRange(
			new [] {
				"Launch",
			});		
		
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
				"InputDevice",
				"VulkanRHI",
				"GSXRLibrary",
				// ... add private dependencies that you statically link with here ...	
			}
		);
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"GSXRLibrary",
				"Projects",
				"HeadMountedDisplay",
                "ProceduralMeshComponent",
                "EyeTracker"
				// ... add other public dependencies that you statically link with here ...
			}
			);

		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
		
		if (Target.bBuildEditor == true)
		{
			PrivateDependencyModuleNames.AddRange(
				new string[]
				{
					"UnrealEd"
				});
		}
		
		if (Target.Platform == UnrealTargetPlatform.Android)
		{
			string PluginPath = Utils.MakePathRelativeTo(ModuleDirectory, Target.RelativeEnginePath);
			AdditionalPropertiesForReceipt.Add("AndroidPlugin", Path.Combine(PluginPath, "GSXR_UPL.xml"));
		}
	}
}
