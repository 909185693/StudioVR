#include "GSXRHMD.h"
#include "Engine.h"
#include "XRThreadUtils.h"
#include "GSXRRenderBridge.h"
#include "GSXRUtils.h"
#include "HardwareInfo.h"
#include "IHeadMountedDisplayVulkanExtensions.h"
#include "PostProcess/RenderingCompositionGraph.h"
#include "gsxr_xrfeature.h"
#if PLATFORM_ANDROID
#include "HardwareInfo.h"
#include "OpenGLDrvPrivate.h"
#include "OpenGLResources.h"
#include "Android/AndroidJNI.h"
#include "Android/AndroidApplication.h"
#include "Android/AndroidEGL.h"
#endif


DEFINE_LOG_CATEGORY_STATIC(GSXR_Unreal_HMD, Log, All);

FGSXRHMD::FGSXRHMD(const FAutoRegister&AutoRegister)
	:FHeadMountedDisplayBase(nullptr)
	,FSceneViewExtensionBase(AutoRegister)
	, feature(nullptr)
	,TrackingOrigin(EHMDTrackingOrigin::Eye)
	,RHIString(FString())	
	,RenderBridge(nullptr)
	,bIsMobileMultiViewEnabled(false)
	,MobileMSAAValue(1)
	,RenderTargetSize(FIntPoint(1600,1600))
	,CurrentLayerId(0)
	, bIsStandaloneStereoOnlyDevice(false)
	,bStartRendering(false)
	,Gsxr_Instance(nullptr)
	,ControllerXrFeatureavlible(false)
{
	Settings = GetMutableDefault<UGSXRSettings>();
	Gsxr_Instance = UGSXRInstance::GetInstance();
	InitGSXRHMD();	
	InitXRControllerMesh();
	FGSXRHMD::SetTrackingOrigin(EHMDTrackingOrigin::Floor);	
	bIsStandaloneStereoOnlyDevice = true;
	
}

FGSXRHMD::~FGSXRHMD()
{
	ShutDown();
}

FName FGSXRHMD::GetSystemName() const
{
	static FName SystemName(TEXT("GSXRHMD"));
	return SystemName;
}

bool FGSXRHMD::EnumerateTrackedDevices(TArray<int32>& OutDevices, EXRTrackedDeviceType Type)
{
	if (Type == EXRTrackedDeviceType::Any || Type == EXRTrackedDeviceType::HeadMountedDisplay)
	{
		OutDevices.Add(HMDDeviceId);
		return true;
	}
	return false;
}

float FGSXRHMD::GetInterpupillaryDistance() const
{
	float Ipd = 0.063f;
#if USE_GSXRAPI
	Ipd = static_cast<float>(Gsxr_ViewSetConfiguration.ipd) / 1000.f;
#endif
	return Ipd;
}

void FGSXRHMD::SetInterpupillaryDistance(float NewInterpupillaryDistance)
{
	
}

bool FGSXRHMD::GetRelativeEyePose(int32 DeviceId, EStereoscopicPass Eye, FQuat& OutOrientation, FVector& OutPosition)
{
	OutOrientation = FQuat::Identity;
	OutPosition = FVector::ZeroVector;
	if (DeviceId == HMDDeviceId && (Eye == eSSP_LEFT_EYE || Eye == eSSP_RIGHT_EYE))
	{
		OutPosition = FVector(0, (Eye == eSSP_LEFT_EYE ? -.5 : .5) * GetInterpupillaryDistance() * GetWorldToMetersScale(), 0);
		return true;
	}
	return false;
}

void FGSXRHMD::ResetOrientationAndPosition(float Yaw)
{
	//ResetOrientation(Yaw);
	//ResetPosition();
	
#if USE_GSXRAPI
	Gsxr_Instance->ResetOrientationAndPosition(0);
#endif
}

void FGSXRHMD::ResetOrientation(float Yaw)
{
}

void FGSXRHMD::ResetPosition()
{
}

bool FGSXRHMD::GetCurrentPose(int32 DeviceId, FQuat& OutOrientation, FVector& OutPosition)
{
	OutOrientation = FQuat::Identity;
	OutPosition = FVector::ZeroVector;
#if USE_GSXRAPI
	if (IsInRenderingThread())
	{
		if (bIsEndGameFrame)
		{
			bIsEndGameFrame = false;
			UpdateSensorData();	
		}
		OutOrientation = CurrentFrame_RenderThread.Orientation;
		OutPosition = CurrentFrame_RenderThread.Position;
	}else if (IsInGameThread())
	{
		OutOrientation = CurrentFrame_GameThread.Orientation;
		OutPosition = CurrentFrame_GameThread.Position;
	}
#endif
	return true;
}

void FGSXRHMD::SetBaseOrientation(const FQuat& BaseOrient)
{
}

FQuat FGSXRHMD::GetBaseOrientation() const
{
	return FQuat::Identity;
}

void FGSXRHMD::SetBasePosition(const FVector& BasePosition)
{
}

FVector FGSXRHMD::GetBasePosition() const
{
	return FVector::ZeroVector;
}

void FGSXRHMD::SetBaseRotation(const FRotator& BaseRot)
{
}

FRotator FGSXRHMD::GetBaseRotation() const
{
	return FRotator::ZeroRotator;
}
#if ENGINE_MINOR_VERSION >25
int32 FGSXRHMD::GetXRSystemFlags() const
{
	return EXRSystemFlags::IsHeadMounted;
}
#endif

void FGSXRHMD::SetTrackingOrigin(EHMDTrackingOrigin::Type NewOrigin)
{
#if USE_GSXRAPI
	if (Gsxr_Instance->SetTrackingOrigin(NewOrigin))
	{
		TrackingOrigin = NewOrigin;
		OnTrackingOriginChanged();
	}
#endif
}

EHMDTrackingOrigin::Type FGSXRHMD::GetTrackingOrigin() const
{
	return TrackingOrigin;
}

IHeadMountedDisplay* FGSXRHMD::GetHMDDevice()
{
	return this;
}

TSharedPtr<IStereoRendering, ESPMode::ThreadSafe> FGSXRHMD::GetStereoRenderingDevice()
{
	return  SharedThis(this);
}

float FGSXRHMD::GetWorldToMetersScale() const
{
	if (IsInGameThread() && GWorld != nullptr)
	{
		return GWorld->GetWorldSettings()->WorldToMeters;
	}
	// Default value, assume Unreal units are in centimeters
	return 100.0f;
}

bool FGSXRHMD::IsHMDConnected()
{
	return true;
}

bool FGSXRHMD::DoesSupportPositionalTracking() const
{
#if USE_GSXRAPI
	GSXR_DegreeOfFreedom HMDDegreeOfFreedom;
	Gsxr_Instance->GetDegreeOfFreedom(GSXR_DeviceType_HMD_VR,HMDDegreeOfFreedom);
	return HMDDegreeOfFreedom == GSXR_DegreeOfFreedom_6DOF;
#endif
	return false;
}

bool FGSXRHMD::IsHMDEnabled() const
{
	return true;
}

void FGSXRHMD::EnableHMD(bool allow)
{
}

bool FGSXRHMD::GetHMDMonitorInfo(MonitorInfo&)
{
	return true;
}

void FGSXRHMD::GetFieldOfView(float& OutHFOVInDegrees, float& OutVFOVInDegrees) const
{
#if USE_GSXRAPI
	OutHFOVInDegrees = Gsxr_ViewSetConfiguration.fovHRad;
	OutVFOVInDegrees = Gsxr_ViewSetConfiguration.fovVRad;
#endif
}

bool FGSXRHMD::IsChromaAbCorrectionEnabled() const
{
	return false;
}

FIntPoint FGSXRHMD::GetIdealRenderTargetSize() const
{
	return RenderTargetSize;
}

void FGSXRHMD::OnBeginRendering_RenderThread(FRHICommandListImmediate& RHICmdList, FSceneViewFamily& ViewFamily)
{
}

void FGSXRHMD::OnBeginRendering_GameThread()
{
}

bool FGSXRHMD::OnStartGameFrame(FWorldContext& WorldContext)
{
#if USE_GSXRAPI
	LastFrame_GameThread = CurrentFrame_GameThread;
	CurrentFrame_GameThread = LastFrame_RenderThread;
	Gsxr_Instance->PollEvent();
#endif
	return true;
}

bool FGSXRHMD::OnEndGameFrame(FWorldContext& WorldContext)
{
#if USE_GSXRAPI
	ExecuteOnRenderThread_DoNotWait([this]()
	{
		LastFrame_RenderThread = CurrentFrame_RenderThread;
		//CurrentFrame_RenderThread = CurrentFrame_GameThread;
		this->bIsEndGameFrame = true;
	});
#endif
	return true;
}

FString FGSXRHMD::GetVersionString() const
{
#if USE_GSXRAPI
	GSXR_RuntimeProperties RuntimeProperties;
	Gsxr_Instance->GetRuntimeProperties(RuntimeProperties);
	FString xrVersion = FString::Printf(TEXT("%s.%s.%s"),
		*FString::FromInt(GSXR_VERSION_MAJOR(RuntimeProperties.gsxrVersion)),
		*FString::FromInt(GSXR_VERSION_MINOR(RuntimeProperties.gsxrVersion)),
		*FString::FromInt(GSXR_VERSION_PATCH(RuntimeProperties.gsxrVersion)));
	FString RuntimeName = ANSI_TO_TCHAR(RuntimeProperties.runtimeName);
	FString RuntimeVersion = FString::Printf(TEXT("%s.%s.%s"),
		*FString::FromInt(GSXR_VERSION_MAJOR(RuntimeProperties.runtimeVersion)),
		*FString::FromInt(GSXR_VERSION_MINOR(RuntimeProperties.runtimeVersion)),
		*FString::FromInt(GSXR_VERSION_PATCH(RuntimeProperties.runtimeVersion)));
	GSXR_DeviceProperties DeviceProperties;
	Gsxr_Instance->GetDeviceProperties(GSXR_DeviceType_HMD_VR,DeviceProperties);
	FString DeviceInfo = FString::Printf(TEXT("DeviceSN:%s,FirmwareVersion:%s,VendorName:%s,AlgorithmVersion:%s,AKgorithmName:%s"),
		ANSI_TO_TCHAR(DeviceProperties.serialNumber),
		ANSI_TO_TCHAR(DeviceProperties.firmwareVersion),
		ANSI_TO_TCHAR(DeviceProperties.vendorName),
		ANSI_TO_TCHAR(DeviceProperties.algorithmVersion),
		ANSI_TO_TCHAR(DeviceProperties.algorithmName));
	return FString::Printf(TEXT("EngineVersion:%s, Engine Branch:%s,gsxrVersion: %s,runtimeVersion:%s,DeviceInfo:%s"), *FEngineVersion::Current().ToString(),*FEngineVersion::Current().GetBranch(),*xrVersion,*RuntimeVersion,*DeviceInfo);
	
#endif
	return FString::Printf(TEXT("EngineVersion:%s, Engine Branch:%s,GSXRSDKVersion: %s"), *FEngineVersion::Current().ToString(),*FEngineVersion::Current().GetBranch(),TEXT("1.0.0"));
}

bool FGSXRHMD::IsStereoEnabled() const
{
	return true;
}

bool FGSXRHMD::EnableStereo(bool stereo)
{
	return true;
}

void FGSXRHMD::AdjustViewRect(EStereoscopicPass StereoPass, int32& X, int32& Y, uint32& SizeX, uint32& SizeY) const
{
	if(!bIsMobileMultiViewEnabled)
	{
		SizeX = GetIdealRenderTargetSize().X / 2;
	}else
	{
		SizeX = GetIdealRenderTargetSize().X;
	}

	SizeY = GetIdealRenderTargetSize().Y;
	if (StereoPass == eSSP_RIGHT_EYE)
	{
		X += SizeX;
	}
	if (bIsMobileMultiViewEnabled) {
		X = 0;
	}

	//UE_LOG(GSXR_Unreal_HMD, Log, TEXT("AdjustViewRectsiez(%d,%d)(%d,%d)"), X,Y, SizeX, SizeY);
}

FMatrix FGSXRHMD::GetStereoProjectionMatrix(const EStereoscopicPass StereoPassType) const
{
	float HFov,VFov;
	float ZNear = GNearClippingPlane;
	GetFieldOfView(HFov,VFov);
	float Left = -FPlatformMath::Tan(HFov/2);
	float Right = FPlatformMath::Tan(HFov/2);
	float Top = FPlatformMath::Tan(VFov/2);
	float Bottom = -FPlatformMath::Tan(VFov/2);
	float SumRL = (Right + Left);
	float SumTB = (Top + Bottom);
	float InvRL = (1.0f / (Right - Left));
	float InvTB = (1.0f / (Top - Bottom));
	FMatrix ProjectionMatrix = FMatrix(
			FPlane((2.0f * InvRL), 0.0f, 0.0f, 0.0f),
			FPlane(0.0f, (2.0f * InvTB), 0.0f, 0.0f),
			FPlane((SumRL * -InvRL), (SumTB * -InvTB), 0.0f, 1.0f),
			FPlane(0.0f, 0.0f, ZNear, 0.0f)
		);
	return ProjectionMatrix;
}

void FGSXRHMD::GetEyeRenderParams_RenderThread(const FRenderingCompositePassContext& Context,FVector2D& EyeToSrcUVScaleValue, FVector2D& EyeToSrcUVOffsetValue) const
{
	if (Context.View.StereoPass == eSSP_LEFT_EYE)
	{
		EyeToSrcUVOffsetValue.X = 0.0f;
		EyeToSrcUVOffsetValue.Y = 0.0f;

		EyeToSrcUVScaleValue.X = 0.5f;
		EyeToSrcUVScaleValue.Y = 1.0f;
	}
	else
	{
		EyeToSrcUVOffsetValue.X = 0.5f;
		EyeToSrcUVOffsetValue.Y = 0.0f;

		EyeToSrcUVScaleValue.X = 0.5f;
		EyeToSrcUVScaleValue.Y = 1.0f;
	}

	//EyeToSrcUVOffsetValue = FVector2D::ZeroVector;
	//EyeToSrcUVScaleValue = FVector2D(1.0f, 1.0f);
	UE_LOG(GSXR_Unreal_HMD, Log, TEXT("GetEyeRenderParams_RenderThread(EyeToSrcUVOffsetValue.X:%f,EyeToSrcUVOffsetValue.Y:%f)(EyeToSrcUVScaleValue.X:%f,EyeToSrcUVScaleValue.Y:%f)"), EyeToSrcUVOffsetValue.X, EyeToSrcUVOffsetValue.Y, EyeToSrcUVScaleValue.X, EyeToSrcUVScaleValue.Y);
}

IStereoRenderTargetManager* FGSXRHMD::GetRenderTargetManager()
{
	return this;
}

void FGSXRHMD::RenderTexture_RenderThread(FRHICommandListImmediate& RHICmdList, FRHITexture2D* BackBuffer,FRHITexture2D* SrcTexture, FVector2D WindowSize) const
{
#if PLATFORM_ANDROID
	if (SpectatorScreenController)
	{
		SpectatorScreenController->RenderSpectatorScreen_RenderThread(RHICmdList, BackBuffer, SrcTexture, WindowSize);
	}
	//UE_LOG(GSXR_Unreal_HMD,Log,TEXT("RenderTexture_RenderThread Textureid:%d "),(*((uint32_t *)SrcTexture->GetNativeResource())));
#endif
}

void FGSXRHMD::SetupViewFamily(FSceneViewFamily& InViewFamily)
{
#if WITH_EDITOR
	InViewFamily.EngineShowFlags.SetScreenPercentage(false);
#endif
	/**
	* Called on game thread when creating the view family.
	*/

}

void FGSXRHMD::SetupView(FSceneViewFamily& InViewFamily, FSceneView& InView)
{
	/**
	* Called on game thread when creating the view.
	*/
}

void FGSXRHMD::BeginRenderViewFamily(FSceneViewFamily& InViewFamily)
{
	/**
	* Called on game thread when view family is about to be rendered.
	*/
}

void FGSXRHMD::PreRenderView_RenderThread(FRHICommandListImmediate& RHICmdList, FSceneView& InView)
{
	/**
	* Called on render thread at the start of rendering, for each view, after PreRenderViewFamily_RenderThread call.
	*/
}

void FGSXRHMD::PreRenderViewFamily_RenderThread(FRHICommandListImmediate& RHICmdList, FSceneViewFamily& InViewFamily)
{

	ExecuteOnRHIThread([this]()
	{
		BeginFrame_RHIThread();
		PreRenderFrame_RHIThread();
	});
}

void FGSXRHMD::PostRenderView_RenderThread(FRHICommandListImmediate& RHICmdList, FSceneView& InView)
{
	/**
	* Allows to render content after the 3D content scene, useful for debugging
	*/
}

bool FGSXRHMD::IsActiveThisFrame_Internal(const FSceneViewExtensionContext& Context) const
{
	// We need to use GEngine->IsStereoscopic3D in case the current viewport disallows running in stereo.
	return GEngine && GEngine->IsStereoscopic3D(Context.Viewport);
}

void FGSXRHMD::PostRenderViewFamily_RenderThread(FRHICommandListImmediate& RHICmdList, FSceneViewFamily& InViewFamily)
{
	/**
	* Allows to render content after the 3D content scene, useful for debugging
	*/

	ExecuteOnRHIThread([this]()
	{
		PostRenderFrame_RHIThread();
	});
	
}

#if ENGINE_MINOR_VERSION > 25
bool FGSXRHMD::AllocateRenderTargetTexture(uint32 Index, uint32 SizeX, uint32 SizeY, uint8 Format, uint32 NumMips, ETextureCreateFlags Flags, ETextureCreateFlags TargetableTextureFlags, FTexture2DRHIRef& OutTargetableTexture, FTexture2DRHIRef& OutShaderResourceTexture, uint32 NumSamples)
#else
bool FGSXRHMD::AllocateRenderTargetTexture(uint32 Index, uint32 SizeX, uint32 SizeY, uint8 Format, uint32 NumMips, uint32 Flags, uint32 TargetableTextureFlags, FTexture2DRHIRef& OutTargetableTexture, FTexture2DRHIRef& OutShaderResourceTexture, uint32 NumSamples /*= 1*/)
#endif
{
#if USE_GSXRAPI
	Gsxr_Instance->CreateRender();
	GSXR_Flags64 RecommendedOptions;
	GSXR_Flags64 SupportedOptions;
	Gsxr_Instance->GetTextureQueueOptions(RecommendedOptions,SupportedOptions);	
	if(static_cast<GSXR_TextureQueueOptions>(RecommendedOptions) == GSXR_TextureQueueOptions_Color_Attachment || static_cast<GSXR_TextureQueueOptions>(RecommendedOptions) == GSXR_TextureQueueOptions_Protected_Content)
	{
		TArray<uint32_t>TextureResources;
		UE_LOG(GSXR_Unreal_HMD,Log,TEXT("Use TextureQueue "));
		uint8 TargetFormat;
		Gsxr_Instance->CreateTextureQueue(RecommendedOptions, Format,SizeX,SizeY,NumSamples,1,NumMips,bIsMobileMultiViewEnabled?2:1,TextureResources,TargetFormat);
		RenderBridge->CreateSwapChain(EPixelFormat(TargetFormat),SizeX,SizeY,bIsMobileMultiViewEnabled?2:1,NumMips,NumSamples,Flags,TargetableTextureFlags,MobileMSAAValue,TextureResources);
		OutTargetableTexture = OutShaderResourceTexture = RenderBridge->SwapChain->GetTexture2DArray() ? RenderBridge->SwapChain->GetTexture2DArray() : RenderBridge->SwapChain->GetTexture2D();
		return RenderBridge != nullptr;
	}
	else
	{	
		UE_LOG(GSXR_Unreal_HMD,Log,TEXT("Use SwapChain"));

		Gsxr_Instance->CreateTextureQueue(RecommendedOptions, Format, SizeX, SizeY, NumSamples, 1, NumMips, bIsMobileMultiViewEnabled ? 2 : 1);

		TArray<uint32_t>TextureResources;
		TextureResources.SetNum(TextureQueueLength);		
		for (int i = 0; i < TextureQueueLength; ++i)
		{
			GLuint TextureID = 0;
			glGenTextures(1, &TextureID);
			GLenum textureType = bIsMobileMultiViewEnabled ? GL_TEXTURE_2D_ARRAY : GL_TEXTURE_2D;			
			glBindTexture(textureType, TextureID);			
			if (bIsMobileMultiViewEnabled) {
				glTexStorage3D(textureType, 1, GL_RGBA8, SizeX, SizeY, 2);
			}
			else {
				glTexStorage2D(textureType, 1, GL_RGBA8, SizeX, SizeY);
			}
			glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glBindTexture(textureType, 0);
			TextureResources[i] = TextureID;
			UE_LOG(GSXR_Unreal_HMD,Log,TEXT("TextureResources[%d]:%d,siez(%d,%d)"),i,TextureResources[i], SizeX, SizeY);
		}		
		RenderBridge->CreateSwapChain(EPixelFormat(PF_R8G8B8A8),SizeX,SizeY,bIsMobileMultiViewEnabled?2:1,NumMips,NumSamples,Flags,TargetableTextureFlags,MobileMSAAValue,TextureResources);
		OutTargetableTexture = OutShaderResourceTexture =RenderBridge->SwapChain->GetTexture2DArray() ? RenderBridge->SwapChain->GetTexture2DArray() : RenderBridge->SwapChain->GetTexture2D();
		return true;
	}
#endif
	return false;
}

bool FGSXRHMD::NeedReAllocateViewportRenderTarget(const FViewport& Viewport)
{
	check(IsInGameThread());

	const uint32 InSizeX = Viewport.GetSizeXY().X;
	const uint32 InSizeY = Viewport.GetSizeXY().Y;

	FIntPoint mRTSize;
	mRTSize.X = Viewport.GetRenderTargetTexture()->GetSizeX();
	mRTSize.Y = Viewport.GetRenderTargetTexture()->GetSizeY();

	uint32 NewSizeX = InSizeX, NewSizeY = InSizeY;
	CalculateRenderTargetSize(Viewport, NewSizeX, NewSizeY);
	if (NewSizeX != mRTSize.X || NewSizeY != mRTSize.Y)
	{
		UE_LOG(GSXR_Unreal_HMD,Log,TEXT("NeedReAllocateViewportRenderTarget "));
		return true;
	}
	return false;
}

FXRRenderBridge* FGSXRHMD::GetActiveRenderBridge_GameThread(bool bUseSeparateRenderTarget)
{
	return RenderBridge;
}

void FGSXRHMD::CalculateRenderTargetSize(const FViewport& Viewport, uint32& InOutSizeX, uint32& InOutSizeY)
{
	InOutSizeX = GetIdealRenderTargetSize().X;
	InOutSizeY = GetIdealRenderTargetSize().Y;
}

void FGSXRHMD::OnBeginPlay(FWorldContext& InWorldContext)
{
	UE_LOG(GSXR_Unreal_HMD,Log,TEXT("OnBeginPlay"));
#if USE_GSXRAPI
	GEngine->SetMaxFPS(Gsxr_ViewSetConfiguration.fps);
#endif	
}

void FGSXRHMD::OnEndPlay(FWorldContext& InWorldContext)
{
}

uint32 FGSXRHMD::CreateLayer(const IStereoLayers::FLayerDesc& InLayerDesc)
{
	check(IsInGameThread());
	const uint32 LayerId = CurrentLayerId++;
	UE_LOG(LogTemp,Log,TEXT(" CreateLayer %d"),LayerId);
	LayerMap.Add(LayerId, MakeShareable(new FGSXRStereoLayer(LayerId, InLayerDesc)));
	return LayerId;
}

void FGSXRHMD::DestroyLayer(uint32 LayerId)
{
	check(IsInGameThread());
	UE_LOG(LogTemp,Log,TEXT(" DestroyLayer %d"),LayerId);
	LayerMap.Remove(LayerId);
}

void FGSXRHMD::SetLayerDesc(uint32 LayerId, const IStereoLayers::FLayerDesc& InLayerDesc)
{
	check(IsInGameThread());
	FGSXRLayerPtr* LayerFound = LayerMap.Find(LayerId);

	if (LayerFound)
	{
		LayerFound->Get()->SetLayerDesc(InLayerDesc);
	}
}

bool FGSXRHMD::GetLayerDesc(uint32 LayerId, IStereoLayers::FLayerDesc& OutLayerDesc)
{
	check(IsInGameThread());
	FGSXRLayerPtr* LayerFound = LayerMap.Find(LayerId);
	if (LayerFound)
	{
		OutLayerDesc = (*LayerFound)->GetLayerDesc();
		return true;
	}
	return false;
}

void FGSXRHMD::MarkTextureForUpdate(uint32 LayerId)
{
	check(IsInGameThread());
}

void FGSXRHMD::UpdateSplashScreen()
{
}

IStereoLayers::FLayerDesc FGSXRHMD::GetDebugCanvasLayerDesc(FTextureRHIRef Texture)
{
	IStereoLayers::FLayerDesc StereoLayerDesc;
	StereoLayerDesc.Priority = INT_MAX;
	StereoLayerDesc.Transform = FTransform(FVector(180.f, 0, 0));
	StereoLayerDesc.Transform.SetScale3D(FVector(1,1,-1));
	StereoLayerDesc.QuadSize = FVector2D(180.f, 180.f);
	StereoLayerDesc.LayerSize = Texture->GetTexture2D()->GetSizeXY();
	StereoLayerDesc.PositionType = IStereoLayers::ELayerType::FaceLocked;
#if ENGINE_MINOR_VERSION >24
	StereoLayerDesc.SetShape<FQuadLayer>();
#else
	StereoLayerDesc.ShapeType = IStereoLayers::ELayerShape::QuadLayer;
#endif
	StereoLayerDesc.Texture = Texture;
	StereoLayerDesc.Flags = IStereoLayers::ELayerFlags::LAYER_FLAG_TEX_CONTINUOUS_UPDATE;
	StereoLayerDesc.Flags |= IStereoLayers::ELayerFlags::LAYER_FLAG_QUAD_PRESERVE_TEX_RATIO;
	return StereoLayerDesc;
}

void FGSXRHMD::GetAllocatedTexture(uint32 LayerId, FTextureRHIRef& Texture, FTextureRHIRef& LeftTexture)
{
	Texture = LeftTexture = nullptr;
}

void FGSXRHMD::InitGSXRHMD()
{
	const FString HardwareDetails = FHardwareInfo::GetHardwareDetailsString();
	const FString RHILookup = NAME_RHI.ToString() + TEXT("=");
	if (!FParse::Value(*HardwareDetails, *RHILookup, RHIString))
	{
		UE_LOG(GSXR_Unreal_HMD,Log,TEXT("Get RHI String failed!"));
		check(0);
	}
 	
	if (RHIString == TEXT("OpenGL"))
	{
		RenderBridge = CreateRenderBridge_OpenGL(this);
	}
	else if (RHIString == TEXT("Vulkan"))
	{
		RenderBridge = CreateRenderBridge_Vulkan(this);
		//RenderBridge->GetVulkanGraphics();
	}
	else
	{
		UE_LOG(GSXR_Unreal_HMD,Log,TEXT("Do not support this type!"));
	}
	UE_LOG(GSXR_Unreal_HMD,Log,TEXT("RHIString: %s"),*RHIString);
#if USE_GSXRAPI
	 if (!Gsxr_Instance->CreateRuntime())
	{
		UE_LOG(GSXR_Unreal_HMD,Log,TEXT("CreateRuntime Faild !"));
	}

	Gsxr_Instance->GetViewSetConfiguration(Gsxr_ViewSetConfiguration);

#endif
	FCoreDelegates::ApplicationWillEnterBackgroundDelegate.AddRaw(this, &FGSXRHMD::ApplicationPauseDelegate);
	FCoreDelegates::ApplicationHasEnteredForegroundDelegate.AddRaw(this, &FGSXRHMD::ApplicationResumeDelegate);
	
	//Config MobileMultiView
#if ENGINE_MINOR_VERSION < 25
	const auto CVarMobileMultiView = IConsoleManager::Get().FindTConsoleVariableDataInt(TEXT("vr.MobileMultiView"));
	const auto CVarMobileMultiViewDirect = IConsoleManager::Get().FindTConsoleVariableDataInt(TEXT("vr.MobileMultiView.Direct"));
	bIsMobileMultiViewEnabled = (CVarMobileMultiView && CVarMobileMultiView->GetValueOnAnyThread() != 0);
	if (bIsMobileMultiViewEnabled == true) {
		GSupportsMobileMultiView = true;
	}
	bIsMobileMultiViewEnabled = GSupportsMobileMultiView && bIsMobileMultiViewEnabled && (CVarMobileMultiViewDirect && CVarMobileMultiViewDirect->GetValueOnAnyThread() != 0);
#else
	const auto CVarMobileMultiView = IConsoleManager::Get().FindTConsoleVariableDataInt(TEXT("vr.MobileMultiView"));
	static const auto CVarMobileHDR = IConsoleManager::Get().FindTConsoleVariableDataInt(TEXT("r.MobileHDR"));
	const bool bMobileHDR = CVarMobileHDR && CVarMobileHDR->GetValueOnAnyThread() == 1;
	bIsMobileMultiViewEnabled =!bMobileHDR && (CVarMobileMultiView && CVarMobileMultiView->GetValueOnAnyThread() != 0);
#endif
	UE_LOG(GSXR_Unreal_HMD,Log,TEXT("bIsMobileMultiViewEnabled = %d"),bIsMobileMultiViewEnabled);

#if USE_GSXRAPI
	if (!bIsMobileMultiViewEnabled) {
		RenderTargetSize.X = Gsxr_ViewSetConfiguration.targetWidth * 2;
		RenderTargetSize.Y = Gsxr_ViewSetConfiguration.targetHeight;
	}
	else
	{
		RenderTargetSize.X = Gsxr_ViewSetConfiguration.targetWidth;
		RenderTargetSize.Y = Gsxr_ViewSetConfiguration.targetHeight;
	}
#endif
	MobileMSAAValue = IConsoleManager::Get().FindTConsoleVariableDataInt(TEXT("r.MobileMSAA"))->GetValueOnAnyThread();
    UE_LOG(GSXR_Unreal_HMD,Log,TEXT("MSAA = %d"),MobileMSAAValue);
}

void FGSXRHMD::ShutDown()
{
#if USE_GSXRAPI
	Gsxr_Instance->ReleaseControllerModelData(feature, ControllerModelData);
	Gsxr_Instance->StopXrFeature(feature);
	ExecuteOnRenderThread([this]()
	{
		ExecuteOnRHIThread([this]()
		{
			this->Gsxr_Instance->ShutDown();
		});
	});
#endif
}

void FGSXRHMD::InitXRControllerMesh()
{
#if USE_GSXRAPI		
	GSXR_Flags64 XrFeatureType;
	Gsxr_Instance->GetSupportedXrFeatures(XrFeatureType);
	if ((GSXR_XrFeatureType::GSXR_XrFeatureType_ControllerModel&XrFeatureType) == GSXR_XrFeatureType::GSXR_XrFeatureType_ControllerModel)
	{
		UE_LOG(GSXR_Unreal_HMD, Log, TEXT("GSXR_XrFeatureType_ControllerModel is Support "));
		Gsxr_Instance->GetXrFeatureOptions(GSXR_XrFeatureType::GSXR_XrFeatureType_ControllerModel, recommendedOptions, supportedOptions);
		Gsxr_Instance->StartXrFeature(GSXR_XrFeatureType::GSXR_XrFeatureType_ControllerModel, &feature, recommendedOptions);
		/*	Gsxr_Instance->IsXrFeatureAvailable(GSXR_XrFeatureType::GSXR_XrFeatureType_ControllerModel, ControllerXrFeatureavlible);
			if (ControllerXrFeatureavlible)
			{
				UE_LOG(GSXR_Unreal_HMD, Log, TEXT("ControllerXrFeatureavlible is true "));
				Gsxr_Instance->AcquireControllerModelData(feature, &ControllerModelData, &controllerModelSize);
			}*/
	}
	else
	{
		UE_LOG(GSXR_Unreal_HMD, Log, TEXT("GSXR_XrFeatureType_ControllerModel not Support "));
	}

#endif
}

void FGSXRHMD::BeginFrame_RHIThread()
{
	if (!bStartRendering)
	{
		return;
	}
	RenderBridge->SwapChain->IncrementSwapChainIndex_RHIThread();
}

void FGSXRHMD::EndFrame_RHIThread()
{
#if USE_GSXRAPI
	GSXR_TextureQueue TextureQueue = nullptr;
	Gsxr_Instance->GetTextureQueue(TextureQueue);
	GSXR_FrameSubmitInfo FrameSubmitInfo;
	TArray<GSXR_CompositionLayerView> LayerViews;

	if (bIsMobileMultiViewEnabled) {
		GSXR_TexSubImage leftTexSubImage;
		leftTexSubImage.next = nullptr;
		leftTexSubImage.textureQueue = TextureQueue;
		leftTexSubImage.height = GetIdealRenderTargetSize().Y;
		leftTexSubImage.width = GetIdealRenderTargetSize().X;	
		const FRHITexture2D* const SwapChainTexture = RenderBridge->SwapChain->GetTexture2DArray() ? RenderBridge->SwapChain->GetTexture2DArray() : RenderBridge->SwapChain->GetTexture2D();		
		leftTexSubImage.textureElement.texture = SwapChainTexture->GetNativeResource();
		leftTexSubImage.xOffset = 0;
		leftTexSubImage.yOffset = 0;
		leftTexSubImage.arrayLayerIndex = 0;//RenderBridge->SwapChain->GetSwapChainIndex_RHIThread();
		GSXR_TexSubImage rightTexSubImage;
		rightTexSubImage.next = nullptr;
		rightTexSubImage.textureQueue = TextureQueue;
		rightTexSubImage.height = GetIdealRenderTargetSize().Y;
		rightTexSubImage.width = GetIdealRenderTargetSize().X;
		rightTexSubImage.textureElement.texture = SwapChainTexture->GetNativeResource();
		rightTexSubImage.xOffset = 0;
		rightTexSubImage.yOffset = 0;
		rightTexSubImage.arrayLayerIndex = 1;//RenderBridge->SwapChain->GetSwapChainIndex_RHIThread();

		GSXR_CompositionLayerView LeftLayerView;
		LeftLayerView.texSubImage = leftTexSubImage;
		LeftLayerView.viewPoseState = CurrentFrame_RenderThread.ViewPoseState;
		LeftLayerView.eyeMask = GSXR_Eye_Left;
		LayerViews.Add(LeftLayerView);

		GSXR_CompositionLayerView RightLayerView;
		RightLayerView.texSubImage = rightTexSubImage;
		RightLayerView.viewPoseState = CurrentFrame_RenderThread.ViewPoseState;
		RightLayerView.eyeMask = GSXR_Eye_Right;
		LayerViews.Add(RightLayerView);
	}
	else {
		GSXR_TexSubImage TexSubImage;
		TexSubImage.next = nullptr;
		TexSubImage.textureQueue = TextureQueue;
		TexSubImage.height = GetIdealRenderTargetSize().Y;
		TexSubImage.width = GetIdealRenderTargetSize().X;
		TexSubImage.textureElement.texture = RenderBridge->SwapChain->GetTexture2D()->GetNativeResource();
		TexSubImage.xOffset = 0;
		TexSubImage.yOffset = 0;
		TexSubImage.arrayLayerIndex = 0;//RenderBridge->SwapChain->GetSwapChainIndex_RHIThread();
		GSXR_CompositionLayerView LayerView;
		LayerView.texSubImage = TexSubImage;
		LayerView.viewPoseState = CurrentFrame_RenderThread.ViewPoseState;
		LayerView.eyeMask = GSXR_Eye_Both;
		LayerViews.Add(LayerView);
	}

	/*for (int32 LayerIndex = 0;LayerIndex<LayerMap.Num();LayerIndex++)
	{
		GSXR_CompositionLayerView CompositionLayerView;
		GSXR_TexSubImage CompositionTexSubImage;
		CompositionTexSubImage.next = nullptr;
		CompositionTexSubImage.textureQueue = TextureQueue;
		CompositionTexSubImage.height = LayerMap[LayerIndex]->GetLayerDesc().LayerSize.Y;
		CompositionTexSubImage.width = LayerMap[LayerIndex]->GetLayerDesc().LayerSize.X;
		CompositionTexSubImage.textureElement.texture = LayerMap[LayerIndex]->GetLayerDesc().Texture->GetNativeResource();
		CompositionTexSubImage.xOffset = 0;
		CompositionTexSubImage.yOffset = 0;
		CompositionTexSubImage.arrayLayerIndex = -1;
		
		CompositionLayerView.texSubImage = CompositionTexSubImage;
		GSXR_ViewPoseState CompositionLayerPoseState;
		FVector CompositionLayerPosition = LayerMap[LayerIndex]->GetLayerDesc().Transform.GetLocation();
		FQuat CompositionLayerRotation = LayerMap[LayerIndex]->GetLayerDesc().Transform.GetRotation();
		CompositionLayerPosition = FGSXRUtils::ConvertUnrealVectorToXRVector(CompositionLayerPosition,GetWorldToMetersScale());
		CompositionLayerRotation = FGSXRUtils::ConvertUnrealQuatToXRQuat(CompositionLayerRotation);
		CompositionLayerPoseState.pose.orientation.x = CompositionLayerRotation.X;
		CompositionLayerPoseState.pose.orientation.y = CompositionLayerRotation.Y;
		CompositionLayerPoseState.pose.orientation.z = CompositionLayerRotation.Z;
		CompositionLayerPoseState.pose.orientation.w = CompositionLayerRotation.W;
		CompositionLayerPoseState.pose.position.x = CompositionLayerPosition.X;
		CompositionLayerPoseState.pose.position.y = CompositionLayerPosition.Y;
		CompositionLayerPoseState.pose.position.z = CompositionLayerPosition.Z;
		CompositionLayerView.viewPoseState = CompositionLayerPoseState;
		CompositionLayerView.eyeMask = GSXR_Eye_Both;
		LayerViews.Add(CompositionLayerView);
	}*/
	
	GSXR_CompositionLayerProjection CompositionLayerInfo;
	CompositionLayerInfo.next = nullptr;
	CompositionLayerInfo.viewCount = LayerViews.Num();
	CompositionLayerInfo.layerOptions = GSXR_CompositionLayerOptions_Distortion_Correction;
	CompositionLayerInfo.layerType = GSXR_CompositionLayerType_Projection;
	CompositionLayerInfo.views = LayerViews.GetData();
	
	const GSXR_CompositionLayerHeader * LayerHeader= reinterpret_cast<GSXR_CompositionLayerHeader*>(&CompositionLayerInfo);
	FrameSubmitInfo.next = nullptr;
	FrameSubmitInfo.layers = &LayerHeader;
	FrameSubmitInfo.displayTime = CurrentFrame_RenderThread.PredictedDisplayTime;
	FrameSubmitInfo.layerCount = 1;
	Gsxr_Instance->SubmitFrame(FrameSubmitInfo);
	//UE_LOG(GSXR_Unreal_HMD,Log,TEXT("GSXR_SubmitFrame Textureid:%d "),(*((uint32_t *)RenderBridge->SwapChain->GetTexture2D()->GetNativeResource())));
	//Gsxr_Instance->ReleaseTexture(CurrentFrame_RenderThread.TextureElement);
	RenderBridge->SwapChain->ReleaseCurrentImage_RHIThread();
	
#endif
}

void FGSXRHMD::StartRendering_RHIThread()
{
#if USE_GSXRAPI 
	UGSXRInstance::GetInstance()->StartRendering_RHIThread();
	bStartRendering = true;
#endif
}

void FGSXRHMD::StopRendering_RHIThread()
{
#if USE_GSXRAPI
	bStartRendering = false;
	this->Gsxr_Instance->StopRendering_RHIThread();
#endif
}

void FGSXRHMD::PreRenderFrame_RHIThread()
{
	if (!bStartRendering)
	{
		return;
	}
#if USE_GSXRAPI
	if (Settings->bEnableFoveated)
	{
		GSXR_TextureQueue TextureQueue = nullptr;
		Gsxr_Instance->GetTextureQueue(TextureQueue);
		if (!(Gsxr_Instance->GetSupportedFunctions() & GSXR_RenderFunctionType_Foveated))
		{
			UE_LOG(GSXR_Unreal_HMD,Log,TEXT("The Runtime is not support GSXR_RenderFunctionType_Foveated"));
			Settings->bEnableFoveated = false;
			return;
		}
		if (!bEnabledFoveatedRender)
		{
			bEnabledFoveatedRender = true;
		}
		GSXR_RenderFunctionFoveated RenderFunctionFoveated;
		TArray<GSXR_FoveatedView> FoveatedViews;
		GSXR_FoveatedView FoveatedView;
		GSXR_FoveatedParameters FoveatedParameters;
		FoveatedParameters.next = nullptr;
		FoveatedParameters.focal.x = 0;
		FoveatedParameters.focal.y = 0;
		FoveatedParameters.foveaFov.angleUp = Gsxr_ViewSetConfiguration.fovVRad*Settings->Fov_Percentage_Up * 0.5;
		FoveatedParameters.foveaFov.angleDown = Gsxr_ViewSetConfiguration.fovVRad*Settings->Fov_Percentage_Down * 0.5;
		FoveatedParameters.foveaFov.angleLeft = Gsxr_ViewSetConfiguration.fovHRad*Settings->Fov_Percentage_Left * 0.5;
		FoveatedParameters.foveaFov.angleRight = Gsxr_ViewSetConfiguration.fovHRad*Settings->Fov_Percentage_Right * 0.5;
		FoveatedParameters.peripheralQuality = static_cast<GSXR_PeripheralQuality>(static_cast<int32>(Settings->FoveationLevel));
		FoveatedView.next = nullptr;
		FoveatedView.foveatedParams = &FoveatedParameters;
		FoveatedView.texSubImage.height = GetIdealRenderTargetSize().Y;
		FoveatedView.texSubImage.width = GetIdealRenderTargetSize().X;
		FoveatedView.texSubImage.next = nullptr;
		FoveatedView.texSubImage.textureQueue = TextureQueue;
		FoveatedView.texSubImage.xOffset = 0;
		FoveatedView.texSubImage.yOffset = 0;
		FoveatedView.texSubImage.arrayLayerIndex = -1;
		const FRHITexture2D* const SwapChainTexture = RenderBridge->SwapChain->GetTexture2DArray() ? RenderBridge->SwapChain->GetTexture2DArray() : RenderBridge->SwapChain->GetTexture2D();
		FoveatedView.texSubImage.textureElement.texture = SwapChainTexture->GetNativeResource();
		FoveatedViews.Add(FoveatedView);
		RenderFunctionFoveated.enable = true;
		RenderFunctionFoveated.next = nullptr;
		RenderFunctionFoveated.foveatedViews = FoveatedViews.GetData();
		RenderFunctionFoveated.functionType = GSXR_RenderFunctionType_Foveated;
		RenderFunctionFoveated.viewCount = 1;
		TArray<GSXR_RenderFunctionHeader*> PreRenderFunctionHeaders;
		GSXR_RenderFunctionHeader* FoveatedFunction = reinterpret_cast<GSXR_RenderFunctionHeader*>(&RenderFunctionFoveated);
		PreRenderFunctionHeaders.Add(FoveatedFunction);
		GSXR_FrameRenderInfo FramePreRenderInfo;
		FramePreRenderInfo.next = nullptr;
		FramePreRenderInfo.functionCount = 1;
		FramePreRenderInfo.functions = PreRenderFunctionHeaders.GetData();
		Gsxr_Instance->PreRenderFrame(&FramePreRenderInfo);
	}
#endif
}

void FGSXRHMD::PostRenderFrame_RHIThread()
{
#if USE_GSXRAPI
	if (bEnabledFoveatedRender)
	{
		if (Settings->bEnableFoveated)
		{
			GSXR_TextureQueue TextureQueue = nullptr;
			Gsxr_Instance->GetTextureQueue(TextureQueue);
			GSXR_RenderFunctionFoveated RenderFunctionFoveated;
			TArray<GSXR_FoveatedView> FoveatedViews;
			GSXR_FoveatedView FoveatedView;
			GSXR_FoveatedParameters FoveatedParameters;
			FoveatedParameters.next = nullptr;
			FoveatedParameters.focal.x = 0;
			FoveatedParameters.focal.y = 0;
			FoveatedParameters.foveaFov.angleUp = Gsxr_ViewSetConfiguration.fovVRad*Settings->Fov_Percentage_Up * 0.5;
			FoveatedParameters.foveaFov.angleDown = Gsxr_ViewSetConfiguration.fovVRad*Settings->Fov_Percentage_Down * 0.5;
			FoveatedParameters.foveaFov.angleLeft = Gsxr_ViewSetConfiguration.fovHRad*Settings->Fov_Percentage_Left * 0.5;
			FoveatedParameters.foveaFov.angleRight = Gsxr_ViewSetConfiguration.fovHRad*Settings->Fov_Percentage_Right * 0.5;
			FoveatedParameters.peripheralQuality = static_cast<GSXR_PeripheralQuality>(static_cast<int32>(Settings->FoveationLevel));
			FoveatedView.next = nullptr;
			FoveatedView.foveatedParams = &FoveatedParameters;
			FoveatedView.texSubImage.height = GetIdealRenderTargetSize().Y;
			FoveatedView.texSubImage.width = GetIdealRenderTargetSize().X;
			FoveatedView.texSubImage.next = nullptr;
			FoveatedView.texSubImage.textureQueue = TextureQueue;
			FoveatedView.texSubImage.xOffset = 0;
			FoveatedView.texSubImage.yOffset = 0;
			FoveatedView.texSubImage.arrayLayerIndex = -1;
			const FRHITexture2D* const SwapChainTexture = RenderBridge->SwapChain->GetTexture2DArray() ? RenderBridge->SwapChain->GetTexture2DArray() : RenderBridge->SwapChain->GetTexture2D();
			FoveatedView.texSubImage.textureElement.texture = SwapChainTexture->GetNativeResource();
			FoveatedViews.Add(FoveatedView);
			RenderFunctionFoveated.enable = true;
			RenderFunctionFoveated.next = nullptr;
			RenderFunctionFoveated.foveatedViews = FoveatedViews.GetData();
			RenderFunctionFoveated.functionType = GSXR_RenderFunctionType_Foveated;
			RenderFunctionFoveated.viewCount = 1;
			TArray<GSXR_RenderFunctionHeader*> PreRenderFunctionHeaders;
			GSXR_RenderFunctionHeader* FoveatedFunction = reinterpret_cast<GSXR_RenderFunctionHeader*>(&RenderFunctionFoveated);
			PreRenderFunctionHeaders.Add(FoveatedFunction);
			GSXR_FrameRenderInfo FramePreRenderInfo;
			FramePreRenderInfo.next = nullptr;
			FramePreRenderInfo.functionCount = 1;
			FramePreRenderInfo.functions = PreRenderFunctionHeaders.GetData();
			Gsxr_Instance->PostRenderFrame(&FramePreRenderInfo);		
		}else
		{
			GSXR_FrameRenderInfo PostFrameRenderInfo;
			TArray<GSXR_RenderFunctionHeader*> RenderFunctionHeaders;
			GSXR_RenderFunctionHeader RenderFunctionHeader;
			RenderFunctionHeader.enable = false;
			RenderFunctionHeader.next = nullptr;
			RenderFunctionHeader.functionType = GSXR_RenderFunctionType_Foveated;
			RenderFunctionHeaders.Add(&RenderFunctionHeader);
			PostFrameRenderInfo.functions = RenderFunctionHeaders.GetData();
			PostFrameRenderInfo.next = nullptr;
			PostFrameRenderInfo.functionCount = 1;
			Gsxr_Instance->PostRenderFrame(&PostFrameRenderInfo);
			bEnabledFoveatedRender = false;
		}	
	}
#endif
}

void FGSXRHMD::ApplicationPauseDelegate()
{
	ExecuteOnRenderThread([this]()
	{
		ExecuteOnRHIThread([this]()
		{
			StopRendering_RHIThread();
		});
	});
}

void FGSXRHMD::ApplicationResumeDelegate()
{
	ExecuteOnRenderThread([this]()
	{
		ExecuteOnRHIThread([this]()
		{
			StartRendering_RHIThread();
		});
	});
	FlushRenderingCommands();

}



void FGSXRHMD::UpdateSensorData()
{
#if USE_GSXRAPI
	Gsxr_Instance->WaitFrame(CurrentFrame_RenderThread.PredictedDisplayTime);	
	Gsxr_Instance->GetViewPoseState(CurrentFrame_RenderThread.PredictedDisplayTime,CurrentFrame_RenderThread.ViewPoseState);
	FVector XRPosition = FVector(CurrentFrame_RenderThread.ViewPoseState.pose.position.x,CurrentFrame_RenderThread.ViewPoseState.pose.position.y,CurrentFrame_RenderThread.ViewPoseState.pose.position.z);
	FQuat XROrientation = FQuat(CurrentFrame_RenderThread.ViewPoseState.pose.orientation.x,CurrentFrame_RenderThread.ViewPoseState.pose.orientation.y,CurrentFrame_RenderThread.ViewPoseState.pose.orientation.z,CurrentFrame_RenderThread.ViewPoseState.pose.orientation.w);
	float Scale = GetWorldToMetersScale();
	CurrentFrame_RenderThread.Position = FGSXRUtils::ConvertXRVectorToUnrealVector(XRPosition,Scale);
	CurrentFrame_RenderThread.Orientation = FGSXRUtils::ConvertXRQuatToUnrealQuat(XROrientation);
#endif
}





