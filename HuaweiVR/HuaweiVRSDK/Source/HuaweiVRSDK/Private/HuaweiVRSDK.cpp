// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "HuaweiVRSDK.h"
#include "Misc/App.h"
#include "Misc/CoreDelegates.h"
#include "Modules/ModuleManager.h"
#include "EngineGlobals.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"
#include "Engine/LocalPlayer.h"
#include "IHuaweiVRSDKPlugin.h"
#include "SceneRendering.h"
#include "PostProcess/PostProcessHMD.h"
#include "GameFramework/WorldSettings.h"
#include "Slate/SceneViewport.h"
#include "HuaweiVRSDKMessageTable.h"
#include "IXRTrackingSystem.h"

#define HUAWEI_RT_WIDTH     1080
#define HUAWEI_RT_HEIGHT    1080

FThreadSafeBool FHuaweiVRSDK::RenderFPSChanged = false;
int FHuaweiVRSDK::RenderFrameRate = 60;
int FHuaweiVRSDK::RenderSizeX = HUAWEI_RT_WIDTH;
int FHuaweiVRSDK::RenderSizeY = HUAWEI_RT_HEIGHT;

static const FString HuaweiVRSDKUnrealVersion = TEXT("HuaweiVR Unreal SDK Version: v2.0.0.32");

//---------------------------------------------------
// HuaweiVRSDK Plugin Implementation
//---------------------------------------------------

class FHuaweiVRSDKPlugin : public IHuaweiVRSDKPlugin {
    /** IHeadMountedDisplayModule implementation */
    virtual TSharedPtr< class IXRTrackingSystem, ESPMode::ThreadSafe > CreateTrackingSystem() override;

    FString GetModuleKeyName() const override {
        return FString(TEXT("HuaweiVRSDK"));
    }
};

IMPLEMENT_MODULE(FHuaweiVRSDKPlugin, HuaweiVRSDK)

TSharedPtr< class IXRTrackingSystem, ESPMode::ThreadSafe > FHuaweiVRSDKPlugin::CreateTrackingSystem() {
    auto HuaweiVRSDK = FSceneViewExtensions::NewExtension<FHuaweiVRSDK>();
    if (HuaweiVRSDK->IsInitialized()) {
        return HuaweiVRSDK;
    }
    return nullptr;
}

FHuaweiVRSDK* FHuaweiVRSDK::GetHuaweiVRSDK() {
    if (GEngine && GEngine->XRSystem.IsValid()) {
        IHeadMountedDisplay* HMDDevice = GEngine->XRSystem->GetHMDDevice();
        if (HMDDevice) {
            return static_cast<FHuaweiVRSDK*>(HMDDevice);
        }
    }
    return nullptr;
}

float FHuaweiVRSDK::GetWorldToMetersScale() const {
    return GWorld ? GWorld->GetWorldSettings()->WorldToMeters : 100.0f;
}

//---------------------------------------------------
// HuaweiVRSDK IHeadMountedDisplay Implementation
//---------------------------------------------------

bool FHuaweiVRSDK::IsHMDEnabled() const {
    return true;
}

void FHuaweiVRSDK::EnableHMD(bool enable) {
}

bool FHuaweiVRSDK::GetHMDMonitorInfo(MonitorInfo& MonitorDesc) {
    MonitorDesc.MonitorName = "";
    MonitorDesc.MonitorId = 0;
    MonitorDesc.DesktopX = MonitorDesc.DesktopY = MonitorDesc.ResolutionX = MonitorDesc.ResolutionY = 0;
    return false;
}

void FHuaweiVRSDK::GetFieldOfView(float& OutHFOVInDegrees, float& OutVFOVInDegrees) const {
    OutHFOVInDegrees = 0.0f;
    OutVFOVInDegrees = 0.0f;
}

bool FHuaweiVRSDK::EnumerateTrackedDevices(TArray<int32>& OutDevices, EXRTrackedDeviceType Type) {
    if (Type == EXRTrackedDeviceType::Any || Type == EXRTrackedDeviceType::HeadMountedDisplay) {
        OutDevices.Add(IXRTrackingSystem::HMDDeviceId);
        return true;
    }
    return false;
}

void FHuaweiVRSDK::SetInterpupillaryDistance(float NewInterpupillaryDistance) {
}

float FHuaweiVRSDK::GetInterpupillaryDistance() const {
    //UE_LOG(LogHuaweiVRSDK, Log, TEXT("FHuaweiVRSDK::GetInterpupillaryDistance"));
    return 0.064f;
}

void FHuaweiVRSDK::GetHMDOrientationAndPosition(FQuat& CurrentOrientation, FVector& CurrentPosition) {
    // very basic.  no head model, no prediction, using debuglocalplayer
    //ULocalPlayer* Player = GEngine->GetDebugLocalPlayer();

    if (CustomPresent && CustomPresent->m_RenderTargets) {
        SHWRenderTarget& RenderTarget = CustomPresent->m_RenderTargets->GetCurrentTarget();
        SensorState& st = RenderTarget.SensorState;		
		
        CurrentOrientation = BaseOrientation * FQuat(st.z, st.x, st.y, st.w);
	    CurrentPosition = FVector(st.pz, st.px, st.py) - BasePosition;
		
    } else {
		LOGW("GetHMDOrientationAndPosition failed");
        CurrentOrientation = FQuat(0.0f, 0.0f, 0.0f, 1.0f);
		CurrentPosition = FVector(0.0f, 0.0f, 0.0f);
    }
}

bool FHuaweiVRSDK::GetCurrentPose(int32 DeviceId, FQuat& CurrentOrientation, FVector& CurrentPosition) {
    if (DeviceId != IXRTrackingSystem::HMDDeviceId) {
        return false;
    }

    CurrentOrientation = CurHmdOrientation;
    CurrentPosition = CurHmdPosition;
	LOGI("WSCCCC GetCurrentPose CurrentOrientation x = %f, y = %f, z = %f, w = %f", CurrentOrientation.X, CurrentOrientation.Y, CurrentOrientation.Z, CurrentOrientation.W);
	LOGI("WSCCCC GetCurrentPose CurrentPosition  x = %f, y = %f, z = %f", CurrentPosition.X, CurrentPosition.Y, CurrentPosition.Z);

    return true;
}

bool FHuaweiVRSDK::IsChromaAbCorrectionEnabled() const {
    return false;
}

void FHuaweiVRSDK::ResetOrientationAndPosition(float yaw) {
    ResetOrientation(yaw);
    ResetPosition();
}

void FHuaweiVRSDK::ResetOrientation(float Yaw) {
#if PLATFORM_ANDROID
    HVR_ResetSensorOrientation();
#endif
}

void FHuaweiVRSDK::ResetPosition() {

}

void FHuaweiVRSDK::SetBaseRotation(const FRotator& BaseRot) {
}

FRotator FHuaweiVRSDK::GetBaseRotation() const {
    return FRotator::ZeroRotator;
}

void FHuaweiVRSDK::SetBaseOrientation(const FQuat& NewBaseOrientation) {
	BaseOrientation = NewBaseOrientation;
	LOGI("WSCCCC SetBaseOrientation BaseOrientation x = %f, y = %f, z = %f, w = %f", BaseOrientation.X, BaseOrientation.Y, BaseOrientation.Z, BaseOrientation.W);
}

FQuat FHuaweiVRSDK::GetBaseOrientation() const {
	LOGI("WSCCCC GetBaseOrientation()");
    return BaseOrientation;
}

void FHuaweiVRSDK::SetBasePosition(const FVector& NewBasePosition) {
	LOGI("WSCCCC SetBasePosition BasePosition  x = %f, y = %f, z = %f", BasePosition.X, BasePosition.Y, BasePosition.Z);
}

FVector FHuaweiVRSDK::GetBasePosition() const {
	LOGI("WSCCCC GetBasePosition()");
	return FVector::ZeroVector;
}

#if 0
void FHuaweiVRSDK::DrawDistortionMesh_RenderThread(struct FRenderingCompositePassContext& Context, const FIntPoint& TextureSize) {
    float ClipSpaceQuadZ = 0.0f;
    FMatrix QuadTexTransform = FMatrix::Identity;
    FMatrix QuadPosTransform = FMatrix::Identity;
    const FViewInfo& View = Context.View;
    //const FIntRect SrcRect = View.ViewRect;

    FRHICommandListImmediate& RHICmdList = Context.RHICmdList;
    const FSceneViewFamily& ViewFamily = *(View.Family);
    FIntPoint ViewportSize = ViewFamily.RenderTarget->GetSizeXY();
    RHICmdList.SetViewport(0, 0, 0.0f, ViewportSize.X, ViewportSize.Y, 1.0f);

    static const uint32 NumVerts = 8;
    static const uint32 NumTris = 4;

    static const FDistortionVertex Verts[8] =
    {
        // left eye
        { FVector2D(-0.9f, -0.9f), FVector2D(0.0f, 1.0f), FVector2D(0.0f, 1.0f), FVector2D(0.0f, 1.0f), 1.0f, 0.0f },
        { FVector2D(-0.1f, -0.9f), FVector2D(0.5f, 1.0f), FVector2D(0.5f, 1.0f), FVector2D(0.5f, 1.0f), 1.0f, 0.0f },
        { FVector2D(-0.1f, 0.9f), FVector2D(0.5f, 0.0f), FVector2D(0.5f, 0.0f), FVector2D(0.5f, 0.0f), 1.0f, 0.0f },
        { FVector2D(-0.9f, 0.9f), FVector2D(0.0f, 0.0f), FVector2D(0.0f, 0.0f), FVector2D(0.0f, 0.0f), 1.0f, 0.0f },
        // right eye
        { FVector2D(0.1f, -0.9f), FVector2D(0.5f, 1.0f), FVector2D(0.5f, 1.0f), FVector2D(0.5f, 1.0f), 1.0f, 0.0f },
        { FVector2D(0.9f, -0.9f), FVector2D(1.0f, 1.0f), FVector2D(1.0f, 1.0f), FVector2D(1.0f, 1.0f), 1.0f, 0.0f },
        { FVector2D(0.9f, 0.9f), FVector2D(1.0f, 0.0f), FVector2D(1.0f, 0.0f), FVector2D(1.0f, 0.0f), 1.0f, 0.0f },
        { FVector2D(0.1f, 0.9f), FVector2D(0.5f, 0.0f), FVector2D(0.5f, 0.0f), FVector2D(0.5f, 0.0f), 1.0f, 0.0f },
    };

    static const uint16 Indices[12] = { /*Left*/ 0, 1, 2, 0, 2, 3, /*Right*/ 4, 5, 6, 4, 6, 7 };

    DrawIndexedPrimitiveUP(Context.RHICmdList, PT_TriangleList, 0, NumVerts, NumTris, &Indices,
        sizeof(Indices[0]), &Verts, sizeof(Verts[0]));
}
#endif

void FHuaweiVRSDK::DrawDistortionMesh_RenderThread(struct FRenderingCompositePassContext& Context, const FIntPoint& TextureSize) {
	float ClipSpaceQuadZ = 0.0f;
	FMatrix QuadTexTransform = FMatrix::Identity;
	FMatrix QuadPosTransform = FMatrix::Identity;
	const FViewInfo& View = Context.View;
	const FIntRect SrcRect = View.UnscaledViewRect;

	FRHICommandListImmediate& RHICmdList = Context.RHICmdList;
	const FSceneViewFamily& ViewFamily = *(View.Family);
	FIntPoint ViewportSize = ViewFamily.RenderTarget->GetSizeXY();
	RHICmdList.SetViewport(0, 0, 0.0f, ViewportSize.X, ViewportSize.Y, 1.0f);

	static const uint32 NumVerts = 8;
	static const uint32 NumTris = 4;

	static const FDistortionVertex Verts[8] =
	{
		// left eye
		{ FVector2D(-0.9f, -0.9f), FVector2D(0.0f, 1.0f), FVector2D(0.0f, 1.0f), FVector2D(0.0f, 1.0f), 1.0f, 0.0f },
		{ FVector2D(-0.1f, -0.9f), FVector2D(0.5f, 1.0f), FVector2D(0.5f, 1.0f), FVector2D(0.5f, 1.0f), 1.0f, 0.0f },
		{ FVector2D(-0.1f, 0.9f), FVector2D(0.5f, 0.0f), FVector2D(0.5f, 0.0f), FVector2D(0.5f, 0.0f), 1.0f, 0.0f },
		{ FVector2D(-0.9f, 0.9f), FVector2D(0.0f, 0.0f), FVector2D(0.0f, 0.0f), FVector2D(0.0f, 0.0f), 1.0f, 0.0f },
		// right eye
		{ FVector2D(0.1f, -0.9f), FVector2D(0.5f, 1.0f), FVector2D(0.5f, 1.0f), FVector2D(0.5f, 1.0f), 1.0f, 0.0f },
		{ FVector2D(0.9f, -0.9f), FVector2D(1.0f, 1.0f), FVector2D(1.0f, 1.0f), FVector2D(1.0f, 1.0f), 1.0f, 0.0f },
		{ FVector2D(0.9f, 0.9f), FVector2D(1.0f, 0.0f), FVector2D(1.0f, 0.0f), FVector2D(1.0f, 0.0f), 1.0f, 0.0f },
		{ FVector2D(0.1f, 0.9f), FVector2D(0.5f, 0.0f), FVector2D(0.5f, 0.0f), FVector2D(0.5f, 0.0f), 1.0f, 0.0f },
	};

	FRHIResourceCreateInfo CreateInfo;
	FVertexBufferRHIRef VertexBufferRHI = RHICreateVertexBuffer(sizeof(FDistortionVertex) * 8, BUF_Volatile, CreateInfo);
	void* VoidPtr = RHILockVertexBuffer(VertexBufferRHI, 0, sizeof(FDistortionVertex) * 8, RLM_WriteOnly);
	FPlatformMemory::Memcpy(VoidPtr, Verts, sizeof(FDistortionVertex) * 8);
	RHIUnlockVertexBuffer(VertexBufferRHI);

	static const uint16 Indices[12] = { /*Left*/ 0, 1, 2, 0, 2, 3, /*Right*/ 4, 5, 6, 4, 6, 7 };

	FIndexBufferRHIRef IndexBufferRHI = RHICreateIndexBuffer(sizeof(uint16), sizeof(uint16) * 12, BUF_Volatile, CreateInfo);
	void* VoidPtr2 = RHILockIndexBuffer(IndexBufferRHI, 0, sizeof(uint16) * 12, RLM_WriteOnly);
	FPlatformMemory::Memcpy(VoidPtr2, Indices, sizeof(uint16) * 12);
	RHIUnlockIndexBuffer(IndexBufferRHI);

	RHICmdList.SetStreamSource(0, VertexBufferRHI, 0);
	RHICmdList.DrawIndexedPrimitive(IndexBufferRHI, PT_TriangleList, 0, NumVerts, 0, NumTris, 1);

	IndexBufferRHI.SafeRelease();
	VertexBufferRHI.SafeRelease();
}

FIntPoint FHuaweiVRSDK::GetIdealRenderTargetSize() const {
    //LOGI("GetIdealRenderTargetSize RenderSizeX = %d, RenderSizeY = %d", RenderSizeX, RenderSizeY);
    return FIntPoint(RenderSizeX * 2, RenderSizeY);
}

bool FHuaweiVRSDK::IsStereoEnabled() const {
    return true;
}

bool FHuaweiVRSDK::EnableStereo(bool stereo) {
    return true;
}

void FHuaweiVRSDK::AdjustViewRect(EStereoscopicPass StereoPass, int32& X, int32& Y, uint32& SizeX, uint32& SizeY) const {

    //LOGI("AdjustViewRect (x, y):(%d, %d) (SizeX, SizeY):(%u, %u)", X, Y, SizeX, SizeY);

    SizeX = SizeX / 2;
    if (StereoPass == eSSP_RIGHT_EYE) {
        X += SizeX;
    }
}

void FHuaweiVRSDK::CalculateStereoViewOffset(const enum EStereoscopicPass StereoPassType, FRotator& ViewRotation, const float WorldToMeters, FVector& ViewLocation) {

    //LOGI("FHuaweiVRSDK::CalculateStereoViewOffset");

    if (StereoPassType != eSSP_FULL) {
        const float EyeOffset = (GetInterpupillaryDistance() * 0.5f) * WorldToMeters;
        const float PassOffset = (StereoPassType == eSSP_LEFT_EYE) ? -EyeOffset : EyeOffset;

        FQuat CurrentOrientation = CurHmdOrientation;
        FVector CurrentPosition = CurHmdPosition;
        //float EyeOffset = 3.20000005f;
        //const float PassOffset = (StereoPassType == eSSP_LEFT_EYE) ? EyeOffset : -EyeOffset;

        if (StereoPassType == eSSP_LEFT_EYE) {
            GetHMDOrientationAndPosition(CurrentOrientation, CurrentPosition);
        }
        CurHmdOrientation = CurrentOrientation;
        CurHmdPosition = CurrentPosition;
		
        ViewRotation = CurrentOrientation.Rotator();
		UWorld* World = GWorld ? GWorld->GetWorld() : nullptr;
		APlayerController* PlayerController = World ? World->GetFirstPlayerController() : nullptr;
		if (PlayerController != nullptr) {
			ViewRotation += PlayerController->GetControlRotation();
		}
		ViewLocation += ViewRotation.Quaternion().RotateVector(FVector(0, PassOffset, 0));
		
    }
}

FMatrix FHuaweiVRSDK::GetStereoProjectionMatrix(const enum EStereoscopicPass StereoPassType) const {
#if PLATFORM_ANDROID
    float fFOV = 95.0f;
    HVR_GetFOVAngle(fFOV);

    const float ProjectionCenterOffset = 0.0f;
    const float PassProjectionOffset = (StereoPassType == eSSP_LEFT_EYE) ? ProjectionCenterOffset : -ProjectionCenterOffset;

    const float HalfFov = FMath::DegreesToRadians(fFOV) / 2.f;
    const float InWidth = HUAWEI_RT_WIDTH;
    const float InHeight = HUAWEI_RT_HEIGHT;
    const float XS = 1.0f / tan(HalfFov);
    const float YS = InWidth / tan(HalfFov) / InHeight;

    const float InNearZ = GNearClippingPlane;
    return FMatrix(
        FPlane(XS, 0.0f, 0.0f, 0.0f),
        FPlane(0.0f, YS, 0.0f, 0.0f),
        FPlane(0.0f, 0.0f, 0.0f, 1.0f),
        FPlane(0.0f, 0.0f, InNearZ, 0.0f))

        * FTranslationMatrix(FVector(PassProjectionOffset, 0, 0));
#else
    const float ProjectionCenterOffset = 0.151976421f;
    const float PassProjectionOffset = (StereoPassType == eSSP_LEFT_EYE) ? ProjectionCenterOffset : -ProjectionCenterOffset;

    const float HalfFov = 2.19686294f / 2.f;
    const float InWidth = 640.f;
    const float InHeight = 480.f;
    const float XS = 1.0f / tan(HalfFov);
    const float YS = InWidth / tan(HalfFov) / InHeight;

    const float InNearZ = GNearClippingPlane;
    return FMatrix(
        FPlane(XS, 0.0f, 0.0f, 0.0f),
        FPlane(0.0f, YS, 0.0f, 0.0f),
        FPlane(0.0f, 0.0f, 0.0f, 1.0f),
        FPlane(0.0f, 0.0f, InNearZ, 0.0f))

        * FTranslationMatrix(FVector(PassProjectionOffset, 0, 0));
    //  to do : get projection matrix from sdk
#endif
}

void FHuaweiVRSDK::GetEyeRenderParams_RenderThread(const FRenderingCompositePassContext& Context, FVector2D& EyeToSrcUVScaleValue, FVector2D& EyeToSrcUVOffsetValue) const {
    EyeToSrcUVOffsetValue = FVector2D::ZeroVector;
    EyeToSrcUVScaleValue = FVector2D(1.0f, 1.0f);
}

#if ENGINE_MINOR_VERSION > 22
void FHuaweiVRSDK::RenderTexture_RenderThread(FRHICommandListImmediate& RHICmdList, FRHITexture2D* BackBuffer, FRHITexture2D* SrcTexture, FVector2D WindowSize) const
{
	check(IsInRenderingThread());

	const uint32 ViewportWidth = BackBuffer->GetSizeX();
	const uint32 ViewportHeight = BackBuffer->GetSizeY();
	const uint32 srcTextureWidth = SrcTexture->GetSizeX();
	const uint32 srcTextureHeight = SrcTexture->GetSizeY();

	//LOGI("RenderTexture_RenderThread() Viewport:(%d, %d) Texture:(%d, %d) BackBuffer=%p SrcTexture=%p", ViewportWidth, ViewportHeight, srcTextureWidth, srcTextureHeight, BackBuffer, SrcTexture);

	if (CustomPresent && CustomPresent->m_RenderTargets) {
		SHWRenderTarget& RenderTarget = CustomPresent->m_RenderTargets->GetCurrentTarget();
		for (int nEye = 0; nEye < HW_EYE_COUNT; nEye++) {
			//RHICmdList.SetBlendState(TStaticBlendState<>::GetRHI());
			//RHICmdList.SetRasterizerState(TStaticRasterizerState<>::GetRHI());
			//RHICmdList.SetDepthStencilState(TStaticDepthStencilState<false, CF_Always>::GetRHI());
			FGraphicsPipelineStateInitializer GraphicsPSOInit;
			GraphicsPSOInit.BlendState = TStaticBlendState<>::GetRHI();
			GraphicsPSOInit.RasterizerState = TStaticRasterizerState<>::GetRHI();
			GraphicsPSOInit.DepthStencilState = TStaticDepthStencilState<false, CF_Always>::GetRHI();
			//GraphicsPSOInit.NumSamples = 4;

			FTexture2DRHIRef eyeTexture = RenderTarget.EyeTexture[nEye];

			int nDstTextureWidth = eyeTexture->GetSizeX();
			int nDstTextureHeight = eyeTexture->GetSizeY();

			//LOGI("RenderTexture_RenderThread eyeTexture nDstTextureWidth = %d, nDstTextureHeight = %d", nDstTextureWidth, nDstTextureHeight);
			FRHIRenderPassInfo RPInfo(eyeTexture, ERenderTargetActions::DontLoad_Store);
			RHICmdList.BeginRenderPass(RPInfo, TEXT("EyeTexture"));
			RHICmdList.ApplyCachedRenderTargets(GraphicsPSOInit);
			RHICmdList.SetViewport(0, 0, 0, nDstTextureWidth, nDstTextureHeight, 1.0f);

			const auto FeatureLevel = GMaxRHIFeatureLevel;
			auto ShaderMap = GetGlobalShaderMap(FeatureLevel);

			TShaderMapRef<FScreenVS> VertexShader(ShaderMap);
			TShaderMapRef<FScreenPS> PixelShader(ShaderMap);

			//static FGlobalBoundShaderState BoundShaderState;
			//SetGlobalBoundShaderState(RHICmdList, FeatureLevel, BoundShaderState, RendererModule->GetFilterVertexDeclaration().VertexDeclarationRHI, *VertexShader, *PixelShader);
			GraphicsPSOInit.BoundShaderState.VertexDeclarationRHI = GFilterVertexDeclaration.VertexDeclarationRHI;
			GraphicsPSOInit.BoundShaderState.VertexShaderRHI = GETSAFERHISHADER_VERTEX(*VertexShader);
			GraphicsPSOInit.BoundShaderState.PixelShaderRHI = GETSAFERHISHADER_PIXEL(*PixelShader);
			GraphicsPSOInit.PrimitiveType = PT_TriangleList;

			SetGraphicsPipelineState(RHICmdList, GraphicsPSOInit);

			PixelShader->SetParameters(RHICmdList, TStaticSamplerState<SF_Bilinear>::GetRHI(), SrcTexture);

			RendererModule->DrawRectangle(
				RHICmdList,
				0, 0,
				srcTextureWidth, srcTextureHeight,
				nEye * 0.5f, 0.0f,
				1.0f, 1.0f,
				FIntPoint(nDstTextureWidth, nDstTextureHeight),
				FIntPoint(1, 1),
				*VertexShader,
				EDRF_Default);

			RHICmdList.EndRenderPass();
		}

		//UE_LOG(LogHuaweiVRSDK, Log, TEXT("TimeWarp RenderTexture_RenderThread  tw_idx=%d threadID=%d"), RenderTarget.SensorState.twIdx, gettid());
		return;
	}


	//RHICmdList.SetBlendState(TStaticBlendState<>::GetRHI());
	//RHICmdList.SetRasterizerState(TStaticRasterizerState<>::GetRHI());
	//RHICmdList.SetDepthStencilState(TStaticDepthStencilState<false, CF_Always>::GetRHI());
	FGraphicsPipelineStateInitializer GraphicsPSOInit;
	GraphicsPSOInit.BlendState = TStaticBlendState<>::GetRHI();
	GraphicsPSOInit.RasterizerState = TStaticRasterizerState<>::GetRHI();
	GraphicsPSOInit.DepthStencilState = TStaticDepthStencilState<false, CF_Always>::GetRHI();
	//GraphicsPSOInit.NumSamples = 4;

	FRHIRenderPassInfo RPInfo(BackBuffer, ERenderTargetActions::DontLoad_Store);
	RHICmdList.BeginRenderPass(RPInfo, TEXT("EyeTexture"));
	RHICmdList.ApplyCachedRenderTargets(GraphicsPSOInit);

	RHICmdList.SetViewport(0, 0, 0, ViewportWidth, ViewportHeight, 1.0f);

	const auto FeatureLevel = GMaxRHIFeatureLevel;
	auto ShaderMap = GetGlobalShaderMap(FeatureLevel);

	TShaderMapRef<FScreenVS> VertexShader(ShaderMap);
	TShaderMapRef<FScreenPS> PixelShader(ShaderMap);

	//static FGlobalBoundShaderState BoundShaderState;
	//SetGlobalBoundShaderState(RHICmdList, FeatureLevel, BoundShaderState, RendererModule->GetFilterVertexDeclaration().VertexDeclarationRHI, *VertexShader, *PixelShader);
	GraphicsPSOInit.BoundShaderState.VertexDeclarationRHI = GFilterVertexDeclaration.VertexDeclarationRHI;
	GraphicsPSOInit.BoundShaderState.VertexShaderRHI = GETSAFERHISHADER_VERTEX(*VertexShader);
	GraphicsPSOInit.BoundShaderState.PixelShaderRHI = GETSAFERHISHADER_PIXEL(*PixelShader);
	GraphicsPSOInit.PrimitiveType = PT_TriangleList;

	SetGraphicsPipelineState(RHICmdList, GraphicsPSOInit);

	PixelShader->SetParameters(RHICmdList, TStaticSamplerState<SF_Bilinear>::GetRHI(), SrcTexture);


	RendererModule->DrawRectangle(
		RHICmdList,
		0, 0,
		srcTextureWidth, srcTextureHeight,
		0.0f, 0.0f,
		1.0f, 1.0f,
		FIntPoint(ViewportWidth, ViewportHeight),
		FIntPoint(1, 1),
		*VertexShader,
		EDRF_Default);

	RHICmdList.EndRenderPass();
}
#else
void FHuaweiVRSDK::RenderTexture_RenderThread(FRHICommandListImmediate& RHICmdList, FTexture2DRHIParamRef BackBuffer, FTexture2DRHIParamRef SrcTexture, FVector2D WindowSize) const {

    check(IsInRenderingThread());

    const uint32 ViewportWidth = BackBuffer->GetSizeX();
    const uint32 ViewportHeight = BackBuffer->GetSizeY();
    const uint32 srcTextureWidth = SrcTexture->GetSizeX();
    const uint32 srcTextureHeight = SrcTexture->GetSizeY();

    //LOGI("RenderTexture_RenderThread() Viewport:(%d, %d) Texture:(%d, %d) BackBuffer=%p SrcTexture=%p", ViewportWidth, ViewportHeight, srcTextureWidth, srcTextureHeight, BackBuffer, SrcTexture);

    if (CustomPresent && CustomPresent->m_RenderTargets) {
        SHWRenderTarget& RenderTarget = CustomPresent->m_RenderTargets->GetCurrentTarget();
        for (int nEye = 0; nEye < HW_EYE_COUNT; nEye++) {
            //RHICmdList.SetBlendState(TStaticBlendState<>::GetRHI());
            //RHICmdList.SetRasterizerState(TStaticRasterizerState<>::GetRHI());
            //RHICmdList.SetDepthStencilState(TStaticDepthStencilState<false, CF_Always>::GetRHI());
            FGraphicsPipelineStateInitializer GraphicsPSOInit;
            GraphicsPSOInit.BlendState = TStaticBlendState<>::GetRHI();
            GraphicsPSOInit.RasterizerState = TStaticRasterizerState<>::GetRHI();
            GraphicsPSOInit.DepthStencilState = TStaticDepthStencilState<false, CF_Always>::GetRHI();
            //GraphicsPSOInit.NumSamples = 4;

            FTexture2DRHIParamRef eyeTexture = RenderTarget.EyeTexture[nEye];

            int nDstTextureWidth = eyeTexture->GetSizeX();
            int nDstTextureHeight = eyeTexture->GetSizeY();

            //LOGI("RenderTexture_RenderThread eyeTexture nDstTextureWidth = %d, nDstTextureHeight = %d", nDstTextureWidth, nDstTextureHeight);

            SetRenderTarget(RHICmdList, eyeTexture, FTextureRHIRef());
            RHICmdList.ApplyCachedRenderTargets(GraphicsPSOInit);

            RHICmdList.SetViewport(0, 0, 0, nDstTextureWidth, nDstTextureHeight, 1.0f);

            const auto FeatureLevel = GMaxRHIFeatureLevel;
            auto ShaderMap = GetGlobalShaderMap(FeatureLevel);

            TShaderMapRef<FScreenVS> VertexShader(ShaderMap);
            TShaderMapRef<FScreenPS> PixelShader(ShaderMap);

            //static FGlobalBoundShaderState BoundShaderState;
            //SetGlobalBoundShaderState(RHICmdList, FeatureLevel, BoundShaderState, RendererModule->GetFilterVertexDeclaration().VertexDeclarationRHI, *VertexShader, *PixelShader);
            GraphicsPSOInit.BoundShaderState.VertexDeclarationRHI = GFilterVertexDeclaration.VertexDeclarationRHI;
            GraphicsPSOInit.BoundShaderState.VertexShaderRHI = GETSAFERHISHADER_VERTEX(*VertexShader);
            GraphicsPSOInit.BoundShaderState.PixelShaderRHI = GETSAFERHISHADER_PIXEL(*PixelShader);
            GraphicsPSOInit.PrimitiveType = PT_TriangleList;

            SetGraphicsPipelineState(RHICmdList, GraphicsPSOInit);

            PixelShader->SetParameters(RHICmdList, TStaticSamplerState<SF_Bilinear>::GetRHI(), SrcTexture);

            RendererModule->DrawRectangle(
                RHICmdList,
                0, 0,
                srcTextureWidth, srcTextureHeight,
                nEye * 0.5f, 0.0f,
                1.0f, 1.0f,
                FIntPoint(nDstTextureWidth, nDstTextureHeight),
                FIntPoint(1, 1),
                *VertexShader,
                EDRF_Default);
        }

        //UE_LOG(LogHuaweiVRSDK, Log, TEXT("TimeWarp RenderTexture_RenderThread  tw_idx=%d threadID=%d"), RenderTarget.SensorState.twIdx, gettid());
        return;
    }


    //RHICmdList.SetBlendState(TStaticBlendState<>::GetRHI());
    //RHICmdList.SetRasterizerState(TStaticRasterizerState<>::GetRHI());
    //RHICmdList.SetDepthStencilState(TStaticDepthStencilState<false, CF_Always>::GetRHI());
    FGraphicsPipelineStateInitializer GraphicsPSOInit;
    GraphicsPSOInit.BlendState = TStaticBlendState<>::GetRHI();
    GraphicsPSOInit.RasterizerState = TStaticRasterizerState<>::GetRHI();
    GraphicsPSOInit.DepthStencilState = TStaticDepthStencilState<false, CF_Always>::GetRHI();
    //GraphicsPSOInit.NumSamples = 4;


    SetRenderTarget(RHICmdList, BackBuffer, FTextureRHIRef());
    RHICmdList.ApplyCachedRenderTargets(GraphicsPSOInit);

    RHICmdList.SetViewport(0, 0, 0, ViewportWidth, ViewportHeight, 1.0f);

    const auto FeatureLevel = GMaxRHIFeatureLevel;
    auto ShaderMap = GetGlobalShaderMap(FeatureLevel);

    TShaderMapRef<FScreenVS> VertexShader(ShaderMap);
    TShaderMapRef<FScreenPS> PixelShader(ShaderMap);

    //static FGlobalBoundShaderState BoundShaderState;
    //SetGlobalBoundShaderState(RHICmdList, FeatureLevel, BoundShaderState, RendererModule->GetFilterVertexDeclaration().VertexDeclarationRHI, *VertexShader, *PixelShader);
    GraphicsPSOInit.BoundShaderState.VertexDeclarationRHI = GFilterVertexDeclaration.VertexDeclarationRHI;
    GraphicsPSOInit.BoundShaderState.VertexShaderRHI = GETSAFERHISHADER_VERTEX(*VertexShader);
    GraphicsPSOInit.BoundShaderState.PixelShaderRHI = GETSAFERHISHADER_PIXEL(*PixelShader);
    GraphicsPSOInit.PrimitiveType = PT_TriangleList;

    SetGraphicsPipelineState(RHICmdList, GraphicsPSOInit);

    PixelShader->SetParameters(RHICmdList, TStaticSamplerState<SF_Bilinear>::GetRHI(), SrcTexture);


    RendererModule->DrawRectangle(
        RHICmdList,
        0, 0,
        srcTextureWidth, srcTextureHeight,
        0.0f, 0.0f,
        1.0f, 1.0f,
        FIntPoint(ViewportWidth, ViewportHeight),
        FIntPoint(1, 1),
        *VertexShader,
        EDRF_Default);
}
#endif

FHuaweiVRCustomPresent* FHuaweiVRSDK::GetCustomPresent() {
    //LOGI("FHuaweiVRSDK::GetCustomPresent");
    return CustomPresent;
}

IStereoRenderTargetManager* FHuaweiVRSDK::GetRenderTargetManager() {
    //UE_LOG(LogHuaweiVRSDK, Log, TEXT("FHuaweiVRSDK::GetRenderTargetManager"));
    return this;
}

void FHuaweiVRSDK::SetupViewFamily(FSceneViewFamily& InViewFamily) {
    InViewFamily.EngineShowFlags.MotionBlur = 0;
    InViewFamily.EngineShowFlags.HMDDistortion = false;
    InViewFamily.EngineShowFlags.SetScreenPercentage(true);
    InViewFamily.EngineShowFlags.ScreenPercentage = true;
    InViewFamily.EngineShowFlags.StereoRendering = IsStereoEnabled();
}

void FHuaweiVRSDK::SetupView(FSceneViewFamily& InViewFamily, FSceneView& InView) {
    InView.BaseHmdOrientation = FQuat(0.0f, 0.0f, 0.0f, 1.0f);
    InView.BaseHmdLocation = FVector(0.f);
    /*
    if (InView.StereoPass == eSSP_LEFT_EYE) {
        //LOGI("FHuaweiVRSDK::SetupView left ViewRect (%d, %d)", RenderSizeX, RenderSizeY);
        *((int32*)&InView.UnscaledViewRect.Min.X) = 1;
        *((int32*)&InView.UnscaledViewRect.Min.Y) = 1;
        *((int32*)&InView.UnscaledViewRect.Max.X) = RenderSizeX - 1;
        *((int32*)&InView.UnscaledViewRect.Max.Y) = RenderSizeY - 1;
    } else if (InView.StereoPass == eSSP_RIGHT_EYE) {
        //LOGI("FHuaweiVRSDK::SetupView right ViewRect (%d, %d)", RenderSizeX, RenderSizeY);
        *((int*)&InView.UnscaledViewRect.Min.X) = RenderSizeX + 1;
        *((int*)&InView.UnscaledViewRect.Min.Y) = 1;
        *((int*)&InView.UnscaledViewRect.Max.X) = RenderSizeX * 2 - 1;
        *((int*)&InView.UnscaledViewRect.Max.Y) = RenderSizeY - 1;
    }*/
}

void FHuaweiVRSDK::PreRenderView_RenderThread(FRHICommandListImmediate& RHICmdList, FSceneView& InView) {
    check(IsInRenderingThread());
}

void FHuaweiVRSDK::PreRenderViewFamily_RenderThread(FRHICommandListImmediate& RHICmdList, FSceneViewFamily& ViewFamily) {
    check(IsInRenderingThread());
}

void FHuaweiVRSDK::PostRenderViewFamily_RenderThread(FRHICommandListImmediate& RHICmdList, FSceneViewFamily& InViewFamily) {
    //UE_LOG(LogHuaweiVRSDK, Log, TEXT("FHuaweiVRSDK::PostRenderViewFamily_RenderThread tid %d"), gettid());
#if PLATFORM_ANDROID
    //LOGI("FHuaweiVRSDK::PostRenderViewFamily_RenderThread tid %d", gettid());
#endif
}

bool FHuaweiVRSDK::IsActiveThisFrame(class FViewport* InViewport) const {
    return GEngine && GEngine->IsStereoscopic3D(InViewport);
}

bool FHuaweiVRSDK::ShouldUseSeparateRenderTarget() const {
    //UE_LOG(LogHuaweiVRSDK, Log, TEXT("FHuaweiVRSDK::ShouldUseSeparateRenderTarget"));
    //LOGI("FHuaweiVRSDK::ShouldUseSeparateRenderTarget");
    return true;
}
/*
void FHuaweiVRSDK::CalculateRenderTargetSize(const class FViewport& Viewport, uint32& InOutSizeX, uint32& InOutSizeY) {
    //LOGI("FHuaweiVRSDK::CalculateRenderTargetSize %u %u, %u %u", Viewport.GetSizeXY().X, Viewport.GetSizeXY().Y, InOutSizeX, InOutSizeY);
    InOutSizeX = RenderSizeX * 2;
    InOutSizeY = RenderSizeY;
}

bool FHuaweiVRSDK::NeedReAllocateViewportRenderTarget(const class FViewport& Viewport) {
    //UE_LOG(LogHuaweiVRSDK, Log, TEXT("FHuaweiVRSDK::NeedReAllocateViewportRenderTarget"));
    //LOGI("FHuaweiVRSDK::NeedReAllocateViewportRenderTarget");
    
    if (IsStereoEnabled()) {
        const uint32 InSizeX = Viewport.GetSizeXY().X;
        const uint32 InSizeY = Viewport.GetSizeXY().Y;
        const FIntPoint RenderTargetSize = Viewport.GetRenderTargetTextureSizeXY();

        uint32 NewSizeX = InSizeX, NewSizeY = InSizeY;
        CalculateRenderTargetSize(Viewport, NewSizeX, NewSizeY);
        //UE_LOG(LogHuaweiVRSDK, Log, TEXT("NeedReAllocateViewportRenderTarget() RenderTargetSize:(%d, %d) NewSize:(%d, %d)"), RenderTargetSize.X, RenderTargetSize.Y, NewSizeX, NewSizeY);
        //LOGI("NeedReAllocateViewportRenderTarget() RenderTargetSize:(%u, %u) NewSize:(%u, %u), InSize:(%u, %u)", RenderTargetSize.X, RenderTargetSize.Y, NewSizeX, NewSizeY, InSizeX, InSizeY);

        if (NewSizeX != RenderTargetSize.X || NewSizeY != RenderTargetSize.Y) {
            return true;
        }
    }
    return false;
}
*/
bool FHuaweiVRSDK::AllocateRenderTargetTexture(uint32 Index, uint32 SizeX, uint32 SizeY, uint8 Format, uint32 NumMips, uint32 Flags, uint32 TargetableTextureFlags, FTexture2DRHIRef& OutTargetableTexture, FTexture2DRHIRef& OutShaderResourceTexture, uint32 NumSamples) {
    //LOGI("AllocateRenderTargetTexture() SizeX, SizeY:(%u, %u)", SizeX, SizeY);

    if (CustomPresent) {
        return CustomPresent->AllocateRenderTargetTexture(SizeX, SizeY, Format, NumMips, Flags, TargetableTextureFlags, OutTargetableTexture, OutShaderResourceTexture, NumSamples);
    }
    return false;
}

/*void FHuaweiVRSDK::UpdateViewportRHIBridge(bool bUseSeparateRenderTarget, const class FViewport& Viewport, FRHIViewport* const ViewportRHI) {
    //UE_LOG(LogHuaweiVRSDK, Log, TEXT("FHuaweiVRSDK::UpdateViewportRHIBridge %d tid %d"), bUseSeparateRenderTarget, gettid());
#if PLATFORM_ANDROID
    //LOGI("FHuaweiVRSDK::UpdateViewportRHIBridge %d tid %d", bUseSeparateRenderTarget, gettid());
#endif
    if (CustomPresent) {
        CustomPresent->UpdateViewport(ViewportRHI);
    }
}*/

FXRRenderBridge* FHuaweiVRSDK::GetActiveRenderBridge_GameThread(bool bUseSeparateRenderTarget) {
	//LOGI("FHuaweiVRSDK::GetActiveRenderBridge_GameThread bUseSeparateRenderTarget = %d", bUseSeparateRenderTarget);
    if (bUseSeparateRenderTarget) {
        return CustomPresent;
    } else {
        return nullptr;
    }
}

void FHuaweiVRSDK::ApplicationPauseDelegate() {
    LOGI("FHuaweiVRSDK::ApplicationPauseDelegate");
}
void FHuaweiVRSDK::ApplicationResumeDelegate() {
    LOGI("FHuaweiVRSDK::ApplicationResumeDelegate");
#if PLATFORM_ANDROID
    int msgIdx = -1;
    //Clear messages when app resume
    HVR_GetMsgIdx(msgIdx);
#endif
}

extern void AndroidThunkCpp_SetDesiredViewSize(int32 Width, int32 Height);

FHuaweiVRSDK::FHuaweiVRSDK(const FAutoRegister& AutoRegister) :
	FHeadMountedDisplayBase(nullptr),
    FSceneViewExtensionBase(AutoRegister),
    CurHmdOrientation(FQuat::Identity),
    CurHmdPosition(FVector::ZeroVector),
    BaseOrientation(FQuat::Identity),
	BasePosition(FVector::ZeroVector),
    DeltaControlRotation(FRotator::ZeroRotator),
    DeltaControlOrientation(FQuat::Identity),
    LastSensorTime(-1.0) {
    // Get renderer module
    static const FName RendererModuleName("Renderer");
    RendererModule = FModuleManager::GetModulePtr<IRendererModule>(RendererModuleName);
    check(RendererModule);

    IConsoleVariable* CVarMCSF = IConsoleManager::Get().FindConsoleVariable(TEXT("r.MobileContentScaleFactor"));
    CVarMCSF->Set(0.0f);

    IConsoleVariable* CVarMSAA = IConsoleManager::Get().FindConsoleVariable(TEXT("r.MobileMSAA"));
    int32 msaa = CVarMSAA->GetInt();
#if PLATFORM_ANDROID
    LOGI("Current MSAA is %d", msaa);
    if (msaa < 2) {
        LOGI("Set default MSAA as 2");
        CVarMSAA->Set(2);
    }
#endif

    UpdateFrameRate();

    HuaweiVRSDKMessageTable::init();
#if PLATFORM_ANDROID
    LOGI("%s COMPILE TIME : " __DATE__ " " __TIME__, TCHAR_TO_ANSI(*HuaweiVRSDKUnrealVersion));
    HVR_SendMessage("pluginVersion", TCHAR_TO_ANSI(*HuaweiVRSDKUnrealVersion));
#endif
    CustomPresent = new FHuaweiVRCustomPresent();

    FCoreDelegates::ApplicationWillEnterBackgroundDelegate.AddRaw(this, &FHuaweiVRSDK::ApplicationPauseDelegate);
    FCoreDelegates::ApplicationHasEnteredForegroundDelegate.AddRaw(this, &FHuaweiVRSDK::ApplicationResumeDelegate);
}

FString FHuaweiVRSDK::GetHuaweiVRSDKVersion() {
    return HuaweiVRSDKUnrealVersion;
}

int FHuaweiVRSDK::GetHMDInfo(HelmetModel& helmetModel) {
#if PLATFORM_ANDROID
    return HVR_Android_GetHelmetInfo(HVR_Android_GetHelmet(), &helmetModel);
#else
    return -1;
#endif
}

FHuaweiVRSDK::~FHuaweiVRSDK() {
	// Cause crash here
    //if (nullptr != CustomPresent) {
    //    delete CustomPresent;
    //    CustomPresent = nullptr;
    //}
	if (CustomPresent.IsValid())
	{
		CustomPresent.SafeRelease();
	}
}

void FHuaweiVRSDK::SetRenderSize(int width, int height) {
    RenderSizeX = width;
    RenderSizeY = height;
}
void FHuaweiVRSDK::SetRenderFrameRate(int frameRate) {
    if (RenderFrameRate != frameRate) {
        RenderFPSChanged = true;
    }
    RenderFrameRate = frameRate;
}

void FHuaweiVRSDK::UpdateFrameRate() {
    check(IsInGameThread());
    IConsoleVariable* MaxFPSCVar = IConsoleManager::Get().FindConsoleVariable(TEXT("t.MaxFPS"));
    if (NULL == MaxFPSCVar) {
        LOGE("FHuaweiVRSDK::UpdateFrameRate Cannot find CVar t.MaxFPS");
        return;
    }
    int currentFPS = MaxFPSCVar->GetInt();
    if (currentFPS != RenderFrameRate) {
        LOGI("FHuaweiVRSDK::UpdateFrameRate RenderFrameRate %d", RenderFrameRate);
        MaxFPSCVar->Set(RenderFrameRate);
    }
    RenderFPSChanged = false;
}

bool FHuaweiVRSDK::IsInitialized() const {
    return true;
}

void FHuaweiVRSDK::UpdateViewportSize() {
    check(IsInGameThread());
    if (GWorld) {
        UGameViewportClient* gvc = GWorld->GetGameViewport();
        if (gvc) {
            FVector2D viewportSize;
            gvc->GetViewportSize(viewportSize);
            FSceneViewport* svp = gvc->GetGameViewport();
            if (svp) {
                FIntPoint fip = svp->GetSize();
                if (fip.X != (RenderSizeX * 2) || fip.Y != RenderSizeY) {
                    LOGI("UpdateViewportSize SetSize (%d, %d)", RenderSizeX, RenderSizeY);
                    svp->SetViewportSize(RenderSizeX * 2, RenderSizeY);
                }
            }
        }
    }
}

bool FHuaweiVRSDK::GetHuaweiVRMessage(int& type, int& priority, int& boxType, TMap<FString, FString>& message) {

	bool ret = HuaweiVRSDKMessageTable::QueryMessage(type, priority, boxType, message);

	if (RenderFPSChanged) {
		UpdateFrameRate();
	}

	UpdateViewportSize();

	return ret;
}

int FHuaweiVRSDK::ResetYaw() {
#if PLATFORM_ANDROID
    return HVR_ResetSensorYaw();
#else
    return 0;
#endif
}

int FHuaweiVRSDK::SetPoseLock(bool enable) {
#if PLATFORM_ANDROID
    return HVR_Android_SetPoseLock(HVR_Android_GetHelmet(), enable);
#else
    return 0;
#endif
}

int FHuaweiVRSDK::SetChromaticAberration(bool enable) {
#if PLATFORM_ANDROID
    HVR_Android_EnableChromaticAberration(enable);
    return 0;
#else
    return 0;
#endif
}

int FHuaweiVRSDK::SetSvsEffect(bool enable) {
#if PLATFORM_ANDROID
    return HVR_Android_EnableSvsEffect(enable);
#else
    return 0;
#endif
}