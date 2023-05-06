#include "GSXRRenderBridge.h"
#include "GSXRHMD.h"
#include "GSXRSwapChain.h"

FGSXRRenderBridge::FGSXRRenderBridge(FGSXRHMD* HMD)
	:FXRRenderBridge()
	,GSXRHMD(HMD)
{
}

FGSXRRenderBridge::~FGSXRRenderBridge()
{
}

bool FGSXRRenderBridge::NeedsNativePresent()
{
	return false;
}

bool FGSXRRenderBridge::Present(int32& InOutSyncInterval)
{
	GSXRHMD->EndFrame_RHIThread();	
	InOutSyncInterval = 0; // VSync off
	const bool bNeedsNativePresent = !GSXRHMD->IsStandaloneStereoOnlyDevice();
	return bNeedsNativePresent;
}

class FGSXRRenderBridge_OpenGL : public FGSXRRenderBridge
{
public:
	FGSXRRenderBridge_OpenGL(FGSXRHMD* HMD)
		:FGSXRRenderBridge(HMD)
	{
		
	}
#if ENGINE_MINOR_VERSION >25
	virtual FXRSwapChainPtr CreateSwapChain(uint8 Format, uint32 SizeX, uint32 SizeY, uint32 ArraySize, uint32 NumMips, uint32 NumSamples, ETextureCreateFlags Flags, ETextureCreateFlags TargetableTextureFlags,uint32 MSAAValue,TArray<uint32> TexutreResources) override
#else	
	virtual FXRSwapChainPtr CreateSwapChain(uint8 Format, uint32 SizeX, uint32 SizeY, uint32 ArraySize, uint32 NumMips, uint32 NumSamples, uint32 Flags, uint32 TargetableTextureFlags,uint32 MSAAValue,TArray<uint32> TexutreResources) override
#endif
	{
		SwapChain = CreateSwapChain_OpenGL(Format, SizeX, SizeY, ArraySize, NumMips, NumSamples, Flags, TargetableTextureFlags,MSAAValue, TexutreResources);
		return  SwapChain;
	}
};

FGSXRRenderBridge* CreateRenderBridge_OpenGL(FGSXRHMD* HMD)
{
	return new FGSXRRenderBridge_OpenGL(HMD);
}

class FGSXRRenderBridge_Vulkan : public FGSXRRenderBridge
{
public:
	FGSXRRenderBridge_Vulkan(FGSXRHMD* HMD)
		:FGSXRRenderBridge(HMD)
	{	
	}

#if ENGINE_MINOR_VERSION >25
	virtual FXRSwapChainPtr CreateSwapChain(uint8 Format, uint32 SizeX, uint32 SizeY, uint32 ArraySize, uint32 NumMips, uint32 NumSamples, ETextureCreateFlags Flags, ETextureCreateFlags TargetableTextureFlags,uint32 MSAAValue,TArray<uint32> TexutreResources) override
#else	
	virtual FXRSwapChainPtr CreateSwapChain(uint8 Format, uint32 SizeX, uint32 SizeY, uint32 ArraySize, uint32 NumMips, uint32 NumSamples, uint32 Flags, uint32 TargetableTextureFlags,uint32 MSAAValue,TArray<uint32> TexutreResources) override
#endif
	{
		return CreateSwapChain_Vulkan(Format, SizeX, SizeY, ArraySize, NumMips, NumSamples, Flags, TargetableTextureFlags, MSAAValue,TexutreResources);
	}
};

FGSXRRenderBridge* CreateRenderBridge_Vulkan(FGSXRHMD* HMD) { return new FGSXRRenderBridge_Vulkan(HMD); }