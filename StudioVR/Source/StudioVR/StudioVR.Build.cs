// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using System;
using System.IO;
using System.Collections.Generic;
using UnrealBuildTool;
#if UE_5_0_OR_LATER
using EpicGames.Core;
#else
using Tools.DotNETCommon;
#endif

public class StudioVR : ModuleRules
{
    public StudioVR(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(
            new string[] {
                Path.Combine(ModuleDirectory, "Public/Interfaces")
            }
        );

        PrivateIncludePaths.AddRange(
            new string[] {
				// ... add other private include paths required here ...
			}
        );


        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "Engine",
                "UMG",
                "InputDevice",
                "HeadMountedDisplay"
            }
        );


        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "Renderer"
            }
        );


        DynamicallyLoadedModuleNames.AddRange(
            new string[]
            {
				// ... add any modules that your module loads dynamically here ...
			}
        );

        Dictionary<string, int> BuildDefinitions = new Dictionary<string, int>();
        BuildDefinitions.Add("BUILD_VR_HUAWEI", 0);
        BuildDefinitions.Add("BUILD_VR_WAVE", 0);
        BuildDefinitions.Add("BUILD_VR_PICO", 0);
        BuildDefinitions.Add("BUILD_VR_OCULUS", 0);
        BuildDefinitions.Add("BUILD_VR_STEAM", 0);
        BuildDefinitions.Add("BUILD_VR_NOLO", 0);
        BuildDefinitions.Add("BUILD_VR_GSXR", 0);

        string VRAdapterType = "None";

        if (Target.Type != TargetType.Server)
        {
            var ProjectDir = Target.ProjectFile.Directory;
            var ConfigFilePath = ProjectDir + "/Config/DefaultStudioVR.ini";
            var ConfigFileReference = new FileReference(ConfigFilePath);
            var ConfigFile = FileReference.Exists(ConfigFileReference) ? new ConfigFile(ConfigFileReference) : new ConfigFile();
            var Config = new ConfigHierarchy(new[] { ConfigFile });
            
            const string Section = "/Script/StudioVR.StudioVRSettings";
            Config.GetString(Section, "VRAdapterType", out VRAdapterType);
        }

        PublicDefinitions.Add("BUILD_VR_MODULE=" + (VRAdapterType == "None" ? 1 : 0));

        System.Console.WriteLine("HMD Current build vr is \"" + VRAdapterType + "\"");

        if (VRAdapterType == "HuaweiVR")
        {
            BuildDefinitions["BUILD_VR_HUAWEI"] = 1;
            DynamicallyLoadedModuleNames.Add("HuaweiVRAdapter");
        }
        else if (VRAdapterType == "WaveVR")
        {
            BuildDefinitions["BUILD_VR_WAVE"] = 1;
            DynamicallyLoadedModuleNames.Add("WaveVRAdapter");
        }
        else if (VRAdapterType == "PicoVR")
        {
            BuildDefinitions["BUILD_VR_PICO"] = 1;
            DynamicallyLoadedModuleNames.Add("PicoVRAdapter");
        }
        else if (VRAdapterType == "OculusVR")
        {
            BuildDefinitions["BUILD_VR_OCULUS"] = 1;
            DynamicallyLoadedModuleNames.Add("OculusVRAdapter");
        }
        else if (VRAdapterType == "SteamVR")
        {
            BuildDefinitions["BUILD_VR_STEAM"] = 1;
            DynamicallyLoadedModuleNames.Add("SteamVRAdapter");
        }
        else if (VRAdapterType == "NoloVR")
        {
            BuildDefinitions["BUILD_VR_NOLO"] = 1;
            DynamicallyLoadedModuleNames.Add("NoloVRAdapter");
        }
        else if (VRAdapterType == "GSXR")
        {
            BuildDefinitions["BUILD_VR_GSXR"] = 1;
            DynamicallyLoadedModuleNames.Add("GSXRAdapter");
        }
        else
        {
            System.Console.WriteLine("Current build vr module \"" + VRAdapterType + "\" not support.");
        }

        foreach (KeyValuePair<string, int> Pair in BuildDefinitions)
        {
            PublicDefinitions.Add(Pair.Key + "=" + Pair.Value);
        }
    }
}
