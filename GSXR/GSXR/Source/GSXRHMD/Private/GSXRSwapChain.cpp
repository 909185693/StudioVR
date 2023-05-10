#include "GSXRSwapChain.h"
#include "GSXRHMD.h"
#include "OpenGLDrv.h"

FGSXRSwapChain::FGSXRSwapChain(TArray<FTextureRHIRef>&& InRHITextureSwapChain, const FTextureRHIRef& AliasedTexture)
	:FXRSwapChain(MoveTemp(InRHITextureSwapChain), AliasedTexture)
{
}

FGSXRSwapChain::~FGSXRSwapChain()
{
}


void FGSXRSwapChain::ReleaseCurrentImage_RHIThread()
{
}

void FGSXRSwapChain::ReleaseResources_RHIThread()
{
}

#if ENGINE_MINOR_VERSION>25
FXRSwapChainPtr CreateSwapChain_OpenGL(uint8 Format, uint32 SizeX, uint32 SizeY, uint32 ArraySize, uint32 NumMips, uint32 NumSamples, ETextureCreateFlags Flags, ETextureCreateFlags TargetableTextureFlags,uint32 MSAAValue,TArray<uint32> TexutreResources)
#else
FXRSwapChainPtr CreateSwapChain_OpenGL(uint8 Format, uint32 SizeX, uint32 SizeY, uint32 ArraySize, uint32 NumMips, uint32 NumSamples, uint32 Flags, uint32 TargetableTextureFlags,uint32 MSAAValue,TArray<uint32> TexutreResources)
#endif
{
	UE_LOG(LogTemp, Log, TEXT("RHICreateTexture2DArrayFromResource 1 (%d,%d,%d,%d,%d,%d)"), SizeX, SizeY, NumMips, NumSamples, Flags, TargetableTextureFlags);
#if USE_GSXRAPI
	TArray<FTextureRHIRef> TextureChains;
	FOpenGLDynamicRHI* DynamicRHI = static_cast<FOpenGLDynamicRHI*>(GDynamicRHI);
	FTextureRHIRef ChainTarget = nullptr;
	if (ArraySize == 1)
	{

		ChainTarget = static_cast<FTextureRHIRef>(DynamicRHI->RHICreateTexture2DFromResource(EPixelFormat(Format), SizeX, SizeY, NumMips, NumSamples, MSAAValue, FClearValueBinding::Black , TexutreResources[0], TargetableTextureFlags));
		for (const auto& TexutreResource : TexutreResources)
		{
			TextureChains.Add(static_cast<FTextureRHIRef>(DynamicRHI->RHICreateTexture2DFromResource(EPixelFormat(Format), SizeX, SizeY, NumMips, NumSamples, MSAAValue, FClearValueBinding::Black, TexutreResource, TargetableTextureFlags)));
		}
	}else if (ArraySize == 2)
	{
		
		//ChainTarget = static_cast<FTextureRHIRef>(DynamicRHI->RHICreateTexture2DArrayFromResource(EPixelFormat(Format), SizeX, SizeY, ArraySize,NumMips, NumSamples, MSAAValue, FClearValueBinding::Black, TexutreResources[0], TargetableTextureFlags));
		for (const auto& TexutreResource : TexutreResources)
		{
			TextureChains.Add(static_cast<FTextureRHIRef>(DynamicRHI->RHICreateTexture2DArrayFromResource(EPixelFormat(Format), SizeX, SizeY, ArraySize, NumMips, NumSamples, MSAAValue, FClearValueBinding::Black, TexutreResource, TargetableTextureFlags)));
		}
		ChainTarget = static_cast<FTextureRHIRef>(GDynamicRHI->RHICreateAliasedTexture((FTextureRHIRef&)TextureChains[0]));
		if (((FOpenGLTexture2DArray*)ChainTarget->GetTexture2DArray()) != NULL) {
			UE_LOG(LogTemp, Log, TEXT("RHICreateTexture2DArrayFromResource 2"));
		}
		else {
			UE_LOG(LogTemp, Log, TEXT("RHICreateTexture2DArrayFromResource 3"));
		}
	}
	for (const auto& TextureChain : TextureChains)
	{
		UE_LOG(LogTemp,Log,TEXT("TextureChainsID = %d"),*(GLuint*)TextureChain->GetNativeResource());
	}
	return CreateXRSwapChain<FGSXRSwapChain>(MoveTemp(TextureChains), ChainTarget);
#endif
	return FXRSwapChainPtr();
}

#if ENGINE_MINOR_VERSION>25
FXRSwapChainPtr CreateSwapChain_Vulkan(uint8 Format, uint32 SizeX, uint32 SizeY, uint32 ArraySize, uint32 NumMips, uint32 NumSamples, ETextureCreateFlags Flags, ETextureCreateFlags TargetableTextureFlags, uint32 MSAAValue,TArray<uint32> TexutreResources)
#else
FXRSwapChainPtr CreateSwapChain_Vulkan(uint8 Format, uint32 SizeX, uint32 SizeY, uint32 ArraySize, uint32 NumMips, uint32 NumSamples, uint32 Flags, uint32 TargetableTextureFlags, uint32 MSAAValue,TArray<uint32> TexutreResources)
#endif
{
	return FXRSwapChainPtr();
}
