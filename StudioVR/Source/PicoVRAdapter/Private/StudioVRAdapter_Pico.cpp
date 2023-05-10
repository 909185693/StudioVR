// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "StudioVRAdapter_Pico.h"
#include "Components/AdaptiveControllerComponent.h"
#include "PXR_HMDFunctionLibrary.h"
#include "PXR_InputFunctionLibrary.h"


void FStudioVRAdapter_Pico::Construction(UObject* Outer, const class FObjectInitializer& ObjectInitializer)
{

}

void FStudioVRAdapter_Pico::Register(AStudioVRPawn* NewPawn)
{
	FStudioVRAdapter_Implementation::Register(NewPawn);

	if (NewPawn != nullptr)
	{
		NewPawn->SetCameraLocation(FVector(0.f, 0.f, 180.f));
	}
}

void FStudioVRAdapter_Pico::Unregister()
{
	FStudioVRAdapter_Implementation::Unregister();
}

void FStudioVRAdapter_Pico::Tick(float DeltaTime)
{

}

bool FStudioVRAdapter_Pico::PollControllerState(EControllerHand TrackingSource, FVector& Position, FRotator& Orientation)
{
	bool bIs6DOF = UPICOXRHMDFunctionLibrary::PXR_DoesSupportPositionalTracking();
	if (!bIs6DOF)
	{
		return true;
	}

	return false;
}

void FStudioVRAdapter_Pico::AdaptationDisplayComponent(UAdaptiveControllerComponent* AdaptiveControllerComponent)
{
	if (AdaptiveControllerComponent->bDisplayDeviceModel && AdaptiveControllerComponent->DisplayModelSource == NAME_None)
	{
		AdaptiveControllerComponent->DisplayModelSource = UMotionControllerComponent::CustomModelSourceId;

		ClearRenderComponent(AdaptiveControllerComponent);

		UStaticMesh* CustomDisplayMesh = nullptr;
		TArray<UMaterialInterface*> Materials;

		EControllerHand TrackingSource = AdaptiveControllerComponent->GetTrackingSource();
		EPICOXRControllerDeviceType ControllerDeviceType = EPICOXRControllerDeviceType::UnKnown;
		UPICOXRInputFunctionLibrary::PXR_GetControllerDeviceType(ControllerDeviceType);
		if (ControllerDeviceType == EPICOXRControllerDeviceType::G2)
		{
			CustomDisplayMesh = LoadObject<UStaticMesh>(AdaptiveControllerComponent, TEXT("/PICOXR/Meshes/Mesh_G2.Mesh_G2"));

			Materials.Add(LoadObject<UMaterialInterface>(AdaptiveControllerComponent, TEXT("/PICOXR/Materials/Mat_G2Controller.Mat_G2Controller")));
		}
		else if (ControllerDeviceType == EPICOXRControllerDeviceType::Neo2)
		{
			if (TrackingSource == EControllerHand::Left)
			{
				CustomDisplayMesh = LoadObject<UStaticMesh>(AdaptiveControllerComponent, TEXT("/PICOXR/Meshes/Mesh_Neo2_Left.Mesh_Neo2_Left"));

				Materials.Add(LoadObject<UMaterialInterface>(AdaptiveControllerComponent, TEXT("/PICOXR/Materials/Mat_Neo2Controller.Mat_Neo2Controller")));
			}
			else
			{
				CustomDisplayMesh = LoadObject<UStaticMesh>(AdaptiveControllerComponent, TEXT("/PICOXR/Meshes/Mesh_Neo2_Right.Mesh_Neo2_Right"));

				Materials.Add(LoadObject<UMaterialInterface>(AdaptiveControllerComponent, TEXT("/PICOXR/Materials/Mat_Neo2Controller.Mat_Neo2Controller")));
			}
		}
		else if (ControllerDeviceType == EPICOXRControllerDeviceType::Neo3)
		{
			if (TrackingSource == EControllerHand::Left)
			{
				CustomDisplayMesh = LoadObject<UStaticMesh>(AdaptiveControllerComponent, TEXT("/PICOXR/Meshes/Mesh_Neo3_Left.Mesh_Neo3_Left"));

				Materials.Add(LoadObject<UMaterialInterface>(AdaptiveControllerComponent, TEXT("/PICOXR/Materials/Mat_Neo3Controller.Mat_Neo3Controller")));
			}
			else
			{
				CustomDisplayMesh = LoadObject<UStaticMesh>(AdaptiveControllerComponent, TEXT("/PICOXR/Meshes/Mesh_Neo3_Right.Mesh_Neo3_Right"));

				Materials.Add(LoadObject<UMaterialInterface>(AdaptiveControllerComponent, TEXT("/PICOXR/Materials/Mat_Neo3Controller.Mat_Neo3Controller")));
			}
		}
		else if (ControllerDeviceType == EPICOXRControllerDeviceType::PICO_4)
		{

		}

		if (CustomDisplayMesh != nullptr)
		{
			AdaptiveControllerComponent->DisplayMeshMaterialOverrides = Materials;
			AdaptiveControllerComponent->SetCustomDisplayMesh(CustomDisplayMesh);
		}
	}
}

FVector FStudioVRAdapter_Pico::GetCameraOffset() const
{
	return FVector(0.f, 0.f, 180.f);
}
