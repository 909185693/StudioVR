// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "StudioVRAdapter_Pico.h"
#include "Components/AdaptiveControllerComponent.h"
#include "PicoBlueprintFunctionLibrary.h"
#include "PicoMobileControllerBFlib.h"


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
	bool bIs6DOF = false;
	UPicoBlueprintFunctionLibrary::PicoIs6Dof(bIs6DOF);

	if (!bIs6DOF)
	{
		UPicoMobileControllerBFlib::GetPicoControllerOrientationAndPosition(Orientation, Position);

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
		int32 ControllerType = 0;
		UPicoBlueprintFunctionLibrary::PicoGetControllerType(ControllerType);
		if (ControllerType == 1)	// Pico G1
		{
			CustomDisplayMesh = LoadObject<UStaticMesh>(AdaptiveControllerComponent, TEXT("/PicoMobileController/G1/Mesh/Mesh_G1.Mesh_G1"));

			Materials.Add(LoadObject<UMaterialInterface>(AdaptiveControllerComponent, TEXT("/PicoMobileController/G1/Material/M_MotionControllerG1.M_MotionControllerG1")));
		}
		else if (ControllerType == 2)	// Pico Neo
		{
			CustomDisplayMesh = LoadObject<UStaticMesh>(AdaptiveControllerComponent, TEXT("/PicoNeoController/CV/Meshs/Mesh_Neo.Mesh_Neo"));

			Materials.Add(LoadObject<UMaterialInterface>(AdaptiveControllerComponent, TEXT("/PicoNeoController/CV/Materials/M_MotionControllerNeo.M_MotionControllerNeo")));
		}
		else if (ControllerType == 3)	// Pico G2
		{
			CustomDisplayMesh = LoadObject<UStaticMesh>(AdaptiveControllerComponent, TEXT("/PicoMobileController/G2/Mesh/Mesh_G2.Mesh_G2"));

			Materials.Add(LoadObject<UMaterialInterface>(AdaptiveControllerComponent, TEXT("/PicoMobileController/G2/Material/M_MotionControllerG2.M_MotionControllerG2")));
		}
		else if (ControllerType == 4)	// Pico Neo2
		{
			if (TrackingSource == EControllerHand::Left)
			{
				TArray<UMaterialInterface*> OverrideMaterials;
				OverrideMaterials.Add(LoadObject<UMaterialInterface>(AdaptiveControllerComponent, TEXT("/PicoNeoController/CV2/Materials/M_MotionControllerNeo2.M_MotionControllerNeo2")));

				CreateRenderComponent(AdaptiveControllerComponent, TEXT("ppController_NEO2_Left"), TEXT("/PicoNeoController/CV2/Meshes/ppController_NEO2_Left.ppController_NEO2_Left"), FTransform(FRotator(0, 90.f, 124.f), FVector::ZeroVector), OverrideMaterials);
			}
			else if (TrackingSource == EControllerHand::Right)
			{
				TArray<UMaterialInterface*> OverrideMaterials;
				OverrideMaterials.Add(LoadObject<UMaterialInterface>(AdaptiveControllerComponent, TEXT("/PicoNeoController/CV2/Materials/M_MotionControllerNeo2.M_MotionControllerNeo2")));

				CreateRenderComponent(AdaptiveControllerComponent, TEXT("ppController_NEO2_Right"), TEXT("/PicoNeoController/CV2/Meshes/ppController_NEO2_Right.ppController_NEO2_Right"), FTransform(FRotator(0, 90.f, 124.f), FVector::ZeroVector), OverrideMaterials);
			}
		}
		else if (ControllerType == 5)	//Pico Neo3
		{
			if (TrackingSource == EControllerHand::Left)
			{
				TArray<UMaterialInterface*> OverrideMaterials;
				OverrideMaterials.Add(LoadObject<UMaterialInterface>(AdaptiveControllerComponent, TEXT("/PicoNeoController/CV3/Materials/M_MotionControllerNeo3.M_MotionControllerNeo3")));

				CreateRenderComponent(AdaptiveControllerComponent, TEXT("ppController_NEO3_L"), TEXT("/PicoNeoController/CV3/Meshes/ppController_NEO3_L.ppController_NEO3_L"), FTransform(FRotator(0, 90.f, 90.f), FVector::ZeroVector), OverrideMaterials);
			}
			else if (TrackingSource == EControllerHand::Right)
			{
				TArray<UMaterialInterface*> OverrideMaterials;
				OverrideMaterials.Add(LoadObject<UMaterialInterface>(AdaptiveControllerComponent, TEXT("/PicoNeoController/CV3/Materials/M_MotionControllerNeo3.M_MotionControllerNeo3")));

				CreateRenderComponent(AdaptiveControllerComponent, TEXT("ppController_NEO3_R"), TEXT("/PicoNeoController/CV3/Meshes/ppController_NEO3_R.ppController_NEO3_R"), FTransform(FRotator(0, 90.f, 90.f), FVector::ZeroVector), OverrideMaterials);
			}
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