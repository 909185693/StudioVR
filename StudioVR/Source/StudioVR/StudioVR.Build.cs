// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System;
using System.IO;
using System.Text;
using System.Runtime.InteropServices;

public class StudioVR : ModuleRules
{
    public StudioVR(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(
            new string[] {
                // ... add other public include paths required here ...
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

        // None, HuaweiVR, WaveVR, PicoVR, OculusVR, SteamVR, NoloVR
        string BulidVR = "None";

        PublicDefinitions.Add("BUILD_VR_MODULE=" + (BulidVR != "None" ? 1 : 0));

        System.Console.WriteLine("HMD Current build vr is \"" + BulidVR + "\"");

        string UPluginFilePath = Path.Combine(PluginDirectory, "StudioVR.uplugin");
        string FindPluginsText = @"""Plugins"": [";

        bool[] EnablePlugins = new bool[11];

        string UPluginText = File.ReadAllText(UPluginFilePath);
        UPluginText = UPluginText.Substring(0, UPluginText.IndexOf(FindPluginsText) + FindPluginsText.Length);

        if (BulidVR == "HuaweiVR")
        {
            EnablePlugins[0] = true;
            EnablePlugins[1] = true;

            PrivateDependencyModuleNames.AddRange(
                new string[]
                {
                    "HuaweiVRSDK",
                    "HuaweiVRController"
                }
            );

            PublicIncludePaths.AddRange(
                new string[] {
                    PluginDirectory + "../../HuaweiVR/HuaweiVRController/Source/HuaweiVRController/Public",
                    PluginDirectory + "../../HuaweiVR/HuaweiVRController/Source/HuaweiVRController/Private",
                }
            );
        }
        else if (BulidVR == "WaveVR")
        {
            EnablePlugins[2] = true;

            PrivateDependencyModuleNames.AddRange(
                new string[]
                {
                    "WVR",
                    "WaveVR",
                    "WaveVRInput",
                    "WaveVRGesture"
                }
            );

            PublicIncludePaths.AddRange(
                new string[] {
                    PluginDirectory + "../../WaveVR/Source/WaveVR/Public",
                    PluginDirectory + "../../WaveVR/Source/WaveVR/Private",
                }
            );
        }
        else if (BulidVR == "PicoVR")
        {
            EnablePlugins[3] = true;
            EnablePlugins[4] = true;
            EnablePlugins[5] = true;
            EnablePlugins[6] = true;

            PrivateDependencyModuleNames.AddRange(
                new string[]
                {
                    "PicoMobile",
                    "PicoMobileController",
                    "PicoNeoController",
                    "OnlineSubsystemPico"
                }
            );
        }
        else if (BulidVR == "OculusVR")
        {
            EnablePlugins[7] = true;

            PrivateDependencyModuleNames.AddRange(
                new string[]
                {
                    "OculusHMD",
                    "OculusMR"
                }
            );
        }
        else if (BulidVR == "SteamVR")
        {
            EnablePlugins[8] = true;
        }
        else if (BulidVR == "NoloVR")
        {
            EnablePlugins[9] = true;
            EnablePlugins[10] = true;

            PrivateDependencyModuleNames.AddRange(
                new string[]
                {
                    "NoloVR",
                    "NibiruHMD"
                }
            );
        }
        else
        {
            System.Console.WriteLine("Current build vr module \"" + BulidVR + "\" not support.");
        }

        string PluginsFormat = @"
        [
            ""Name"": ""HuaweiVRSDK"",
            ""Enabled"": {0}
        ],
        [
            ""Name"": ""HuaweiVRController"",
            ""Enabled"": {1}
        ],
        [
            ""Name"": ""WaveVR"",
            ""Enabled"": {2}
        ],
        [
            ""Name"": ""PicoMobile"",
            ""Enabled"": {3}
        ],
        [
            ""Name"": ""PicoMobileController"",
            ""Enabled"": {4}
        ],
        [
            ""Name"": ""PicoNeoController"",
            ""Enabled"": {5}
        ],
        [
            ""Name"": ""OnlineSubsystemPico"",
            ""Enabled"": {6}
        ],
        [
            ""Name"": ""OculusVR"",
            ""Enabled"": {7}
        ],
        [
            ""Name"": ""SteamVR"",
            ""Enabled"": {8}
        ],
        [
            ""Name"": ""NoloVR"",
            ""Enabled"": {9}
        ],
        [
            ""Name"": ""NibiruHMD"",
            ""Enabled"": {10}
        ]";

        string PluginsListText = System.String.Format(PluginsFormat, EnablePlugins[0].ToString().ToLower(),
            EnablePlugins[1].ToString().ToLower(),
            EnablePlugins[2].ToString().ToLower(),
            EnablePlugins[3].ToString().ToLower(),
            EnablePlugins[4].ToString().ToLower(),
            EnablePlugins[5].ToString().ToLower(),
            EnablePlugins[6].ToString().ToLower(),
            EnablePlugins[7].ToString().ToLower(),
            EnablePlugins[8].ToString().ToLower(),
            EnablePlugins[9].ToString().ToLower(),
            EnablePlugins[10].ToString().ToLower());
        PluginsListText = PluginsListText.Replace('[', '{');
        PluginsListText = PluginsListText.Replace(']', '}');

        UPluginText += PluginsListText;
        UPluginText += @"
    ]
}";

        File.WriteAllText(UPluginFilePath, UPluginText);

        string[] BuildVRDefinitions = {
            "BUILD_VR_HUAWEI",
            "",
            "BUILD_VR_WAVE",
            "BUILD_VR_PICO",
            "",
            "",
            "",
            "BUILD_VR_OCULUS",
            "BUILD_VR_STEAM",
            "BUILD_VR_NOLO",
            ""
        };

        for (int Index = 0; Index < EnablePlugins.Length; Index++)
        {
            string BuildVRDefinition = BuildVRDefinitions[Index];
            if (!String.IsNullOrEmpty(BuildVRDefinition))
            {
                string Definition = String.Format("{0}={1}", BuildVRDefinition, EnablePlugins[Index] ? 1 : 0);
                PublicDefinitions.Add(Definition);
            }
        }
    }
}
