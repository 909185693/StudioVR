// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "VRAdapter/VRAdapter_Implementation.h"
#include "Engine/StaticMesh.h"
#include "Components/StaticMeshComponent.h"


const static FString NameFlag = TEXT("_RENDER");

void FVRAdapter_Implementation::Register(AStudioVRPawn* NewPawn)
{
	StudioVRPawn = NewPawn;
}

void FVRAdapter_Implementation::Unregister()
{
	StudioVRPawn = nullptr;
}

FVector FVRAdapter_Implementation::GetCameraOffset() const
{
	return FVector::ZeroVector;
}

uint8 FVRAdapter_Implementation::GetDeviceType() const
{
	return EDeviceType::Defualt;
}

void FVRAdapter_Implementation::ClearRenderComponent(class USceneComponent* InParent)
{
	if (InParent != nullptr)
	{
		TArray<USceneComponent*> Children;

		InParent->GetChildrenComponents(true, Children);
		for (USceneComponent* ChildComponent : Children)
		{
			if (ChildComponent && ChildComponent->IsA(UStaticMeshComponent::StaticClass()) && ChildComponent->GetName().Contains(NameFlag))
			{
				ChildComponent->DestroyComponent(true);
			}
		}
	}
}

UStaticMeshComponent* FVRAdapter_Implementation::CreateRenderComponent(USceneComponent* InParent, FName Name, const TCHAR* Filename, const FTransform& RelativeTransform, const TArray<UMaterialInterface*>& OverrideMaterials)
{
	UStaticMesh* StaticMesh = LoadObject<UStaticMesh>(InParent, Filename);
	if (StaticMesh != nullptr)
	{
		const EObjectFlags SubObjFlags = RF_Transactional | RF_TextExportTransient;
		const FName SubObjName = MakeUniqueObjectName(InParent, UStaticMeshComponent::StaticClass(), *FString::Printf(TEXT("%s%s"), *Name.ToString(), *NameFlag));

		bool bIsActive = InParent->IsActive();

		UStaticMeshComponent* MeshComponent = NewObject<UStaticMeshComponent>(InParent, SubObjName, SubObjFlags);
		if (MeshComponent != nullptr)
		{
			MeshComponent->RegisterComponent();
			MeshComponent->SetStaticMesh(StaticMesh);
			MeshComponent->SetHiddenInGame(!bIsActive);
			MeshComponent->SetCollisionProfileName(TEXT("NoCollision"));
			MeshComponent->AttachToComponent(InParent, FAttachmentTransformRules::SnapToTargetIncludingScale);
			MeshComponent->SetRelativeTransform(RelativeTransform);
			for (int32 Index = 0; Index < OverrideMaterials.Num(); Index++)
			{
				MeshComponent->SetMaterial(Index, OverrideMaterials[Index]);
			}
		}

		return MeshComponent;
	}

	return nullptr;
}