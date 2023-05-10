#include "GSXRHMDModule.h"
#include "GSXRHMD.h"
#include "GSXRSetting.h"
#include "SceneViewExtension.h"
#if WITH_EDITOR
#include "ISettingsModule.h"
#include "ISettingsSection.h"
#include "PropertyEditorModule.h"
#endif
#define LOCTEXT_NAMESPACE "FGSXRHMDModule"
FGSXRHMDModule::FGSXRHMDModule()
{
}

FGSXRHMDModule::~FGSXRHMDModule()
{
}

TSharedPtr<IXRTrackingSystem, ESPMode::ThreadSafe> FGSXRHMDModule::CreateTrackingSystem()
{
	TSharedPtr<FGSXRHMD, ESPMode::ThreadSafe > GSXRHMD = FSceneViewExtensions::NewExtension<FGSXRHMD>();
	return GSXRHMD;
}

TSharedPtr<IHeadMountedDisplayVulkanExtensions, ESPMode::ThreadSafe> FGSXRHMDModule::GetVulkanExtensions()
{
	return nullptr;
}

bool FGSXRHMDModule::IsHMDConnected()
{
	return true;
}

bool FGSXRHMDModule::PreInit()
{
	return true;
}

void FGSXRHMDModule::StartupModule()
{
	IHeadMountedDisplayModule::StartupModule();
#if WITH_EDITOR
	if (GIsEditor)
	{
		// register settings
		ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");
		if (SettingsModule != nullptr)
		{
			SettingsModule->RegisterSettings("Project", "Plugins", "GSXRSetting",
			LOCTEXT("GSXRSettingsName", "GSXR Settings"),
						LOCTEXT("GSXRSettingsDescription", "Configure the GSXR SDK."),
				GetMutableDefault<UGSXRSettings>()
			);
		}
	}
#endif //WITH_EDITOR
}

void FGSXRHMDModule::ShutdownModule()
{
	IHeadMountedDisplayModule::ShutdownModule();
}
IMPLEMENT_MODULE(FGSXRHMDModule, GSXRHMD)
#undef LOCTEXT_NAMESPACE