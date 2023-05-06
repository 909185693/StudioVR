#pragma once
#include "IGSXRHMDPlugin.h"

class FGSXRHMDModule : public IGSXRHMDPlugin
{
public:
	FGSXRHMDModule();
	virtual ~FGSXRHMDModule() override;

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	virtual TSharedPtr< class IXRTrackingSystem, ESPMode::ThreadSafe > CreateTrackingSystem() override;
	virtual TSharedPtr< IHeadMountedDisplayVulkanExtensions, ESPMode::ThreadSafe > GetVulkanExtensions() override;
	virtual bool IsHMDConnected() override;
	virtual bool PreInit() override;

	FString GetModuleKeyName() const override
	{
		return FString(TEXT("GSXRHMD"));
	}

	void GetModuleAliases(TArray<FString>& AliasesOut) const override
	{
		AliasesOut.Add(TEXT("GSXRHMD"));
	}

	static inline FGSXRHMDModule& Get()
	{
		return FModuleManager::LoadModuleChecked< FGSXRHMDModule >("GSXRHMD");
	}
};
