// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "StudioVRAdapter_GSXR.h"
#include "Components/AdaptiveControllerComponent.h"
#include "Engine/Engine.h"
#include "IXRTrackingSystem.h"


void FStudioVRAdapter_GSXR::Construction(UObject* Outer, const class FObjectInitializer& ObjectInitializer)
{

}

void FStudioVRAdapter_GSXR::Register(AStudioVRPawn* NewPawn)
{
	FStudioVRAdapter_Implementation::Register(NewPawn);

	if (GEngine && GEngine->XRSystem.IsValid())
	{
		GEngine->XRSystem->SetTrackingOrigin(EHMDTrackingOrigin::Floor);
	}
}

void FStudioVRAdapter_GSXR::Unregister()
{
	FStudioVRAdapter_Implementation::Unregister();
}

void FStudioVRAdapter_GSXR::Tick(float DeltaTime)
{

}

bool FStudioVRAdapter_GSXR::PollControllerState(EControllerHand TrackingSource, FVector& Position, FRotator& Orientation)
{
	return false;
}

void FStudioVRAdapter_GSXR::AdaptationDisplayComponent(UAdaptiveControllerComponent* AdaptiveControllerComponent)
{
	if (AdaptiveControllerComponent == nullptr)
	{
		return;
	}
}

uint8 FStudioVRAdapter_GSXR::GetDeviceType() const
{
	return EDeviceType::GSXR;
}