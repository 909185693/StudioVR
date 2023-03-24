// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#if BUILD_VR_OCULUS

#include "VRAdapter/VRAdapter_Oculus.h"
#include "Components/AdaptiveControllerComponent.h"
#include "IXRTrackingSystem.h"
#include "Engine/Engine.h"
#include "OculusFunctionLibrary.h"


void FVRAdapter_Oculus::Construction(UObject* Outer, const class FObjectInitializer& ObjectInitializer)
{

}

void FVRAdapter_Oculus::Register(AStudioVRPawn* NewPawn)
{
	FVRAdapter_Implementation::Register(NewPawn);

	//if (NewPawn != nullptr)
	//{
	//	NewPawn->SetCameraLocation(FVector(0.f, 0.f, 180.f));
	//}

	if (GEngine && GEngine->XRSystem.IsValid())
	{
		GEngine->XRSystem->SetTrackingOrigin(EHMDTrackingOrigin::Floor);
	}
}

void FVRAdapter_Oculus::Unregister()
{
	FVRAdapter_Implementation::Unregister();
}

void FVRAdapter_Oculus::Tick(float DeltaTime)
{

}

bool FVRAdapter_Oculus::PollControllerState(EControllerHand TrackingSource, FVector& Position, FRotator& Orientation)
{
	return false;
}

void FVRAdapter_Oculus::AdaptationDisplayComponent(UAdaptiveControllerComponent* AdaptiveControllerComponent)
{
	if (AdaptiveControllerComponent == nullptr)
	{
		return;
	}

	const FString DeviceName = UOculusFunctionLibrary::GetDeviceName();
	if (DeviceName == TEXT("Oculus Quest") || DeviceName == TEXT("Oculus Quest2"))
	{
		if (AdaptiveControllerComponent->bDisplayDeviceModel && AdaptiveControllerComponent->DisplayModelSource == NAME_None)
		{
			AdaptiveControllerComponent->DisplayModelSource = UMotionControllerComponent::CustomModelSourceId;

			UStaticMesh* CustomDisplayMesh = nullptr;
			if (AdaptiveControllerComponent->GetTrackingSource() == EControllerHand::Left)
			{
				CustomDisplayMesh = LoadObject<UStaticMesh>(AdaptiveControllerComponent, TEXT("/OculusVR/Meshes/LeftTouchForQuestRiftSController.LeftTouchForQuestRiftSController"));
			}
			else if (AdaptiveControllerComponent->GetTrackingSource() == EControllerHand::Right)
			{
				CustomDisplayMesh = LoadObject<UStaticMesh>(AdaptiveControllerComponent, TEXT("/OculusVR/Meshes/RightTouchForQuestRiftSController.RightTouchForQuestRiftSController"));
			}

			if (CustomDisplayMesh)
			{
				AdaptiveControllerComponent->SetCustomDisplayMesh(CustomDisplayMesh);
			}			
		}
	}
}

#endif // BUILD_VR_OCULUS