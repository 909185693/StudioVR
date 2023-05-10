// Fill out your copyright notice in the Description page of Project Settings.


#include "GSXRInstance.h"

#include "GSXRUtils.h"
#include "Features/IModularFeatures.h"
#include "GSXRInput/Private/GSXRInput.h"
#if PLATFORM_ANDROID
#include "OpenGLDrvPrivate.h"
#include "OpenGLResources.h"
#include "Android/AndroidJNI.h"
#include "Android/AndroidApplication.h"
#include "Android/AndroidEGL.h"
#endif

DEFINE_LOG_CATEGORY_STATIC(GSXR_Unreal_Instance, Log, All);
UGSXRInstance* UGSXRInstance::Gsxr_Instance = NULL;

UGSXRInstance* UGSXRInstance::GetInstance()
{
	if (!Gsxr_Instance)
	{
		Gsxr_Instance = NewObject<UGSXRInstance>();
		Gsxr_Instance->AddToRoot();
	}
	return Gsxr_Instance;
}

UGSXRInstance::UGSXRInstance()	
#if USE_GSXRAPI
	:Gsxr_Runtime(nullptr)
	,Gsxr_Renderer(nullptr)
	,Gsxr_TextureQueue(nullptr)
	,Gsxr_RenderLooper(nullptr)
	,Gsxr_TextureElement()
#endif
{
}

#if USE_GSXRAPI
bool UGSXRInstance::CreateRuntime()
{
	GSXR_AndrosSystemBinding AndroidSystemBinding;
	AndroidSystemBinding.next = nullptr;
#if PLATFORM_ANDROID
	AndroidSystemBinding.androsAppVM = GJavaVM;
	AndroidSystemBinding.androsAppActivity = FAndroidApplication::GetGameActivityThis();
#endif
	GSXR_RuntimeCreateInfo RuntimeCreateInfo;
	RuntimeCreateInfo.next = nullptr;
	RuntimeCreateInfo.appType = GSXR_ApplicationType_VR;
	RuntimeCreateInfo.systemBinding = &AndroidSystemBinding;
	GSXR_Result Result = GSXR_CreateRuntime(&RuntimeCreateInfo,&Gsxr_Runtime);
	
	if (Result == GSXR_Result_Success)
	{
		return true;
	}
	UE_LOG(GSXR_Unreal_Instance, Log, TEXT("GSXR_CreateRuntime Falid,ErrorCode:%d"), Result);
	return false;
}

bool UGSXRInstance::GetDeviceConnectState(GSXR_DeviceType DeviceType,bool& bConnectState)
{
	GSXR_Bool32 ConnectState = false;
	GSXR_Result Result = GSXR_IsDeviceConnected(Gsxr_Runtime,DeviceType,&ConnectState);
	if (Result == GSXR_Result_Success)
	{
		bConnectState = ConnectState;
		return true;
	}
	UE_LOG(GSXR_Unreal_Instance, Log, TEXT("GSXR_IsDeviceConnected Falid,ErrorCode:%d"), Result);
	return false;
}

bool UGSXRInstance::CreateRender()
{
	if (Gsxr_Renderer == nullptr)
	{
		GSXR_DisplayId DisplayId = 0;
		GSXR_GetDisplay(Gsxr_Runtime,GSXR_DeviceType_HMD_VR,&DisplayId);
		GSXR_RendererCreateInfo RendererCreateInfo;
		RendererCreateInfo.graphicsApi = GSXR_GraphicsApi_OpenGLES;
		RendererCreateInfo.displayId = DisplayId;
		GSXR_Result Result = GSXR_CreateRenderer(Gsxr_Runtime,&RendererCreateInfo,&Gsxr_Renderer);
		if (Result == GSXR_Result_Success)
		{
			return true;
		}
		UE_LOG(GSXR_Unreal_Instance, Log, TEXT("GSXR_CreateRenderer Falid,ErrorCode:%d"), Result);
		return false;
	}
	UE_LOG(GSXR_Unreal_Instance,Log,TEXT("Gsxr_Renderer is not null!"));
	return true;
}

bool UGSXRInstance::GetTextureQueueOptions(GSXR_Flags64 &RecommendedOptions,GSXR_Flags64 &SupportedOptions)
{
	GSXR_Result Result = GSXR_GetTextureQueueOptions(Gsxr_Renderer,&RecommendedOptions,&SupportedOptions);
	if (Result == GSXR_Result_Success)
	{
		return true;
	}
	UE_LOG(GSXR_Unreal_Instance, Log, TEXT("GSXR_GetTextureQueueOptions Falid,ErrorCode:%d"), Result);
	return false;
}

bool UGSXRInstance::GetViewSetConfiguration(GSXR_ViewSetConfiguration &InGsxr_ViewSetConfiguration)
{
	if (Gsxr_Runtime)
	{
		uint32_t ViewSetCount = 0;
		GSXR_GetViewSetCount(Gsxr_Runtime,&ViewSetCount);
		Gsxr_ViewSetConfigurations.Empty();
		Gsxr_ViewSetConfigurations.SetNum(ViewSetCount);
		GSXR_Result Result = GSXR_GetViewSetConfigurations(Gsxr_Runtime,ViewSetCount,Gsxr_ViewSetConfigurations.GetData());
		if (Result != GSXR_Result_Success)
		{
			UE_LOG(GSXR_Unreal_Instance,Log,TEXT("GSXR_GetViewSetConfigurations Falid,ErrorCode:%d"),Result);
			return false;
		}
		for (auto ViewSetConfiguration : Gsxr_ViewSetConfigurations)
		{
			if (ViewSetConfiguration.viewType == GSXR_ViewType_Primary_Stereo)
			{
				InGsxr_ViewSetConfiguration = ViewSetConfiguration;
				Gsxr_StereoViewSetConfiguration = ViewSetConfiguration;
				return true;
			}
		}
		UE_LOG(GSXR_Unreal_Instance,Log,TEXT("GetViewSetConfiguration Falid, don't have GSXR_ViewType_Primary_Stereo viewset !"),Result);
	}
	UE_LOG(GSXR_Unreal_Instance,Log,TEXT("GetViewSetConfiguration Falid,Gsxr_Runtime is null"));
	return false;
	
}

bool UGSXRInstance::CreateTextureQueue(GSXR_Flags64 TextureQueueOptions, uint8 Format, uint32 SizeX, uint32 SizeY, uint32 SampleCount, uint32 FaceCount, uint32 NumMips, uint32 ArrayCount) {
	GSXR_TextureQueueCreateInfo TextureQueueCreateInfo;
	TextureQueueCreateInfo.options =TextureQueueOptions;
#if PLATFORM_ANDROID
	//TextureQueueCreateInfo.format = GOpenGLTextureFormats[TargetFormat].Format;
	TextureQueueCreateInfo.format = GOpenGLTextureFormats[Format].Format;
	UE_LOG(GSXR_Unreal_Instance, Log, TEXT("CreateTextureQueue Format is  %i"), TextureQueueCreateInfo.format);
#endif
	TextureQueueCreateInfo.height = SizeY;
	TextureQueueCreateInfo.width = SizeX;
	TextureQueueCreateInfo.sampleCount = SampleCount;
	TextureQueueCreateInfo.faceCount = FaceCount;
	TextureQueueCreateInfo.mipLevelCount = NumMips;
	TextureQueueCreateInfo.arrayLayerCount = ArrayCount;
	GSXR_Result Result = GSXR_CreateTextureQueue(Gsxr_Renderer, &TextureQueueCreateInfo, &Gsxr_TextureQueue);
	if (Result != GSXR_Result_Success)
	{
		UE_LOG(GSXR_Unreal_Instance, Log, TEXT("GSXR_CreateTextureQueue Falid,ErrorCode:%d"), Result);
		return false;
	}
	return true;
}

bool UGSXRInstance::CreateTextureQueue(GSXR_Flags64 TextureQueueOptions,uint8 Format,uint32 SizeX, uint32 SizeY, uint32 SampleCount, uint32 FaceCount, uint32 NumMips,uint32 ArrayCount,TArray<uint32_t>& TextureResources,uint8 & TargetFormat)
{
	TArray<GSXR_TextureFormat> SupportFormats;
	SupportFormats.SetNum(Gsxr_StereoViewSetConfiguration.textureFormatCount);
	GSXR_GetSupportedTextureFormats(Gsxr_Renderer,Gsxr_StereoViewSetConfiguration.textureFormatCount,SupportFormats.GetData());
	TargetFormat = Format;
	GSXR_TextureQueueCreateInfo TextureQueueCreateInfo;
#if PLATFORM_ANDROID
	/*	if (SupportFormats.Contains(GOpenGLTextureFormats[Format].Format))
	{
		TargetFormat = Format;
	}
	else*/ if (SupportFormats.Contains(GOpenGLTextureFormats[PF_R8G8B8A8].InternalFormat[0]))
	{
		UE_LOG(GSXR_Unreal_Instance, Log, TEXT("CreateTextureQueue   PF_R8G8B8A8"));
		TargetFormat = PF_R8G8B8A8;
		TextureQueueCreateInfo.format = GOpenGLTextureFormats[TargetFormat].InternalFormat[0];
	}	
	else if (SupportFormats.Contains(GOpenGLTextureFormats[PF_FloatRGBA].Format))
	{
		UE_LOG(GSXR_Unreal_Instance, Log, TEXT("CreateTextureQueue   PF_FloatRGBA"));
		TargetFormat = PF_FloatRGBA;
		TextureQueueCreateInfo.format = GOpenGLTextureFormats[TargetFormat].Format;
	}
	else
	{
		UE_LOG(GSXR_Unreal_Instance, Log, TEXT("Can not find support Format!!"));
		TargetFormat = PF_Unknown;
		return false;
	}
#endif	
	UE_LOG(GSXR_Unreal_Instance, Log, TEXT("GOpenGLTextureFormats[PF_R8G8B8A8].InternalFormat[0]  is  %i"), GOpenGLTextureFormats[PF_R8G8B8A8].InternalFormat[0]);
	UE_LOG(GSXR_Unreal_Instance, Log, TEXT("GOpenGLTextureFormats[PF_FloatRGBA].Format  is  %i"), GOpenGLTextureFormats[PF_FloatRGBA].Format);
	UE_LOG(GSXR_Unreal_Instance, Log, TEXT("CreateTextureQueue Format is  %i"), GOpenGLTextureFormats[TargetFormat].InternalFormat[0]);	
	TextureQueueCreateInfo.options = TextureQueueOptions;
//#if PLATFORM_ANDROID	
//	TextureQueueCreateInfo.format = GOpenGLTextureFormats[TargetFormat].InternalFormat[0];
//#endif
	TextureQueueCreateInfo.height = SizeY;
	TextureQueueCreateInfo.width = SizeX;
	TextureQueueCreateInfo.sampleCount = SampleCount;
	TextureQueueCreateInfo.faceCount = FaceCount;
	TextureQueueCreateInfo.mipLevelCount = NumMips;
	TextureQueueCreateInfo.arrayLayerCount = ArrayCount;
	GSXR_Result Result = GSXR_CreateTextureQueue(Gsxr_Renderer,&TextureQueueCreateInfo,&Gsxr_TextureQueue);
	if (Result != GSXR_Result_Success)
	{
		UE_LOG(GSXR_Unreal_Instance,Log,TEXT("GSXR_CreateTextureQueue Falid,ErrorCode:%d"),Result);
		return false;
	}
	uint32_t TextureCount = 0;
	GSXR_GetTextureCount(Gsxr_TextureQueue,&TextureCount);
	UE_LOG(GSXR_Unreal_Instance,Log,TEXT("GSXR_GetTextureCount   Count: %lx"),TextureCount);
	TextureResources.Empty();
	TextureResources.SetNum(TextureCount);
	for (uint32_t i = 0;i<TextureCount;i++)
	{
		GSXR_TextureElement TextureElement;
		GSXR_Result RetResult = GSXR_WaitAndAcquireAvailableTexture(Gsxr_TextureQueue,14,&TextureElement);
		UE_LOG(GSXR_Unreal_Instance,Log,TEXT("GSXR_WaitAndAcquireAvailableTexture %d ,Return :%d TextureID: %d"),i,RetResult,(*((uint32_t *)TextureElement.texture)));
		TextureResources[i] = *static_cast<uint32_t*>(TextureElement.texture);	
		GSXR_ReleaseTexture(Gsxr_TextureQueue,&TextureElement);
	}
	return true;
}

bool UGSXRInstance::SetTrackingOrigin(EHMDTrackingOrigin::Type NewTrackingOrigin)
{
	if (Gsxr_Runtime)
	{
		GSXR_SpaceOrigin SpaceOrigin = NewTrackingOrigin == EHMDTrackingOrigin::Eye?GSXR_SpaceOrigin_OnHead: GSXR_SpaceOrigin_OnFloor;
		GSXR_Result Result = GSXR_SetSpaceOrigin(Gsxr_Runtime,SpaceOrigin);
		if (Result == GSXR_Result_Success)
		{
			return true;
		}
		UE_LOG(GSXR_Unreal_Instance, Log, TEXT("GSXR_SetSpaceOrigin Falid,ErrorCode:%d"), Result);
		return false;
	}
	UE_LOG(GSXR_Unreal_Instance,Log,TEXT("GSXR_SetSpaceOrigin Falid,Gsxr_Runtime is null"));
	return false;
}

bool UGSXRInstance::GetDegreeOfFreedom(GSXR_DeviceType DeviceType,GSXR_DegreeOfFreedom& DegreeOfFreedom)
{
	if (Gsxr_Runtime)
	{	
		GSXR_Result Result = GSXR_GetDegreeOfFreedom(Gsxr_Runtime,DeviceType,&DegreeOfFreedom);
		if (Result == GSXR_Result_Success)
		{
			return true;
		}
		UE_LOG(GSXR_Unreal_Instance, Log, TEXT("GSXR_GetDegreeOfFreedom Falid,ErrorCode:%d"), Result);
		return false;
	}
	UE_LOG(GSXR_Unreal_Instance,Log,TEXT("GSXR_GetDegreeOfFreedom Falid,Gsxr_Runtime is null"));
	return false;
}

bool UGSXRInstance::GetRuntimeProperties(GSXR_RuntimeProperties &RuntimeProperties)
{
	if (Gsxr_Runtime)
	{	
		GSXR_Result Result = GSXR_GetRuntimeProperties(Gsxr_Runtime,&RuntimeProperties);
		if (Result == GSXR_Result_Success)
		{
			return true;
		}
		UE_LOG(GSXR_Unreal_Instance, Log, TEXT("GSXR_GetRuntimeProperties Falid,ErrorCode:%d"), Result);
		return false;
	}
	UE_LOG(GSXR_Unreal_Instance,Log,TEXT("GSXR_GetRuntimeProperties Falid,Gsxr_Runtime is null"));
	return false;
}

bool UGSXRInstance::GetDeviceProperties(GSXR_DeviceType DeviceType,GSXR_DeviceProperties &DeviceProperties)
{
	if (Gsxr_Runtime)
	{	
		GSXR_Result Result = GSXR_GetDeviceProperties(Gsxr_Runtime,DeviceType,&DeviceProperties);
		if (Result == GSXR_Result_Success)
		{
			return true;
		}
		UE_LOG(GSXR_Unreal_Instance, Log, TEXT("GSXR_GetDeviceProperties Falid,ErrorCode:%d"), Result);
		return false;
	}
	UE_LOG(GSXR_Unreal_Instance,Log,TEXT("GSXR_GetDeviceProperties Falid,Gsxr_Runtime is null"));
	return false;
}

bool UGSXRInstance::WaitFrame(GSXR_Time& PredictedDisplayTime)
{
	if (Gsxr_RenderLooper)
	{
		GSXR_Result Result = GSXR_WaitFrame(Gsxr_RenderLooper,&PredictedDisplayTime);
		if (Result == GSXR_Result_Success)
		{
			return true;
		}
		UE_LOG(GSXR_Unreal_Instance, Log, TEXT("GSXR_WaitFrame Falid,ErrorCode:%d"), Result);
		return false;
	}
	UE_LOG(GSXR_Unreal_Instance,Log,TEXT("GSXR_WaitFrame Falid,Gsxr_RenderLooper is null"));
	return false;
}

bool UGSXRInstance::WaitAndAcquireAvailableTexture(GSXR_Duration Duration,GSXR_TextureElement &TextureElement)
{
	if (Gsxr_RenderLooper)
	{
		GSXR_Result Result = GSXR_WaitAndAcquireAvailableTexture(Gsxr_TextureQueue,Duration,&TextureElement);
		if (Result == GSXR_Result_Success)
		{
			return true;
		}
		UE_LOG(GSXR_Unreal_Instance, Log, TEXT("GSXR_WaitAndAcquireAvailableTexture Falid,ErrorCode:%d"), Result);
		return false;
	}
	UE_LOG(GSXR_Unreal_Instance,Log,TEXT("GSXR_WaitAndAcquireAvailableTexture Falid,Gsxr_RenderLooper is null"));
	return false;
}

bool UGSXRInstance::PreRenderFrame(GSXR_FrameRenderInfo* PreRenderInfo)
{
	if (Gsxr_RenderLooper)
	{
		GSXR_Result Result = GSXR_PreRenderFrame(Gsxr_RenderLooper,PreRenderInfo);
		if (Result == GSXR_Result_Success)
		{
			return true;
		}
		UE_LOG(GSXR_Unreal_Instance, Log, TEXT("GSXR_PreRenderFrame Falid,ErrorCode:%d"), Result);
		return false;
	}
	UE_LOG(GSXR_Unreal_Instance,Log,TEXT("GSXR_PreRenderFrame Falid,Gsxr_RenderLooper is null"));
	return false;
}

bool UGSXRInstance::PostRenderFrame(GSXR_FrameRenderInfo* PostRenderInfo)
{
	if (Gsxr_RenderLooper)
	{
		GSXR_Result Result = GSXR_PostRenderFrame(Gsxr_RenderLooper,PostRenderInfo);
		if (Result == GSXR_Result_Success)
		{
			return true;
		}
		UE_LOG(GSXR_Unreal_Instance, Log, TEXT("GSXR_PostRenderFrame Falid,ErrorCode:%d"), Result);
		return false;
	}
	UE_LOG(GSXR_Unreal_Instance,Log,TEXT("GSXR_PostRenderFrame Falid,Gsxr_RenderLooper is null"));
	return false;
}

uint64_t UGSXRInstance::GetSupportedFunctions()
{
	return SupportedFunctions;
}

bool UGSXRInstance::GetViewPoseState(GSXR_Time Time, GSXR_ViewPoseState& ViewPoseState)
{
	if (Gsxr_RenderLooper)
	{
		uint32_t ViewCount = Gsxr_StereoViewSetConfiguration.viewCount;
		TArray<GSXR_ViewPoseState> ViewPoseStates;
		ViewPoseStates.SetNum(ViewCount);
		GSXR_Result Result = GSXR_GetViewPoseState(Gsxr_RenderLooper,Time,ViewCount,ViewPoseStates.GetData());
		if (Result == GSXR_Result_Success)
		{
			ViewPoseState = ViewPoseStates[0];
			return true;
		}
		UE_LOG(GSXR_Unreal_Instance, Log, TEXT("GSXR_GetViewPoseState Falid,ErrorCode:%d"), Result);
		return false;
	}
	UE_LOG(GSXR_Unreal_Instance,Log,TEXT("GSXR_GetViewPoseState Falid,Gsxr_RenderLooper is null"));
	return false;
}

bool UGSXRInstance::SubmitFrame(GSXR_FrameSubmitInfo FrameSubmitInfo)
{
	if (Gsxr_RenderLooper)
	{
		GSXR_Result Result = GSXR_SubmitFrame(Gsxr_RenderLooper,&FrameSubmitInfo);
		if (Result == GSXR_Result_Success)
		{
			return true;
		}
		UE_LOG(GSXR_Unreal_Instance, Log, TEXT("GSXR_SubmitFrame Falid,ErrorCode:%d"), Result);
		return false;
	}
	UE_LOG(GSXR_Unreal_Instance,Log,TEXT("GSXR_SubmitFrame Falid,Gsxr_RenderLooper is null"));
	return false;
}

bool UGSXRInstance::ReleaseTexture(GSXR_TextureElement TextureElement)
{
	if (Gsxr_TextureQueue)
	{
		GSXR_Result Result = GSXR_ReleaseTexture(Gsxr_TextureQueue,&TextureElement);
		if (Result == GSXR_Result_Success)
		{
			return true;
		}
		UE_LOG(GSXR_Unreal_Instance, Log, TEXT("GSXR_ReleaseTexture Falid,ErrorCode:%d"), Result);
		return false;
	}
	return false;
}

bool UGSXRInstance::StartRendering_RHIThread()
{
	if (Gsxr_Renderer != nullptr && Gsxr_RenderLooper == nullptr)
	{
		//开始渲染循环
		GSXR_AndrosOpenGLESBinding AndroidOpenGLESBinding;
#if PLATFORM_ANDROID
		//TODO:need a unreal interface to get android egl config and extensions
		//AndroidOpenGLESBinding.eglConfig
		//AndroidOpenGLESBinding.eglExtensions
		AndroidOpenGLESBinding.eglContext = AndroidEGL::GetInstance()->GetRenderingContext()->eglContext;
		AndroidOpenGLESBinding.eglDisplay = AndroidEGL::GetInstance()->GetDisplay();
		UE_LOG(GSXR_Unreal_Instance,Log,TEXT("CreateRender  NativeWindow: %p"),AndroidEGL::GetInstance()->GetNativeWindow());
		UE_LOG(GSXR_Unreal_Instance,Log,TEXT("CreateRender  RenderingContext: %p"),AndroidEGL::GetInstance()->GetRenderingContext()->eglContext);
		UE_LOG(GSXR_Unreal_Instance,Log,TEXT("CreateRender  Display: %p"),AndroidEGL::GetInstance()->GetDisplay());
#endif
		GSXR_RenderLooperStartInfo RenderLooperStartInfo;
		RenderLooperStartInfo.next = nullptr;
		RenderLooperStartInfo.graphicsBinding = &AndroidOpenGLESBinding;
#if PLATFORM_ANDROID
		RenderLooperStartInfo.m_androsAppNativeWindow = AndroidEGL::GetInstance()->GetNativeWindow();
#endif
		RenderLooperStartInfo.m_colorSpace = GSXR_ColorSpace_Linear;
		RenderLooperStartInfo.viewType = GSXR_ViewType_Primary_Stereo;
		GSXR_Result Result = GSXR_StartRenderLooper(Gsxr_Renderer,&RenderLooperStartInfo,&Gsxr_RenderLooper);
		if (Result == GSXR_Result_Success)
		{
			GSXR_SetRenderLooperThread(Gsxr_RenderLooper,GRenderThreadId);
			Result = GSXR_GetRenderFunctions(Gsxr_RenderLooper,&RecommendedFunctions,&SupportedFunctions);
			if (Result != GSXR_Result_Success)
			{
				UE_LOG(GSXR_Unreal_Instance, Log, TEXT("GSXR_GetRenderFunctions Falid,ErrorCode:%d"), Result);
			}
			return true;
		}
		UE_LOG(GSXR_Unreal_Instance, Log, TEXT("GSXR_StartRenderLooper Falid,ErrorCode:%d"), Result);
		return false;
	}
	UE_LOG(GSXR_Unreal_Instance,Log,TEXT("GSXR_StartRenderLooper Falid,Gsxr_Renderer is null or Gsxr_RenderLooper is not null !"));
	return false;
}

bool UGSXRInstance::StopRendering_RHIThread()
{
	
	if (Gsxr_RenderLooper != nullptr)
	{
		GSXR_Result Result = GSXR_StopRenderLooper(Gsxr_RenderLooper);
		if (Result == GSXR_Result_Success)
		{
			Gsxr_RenderLooper = nullptr;
			return true;
		}
		UE_LOG(GSXR_Unreal_Instance, Log, TEXT("GSXR_StopRenderLooper Falid,ErrorCode:%d"), Result);
		return false;
	}
	UE_LOG(GSXR_Unreal_Instance,Log,TEXT("GSXR_StopRenderLooper Falid,Gsxr_RenderLooper is null !"));
	return false;
}

bool UGSXRInstance::GetBatteryStatus(GSXR_DeviceType DeviceType,float& Battery, bool& bIsCharging)
{
	if (Gsxr_Runtime)
	{
		GSXR_BatteryStatus BatteryStatus;
		GSXR_Result Result = GSXR_GetBatteryStatus(Gsxr_Runtime,DeviceType,&BatteryStatus);
		if (Result == GSXR_Result_Success)
		{
			Battery = BatteryStatus.percentange;
			bIsCharging =  BatteryStatus.charging != 0;
			return true;
		}
		UE_LOG(GSXR_Unreal_Instance, Log, TEXT("GSXR_GetBatteryStatus Falid,ErrorCode:%d"), Result);
		return false;
	}
	UE_LOG(GSXR_Unreal_Instance,Log,TEXT("GSXR_GetBatteryStatus Falid,Gsxr_Runtime is null !"));
	return false;
}

bool UGSXRInstance::ShutDown()
{
	GSXR_StopRenderLooper(Gsxr_RenderLooper);
	GSXR_DestroyTextureQueue(Gsxr_TextureQueue);
	GSXR_DestroyRenderer(Gsxr_Renderer);
	GSXR_DestroyRuntime(Gsxr_Runtime);
	return true;
}

/*****************ControllerInterface**************************************/
bool UGSXRInstance::SetXrActionManifestPath(FString ManifestPath)
{
	if (Gsxr_Runtime)
	{
		GSXR_Result Result = GSXR_XrActionManifestPath(Gsxr_Runtime,TCHAR_TO_UTF8(*ManifestPath));
		if (Result == GSXR_Result_Success)
		{
			return true;
		}
		UE_LOG(GSXR_Unreal_Instance, Log, TEXT("GSXR_XrActionManifestPath Falid,ErrorCode:%d"), Result);
		return false;
	}
	UE_LOG(GSXR_Unreal_Instance,Log,TEXT("GSXR_XrActionManifestPath Falid,Gsxr_Runtime is null !"));
	return false;
}

bool UGSXRInstance::GetXrActionSetHandle(FString ActionSetName, GSXR_XrActionSetHandle_t& ActionSetHandle)
{
	if (Gsxr_Runtime)
	{
		GSXR_Result Result = GSXR_GetXrActionSetHandle(Gsxr_Runtime,TCHAR_TO_UTF8(*ActionSetName),&ActionSetHandle);
		if (Result == GSXR_Result_Success)
		{
			return true;
		}
		UE_LOG(GSXR_Unreal_Instance, Log, TEXT("GSXR_GetXrActionSetHandle Falid,ErrorCode:%d"), Result);
		return false;
	}
	UE_LOG(GSXR_Unreal_Instance,Log,TEXT("GSXR_GetXrActionSetHandle Falid,Gsxr_Runtime is null !"));
	return false;
}

bool UGSXRInstance::GetXrActionHandle(FString ActionName, GSXR_XrActionHandle_t& ActionHandle)
{
	if (Gsxr_Runtime)
	{
		GSXR_Result Result = GSXR_GetXrActionHandle(Gsxr_Runtime,TCHAR_TO_UTF8(*ActionName),&ActionHandle);
		if (Result == GSXR_Result_Success)
		{
			return true;
		}
		UE_LOG(GSXR_Unreal_Instance, Log, TEXT("GSXR_GetXrActionHandle Falid,ErrorCode:%d"), Result);
		return false;
	}
	UE_LOG(GSXR_Unreal_Instance,Log,TEXT("GSXR_GetXrActionHandle Falid,Gsxr_Runtime is null !"));
	return false;
}

bool UGSXRInstance::GetXrInputSourceHandle(FString InputSourcePath, GSXR_XrInputValueHandle_t& InputValueHandle)
{
	if (Gsxr_Runtime)
	{
		GSXR_Result Result = GSXR_GetXrInputSourceHandle(Gsxr_Runtime,TCHAR_TO_UTF8(*InputSourcePath),&InputValueHandle);
		if (Result == GSXR_Result_Success)
		{
			return true;
		}
		UE_LOG(GSXR_Unreal_Instance, Log, TEXT("GetInputSourceHandle Falid,ErrorCode:%d"), Result);
		return false;
	}
	UE_LOG(GSXR_Unreal_Instance,Log,TEXT("GSXR_GetXrActionHandle Falid,Gsxr_Runtime is null !"));
	return false;
}

bool UGSXRInstance::UpdateXrActionState(GSXR_XrActiveActionSet ActionSetHandle, uint32_t SizeOfVRSelectedActionSet,
	uint32_t ActionSetCount)
{
	if (Gsxr_Runtime)
	{
		GSXR_Result Result = GSXR_UpdateXrActionState(Gsxr_Runtime, &ActionSetHandle,SizeOfVRSelectedActionSet,ActionSetCount);
		if (Result == GSXR_Result_Success)
		{
			return true;
		}
		UE_LOG(GSXR_Unreal_Instance, Log, TEXT("GSXR_UpdateXrActionState Falid,ErrorCode:%d"), Result);
		return false;
	}
	UE_LOG(GSXR_Unreal_Instance,Log,TEXT("GSXR_UpdateXrActionState Falid,Gsxr_Runtime is null !"));
	return false;
}

bool UGSXRInstance::GetXrActionDigitalActionData(GSXR_XrActionHandle_t ActionHandle,GSXR_XrInputDigitalActionData& ActionData, uint32_t ActionDateSize, GSXR_XrInputValueHandle_t InputValueHandle)
{
	if (Gsxr_Runtime)
	{
		GSXR_Result Result = GSXR_GetXrActionDigitalData(Gsxr_Runtime, ActionHandle,&ActionData,ActionDateSize,InputValueHandle);
		if (Result == GSXR_Result_Success)
		{
			return true;
		}
		UE_LOG(GSXR_Unreal_Instance, Log, TEXT("GSXR_GetXrActionDigitalData Falid,ErrorCode:%d"), Result);
		return false;
	}
	UE_LOG(GSXR_Unreal_Instance,Log,TEXT("GSXR_GetXrActionDigitalData Falid,Gsxr_Runtime is null !"));
	return false;
}

bool UGSXRInstance::GetXrAnalogActionData(GSXR_XrActionHandle_t ActionHandle, GSXR_XrInputAnalogActionData& ActionData,
	uint32_t ActionDateSize, GSXR_XrInputValueHandle_t InputValueHandle)
{
	if (Gsxr_Runtime)
	{
		GSXR_Result Result = GSXR_GetXrActionAnalogData(Gsxr_Runtime,ActionHandle,&ActionData,ActionDateSize,InputValueHandle);
		if (Result == GSXR_Result_Success)
		{
			return true;
		}
		UE_LOG(GSXR_Unreal_Instance, Log, TEXT("GetAnalogActionData Falid,ErrorCode:%d"), Result);
		return false;
	}
	UE_LOG(GSXR_Unreal_Instance,Log,TEXT("GSXR_GetXrActionDigitalData Falid,Gsxr_Runtime is null !"));
	return false;	
}

bool UGSXRInstance::TriggerVibration(int32 DeviceHand,bool bUseParameter,uint32 VibrationTime,uint32 VibrationFrequency,float VibrationIntensity)
{
	if (Gsxr_Runtime)
	{
		GSXR_DeviceType DeviceType;
		if (DeviceHand == 0)
		{
			DeviceType = GSXR_DeviceType_Controller_Left;
		}else
		{
			DeviceType = GSXR_DeviceType_Controller_Right;
		}
		GSXR_HapticVibration HapticVibration;
		HapticVibration.duration = VibrationTime;
		HapticVibration.frequency = VibrationFrequency;
		HapticVibration.intensity = VibrationIntensity;
		GSXR_Result Result = GSXR_TriggerVibration(Gsxr_Runtime,DeviceType,static_cast<GSXR_Bool32>(bUseParameter),&HapticVibration);
		if (Result == GSXR_Result_Success)
		{
			return true;
		}
		UE_LOG(GSXR_Unreal_Instance, Log, TEXT("GSXR_TriggerVibration Falid,ErrorCode:%d"), Result);
		return false;
	}
	UE_LOG(GSXR_Unreal_Instance,Log,TEXT("GSXR_TriggerVibration Falid,Gsxr_Runtime is null !"));
	return false;
	
}

bool UGSXRInstance::GetPoseState(GSXR_DeviceType DeviceType, float WorldScale,FVector& Position, FRotator& Rotation)
{
	if (Gsxr_Runtime)
	{
		GSXR_PoseState PoseState;
		GSXR_Result Result = GSXR_GetPoseState(Gsxr_Runtime,DeviceType,0,&PoseState);
		if (Result == GSXR_Result_Success)
		{
			FVector SourcePosition = FVector(PoseState.pose.position.x,PoseState.pose.position.y,PoseState.pose.position.z);
			FQuat SourceRotation = FQuat(PoseState.pose.orientation.x,PoseState.pose.orientation.y,PoseState.pose.orientation.z,PoseState.pose.orientation.w);
			Position = FGSXRUtils::ConvertXRVectorToUnrealVector(SourcePosition,WorldScale);
			Rotation = FGSXRUtils::ConvertXRQuatToUnrealQuat(SourceRotation).Rotator();
			return true;
		}
		UE_LOG(GSXR_Unreal_Instance, Log, TEXT("GSXR_GetPoseState Falid,ErrorCode:%d"), Result);
		return false;
	}
	UE_LOG(GSXR_Unreal_Instance,Log,TEXT("GSXR_GetPoseState Falid,Gsxr_Runtime is null !"));
	return false;
}

bool UGSXRInstance::GetControllerVelocityState(GSXR_DeviceType DeviceType, float WorldScale, FVector& LinearVelocity, FVector& AngularVelocity)
{
	if (Gsxr_Runtime)
	{
		GSXR_PoseState PoseState;
		GSXR_Result Result = GSXR_GetPoseState(Gsxr_Runtime, DeviceType, 0, &PoseState);
		if (Result == GSXR_Result_Success)
		{
			FVector SourceLinearVelocity = FVector(PoseState.velocity.linearVelocity.x, PoseState.velocity.linearVelocity.y, PoseState.velocity.linearVelocity.z);
			FVector SourceAngularVelocity = FVector(PoseState.velocity.angularVelocity.x, PoseState.velocity.angularVelocity.y, PoseState.velocity.angularVelocity.z);
			LinearVelocity = FGSXRUtils::ConvertXRVectorToUnrealVector(SourceLinearVelocity, WorldScale);
			AngularVelocity = FGSXRUtils::ConvertXRVectorToUnrealVector(SourceAngularVelocity, WorldScale);
			return true;
		}
		UE_LOG(GSXR_Unreal_Instance, Log, TEXT("GetControllerVelocityState Falid,ErrorCode:%d"), Result);
		return false;
	}
	UE_LOG(GSXR_Unreal_Instance, Log, TEXT("GetControllerVelocityState Falid,Gsxr_Runtime is null !"));
	return false;
}

bool UGSXRInstance::GetPrimaryDeviceType(GSXR_Flags64& Flags)
{
	if (Gsxr_Runtime)
	{
		GSXR_Result Result = GSXR_GetPrimaryDeviceType(Gsxr_Runtime,&Flags);
		if (Result == GSXR_Result_Success)
		{
			return true;
		}
		UE_LOG(GSXR_Unreal_Instance, Log, TEXT("GSXR_GetPrimaryDeviceType Falid,ErrorCode:%d"), Result);
		return false;
	}
	UE_LOG(GSXR_Unreal_Instance,Log,TEXT("GSXR_GetPrimaryDeviceType Falid,Gsxr_Runtime is null !"));
	return false;
}

bool UGSXRInstance::SetPrimaryDeviceType(GSXR_DeviceType& DeviceType)
{
	if (Gsxr_Runtime)
	{
		GSXR_Result Result = GSXR_SetPrimaryDeviceType(Gsxr_Runtime,DeviceType);
		if (Result == GSXR_Result_Success)
		{
			return true;
		}
		UE_LOG(GSXR_Unreal_Instance, Log, TEXT("GSXR_SetPrimaryDeviceType Falid,ErrorCode:%d"), Result);
		return false;
	}
	UE_LOG(GSXR_Unreal_Instance,Log,TEXT("GSXR_SetPrimaryDeviceType Falid,Gsxr_Runtime is null !"));
	return false;
}

bool UGSXRInstance::GetSupportedXrFeatures(GSXR_Flags64& featureType)
{
	if (Gsxr_Runtime)
	{

		GSXR_Result Result = GSXR_GetSupportedXrFeatures(Gsxr_Runtime, &featureType);
		if (Result == GSXR_Result_Success)
		{

			return true;
		}
		UE_LOG(GSXR_Unreal_Instance, Log, TEXT("GSXR_Result_Success Falid,ErrorCode:%d"), Result);
		return false;
	}
	UE_LOG(GSXR_Unreal_Instance, Log, TEXT("GSXR_Result_Success Falid,Gsxr_Runtime is null !"));
	return false;
}

bool UGSXRInstance::IsXrFeatureAvailable(GSXR_XrFeatureType featureType,bool& bavailable)
{	
	bavailable = false;
	if (Gsxr_Runtime)
	{
		GSXR_Bool32 available=false;
		GSXR_Result Result = GSXR_IsXrFeatureAvailable(Gsxr_Runtime, featureType, &available);
		if (Result == GSXR_Result_Success)
		{			
			bavailable = available;
			return true;				
		}
		UE_LOG(GSXR_Unreal_Instance, Log, TEXT("GSXR_IsXrFeatureAvailable Falid,ErrorCode:%d"), Result);
		return false;
	}
	UE_LOG(GSXR_Unreal_Instance, Log, TEXT("GSXR_IsXrFeatureAvailable Falid,Gsxr_Runtime is null !"));
	return false;	
}

bool UGSXRInstance::GetXrFeatureOptions(GSXR_XrFeatureType featureType, GSXR_Flags64& recommendedOptions, GSXR_Flags64& supportedOptions)
{
	if (Gsxr_Runtime)
	{	
		GSXR_Result Result = GSXR_GetXrFeatureOptions(Gsxr_Runtime, featureType, &recommendedOptions, &supportedOptions);
		if (Result == GSXR_Result_Success)
		{
			
			return true;
		}
		UE_LOG(GSXR_Unreal_Instance, Log, TEXT("GSXR_IsXrFeatureAvailable Falid,ErrorCode:%d"), Result);
		return false;
	}
	UE_LOG(GSXR_Unreal_Instance, Log, TEXT("GSXR_IsXrFeatureAvailable Falid,Gsxr_Runtime is null !"));
	return false;
}

bool UGSXRInstance::StartXrFeature(GSXR_XrFeatureType featureType, GSXR_XrFeature* feature, GSXR_Flags64 recommendedOptions)
{
	if (Gsxr_Runtime)
	{
		GSXR_XrFeatureStartInfo startInfo;
		startInfo.next = nullptr;
		startInfo.featureType = featureType;
		startInfo.options = recommendedOptions;
		GSXR_Result Result = GSXR_StartXrFeature(Gsxr_Runtime, &startInfo, feature);		
		if (Result == GSXR_Result_Success)
		{

			return true;
		}
		UE_LOG(GSXR_Unreal_Instance, Log, TEXT("GSXR_StartXrFeature Falid,ErrorCode:%d"), Result);
		return false;
	}
	UE_LOG(GSXR_Unreal_Instance, Log, TEXT("GSXR_StartXrFeature Falid,Gsxr_Runtime is null !"));
	return false;
}

bool UGSXRInstance::StopXrFeature(GSXR_XrFeature feature)
{
	GSXR_Result Result = GSXR_StopXrFeature(feature);
	if (Result == GSXR_Result_Success)
	{
		return true;
	}
	UE_LOG(GSXR_Unreal_Instance, Log, TEXT("GSXR_StopXrFeature Falid,ErrorCode:%d"), Result);
	return false;

}

bool UGSXRInstance::AcquireControllerModelData(GSXR_XrFeature feature, GSXR_ControllerModelData** controllerModel, uint32* controllerModelSize)
{	
	GSXR_Result Result=GSXR_AcquireControllerModelData(feature, controllerModel, controllerModelSize);
	if (Result == GSXR_Result_Success)
	{		
		return true;
	}
	UE_LOG(GSXR_Unreal_Instance, Log, TEXT("GSXR_AcquireControllerModelData Falid,ErrorCode:%d"), Result);
	return false;
}


void UGSXRInstance::ReleaseControllerModelData(GSXR_XrFeature feature, GSXR_ControllerModelData* controllerModel)
{
	 GSXR_ReleaseControllerModelData(feature, controllerModel);


}

bool UGSXRInstance::GetSupportedDeviceInputType(GSXR_DeviceType deviceType, GSXR_Flags64* deviceInputTypeFlag)
{

	if (Gsxr_Runtime)
	{
		GSXR_Result Result = GSXR_GetSupportedDeviceInputType(Gsxr_Runtime, deviceType, deviceInputTypeFlag);
		if (Result == GSXR_Result_Success)
		{
			return true;
		}
		UE_LOG(GSXR_Unreal_Instance, Log, TEXT("GSXR_GetSupportedDeviceInputType Falid,ErrorCode:%d"), Result);
		return false;
	}
	UE_LOG(GSXR_Unreal_Instance, Log, TEXT("GSXR_GetSupportedDeviceInputType Falid,Gsxr_Runtime is null !"));
	return false;
}

bool UGSXRInstance::GetSupportedDeviceType(GSXR_Flags64* deviceTypeFlags)
{
	if (Gsxr_Runtime)
	{
		GSXR_Result Result = GSXR_GetSupportedDevices(Gsxr_Runtime, deviceTypeFlags);
		if (Result == GSXR_Result_Success)
		{
			return true;
		}
		UE_LOG(GSXR_Unreal_Instance, Log, TEXT("GSXR_GetSupportedDevices Falid,ErrorCode:%d"), Result);
		return false;
	}
	UE_LOG(GSXR_Unreal_Instance, Log, TEXT("GSXR_GetSupportedDevices Falid,Gsxr_Runtime is null !"));
	return false;
}

bool UGSXRInstance::SetSystemSetting(GSXR_XrSystemSettingType settingType)
{	
	if (Gsxr_Runtime)
	{	
		GSXR_Result Result = GSXR_SetSystemSetting(Gsxr_Runtime, settingType);
		if (Result == GSXR_Result_Success)
		{			
			return true;
		}
		UE_LOG(GSXR_Unreal_Instance, Log, TEXT("GSXR_SetSystemSetting Falid,ErrorCode:%d"), Result);
		return false;
	}
	UE_LOG(GSXR_Unreal_Instance, Log, TEXT("GSXR_SetSystemSetting Falid,Gsxr_Runtime is null !"));
	return false;
}

bool UGSXRInstance::ResetOrientationAndPosition(float yaw)
{
	if (Gsxr_Runtime)
	{
		GSXR_Result Result = GSXR_ResetOrientationAndPosition(Gsxr_Runtime, yaw);
		if (Result == GSXR_Result_Success)
		{
			return true;
		}
		UE_LOG(GSXR_Unreal_Instance, Log, TEXT("GSXR_ResetOrientationAndPosition Falid,ErrorCode:%d"), Result);
		return false;
	}
	UE_LOG(GSXR_Unreal_Instance, Log, TEXT("GSXR_ResetOrientationAndPosition Falid,Gsxr_Runtime is null !"));
	return false;
}

bool UGSXRInstance::PollEvent()
{
	GSXR_Event GsxrEvent;
	while(GSXR_Result_Success == GSXR_PollEvent(Gsxr_Runtime,&GsxrEvent))
	{
		ProcessXREvent(GsxrEvent);
	}
	return true;
}

bool UGSXRInstance::ProcessXREvent(GSXR_Event GsxrEvent)
{	
	UE_LOG(GSXR_Unreal_Instance,Log,TEXT("Receive Event,Event type is %d"),GsxrEvent.type);
	switch (GsxrEvent.type)
	{
		case GSXR_EventType_DeviceState_Connected:
		case GSXR_EventType_DeviceState_Disconnected:
		{
			if (GsxrEvent.gsxrEventData.gsxr_devicesatate_eventdata.deviceType == GSXR_DeviceType_Controller_Left ||
				GsxrEvent.gsxrEventData.gsxr_devicesatate_eventdata.deviceType == GSXR_DeviceType_Controller_Right)
			{
				static FGSXRInput* Input = GetGSXRInput(); 
				Input->OnDeviceStateChange();
			}
			DeviceStateChangedDelegate.Broadcast(GsxrEvent.gsxrEventData.gsxr_devicesatate_eventdata.deviceType);
			break;	
		}
		case GSXR_EventType_FeatureState_Available:
		{
			if (GsxrEvent.gsxrEventData.gsxr_xrfeaturestate_eventdata.featureType == GSXR_XrFeatureType_ControllerModel)
			
				ControllerModelTextureLoadComplete.Broadcast();
			break;
		}
		case GSXR_EventType_RendererState_Looper_Unavailable:
			UE_LOG(GSXR_Unreal_Instance,Log,TEXT("Receive Event GSXR_EventType_RendererState_Looper_Unavailable !"));
			break;
		case GSXR_EventType_RendererState_Looper_Unready:
			UE_LOG(GSXR_Unreal_Instance,Log,TEXT("Receive Event GSXR_EventType_RendererState_Looper_Unready !"));
			break;
		default:
			break;
	}
	return true;
}

bool UGSXRInstance::GetTextureQueue(GSXR_TextureQueue& TextureQueue)
{
	if (Gsxr_TextureQueue)
	{
		TextureQueue = Gsxr_TextureQueue;
		return true;
	}
	return false;
}

FGSXRInput* UGSXRInstance::GetGSXRInput()
{
	TArray<IMotionController*> MotionControllers = IModularFeatures::Get().GetModularFeatureImplementations<IMotionController>(IMotionController::GetModularFeatureName());
	for (auto MotionController : MotionControllers)
	{
		if (MotionController != nullptr && MotionController->GetMotionControllerDeviceTypeName() == FName(TEXT("GSXRInput")))
		{
			return  static_cast<FGSXRInput*>(MotionController);
		}
	}
	return nullptr;
}

bool UGSXRInstance::GetInputClickStates(GSXR_DeviceType Device, GSXR_Device_InputType Input, GSXR_Flags64& Click)
{
	if (Gsxr_Runtime)
	{
		GSXR_Result Result = GSXR_GetInputClickStates(Gsxr_Runtime,Device,Input,&Click);
		if (Result == GSXR_Result_Success)
		{
			return true;
		}
		UE_LOG(GSXR_Unreal_Instance, Log, TEXT("GSXR_GetInputClickStates Falid,ErrorCode:%d"), Result);
		return false;
	}
	UE_LOG(GSXR_Unreal_Instance,Log,TEXT("GSXR_GetInputClickStates Falid,Gsxr_Runtime is null !"));
	return false;
}
#endif
