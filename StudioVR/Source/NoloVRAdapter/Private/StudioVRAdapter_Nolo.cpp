// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "StudioVRAdapter_Nolo.h"
#include "Components/AdaptiveControllerComponent.h"
#include "Engine/Engine.h"
#include "IXRTrackingSystem.h"


void FStudioVRAdapter_Nolo::Construction(UObject* Outer, const class FObjectInitializer& ObjectInitializer)
{

}

void FStudioVRAdapter_Nolo::Register(AStudioVRPawn* NewPawn)
{
	FStudioVRAdapter_Implementation::Register(NewPawn);

	if (GEngine && GEngine->XRSystem.IsValid())
	{
		GEngine->XRSystem->SetTrackingOrigin(EHMDTrackingOrigin::Floor);
	}
}

void FStudioVRAdapter_Nolo::Unregister()
{
	FStudioVRAdapter_Implementation::Unregister();
}

void FStudioVRAdapter_Nolo::Tick(float DeltaTime)
{

}

bool FStudioVRAdapter_Nolo::PollControllerState(EControllerHand TrackingSource, FVector& Position, FRotator& Orientation)
{
	return false;
}

void FStudioVRAdapter_Nolo::AdaptationDisplayComponent(UAdaptiveControllerComponent* AdaptiveControllerComponent)
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

uint8 FStudioVRAdapter_Nolo::GetDeviceType() const
{
	return EDeviceType::Nolo;
}