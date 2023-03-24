// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RendererPrivate.h"
#include "HuaweiVRSDKVolume.h"
#include "XRRenderBridge.h"
#include "HAL/ThreadSafeBool.h"

#define HW_RENDERTARGET_COUNT 6
#define HW_EYE_COUNT 2

struct SensorState {
    int twIdx;
    float w, x, y, z, fov;
	float px, py, pz;
    SensorState() {
        Reset();
    }
    void Reset() {
        w = 1.0f;
        x = y = z = 0.0f;
		px = py = pz = 0.0f;
        fov = 95.0f;
        twIdx = 0;
    }
};


struct SHWRenderTarget {
    FTexture2DRHIRef	EyeTexture[HW_EYE_COUNT];
    SensorState			SensorState;
};

class FHWRenderTargets {
    friend class FHuaweiVRCustomPresent;
    friend class FHuaweiVRSDK;
public:
    FHWRenderTargets();
    ~FHWRenderTargets();

    void ReleaseResources() {}

    void SwitchToNextElement();

    void InitRenderTarget(uint32 InSizeX, uint32 InSizeY);

    SHWRenderTarget&	GetCurrentTarget();
    SHWRenderTarget&	GetLastTarget();

protected:
    uint32				CurrentIndex;
    FTexture2DRHIRef	TextureRef;
    SHWRenderTarget		Target[HW_RENDERTARGET_COUNT];
};
/**
 *
 */
class FHuaweiVRCustomPresent : public FXRRenderBridge {
    friend class FHuaweiVRSDK;
public:
    FHuaweiVRCustomPresent();
    ~FHuaweiVRCustomPresent();

    // Called when viewport is resized.
    virtual void OnBackBufferResize() override;

    // Called from render thread to see if a native present will be requested for this frame.
    // @return	true if native Present will be requested for this frame; false otherwise.  Must
    // match value subsequently returned by Present for this frame.
    virtual bool NeedsNativePresent() override;

    // Called from RHI thread to perform custom present.
    // @param InOutSyncInterval - in out param, indicates if vsync is on (>0) or off (==0).
    // @return	true if native Present should be also be performed; false otherwise. If it returns
    // true, then InOutSyncInterval could be modified to switch between VSync/NoVSync for the normal 
    // Present.  Must match value previously returned by NeedsNormalPresent for this frame.
    virtual bool Present(int32& InOutSyncInterval) override;

    bool AllocateRenderTargetTexture(uint32 SizeX, uint32 SizeY, uint8 Format, uint32 NumMips, uint32 Flags, uint32 TargetableTextureFlags, FTexture2DRHIRef& OutTargetableTexture, FTexture2DRHIRef& OutShaderResourceTexture, uint32 NumSamples);

    void Init();
    void Shutdown();

    // Frame operations
    void BeginRendering(FRHICommandListImmediate& RHICmdList, FSceneViewFamily& InViewFamily);
    void FinishRendering();
	//void UpdateViewport(const class FViewport& Viewport, class FRHIViewport* InViewportRHI) override;

    // Called when rendering thread is acquired
    virtual void OnAcquireThreadOwnership() override;
    // Called when rendering thread is released
    virtual void OnReleaseThreadOwnership() override;

    void MainThreadRunningNotify(bool running);

    void ConditionalUpdateCache();

public:
    FHWRenderTargets* m_RenderTargets;

private:
    bool mOnApplicationStart;

    FThreadSafeBool mIsThreadOwnershipAcquired;
    FThreadSafeBool mMainThreadRunning;

    HuaweiVRSDKVolume* mVolumeBarDisplay;
};