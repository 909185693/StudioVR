// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "StudioVRPrivate.h"
#include "StudioVRSettings.h"
#include "Modules/ModuleManager.h"

#if WITH_EDITOR
#include "ISettingsModule.h"
#include "ISettingsSection.h"
#endif


#define LOCTEXT_NAMESPACE "FStudioVRModule"

IStudioVRAdapterInterface* GVRAdapter = nullptr;

class FStudioVRModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override
	{
	#if BUILD_VR_HUAWEI
		FModuleManager::Get().LoadModuleChecked(TEXT("HuaweiVRAdapter"));
	#elif BUILD_VR_WAVE
		FModuleManager::Get().LoadModuleChecked(TEXT("WaveVRAdapter"));
	#elif BUILD_VR_PICO
		FModuleManager::Get().LoadModuleChecked(TEXT("PicoVRAdapter"));
	#elif BUILD_VR_OCULUS
		FModuleManager::Get().LoadModuleChecked(TEXT("OculusVRAdapter"));
	#elif BUILD_VR_STEAM
		FModuleManager::Get().LoadModuleChecked(TEXT("SteamVRAdapter"));
	#elif BUILD_VR_NOLO
		FModuleManager::Get().LoadModuleChecked(TEXT("NoloVRAdapter"));
	#elif BUILD_VR_GSXR
		FModuleManager::Get().LoadModuleChecked(TEXT("GSXRAdapter"));
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

	virtual void RegisterVRAdapter(IStudioVRAdapterInterface* VRAdapter)
	{
		GVRAdapter = VRAdapter;
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