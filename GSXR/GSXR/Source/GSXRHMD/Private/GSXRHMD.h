#pragma once
#include "CoreMinimal.h"
#include "XRRenderBridge.h"
#include "GSXRRenderBridge.h"
#include "GSXRSetting.h"
#include "SceneViewExtension.h"
#include "XRTrackingSystemBase.h"
#include "XRRenderTargetManager.h"
#include "HeadMountedDisplayBase.h"
#include "GSXRInstance.h"
#include "GSXRStereoLayer.h"
#include "IStereoLayers.h"
#define TextureQueueLength 3
#if PLATFORM_ANDROID
#include "Android/AndroidJNI.h"
#include "Android/AndroidApplication.h"
#include "OpenGLDrvPrivate.h"
#include "OpenGLResources.h"

class FXRHMDTextureSet :public FOpenGLTexture2D
{
public:
	static FXRHMDTextureSet* CreateTexture2DSet(
	FOpenGLDynamicRHI* InGLRHI,
	uint32 SizeX,
	uint32 SizeY,
	uint32 InNumSamples,
	uint32 InNumSamplesTileMem,
	uint32 InArraySize,
	EPixelFormat InFormat,
#if ENGINE_MINOR_VERSION > 25	
	ETextureCreateFlags InFlags,
	ETextureCreateFlags InTargetableTextureFlags
#else	
	uint32 InFlags,
	uint32 InTargetableTextureFlags
#endif
	);

	FXRHMDTextureSet(
		class FOpenGLDynamicRHI* InGLRHI,
		GLenum InTarget,
		GLenum InAttachment,
		uint32 InSizeX,
		uint32 InSizeY,
		uint32 InSizeZ,
		uint32 InNumMips,
		uint32 InNumSamples,
		uint32 InNumSamplesTileMem,
		uint32 InArraySize,
		EPixelFormat InFormat,
		bool bInCubemap,
		bool bInAllocatedStorage,
#if ENGINE_MINOR_VERSION > 25	
		ETextureCreateFlags InFlags,
#else	
		uint32 InFlags,
#endif
		uint8* InTextureRange,
#if ENGINE_MINOR_VERSION > 25	
		ETextureCreateFlags InTargetableTextureFlags
#else	
		uint32 InTargetableTextureFlags
#endif
	);

	virtual ~FXRHMDTextureSet() override;

	void SwitchToNextElement();
	void SetIndexAsCurrentRenderTarget();
	GLuint* GetRenderTargetResource();
	FTexture2DRHIRef GetRenderTarget();
private:

	FXRHMDTextureSet(const FXRHMDTextureSet&) = delete;
	FXRHMDTextureSet(FXRHMDTextureSet&&) = delete;
	FXRHMDTextureSet& operator=(const FXRHMDTextureSet&) = delete;

	enum { mkRenderTargetTextureNum = 3 };
	FTexture2DRHIRef    mRenderTargetTexture2DHIRef[mkRenderTargetTextureNum];
	int                 mRenderTargetTexture2DHIRefIndex;
};

#endif
#include "gsxr_controller_model.h"
#include "gsxr_xrfeature.h"
#include "Chaos/Array.h"

#if USE_GSXRAPI
#include "gsxr.h"
#include "gsxr_types.h"
#include "gsxr_device.h"
#include "gsxr_andros.h"
#include "gsxr_renderer.h"

struct FFrameSensorState
{
	GSXR_Time PredictedDisplayTime;
	GSXR_ViewPoseState ViewPoseState;
	FVector Position;
	FQuat Orientation;
	GSXR_TextureElement TextureElement;
};

#endif

struct FGSXRFrustum
{
	float FovLeft;
	float FovRight;
	float FovUp;
	float FovDown;
	float Near;
	float Far;

	FGSXRFrustum()
	{
		FovUp = 0.88119f;
		FovDown = -0.88119f;
		FovLeft = -0.88119f;
		FovRight = 0.88119f;
		Near = 0.0508f;
		Far = 100;
	}
	FString ToString() const
	{
		return TEXT(" FGSXRFrustum Left : ") + FString::SanitizeFloat(FovLeft) +
			TEXT(" FGSXRFrustum Right : ") + FString::SanitizeFloat(FovRight) +
			TEXT(" FGSXRFrustum Up : ") + FString::SanitizeFloat(FovUp) +
			TEXT(" FGSXRFrustum Down : ") + FString::SanitizeFloat(FovDown) +
			TEXT(" FGSXRFrustum H : ") + FString::SanitizeFloat(FovRight - FovLeft) +
			TEXT(" FGSXRFrustum V : ") + FString::SanitizeFloat(FovUp - FovDown )+
			TEXT(" FGSXRFrustum Near : ") + FString::SanitizeFloat(Near) +
			TEXT(" FGSXRFrustum Far : ") + FString::SanitizeFloat(Far) ;
	}
};

class FGSXRHMD : public FHeadMountedDisplayBase,public FXRRenderTargetManager,public FSceneViewExtensionBase, public IStereoLayers
{
public:
	FGSXRHMD(const FAutoRegister&);
	virtual ~FGSXRHMD() override;
	
	virtual FName GetSystemName() const override;
	virtual bool EnumerateTrackedDevices(TArray<int32>& OutDevices, EXRTrackedDeviceType Type) override;
	virtual float GetInterpupillaryDistance() const override;
	virtual void SetInterpupillaryDistance(float NewInterpupillaryDistance) override;
	virtual bool GetRelativeEyePose(int32 DeviceId, EStereoscopicPass Eye, FQuat& OutOrientation, FVector& OutPosition) override;

	virtual void ResetOrientationAndPosition(float Yaw) override;
	virtual void ResetOrientation(float Yaw) override;
	virtual void ResetPosition() override;

	virtual bool GetCurrentPose(int32 DeviceId, FQuat& OutOrientation, FVector& OutPosition) override;
	virtual void SetBaseOrientation(const FQuat& BaseOrient) override;
	virtual FQuat GetBaseOrientation() const override;
	virtual void SetBasePosition(const FVector& BasePosition) override;
	virtual FVector GetBasePosition() const override;
	virtual void SetBaseRotation(const FRotator& BaseRot) override;
	virtual FRotator GetBaseRotation() const override;
#if ENGINE_MINOR_VERSION >25
	virtual int32 GetXRSystemFlags() const override;
#endif
	
	virtual void SetTrackingOrigin(EHMDTrackingOrigin::Type NewOrigin) override;
	virtual EHMDTrackingOrigin::Type GetTrackingOrigin() const override;
	virtual class IHeadMountedDisplay* GetHMDDevice() override;
	virtual class TSharedPtr< class IStereoRendering, ESPMode::ThreadSafe > GetStereoRenderingDevice() override;
	virtual float GetWorldToMetersScale() const override;

/** IHeadMountedDisplay interface */
	virtual bool IsHMDConnected() override;
	virtual bool DoesSupportPositionalTracking() const override;
	virtual bool IsHMDEnabled() const override;
	virtual void EnableHMD(bool allow = true) override;
	virtual bool GetHMDMonitorInfo(MonitorInfo&) override;
	virtual void GetFieldOfView(float& OutHFOVInDegrees, float& OutVFOVInDegrees) const override;
	virtual bool IsChromaAbCorrectionEnabled() const override;
	virtual FIntPoint GetIdealRenderTargetSize() const override;
	virtual bool GetHMDDistortionEnabled(EShadingPath ShadingPath) const override { return false; }
	virtual void OnBeginRendering_RenderThread(FRHICommandListImmediate& RHICmdList, FSceneViewFamily& ViewFamily) override;
	virtual void OnBeginRendering_GameThread() override;
	//virtual void OnLateUpdateApplied_RenderThread(const FTransform& NewRelativeTransform) override;
	virtual bool OnStartGameFrame(FWorldContext& WorldContext) override;
	virtual bool OnEndGameFrame(FWorldContext& WorldContext) override;
	virtual FString GetVersionString() const override;
	
	/** IStereoRendering interface */
	virtual bool IsStereoEnabled() const override;
	virtual bool EnableStereo(bool stereo = true) override;
	virtual void AdjustViewRect(EStereoscopicPass StereoPass, int32& X, int32& Y, uint32& SizeX, uint32& SizeY) const override;
	virtual FMatrix GetStereoProjectionMatrix(const enum EStereoscopicPass StereoPassType) const override;
	virtual void GetEyeRenderParams_RenderThread(const struct FRenderingCompositePassContext& Context, FVector2D& EyeToSrcUVScaleValue, FVector2D& EyeToSrcUVOffsetValue) const override;
	virtual IStereoRenderTargetManager* GetRenderTargetManager() override;
	virtual void RenderTexture_RenderThread(class FRHICommandListImmediate& RHICmdList, class FRHITexture2D* BackBuffer, class FRHITexture2D* SrcTexture, FVector2D WindowSize) const override;

	/** ISceneViewExtension interface */
	virtual void SetupViewFamily(FSceneViewFamily& InViewFamily) override;
	virtual void SetupView(FSceneViewFamily& InViewFamily, FSceneView& InView) override;
	virtual void BeginRenderViewFamily(FSceneViewFamily& InViewFamily) override;
	virtual void PreRenderViewFamily_RenderThread(FRHICommandListImmediate& RHICmdList, FSceneViewFamily& InViewFamily) override;	
	virtual void PreRenderView_RenderThread(FRHICommandListImmediate& RHICmdList, FSceneView& InView) override;
	virtual void PostRenderViewFamily_RenderThread(FRHICommandListImmediate& RHICmdList, FSceneViewFamily& InViewFamily) override;
	virtual void PostRenderView_RenderThread(FRHICommandListImmediate& RHICmdList, FSceneView& InView) override;
	virtual bool IsActiveThisFrame_Internal(const FSceneViewExtensionContext& Context) const override;

	/** IStereoRenderTargetManager */
	virtual bool ShouldUseSeparateRenderTarget() const override { return IsStereoEnabled(); }
#if ENGINE_MINOR_VERSION >25
	virtual bool AllocateRenderTargetTexture(uint32 Index, uint32 SizeX, uint32 SizeY, uint8 Format, uint32 NumMips, ETextureCreateFlags InTexFlags, ETextureCreateFlags InTargetableTextureFlags, FTexture2DRHIRef& OutTargetableTexture, FTexture2DRHIRef& OutShaderResourceTexture, uint32 NumSamples = 1) override;
#else
	virtual bool AllocateRenderTargetTexture(uint32 Index, uint32 SizeX, uint32 SizeY, uint8 Format, uint32 NumMips, uint32 Flags, uint32 TargetableTextureFlags, FTexture2DRHIRef& OutTargetableTexture, FTexture2DRHIRef& OutShaderResourceTexture, uint32 NumSamples = 1) override;
#endif
	virtual bool NeedReAllocateViewportRenderTarget(const FViewport& Viewport) override;
	virtual FXRRenderBridge* GetActiveRenderBridge_GameThread(bool bUseSeparateRenderTarget) override;
	virtual void CalculateRenderTargetSize(const FViewport& Viewport, uint32& InOutSizeX, uint32& InOutSizeY) override;
	/** IStereoRenderTargetManager */
	virtual void OnBeginPlay(FWorldContext& InWorldContext) override;
	virtual void OnEndPlay(FWorldContext& InWorldContext) override;

	virtual IStereoLayers* GetStereoLayers() override { return this; }
	virtual uint32 CreateLayer(const IStereoLayers::FLayerDesc& InLayerDesc) override;
	virtual void DestroyLayer(uint32 LayerId) override;
	virtual void SetLayerDesc(uint32 LayerId, const IStereoLayers::FLayerDesc& InLayerDesc) override;
	virtual bool GetLayerDesc(uint32 LayerId, IStereoLayers::FLayerDesc& OutLayerDesc) override;
	virtual void MarkTextureForUpdate(uint32 LayerId) override;
	virtual void UpdateSplashScreen() override;
	virtual FLayerDesc GetDebugCanvasLayerDesc(FTextureRHIRef Texture) override;
	virtual void GetAllocatedTexture(uint32 LayerId, FTextureRHIRef &Texture, FTextureRHIRef &LeftTexture) override;
	virtual bool ShouldCopyDebugLayersToSpectatorScreen() const override { return true; }
private:	
	void InitGSXRHMD();
	void ShutDown();
	void InitXRControllerMesh();
	
	
	EHMDTrackingOrigin::Type TrackingOrigin;
	FString RHIString;
	TRefCountPtr<FGSXRRenderBridge> RenderBridge;
	bool bIsMobileMultiViewEnabled;
	int32 MobileMSAAValue;
	UGSXRSettings* Settings;
	FIntPoint RenderTargetSize;
	TMap<uint32,FGSXRLayerPtr> LayerMap;
	uint32 CurrentLayerId;
	bool bIsStandaloneStereoOnlyDevice;
	bool bStartRendering;
public:
	
	void ApplicationPauseDelegate();
	void ApplicationResumeDelegate();
	void StartRendering_RHIThread();
	void BeginFrame_RHIThread();
	void PreRenderFrame_RHIThread();
	void PostRenderFrame_RHIThread();
	void EndFrame_RHIThread();
	void StopRendering_RHIThread();
	void UpdateSensorData();
	GSXRHMD_API	bool IsStandaloneStereoOnlyDevice() const { return bIsStandaloneStereoOnlyDevice; }
	bool bEnabledFoveatedRender;
	UGSXRInstance* Gsxr_Instance;
	GSXR_XrFeature feature;
	bool ControllerXrFeatureavlible ;
	GSXR_ControllerModelData* ControllerModelData;
	uint32 controllerModelSize;
#if USE_GSXRAPI
	
	GSXR_Flags64 recommendedOptions;
	GSXR_Flags64 supportedOptions;
	FFrameSensorState CurrentFrame_RenderThread;
	FFrameSensorState CurrentFrame_GameThread;
	FFrameSensorState LastFrame_RenderThread;
	FFrameSensorState LastFrame_GameThread;
	
	bool bIsEndGameFrame;
	GSXR_ViewSetConfiguration Gsxr_ViewSetConfiguration;
#endif
};
