// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#if BUILD_VR_WAVE

#include "VRAdapter/VRAdapter_Wave.h"
#include "Components/AdaptiveControllerComponent.h"
#include "IXRTrackingSystem.h"

#if (PLATFORM_ANDROID || WAVEVR_WINDOWS_TEST)
#include "WaveVREventCommon.h"
#include "WaveVRBlueprintFunctionLibrary.h"
#include "WaveVRInputFunctionLibrary.h"
#endif

void FVRAdapter_Wave::Construction(UObject* Outer, const class FObjectInitializer& ObjectInitializer)
{
#if (PLATFORM_ANDROID || WAVEVR_WINDOWS_TEST)
	WaveVREventCommon = ObjectInitializer.CreateDefaultSubobject<UWaveVREventCommon>(Outer, TEXT("WaveVREventCommon"));

	FCoreDelegates::ApplicationWillEnterBackgroundDelegate.AddRaw(this, &FVRAdapter_Wave::ApplicationWillEnterBackground);
#endif
}

void FVRAdapter_Wave::Register(AStudioVRPawn* NewPawn)
{
	FVRAdapter_Implementation::Register(NewPawn);

	if (NewPawn != nullptr)
	{
		NewPawn->SetCameraLocation(FVector(0.f, 0.f, 160.f));
	}

	if (GEngine && GEngine->XRSystem.IsValid())
	{
		GEngine->XRSystem->SetTrackingOrigin(EHMDTrackingOrigin::Eye);
	}
}

void FVRAdapter_Wave::Unregister()
{
	FVRAdapter_Implementation::Unregister();

#if (PLATFORM_ANDROID || WAVEVR_WINDOWS_TEST)
	bWaveVRSetupDone = false;

	FCoreDelegates::ApplicationWillEnterBackgroundDelegate.RemoveAll(this);

	if (WaveVREventCommon != nullptr)
	{
		WaveVREventCommon->DestroyComponent();
		WaveVREventCommon = nullptr;
	}
#endif
}

void FVRAdapter_Wave::Tick(float DeltaTime)
{
#if (PLATFORM_ANDROID || WAVEVR_WINDOWS_TEST)
	if (!bWaveVRSetupDone)
	{
		bool bIsWaveVRInputInitialized = UWaveVRInputFunctionLibrary::IsWaveVRInputInitialized();
		if (bIsWaveVRInputInitialized)
		{
			UWaveVRInputFunctionLibrary::UseSimulationPose(SimulatePosition::WhenNoPosition);
			UWaveVRInputFunctionLibrary::FollowHeadPosition(false);

			//#if UE_EDITOR
			//UWaveVRInputFunctionLibrary::EnableInputSimulator(StudioVRPawn);
			//#endif

			if (GEngine && GEngine->XRSystem.IsValid())
			{
				GEngine->XRSystem->ResetOrientation();
			}
			bWaveVRSetupDone = true;
		}
	}
#endif
}

bool FVRAdapter_Wave::PollControllerState(EControllerHand TrackingSource, FVector& Position, FRotator& Orientation)
{
	// 兼容3Dof控制器，模拟定位
	if (bWaveVRSetupDone)
	{
		EWVR_DeviceType DeviceType = EWVR_DeviceType::DeviceType_Controller_Right;
		switch (TrackingSource)
		{
		case EControllerHand::Left:
			DeviceType = EWVR_DeviceType::DeviceType_Controller_Left;
			break;
		case EControllerHand::Right:
			DeviceType = EWVR_DeviceType::DeviceType_Controller_Right;
			break;
		default:
			break;
		}

		if (UWaveVRBlueprintFunctionLibrary::GetSupportedNumOfDoF(DeviceType) == EWVR_DOF::DOF_3)
		{
			Orientation = UWaveVRInputFunctionLibrary::GetWaveVRInputDeviceOrientation(DeviceType);
			Position = UWaveVRInputFunctionLibrary::GetWaveVRInputDevicePosition(DeviceType);

			return true;
		}
	}

	return false;
}

void FVRAdapter_Wave::AdaptationDisplayComponent(UAdaptiveControllerComponent* AdaptiveControllerComponent)
{
	if (AdaptiveControllerComponent == nullptr)
	{
		return;
	}

	if (AdaptiveControllerComponent->bDisplayDeviceModel && AdaptiveControllerComponent->DisplayModelSource == NAME_None)
	{
		EControllerHand TrackingSource = AdaptiveControllerComponent->GetTrackingSource();
		EWVR_DeviceType DeviceType = EWVR_DeviceType::DeviceType_Controller_Right;
		switch (TrackingSource)
		{
		case EControllerHand::Left:
			DeviceType = EWVR_DeviceType::DeviceType_Controller_Left;
			break;
		case EControllerHand::Right:
			DeviceType = EWVR_DeviceType::DeviceType_Controller_Right;
			break;
		default:
			break;
		}

		ClearRenderComponent(AdaptiveControllerComponent);

		if (UWaveVRBlueprintFunctionLibrary::GetSupportedNumOfDoF(DeviceType) == EWVR_DOF::DOF_3)
		{
			CreateRenderComponent(AdaptiveControllerComponent, TEXT("__CM__BodyComponent"), TEXT("/WaveVR/Materials/ControllerModel/Generic/__CM__Body.__CM__Body"));
			CreateRenderComponent(AdaptiveControllerComponent, TEXT("__CM__AppButton"), TEXT("/WaveVR/Materials/ControllerModel/Generic/__CM__AppButton.__CM__AppButton"));
			CreateRenderComponent(AdaptiveControllerComponent, TEXT("__CM__DigitalTriggerKey"), TEXT("/WaveVR/Materials/ControllerModel/Generic/__CM__DigitalTriggerKey.__CM__DigitalTriggerKey"));
			CreateRenderComponent(AdaptiveControllerComponent, TEXT("__CM__HomeButton"), TEXT("/WaveVR/Materials/ControllerModel/Generic/__CM__HomeButton.__CM__HomeButton"));
			CreateRenderComponent(AdaptiveControllerComponent, TEXT("__CM__TouchPad"), TEXT("/WaveVR/Materials/ControllerModel/Generic/__CM__TouchPad.__CM__TouchPad"));
			CreateRenderComponent(AdaptiveControllerComponent, TEXT("__CM__VolumeKey"), TEXT("/WaveVR/Materials/ControllerModel/Generic/__CM__VolumeKey.__CM__VolumeKey"));
		}
		else
		{
			CreateRenderComponent(AdaptiveControllerComponent, TEXT("__CM__BodyComponent"), TEXT("/WaveVR/Materials/ControllerModel/Generic/__CM__Body.__CM__Body"));
			CreateRenderComponent(AdaptiveControllerComponent, TEXT("__CM__AppButton"), TEXT("/WaveVR/Materials/ControllerModel/Generic/__CM__AppButton.__CM__AppButton"));
			CreateRenderComponent(AdaptiveControllerComponent, TEXT("__CM__DigitalTriggerKey"), TEXT("/WaveVR/Materials/ControllerModel/Generic/__CM__DigitalTriggerKey.__CM__DigitalTriggerKey"));
			CreateRenderComponent(AdaptiveControllerComponent, TEXT("__CM__HomeButton"), TEXT("/WaveVR/Materials/ControllerModel/Generic/__CM__HomeButton.__CM__HomeButton"));
			CreateRenderComponent(AdaptiveControllerComponent, TEXT("__CM__TouchPad"), TEXT("/WaveVR/Materials/ControllerModel/Generic/__CM__TouchPad.__CM__TouchPad"));
			CreateRenderComponent(AdaptiveControllerComponent, TEXT("__CM__VolumeKey"), TEXT("/WaveVR/Materials/ControllerModel/Generic/__CM__VolumeKey.__CM__VolumeKey"));
		}
	}	
}

FVector FVRAdapter_Wave::GetCameraOffset() const
{
	return FVector(0.f, 0.f, 180.f);
}

#if (PLATFORM_ANDROID || WAVEVR_WINDOWS_TEST)
void FVRAdapter_Wave::ApplicationWillEnterBackground()
{
	if (WaveVREventCommon != nullptr)
	{
		WaveVREventCommon->ShouldResetButtonStatesOnResume(true);
		WaveVREventCommon->ShouldCheckDoFOnResume(true);
	}
}
#endif

#endif // BUILD_VR_WAVE