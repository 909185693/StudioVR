// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "StudioVRPawn.h"
#include "StudioVRPrivate.h"
#include "IXRTrackingSystem.h"
#include "Camera/CameraComponent.h"

AStudioVRPawn::AStudioVRPawn(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	VROriginComponent = CreateDefaultSubobject<USceneComponent>(TEXT("VROriginComponent"));
	SetRootComponent(VROriginComponent);

	VRRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("VRRootComponent"));
	VRRootComponent->SetupAttachment(RootComponent);

	VRCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("VRCameraComponent"));
	VRCameraComponent->SetupAttachment(VRRootComponent);

	PrimaryActorTick.bTickEvenWhenPaused = true;

	if (!HasAnyFlags(RF_ClassDefaultObject | RF_ArchetypeObject) && GVRAdapter != nullptr)
	{
		GVRAdapter->Construction(this, ObjectInitializer);
	}
}

void AStudioVRPawn::BeginPlay()
{
	Super::BeginPlay();
	
	if (GVRAdapter != nullptr)
	{
		GVRAdapter->Register(this);
	}

	{
		const FVector& ActorLocation = GetActorLocation();

		SnapToGround(ActorLocation);
	}
}

void AStudioVRPawn::Destroyed()
{
	if (GVRAdapter != nullptr)
	{
		GVRAdapter->Unregister();
	}

	Super::Destroyed();
}

void AStudioVRPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (GVRAdapter != nullptr)
	{
		GVRAdapter->Tick(DeltaSeconds);
	}
}

void AStudioVRPawn::SetCameraLocation(const FVector& NewRelativeLocation)
{
	if (VRRootComponent != nullptr)
	{
		VRRootComponent->SetRelativeLocation(NewRelativeLocation);
	}
}

bool AStudioVRPawn::TeleportTo(const FVector& DestLocation, const FRotator& DestRotation, bool bIsATest, bool bNoCheck)
{
	bool bResult = Super::TeleportTo(DestLocation, DestRotation, bIsATest, bNoCheck);

	if (bResult)
	{
		SnapToGround(DestLocation);
	}

	return bResult;
}

USceneComponent* AStudioVRPawn::GetVRRootComponent() const
{
	return VRRootComponent;
}

UCameraComponent* AStudioVRPawn::GetCameraComponent() const
{
	return VRCameraComponent;
}

bool AStudioVRPawn::SnapToGround(const FVector& SweepStartLocation)
{
	const FVector StartLocation = GetRootComponent()->Bounds.Origin;

	const FVector Extent = GetRootComponent()->Bounds.BoxExtent;
	const FVector LocationOffset = StartLocation - GetActorLocation();

	// Do the actual actor->world check.  We try to collide against the world, straight down from our current position.
	// If we hit anything, we will move the actor to a position that lets it rest on the floor.
	FHitResult Hit(1.0f);
	FCollisionQueryParams Params(SCENE_QUERY_STAT(MoveActorToTrace), false);

	Params.AddIgnoredActor(this);

	const FVector Direction = FVector(0.f, 0.f, -1.f);
	if (GetWorld()->LineTraceSingleByProfile(Hit, StartLocation, StartLocation + Direction * WORLD_MAX, TEXT("IgnoreOnlyPawn"), Params))
	{
		FVector NewLocation = Hit.Location - LocationOffset;
		NewLocation.Z += KINDA_SMALL_NUMBER;	// Move the new desired location up by an error tolerance

		SetActorLocation(NewLocation);

		return true;
	}

	return false;
}