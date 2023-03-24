// Fill out your copyright notice in the Description page of Project Settings.
#include "HuaweiVRCustomPresent.h"
#include "HuaweiVRSDK.h"

#include "HuaweiVRSDKAPI.h"
#include "HuaweiVRSDKJNI.h"

#if !WITH_EDITOR
#include "OpenGLDrvPrivate.h"
#include "OpenGLResources.h"
#endif

#if PLATFORM_ANDROID
extern void AndroidThunkCpp_KeepScreenOn(bool Enable);
#endif

FHWRenderTargets::FHWRenderTargets() {
    //UE_LOG(LogHuaweiVRSDK, Log, TEXT("new FHWRenderTargets"));
    LOGI("new FHWRenderTargets");
}

FHWRenderTargets::~FHWRenderTargets() {
    //UE_LOG(LogHuaweiVRSDK, Log, TEXT("delete FHWRenderTargets"));
    LOGI("delete FHWRenderTargets");
}

void FHWRenderTargets::InitRenderTarget(uint32 InSizeX, uint32 InSizeY) {
    CurrentIndex = 0;

    FRHIResourceCreateInfo CreateInfo;
    FTexture2DRHIRef BufferedSRVRHI;

    IConsoleVariable* MobileMSAACVar = IConsoleManager::Get().FindConsoleVariable(TEXT("r.MobileMSAA"));
    int msaa = MobileMSAACVar->GetInt();

    RHICreateTargetableShaderResource2D(InSizeX, InSizeY, PF_B8G8R8A8, 1, TexCreate_None, TexCreate_RenderTargetable, false, CreateInfo, TextureRef, BufferedSRVRHI, msaa);

    //UE_LOG(LogHuaweiVRSDK, Log, TEXT(" RHICreateTargetableShaderResource2D InSizeX=%d, InSizeY=%d"), InSizeX, InSizeY);
    LOGI(" RHICreateTargetableShaderResource2D InSizeX=%u, InSizeY=%u", InSizeX, InSizeY);

    for (int i = 0; i < HW_RENDERTARGET_COUNT; i++) {
        SHWRenderTarget& tg = Target[i];
        tg.SensorState.Reset();
        for (int j = 0; j < HW_EYE_COUNT; j++) {
            //UE_LOG(LogHuaweiVRSDK, Log, TEXT(" RHICreateTexture2D InSizeX=%d, InSizeY=%d"), InSizeX / 2, InSizeY);
            LOGI(" RHICreateTexture2D InSizeX=%u, InSizeY=%u", InSizeX / 2, InSizeY);
            RHICreateTargetableShaderResource2D(InSizeX / 2, InSizeY, PF_B8G8R8A8, 1, TexCreate_None, TexCreate_RenderTargetable, false, CreateInfo, tg.EyeTexture[j], BufferedSRVRHI, msaa);
        }
    }
}

void FHWRenderTargets::SwitchToNextElement() {
    CurrentIndex = (CurrentIndex + 1) % HW_RENDERTARGET_COUNT;
}

SHWRenderTarget& FHWRenderTargets::GetCurrentTarget() {
    return Target[CurrentIndex];
}

SHWRenderTarget& FHWRenderTargets::GetLastTarget() {
    int LastIndex = (CurrentIndex + HW_RENDERTARGET_COUNT - 1) % HW_RENDERTARGET_COUNT;
    return Target[LastIndex];
}

//extern void AndroidThunkCpp_SetDesiredViewSize(int32 Width, int32 Height);
/****************************************************************/
FHuaweiVRCustomPresent::FHuaweiVRCustomPresent() :
	m_RenderTargets(nullptr),
    mOnApplicationStart(false),
	mIsThreadOwnershipAcquired(false),
    mMainThreadRunning(true),
    mVolumeBarDisplay(nullptr) {
    Init();
}

FHuaweiVRCustomPresent::~FHuaweiVRCustomPresent() {
    Shutdown();
    if (m_RenderTargets) {
        delete m_RenderTargets;
        m_RenderTargets = nullptr;
    }
}

void FHuaweiVRCustomPresent::Init() {
#if !WITH_EDITOR
    auto GLRHI = static_cast<FOpenGLDynamicRHI*>(GDynamicRHI);
    GLRHI->SetCustomPresent(this);
#if PLATFORM_ANDROID
    HVR_SetInitVariables(FJavaWrapper::GameActivityThis);
    HVR_SendMessage(UNREAL_PLUGIN_FLAG, "true");
#endif
#endif
}

void FHuaweiVRCustomPresent::Shutdown() {
#if !WITH_EDITOR
    auto GLRHI = static_cast<FOpenGLDynamicRHI*>(GDynamicRHI);
    GLRHI->SetCustomPresent(nullptr);
#if PLATFORM_ANDROID
    HVR_ShutdownRenderThread();
#endif
#endif
}

// Called when viewport is resized.
void FHuaweiVRCustomPresent::OnBackBufferResize() {
    //UE_LOG(LogHuaweiVRSDK, Log, TEXT("OnBackBufferResize"));
    LOGI("OnBackBufferResize");
}

void FHuaweiVRCustomPresent::MainThreadRunningNotify(bool running) {
    LOGI("FHuaweiVRCustomPresent::MainThreadRunningNotify %d", running);
    mMainThreadRunning = running;
}

// @param InOutSyncInterval - in out param, indicates if vsync is on (>0) or off (==0).
// @return	true if normal Present should be performed; false otherwise. If it returns
// true, then InOutSyncInterval could be modified to switch between VSync/NoVSync for the normal Present.

static bool delayResume = false;

bool FHuaweiVRCustomPresent::Present(int32& InOutSyncInterval) {
    check(IsInRenderingThread());

    if (mIsThreadOwnershipAcquired) {
        //GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Green, DebugVersion, true, FVector2D(5, 5));
#if PLATFORM_ANDROID
		if (delayResume) {
			LOGI("sksk delayResume");
			HVR_Resume();
			delayResume = false;
		}
#endif
        FinishRendering();
    }

    //Since the timing of the system call OnReleaseThreadOwnership is too late, 
    //it will cause the Vsync initialization and destruction timing to be disorderly, 
    //so it is modified to destroy the rendering thread when the Activity pauses, 
    //and try to ensure that the Vsync timing is correct.
    if (!mMainThreadRunning) {
        LOGI("FHuaweiVRCustomPresent::Present OnReleaseThreadOwnership");
        OnReleaseThreadOwnership();
    }

    // Note: true causes normal swapbuffers(), false prevents normal swapbuffers()
    return false;
}

// Called when rendering thread is acquired
void FHuaweiVRCustomPresent::OnAcquireThreadOwnership() {
#if PLATFORM_ANDROID
    AndroidThunkCpp_KeepScreenOn(true);
#endif
    //LOGI("FHuaweiVRCustomPresent::OnAcquireThreadOwnership");
    check(IsInRenderingThread());

    if (!mOnApplicationStart) {
#if PLATFORM_ANDROID
        if (HVR_SensorGetContainer() < 0) {
            FinishActivity();
        }
        LOGI("FHuaweiVRCustomPresent::OnAcquireThreadOwnership HVR_InitRenderThread %d", gettid());
        HVR_InitRenderThread();
#endif
        mOnApplicationStart = true;
    } else {
#if PLATFORM_ANDROID
        LOGI("FHuaweiVRCustomPresent::OnAcquireThreadOwnership HVR_Resume %d", gettid());
		delayResume = true;
        //HVR_Resume();
#endif
    }
#if PLATFORM_ANDROID
    if (!mVolumeBarDisplay) {
        mVolumeBarDisplay = new HuaweiVRSDKVolume();
    }
#endif

    mIsThreadOwnershipAcquired = true;
}

// Called when rendering thread is released
void FHuaweiVRCustomPresent::OnReleaseThreadOwnership() {
    check(IsInRenderingThread());

    if (mIsThreadOwnershipAcquired) {
#if PLATFORM_ANDROID
        LOGI("FHuaweiVRCustomPresent::OnReleaseThreadOwnership HVR_Pause %d", gettid());
        HVR_Pause();
#endif
    }

#if PLATFORM_ANDROID
    if (mVolumeBarDisplay) {
        delete mVolumeBarDisplay;
        mVolumeBarDisplay = nullptr;
    }
#endif

    mIsThreadOwnershipAcquired = false;

	delayResume = false;

#if PLATFORM_ANDROID
    AndroidThunkCpp_KeepScreenOn(false);
#endif
}

bool FHuaweiVRCustomPresent::AllocateRenderTargetTexture(uint32 SizeX, uint32 SizeY, uint8 Format, uint32 NumMips, uint32 Flags, uint32 TargetableTextureFlags, FTexture2DRHIRef& OutTargetableTexture, FTexture2DRHIRef& OutShaderResourceTexture, uint32 NumSamples) {
    check(SizeX != 0 && SizeY != 0);

    Flags |= TargetableTextureFlags;

    LOGI("Allocated a new swap texture set (size %u x %u)", SizeX, SizeY);

    if (m_RenderTargets == nullptr) {
        m_RenderTargets = new FHWRenderTargets();
    }
    m_RenderTargets->InitRenderTarget(SizeX, SizeY);

    OutTargetableTexture = m_RenderTargets->TextureRef;
    OutShaderResourceTexture = m_RenderTargets->TextureRef;

    if (!OutTargetableTexture.IsValid()) {
        UE_LOG(LogHuaweiVRSDK, Error, TEXT("OutTargetableTexture is not valid "));
    }

    if (!OutShaderResourceTexture.IsValid()) {
        UE_LOG(LogHuaweiVRSDK, Error, TEXT("OutShaderResourceTexture is not valid "));
    }

#if PLATFORM_ANDROID
    HVR_SetEyeParms(SizeX / 2, SizeY);
#endif

    return true;
}

void FHuaweiVRCustomPresent::BeginRendering(FRHICommandListImmediate& RHICmdList, FSceneViewFamily& InViewFamily) {
}

void FHuaweiVRCustomPresent::FinishRendering() {
    if (m_RenderTargets == NULL)
        return;

    SHWRenderTarget& RenderTarget = m_RenderTargets->GetCurrentTarget();

#if PLATFORM_ANDROID
    uint glTexID = *(uint*)RenderTarget.EyeTexture[0]->GetNativeResource();

    //UE_LOG(LogHuaweiVRSDK, Log, TEXT("TimeWarp FinishRendering glTexID=%u"), glTexID);

    HVR_CameraEndFrame(0, (int)glTexID, 0);

    glTexID = *(uint*)RenderTarget.EyeTexture[1]->GetNativeResource();
    HVR_CameraEndFrame(1, (int)glTexID, 0);

    //HVR_Flush();

    HVR_TimeWarpEvent(RenderTarget.SensorState.twIdx);

    if (mVolumeBarDisplay && mMainThreadRunning) {
        mVolumeBarDisplay->SubmitVolumeTextures();
    }
#endif
    m_RenderTargets->SwitchToNextElement();

    ConditionalUpdateCache();
}

void FHuaweiVRCustomPresent::ConditionalUpdateCache() {
    SHWRenderTarget& RenderTarget = m_RenderTargets->GetCurrentTarget();

    SensorState& st = RenderTarget.SensorState;

#if PLATFORM_ANDROID
    HVR_GetSensorState(false, st.w, st.x, st.y, st.z, st.fov, st.twIdx);

	float ow = 0.0f, ox = 0.0f, oy = 0.0f, oz = 0.0f;
	HVR_GetCameraPositionOrientation(st.px, st.py, st.pz, ox, oy, oz, ow);

	FHuaweiVRSDK* huaweiVRSDK = FHuaweiVRSDK::GetHuaweiVRSDK();

	st.px *= huaweiVRSDK->GetWorldToMetersScale();
	st.py *= huaweiVRSDK->GetWorldToMetersScale();
	st.pz *= huaweiVRSDK->GetWorldToMetersScale();

#endif
}
/*
void FHuaweiVRCustomPresent::UpdateViewport(const class FViewport& Viewport, class FRHIViewport* InViewportRHI) {
    check(IsInGameThread());
    check(ViewportRHI);

    if (m_RenderTargets) {
        ViewportRHI->SetCustomPresent(this);
    }
}*/

bool FHuaweiVRCustomPresent::NeedsNativePresent() {
    UE_LOG(LogHuaweiVRSDK, Log, TEXT("NeedsNativePresent"));
    return true;
}