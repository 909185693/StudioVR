// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/VRAdaptiveInterface.h"

class AStudioVRPawn;
class UStaticMeshComponent;

class FVRAdapter_Implementation : public IVRAdaptiveInterface
{
public:
	virtual void Register(AStudioVRPawn* NewPawn) override;
	virtual void Unregister() override;
	virtual FVector GetCameraOffset() const;
	virtual uint8 GetDeviceType() const;

protected:	
	void ClearRenderComponent(class USceneComponent* InParent);
	UStaticMeshComponent* CreateRenderComponent(class USceneComponent* InParent, FName Name, const TCHAR* Filename, const FTransform& RelativeTransform = FTransform::Identity, const TArray<UMaterialInterface*>& OverrideMaterials = TArray<UMaterialInterface*>());

protected:
	AStudioVRPawn* StudioVRPawn;
};

