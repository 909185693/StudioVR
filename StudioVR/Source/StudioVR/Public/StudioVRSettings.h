// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "StudioVRSettings.generated.h"

UENUM(BlueprintType)
enum class EVRAdapterType : uint8
{
	None,
	HuaweiVR,
	WaveVR,
	PicoVR,
	OculusVR,
	SteamVR,
	NoloVR,
	GSXR
};

/**
 * UStudioVRSettings
 */
UCLASS(Config = StudioVR, DefaultConfig, Meta = (DisplayName = "StudioVR"))
class STUDIOVR_API UStudioVRSettings : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Config, EditAnywhere, Category = Build)
	EVRAdapterType VRAdapterType;
};

