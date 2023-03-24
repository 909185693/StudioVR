// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class HuaweiVRSDK : ModuleRules
{
    public HuaweiVRSDK(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PrivateIncludePaths.AddRange(
            new string[] {
                "HuaweiVRSDK/Private",
                Path.Combine(Path.GetFullPath(Target.RelativeEnginePath), "Source/Runtime/Renderer/Private"),
                Path.Combine(Path.GetFullPath(Target.RelativeEnginePath), "Source/Runtime/Launch/Public"),
				// ... add other private include paths required here ...
			}
            );

        if (Target.Platform == UnrealTargetPlatform.Android)
        {
            PrivateDependencyModuleNames.Add("Launch");

            PublicSystemLibraryPaths.Add(Path.Combine(ModuleDirectory, "libs", "android_arm"));
            //PublicSystemLibraryPaths.Add(Path.Combine(ModuleDirectory, "lib", "android_arm64"));

            PublicAdditionalLibraries.Add("PluginProxy");

            AdditionalPropertiesForReceipt.Add("AndroidPlugin", Path.Combine(ModuleDirectory, "HuaweiVRSDK_APL.xml"));
        }
        /*
        if (Target.Platform != UnrealTargetPlatform.Mac)
        {
            PrivateDependencyModuleNames.AddRange(new string[] { "OpenGLDrv" });
            AddEngineThirdPartyPrivateStaticDependencies(Target, "OpenGL");
            PrivateIncludePaths.AddRange(
                new string[] {
                    Path.Combine(Path.GetFullPath(BuildConfiguration.RelativeEnginePath), "Source/Runtime/OpenGLDrv/Private"),
                    // ... add other private include paths required here ...
                }
            );
        }*/

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "InputCore",
                "RHI",
                "RenderCore",
                "Renderer",
                "HeadMountedDisplay",
            }
            );

        if (Target.bBuildEditor == true)
        {
            PrivateDependencyModuleNames.Add("UnrealEd");
        }
        else
        {
            PrivateDependencyModuleNames.AddRange(new string[] { "OpenGLDrv" });
            AddEngineThirdPartyPrivateStaticDependencies(Target, "OpenGL");
            PrivateIncludePaths.AddRange(
                new string[] {
                    Path.Combine(Path.GetFullPath(Target.RelativeEnginePath), "Source/Runtime/OpenGLDrv/Private"),
                }
            );
        }
    }
}
