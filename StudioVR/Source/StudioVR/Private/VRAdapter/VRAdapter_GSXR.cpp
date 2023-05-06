// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#if BUILD_VR_GSXR

#include "VRAdapter/VRAdapter_GSXR.h"
#include "Components/AdaptiveControllerComponent.h"
#include "Engine/Engine.h"
#include "IXRTrackingSystem.h"


void FVRAdapter_GSXR::Construction(UObject* Outer, const class FObjectInitializer& ObjectInitializer)
{

}

void FVRAdapter_GSXR::Register(AStudioVRPawn* NewPawn)
{
	FVRAdapter_Implementation::Register(NewPawn);

	if (GEngine && GEngine->XRSystem.IsValid())
	{
		GEngine->XRSystem->SetTrackingOrigin(EHMDTrackingOrigin::Floor);
	}
}

void FVRAdapter_GSXR::Unregister()
{
	FVRAdapter_Implementation::Unregister();
}

void FVRAdapter_GSXR::Tick(float DeltaTime)
{

}

bool FVRAdapter_GSXR::PollControllerState(EControllerHand TrackingSource, FVector& Position, FRotator& Orientation)
{
	return false;
}

void FVRAdapter_GSXR::AdaptationDisplayComponent(UAdaptiveControllerComponent* AdaptiveControllerComponent)
{
	if (AdaptiveControllerComponent == nullptr)
	{
		return;
	}
}

uint8 FVRAdapter_GSXR::GetDeviceType() const
{
	return EDeviceType::GSXR;
}

#endif // BUILD_VR_GSXR