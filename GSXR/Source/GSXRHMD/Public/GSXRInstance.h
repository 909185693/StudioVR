// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Logging/LogMacros.h"
#define USE_GSXRAPI  0 | PLATFORM_ANDROID
#if USE_GSXRAPI
#include "gsxr.h"
#include "gsxr_types.h"
#include "gsxr_andros.h"
#include "gsxr_device.h"
#include "gsxr_renderer.h"
#include "gsxr_input_types.h"
#include "gsxr_system.h"
#include "HeadMountedDisplayTypes.h"
#endif
#include "gsxr_controller_model.h"

#include "GSXRInstance.generated.h"
/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGSXRDeviceConnectStateChangeDelegage,int32,DeviceType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGSXRControllerModelTextureLoadComplete);
UCLASS()
class GSXRHMD_API UGSXRInstance : public UObject
{
	GENERATED_BODY()
public:
	static UGSXRInstance* GetInstance();
	static UGSXRInstance* Gsxr_Instance;
	UGSXRInstance();

	UPROPERTY(BlueprintAssignable)
	FGSXRDeviceConnectStateChangeDelegage DeviceStateChangedDelegate;
	UPROPERTY(BlueprintAssignable)
		FGSXRControllerModelTextureLoadComplete ControllerModelTextureLoadComplete;
#if USE_GSXRAPI
	bool CreateRuntime();
	bool GetDeviceConnectState(GSXR_DeviceType DeviceType,bool& bConnectState);
	bool CreateRender();
	bool GetTextureQueueOptions(GSXR_Flags64 &RecommendedOptions,GSXR_Flags64 &SupportedOptions);
	bool GetViewSetConfiguration(GSXR_ViewSetConfiguration &Gsxr_ViewSetConfigurations);
	bool CreateTextureQueue(GSXR_Flags64 TextureQueueOptions,uint8 Format,uint32 SizeX, uint32 SizeY, uint32 SampleCount, uint32 FaceCount, uint32 NumMips,uint32 ArrayCount,TArray<uint32_t>& TextureResources,uint8 & TargetFormat);
	bool CreateTextureQueue(GSXR_Flags64 TextureQueueOptions, uint8 Format, uint32 SizeX, uint32 SizeY, uint32 SampleCount, uint32 FaceCount, uint32 NumMips, uint32 ArrayCount);
	bool SetTrackingOrigin(EHMDTrackingOrigin::Type NewTrackingOrigin);
	bool GetDegreeOfFreedom(GSXR_DeviceType DeviceType,GSXR_DegreeOfFreedom& DegreeOfFreedom);
	bool GetRuntimeProperties(GSXR_RuntimeProperties &RuntimeProperties);
	bool GetDeviceProperties(GSXR_DeviceType DeviceType,GSXR_DeviceProperties &DeviceProperties);
	bool WaitFrame(GSXR_Time& PredictedDisplayTime);
	bool WaitAndAcquireAvailableTexture(GSXR_Duration Duration,GSXR_TextureElement &TextureElement);
	bool PreRenderFrame(GSXR_FrameRenderInfo*     PreRenderInfo);
	bool PostRenderFrame(GSXR_FrameRenderInfo*     PostRenderInfo);
	uint64_t GetSupportedFunctions();
	bool GetViewPoseState(GSXR_Time Time,GSXR_ViewPoseState & ViewPoseState);
	bool SubmitFrame(GSXR_FrameSubmitInfo FrameSubmitInfo);
	bool ReleaseTexture(GSXR_TextureElement TextureElement);
	bool StartRendering_RHIThread();
	bool StopRendering_RHIThread();
	bool GetBatteryStatus(GSXR_DeviceType DeviceType,float& Battery, bool& bIsCharging);
	bool ShutDown();
	bool PollEvent();
	bool ProcessXREvent(GSXR_Event GsxrEvent);
	bool GetTextureQueue(GSXR_TextureQueue& TextureQueue);
	class FGSXRInput* GetGSXRInput();
	bool GetInputClickStates(GSXR_DeviceType Device,GSXR_Device_InputType Input,GSXR_Flags64  &Click);
	/*****************ControllerInterface**************************************/
	bool SetXrActionManifestPath(FString ManifestPath);
	bool GetXrActionSetHandle(FString ActionSetName, GSXR_XrActionSetHandle_t& ActionSetHandle);
	bool GetXrActionHandle(FString ActionName,GSXR_XrActionHandle_t& ActionHandle);
	bool GetXrInputSourceHandle(FString InputSourcePath,  GSXR_XrInputValueHandle_t& InputValueHandle);
	bool UpdateXrActionState(GSXR_XrActiveActionSet ActionSetHandle,uint32_t SizeOfVRSelectedActionSet,uint32_t ActionSetCount);
	bool GetXrActionDigitalActionData(GSXR_XrActionHandle_t ActionHandle,GSXR_XrInputDigitalActionData &ActionData,uint32_t ActionDateSize,GSXR_XrInputValueHandle_t InputValueHandle);
	bool GetXrAnalogActionData(GSXR_XrActionHandle_t ActionHandle,GSXR_XrInputAnalogActionData &ActionData,uint32_t ActionDateSize,GSXR_XrInputValueHandle_t InputValueHandle);
	bool TriggerVibration(int32 DeviceHand,bool bUseParameter,uint32 VibrationTime,uint32 VibrationFrequency,float VibrationIntensity);
	bool GetPoseState(GSXR_DeviceType DeviceType, float WorldScale,FVector& Position, FRotator& Rotation);
	bool GetControllerVelocityState(GSXR_DeviceType DeviceType, float WorldScale, FVector& LinearVelocity, FVector& AngularVelocity);
	bool GetPrimaryDeviceType(GSXR_Flags64 &Flags);
	bool SetPrimaryDeviceType(GSXR_DeviceType &DeviceType);
	bool GetSupportedXrFeatures(GSXR_Flags64& featureType);
	bool IsXrFeatureAvailable(GSXR_XrFeatureType featureType, bool& bavailable);
	bool GetXrFeatureOptions(GSXR_XrFeatureType featureType, GSXR_Flags64& recommendedOptions, GSXR_Flags64& supportedOptions);
	bool StartXrFeature(GSXR_XrFeatureType featureType, GSXR_XrFeature* feature, GSXR_Flags64 recommendedOptions);
	bool StopXrFeature(GSXR_XrFeature feature);
	bool AcquireControllerModelData(GSXR_XrFeature feature, GSXR_ControllerModelData** controllerModel, uint32* controllerModelSize);
	void ReleaseControllerModelData(GSXR_XrFeature feature, GSXR_ControllerModelData* controllerModel);
	bool GetSupportedDeviceInputType(GSXR_DeviceType deviceType, GSXR_Flags64*  deviceInputTypeFlag);
	bool GetSupportedDeviceType(GSXR_Flags64* deviceTypeFlags);
	bool SetSystemSetting(GSXR_XrSystemSettingType settingType);
	bool ResetOrientationAndPosition(float yaw);

private:
	GSXR_Runtime Gsxr_Runtime;
	GSXR_Renderer Gsxr_Renderer;
	TArray<GSXR_ViewSetConfiguration> Gsxr_ViewSetConfigurations;
	GSXR_ViewSetConfiguration Gsxr_StereoViewSetConfiguration;
	GSXR_TextureQueue Gsxr_TextureQueue;
	GSXR_RenderLooper Gsxr_RenderLooper;
	GSXR_TextureElement Gsxr_TextureElement;
	uint64_t RecommendedFunctions;
	uint64_t SupportedFunctions;
	
#endif

	
};
