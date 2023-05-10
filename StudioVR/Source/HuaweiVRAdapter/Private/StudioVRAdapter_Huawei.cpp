// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "StudioVRAdapter_Huawei.h"
#include "Components/AdaptiveControllerComponent.h"
#include "IXRTrackingSystem.h"
#include "HuaweiVRSDKBPFunctionLibrary.h"


void FStudioVRAdapter_Huawei::Construction(UObject* Outer, const class FObjectInitializer& ObjectInitializer)
{

}

void FStudioVRAdapter_Huawei::Register(AStudioVRPawn* NewPawn)
{
	FStudioVRAdapter_Implementation::Register(NewPawn);

	if (NewPawn != nullptr)
	{
		NewPawn->SetCameraLocation(FVector(0.f, 0.f, 180.f));
	}
}

void FStudioVRAdapter_Huawei::Unregister()
{
	FStudioVRAdapter_Implementation::Unregister();
}

void FStudioVRAdapter_Huawei::Tick(float DeltaTime)
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
	}
}

bool FStudioVRAdapter_Huawei::PollControllerState(EControllerHand TrackingSource, FVector& Position, FRotator& Orientation)
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

		return true;
	}
	
	return false;
}

void FStudioVRAdapter_Huawei::AdaptationDisplayComponent(UAdaptiveControllerComponent* AdaptiveControllerComponent)
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

HVRControllerType FStudioVRAdapter_Huawei::GetHVRControllerType(EControllerHand TrackingSource) const
{
	//HVRControllerType ControllerType = HVRControllerType::HVRController_Other;
	//UHVRControllerFunctionLibrary::GetHVRControllerType(TrackingSource, ControllerType);
	//
	//return ControllerType;

	return HVRControllerType::HVRController_6DOF;
}

void FStudioVRAdapter_Huawei::GetControllerSimulationRotationAndLocation(const FVector& HMDLocation, const FQuat& HMDRotation, const FQuat& ControllerRotation, bool bLeftOrRight, FRotator& OutRotation, FVector& OutLocation) const
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
