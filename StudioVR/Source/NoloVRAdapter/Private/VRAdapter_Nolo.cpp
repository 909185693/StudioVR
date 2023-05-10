// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#if BUILD_VR_NOLO

#include "VRAdapter/VRAdapter_Nolo.h"
#include "Components/AdaptiveControllerComponent.h"
#include "Engine/Engine.h"
#include "IXRTrackingSystem.h"


void FVRAdapter_Nolo::Construction(UObject* Outer, const class FObjectInitializer& ObjectInitializer)
{

}

void FVRAdapter_Nolo::Register(AStudioVRPawn* NewPawn)
{
	FVRAdapter_Implementation::Register(NewPawn);

	if (GEngine && GEngine->XRSystem.IsValid())
	{
		GEngine->XRSystem->SetTrackingOrigin(EHMDTrackingOrigin::Floor);
	}
}

void FVRAdapter_Nolo::Unregister()
{
	FVRAdapter_Implementation::Unregister();
}

void FVRAdapter_Nolo::Tick(float DeltaTime)
{

}

bool FVRAdapter_Nolo::PollControllerState(EControllerHand TrackingSource, FVector& Position, FRotator& Orientation)
{
	return false;
}

void FVRAdapter_Nolo::AdaptationDisplayComponent(UAdaptiveControllerComponent* AdaptiveControllerComponent)
{
	if (AdaptiveControllerComponent == nullptr)
	{
		return;
	}

	if (AdaptiveControllerComponent->bDisplayDeviceModel && AdaptiveControllerComponent->DisplayModelSource == NAME_None)
	{
		AdaptiveControllerComponent->DisplayModelSource = UMotionControllerComponent::CustomModelSourceId;

		UStaticMesh* CustomDisplayMesh = nullptr;
		if (AdaptiveControllerComponent->GetTrackingSource() == EControllerHand::Left)
		{
			CustomDisplayMesh = LoadObject<UStaticMesh>(AdaptiveControllerComponent, TEXT("/NoloVR/LeftController/LCtrl.LCtrl"));
		}
		else if (AdaptiveControllerComponent->GetTrackingSource() == EControllerHand::Right)
		{
			CustomDisplayMesh = LoadObject<UStaticMesh>(AdaptiveControllerComponent, TEXT("/NoloVR/RightController/RCtrl.RCtrl"));
		}

		if (CustomDisplayMesh)
		{
			AdaptiveControllerComponent->SetCustomDisplayMesh(CustomDisplayMesh);
		}
	}
}

uint8 FVRAdapter_Nolo::GetDeviceType() const
{
	return EDeviceType::Nolo;
}

#endif // BUILD_VR_NOLO