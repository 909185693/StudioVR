// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#if BUILD_VR_HUAWEI

#include "VRAdapter/VRAdapter_Huawei.h"
#include "StudioVR.h"
#include "Components/AdaptiveControllerComponent.h"
#include "IXRTrackingSystem.h"
#include "HuaweiVRSDKBPFunctionLibrary.h"


void FVRAdapter_Huawei::Construction(UObject* Outer, const class FObjectInitializer& ObjectInitializer)
{

}

void FVRAdapter_Huawei::Register(AStudioVRPawn* NewPawn)
{
	FVRAdapter_Implementation::Register(NewPawn);

	if (NewPawn != nullptr)
	{
		NewPawn->SetCameraLocation(FVector(0.f, 0.f, 180.f));
	}
}

void FVRAdapter_Huawei::Unregister()
{
	FVRAdapter_Implementation::Unregister();
}

void FVRAdapter_Huawei::Tick(float DeltaTime)
{
	HuaweiVRMessageType MessageType;
	int32 Priority;
	int32 BoxType;
	TMap<FString, FString> Message;
	if (UHuaweiVRSDKBPFunctionLibrary::GetHuaweiVRMessage(MessageType, Priority, BoxType, Message))
	{
		FString Messages = TEXT("");
		for (auto Item : Message)
		{
			Messages = FString::Printf(TEXT("[%s:%s] "), *Item.Key, *Item.Value);
		}

		UE_LOG(LogStudioVR, Log, TEXT("MessageType[%d] Priority[%d] BoxType[%s] Messages[%s]"), (int32)MessageType, Priority, BoxType, *Messages);
	}
}

bool FVRAdapter_Huawei::PollControllerState(EControllerHand TrackingSource, FVector& Position, FRotator& Orientation)
{
	const HVRControllerType ControllerType = GetHVRControllerType(TrackingSource);
	if (ControllerType != HVRControllerType::HVRController_6DOF)
	{
		FVector HMDLocation;
		FQuat HMDRotation;
		UHuaweiVRSDKBPFunctionLibrary::GetHMDOrientationAndPosition(HMDRotation, HMDLocation);

		const bool bLeftOrRight = UHVRControllerFunctionLibrary::IsLeftHandMode();
		FRotator MotionRotation = UHVRControllerFunctionLibrary::GetHVRControllerOrientation(TrackingSource);

		GetControllerSimulationRotationAndLocation(HMDLocation, HMDRotation, MotionRotation.Quaternion(), bLeftOrRight, Orientation, Position);

		UE_LOG(LogStudioVR, Log, TEXT("FVRAdapter_Huawei::PollControllerState ControllerType[%d] TrackingSource[%d] Position[%s] Orientation[%s]"), (int32)ControllerType, (int32)TrackingSource, *Position.ToString(), *Orientation.ToString());

		return true;
	}
	
	return false;
}

void FVRAdapter_Huawei::AdaptationDisplayComponent(UAdaptiveControllerComponent* AdaptiveControllerComponent)
{
	if (AdaptiveControllerComponent == nullptr)
	{
		return;
	}

	if (AdaptiveControllerComponent->bDisplayDeviceModel && AdaptiveControllerComponent->DisplayModelSource == NAME_None)
	{
		const EControllerHand TrackingSource = AdaptiveControllerComponent->GetTrackingSource();
		const HVRControllerType ControllerType = GetHVRControllerType(TrackingSource);
		if (ControllerType == HVRControllerType::HVRController_6DOF)
		{
			if (TrackingSource == EControllerHand::Left)
			{
				ClearRenderComponent(AdaptiveControllerComponent);
				CreateRenderComponent(AdaptiveControllerComponent, TEXT("Handle_Rig_L"), TEXT("/HuaweiVRController/ControllerModel/SM_Handle_Rig_L.SM_Handle_Rig_L"), FTransform(FRotator(0, 0, 0), FVector::ZeroVector));
			}
			else if (TrackingSource == EControllerHand::Right)
			{
				ClearRenderComponent(AdaptiveControllerComponent);
				CreateRenderComponent(AdaptiveControllerComponent, TEXT("Handle_Rig_R"), TEXT("/HuaweiVRController/ControllerModel/SM_Handle_Rig_R.SM_Handle_Rig_R"), FTransform(FRotator(0, 0, 0), FVector::ZeroVector));
			}
		}
		else
		{
			ClearRenderComponent(AdaptiveControllerComponent);
			CreateRenderComponent(AdaptiveControllerComponent, TEXT("controller_9_7"), TEXT("/HuaweiVRController/ControllerModel/controller_9_7.controller_9_7"), FTransform(FRotator(-90.f, 0, 90.f), FVector::ZeroVector));
		}		
	}	
}

HVRControllerType FVRAdapter_Huawei::GetHVRControllerType(EControllerHand TrackingSource) const
{
	//HVRControllerType ControllerType = HVRControllerType::HVRController_Other;
	//UHVRControllerFunctionLibrary::GetHVRControllerType(TrackingSource, ControllerType);
	//
	//return ControllerType;

	return HVRControllerType::HVRController_6DOF;
}

void FVRAdapter_Huawei::GetControllerSimulationRotationAndLocation(const FVector& HMDLocation, const FQuat& HMDRotation, const FQuat& ControllerRotation, bool bLeftOrRight, FRotator& OutRotation, FVector& OutLocation) const
{
	FRotator ControllerXYRotation = FRotator(0.f, ControllerRotation.Rotator().Yaw + (bLeftOrRight ? -UE_HANDOFFSET_ANGLE : UE_HANDOFFSET_ANGLE), 0.f);
	FVector WristOffset = ControllerXYRotation.Vector() * UE_ELBOWTOWRIST_OFFSET.X + UE_HEADTOELBOW_OFFSET;

	FVector ControllerForward = ControllerRotation * UE_FORWARD;
	float ControllerPitch = 90.0f - FMath::RadiansToDegrees(acosf(FVector::DotProduct(ControllerForward, UE_UP)));
	float ControllerPitchRadio = (ControllerPitch - UE_ELBOW_PITCH_ANGLE_MIN) / (UE_ELBOW_PITCH_ANGLE_MAX - UE_ELBOW_PITCH_ANGLE_MIN);
	ControllerPitchRadio = FMath::Clamp<float>(ControllerPitchRadio, 0.0f, 1.0f);
	const FVector ChangeArmYAxis = bLeftOrRight ? FVector(1.f, -1.f, 1.f) : FVector(1.f, 1.f, 1.f);
	FVector ElbowPitchOffset = UE_ELBOW_PITCH_OFFSET * ChangeArmYAxis;
	FVector ElbowOffset = ElbowPitchOffset * ControllerPitchRadio;

	OutLocation = HMDLocation + WristOffset + ElbowOffset;
	OutRotation = ControllerRotation.Rotator();
}

#endif // BUILD_VR_HUAWEI