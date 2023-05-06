#pragma once
#include "CoreMinimal.h"
#include "XRSwapChain.h"
class FGSXRSwapChain: public FXRSwapChain
{
public:
	FGSXRSwapChain(TArray<FTextureRHIRef>&& InRHITextureSwapChain, const FTextureRHIRef& AliasedTexture);
	virtual ~FGSXRSwapChain() override;

	virtual void ReleaseCurrentImage_RHIThread() override;
	

protected:
	virtual void ReleaseResources_RHIThread() override;

};

#if ENGINE_MINOR_VERSION>25
FXRSwapChainPtr CreateSwapChain_OpenGL(uint8 Format, uint32 SizeX, uint32 SizeY, uint32 ArraySize, uint32 NumMips, uint32 NumSamples, ETextureCreateFlags Flags, ETextureCreateFlags TargetableTextureFlags, uint32 MSAAValue,TArray<uint32> TexutreResources);
#else
FXRSwapChainPtr CreateSwapChain_OpenGL(uint8 Format, uint32 SizeX, uint32 SizeY, uint32 ArraySize, uint32 NumMips, uint32 NumSamples, uint32 Flags, uint32 TargetableTextureFlags,uint32 MASSValue,TArray<uint32> TexutreResources);
#endif

#if ENGINE_MINOR_VERSION>25
FXRSwapChainPtr CreateSwapChain_Vulkan(uint8 Format, uint32 SizeX, uint32 SizeY, uint32 ArraySize, uint32 NumMips, uint32 NumSamples, ETextureCreateFlags Flags, ETextureCreateFlags TargetableTextureFlags, uint32 MSAAValue,TArray<uint32> TexutreResources);
#else
FXRSwapChainPtr CreateSwapChain_Vulkan(uint8 Format, uint32 SizeX, uint32 SizeY, uint32 ArraySize, uint32 NumMips, uint32 NumSamples, uint32 Flags, uint32 TargetableTextureFlags,uint32 MASSValue,TArray<uint32> TexutreResources);
#endif