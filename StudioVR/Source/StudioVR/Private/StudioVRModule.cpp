// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "StudioVRPrivate.h"
#include "StudioVRSettings.h"
#include "Modules/ModuleManager.h"

#if WITH_EDITOR
#include "ISettingsModule.h"
#include "ISettingsSection.h"
#endif

#if BUILD_VR_HUAWEI
#include "VRAdapter/VRAdapter_Huawei.h"
#elif BUILD_VR_WAVE
#include "VRAdapter/VRAdapter_Wave.h"
#elif BUILD_VR_PICO
#include "VRAdapter/VRAdapter_Pico.h"
#elif BUILD_VR_OCULUS
#include "VRAdapter/VRAdapter_Oculus.h"
#elif BUILD_VR_STEAM
#include "VRAdapter/VRAdapter_Steam.h"
#elif BUILD_VR_NOLO
#include "VRAdapter/VRAdapter_Nolo.h"
#elif BUILD_VR_GSXR
#include "VRAdapter/VRAdapter_GSXR.h"
#endif


#define LOCTEXT_NAMESPACE "FStudioVRModule"

IVRAdaptiveInterface* GVRAdapter = nullptr;

class FStudioVRModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override
	{
	#if BUILD_VR_HUAWEI
		FModuleManager::Get().LoadModuleChecked(TEXT("HuaweiVRSDK"));
		FModuleManager::Get().LoadModuleChecked(TEXT("HuaweiVRController"));
		GVRAdapter = new FVRAdapter_Huawei();
	#elif BUILD_VR_WAVE
		FModuleManager::Get().LoadModuleChecked(TEXT("WVR"));
		FModuleManager::Get().LoadModuleChecked(TEXT("WaveVR"));
		FModuleManager::Get().LoadModuleChecked(TEXT("WaveVRInput"));
		FModuleManager::Get().LoadModuleChecked(TEXT("WaveVRGesture"));
		GVRAdapter = new FVRAdapter_Wave();
	#elif BUILD_VR_PICO
		FModuleManager::Get().LoadModuleChecked(TEXT("PicoMobile"));
		FModuleManager::Get().LoadModuleChecked(TEXT("PicoMobileController"));
		FModuleManager::Get().LoadModuleChecked(TEXT("PicoNeoController"));
		FModuleManager::Get().LoadModuleChecked(TEXT("OnlineSubsystemPico"));
		GVRAdapter = new FVRAdapter_Pico();
	#elif BUILD_VR_OCULUS
		FModuleManager::Get().LoadModuleChecked(TEXT("OculusHMD"));
		FModuleManager::Get().LoadModuleChecked(TEXT("OculusMR"));
		GVRAdapter = new FVRAdapter_Oculus();
	#elif BUILD_VR_STEAM
		FModuleManager::Get().LoadModuleChecked(TEXT("SteamVR"));
		GVRAdapter = new FVRAdapter_Steam();
	#elif BUILD_VR_NOLO
		FModuleManager::Get().LoadModuleChecked(TEXT("NoloVR"));
		FModuleManager::Get().LoadModuleChecked(TEXT("NibiruHMD"));
		GVRAdapter = new FVRAdapter_Nolo();
	#elif BUILD_VR_GSXR
		FModuleManager::Get().LoadModuleChecked(TEXT("GSXRHMD"));
		FModuleManager::Get().LoadModuleChecked(TEXT("GSXRInput"));
		GVRAdapter = new FVRAdapter_GSXR();
	#endif

		RegisterSettings();
	}

	virtual void ShutdownModule() override
	{
		if (GVRAdapter != nullptr)
		{
			delete GVRAdapter;
			GVRAdapter = nullptr;
		}

		UnregisterSettings();
	}

	void RegisterSettings()
	{
	#if WITH_EDITOR
		ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");
		if (!SettingsModule)
			return;

		const auto Section = SettingsModule->RegisterSettings("Project", "Plugins", "StudioVR",
			LOCTEXT("StudioVREditorSettingsName", "StudioVR"),
			LOCTEXT("StudioVREditorSettingsDescription", "StudioVR Settings"),
			GetMutableDefault<UStudioVRSettings>());
		Section->OnModified().BindRaw(this, &FStudioVRModule::OnSettingsModified);
	#endif
	}

	void UnregisterSettings()
	{
	#if WITH_EDITOR
		ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");
		if (SettingsModule)
			SettingsModule->UnregisterSettings("Project", "Plugins", "StudioVR");
	#endif
	}

	bool OnSettingsModified()
	{
		return true;
	}
};


#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FStudioVRModule, StudioVR)


DEFINE_LOG_CATEGORY(LogStudioVR)