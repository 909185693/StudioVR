// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "StudioVRPawn.generated.h"


class USceneComponent;
class UCameraComponent;

UCLASS()
class STUDIOVR_API AStudioVRPawn : public APawn
{
	GENERATED_UCLASS_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual bool TeleportTo(const FVector& DestLocation, const FRotator& DestRotation, bool bIsATest, bool bNoCheck) override;

public:
	virtual void SetCameraLocation(const FVector& NewRelativeLocation);

	virtual bool SnapToGround(const FVector& SweepStartLocation);

public:
	UFUNCTION(BlueprintCallable, Category = VR)
	USceneComponent* GetVRRootComponent() const;

	UFUNCTION(BlueprintCallable, Category = VR)
	UCameraComponent* GetCameraComponent() const;
	
protected:
	UPROPERTY(Category = VR, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* VROriginComponent;

	UPROPERTY(Category = VR, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* VRRootComponent;

	UPROPERTY(Category = VR, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* VRCameraComponent;
};