// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "StudioVRAdapter_Steam.h"
#include "Components/AdaptiveControllerComponent.h"
#include "IXRTrackingSystem.h"


void FStudioVRAdapter_Steam::Construction(UObject* Outer, const class FObjectInitializer& ObjectInitializer)
{

}

void FStudioVRAdapter_Steam::Register(AStudioVRPawn* NewPawn)
{
	FStudioVRAdapter_Implementation::Register(NewPawn);

	if (GEngine && GEngine->XRSystem.IsValid())
	{
		GEngine->XRSystem->SetTrackingOrigin(EHMDTrackingOrigin::Floor);
	}
}

void FStudioVRAdapter_Steam::Unregister()
{
	FStudioVRAdapter_Implementation::Unregister();
}

void FStudioVRAdapter_Steam::Tick(float DeltaTime)
{

}

bool FStudioVRAdapter_Steam::PollControllerState(EControllerHand TrackingSource, FVector& Position, FRotator& Orientation)
{
	return false;
}

void FStudioVRAdapter_Steam::AdaptationDisplayComponent(UAdaptiveControllerComponent* AdaptiveControllerComponent)
{

}

uint8 FStudioVRAdapter_Steam::GetDeviceType() const
{
	uint8 DeviceType = EDeviceType::Defualt;
	if (GConfig != nullptr)
	{
		FString DeviceTypeStr;
		GConfig->GetString(TEXT("StudioVR"), TEXT("DeviceType"), DeviceTypeStr, GGameIni);

		DeviceTypeStr = DeviceTypeStr.ToLower();
		if (DeviceTypeStr == TEXT("oculus_quest") || DeviceTypeStr == TEXT("oculus quest"))
		{
			DeviceType = EDeviceType::Oculus_Quest;
		}
		else if (DeviceTypeStr == TEXT("oculus_quest2") || DeviceTypeStr == TEXT("oculus quest2"))
		{
			DeviceType = EDeviceType::Oculus_Quest2;
		}
		else if (DeviceTypeStr == TEXT("htc_vive") || DeviceTypeStr == TEXT("htc vive"))
		{
			DeviceType = EDeviceType::HTC_Vive;
		}
		else if (DeviceTypeStr == TEXT("htc_vive_cosmos") || DeviceTypeStr == TEXT("htc vive cosmos"))
		{
			DeviceType = EDeviceType::HTC_Vive_Cosmos;
		}
		else if (DeviceTypeStr == TEXT("htc_vive_focus") || DeviceTypeStr == TEXT("htc vive focus"))
		{
			DeviceType = EDeviceType::HTC_Vive_Focus;
		}
		else if (DeviceTypeStr == TEXT("htc_vive_focus_plus") || DeviceTypeStr == TEXT("htc vive focus plus"))
		{
			DeviceType = EDeviceType::HTC_Vive_Focus_Plus;
		}
		else if (DeviceTypeStr == TEXT("pico_g2") || DeviceTypeStr == TEXT("pico g2"))
		{
			DeviceType = EDeviceType::Pico_G2;
		}
		else if (DeviceTypeStr == TEXT("pico_g2_4k") || DeviceTypeStr == TEXT("pico g2 4k"))
		{
			DeviceType = EDeviceType::Pico_G2_4K;
		}
		else if (DeviceTypeStr == TEXT("pico_noe") || DeviceTypeStr == TEXT("pico noe"))
		{
			DeviceType = EDeviceType::Pico_Noe;
		}
		else if (DeviceTypeStr == TEXT("pico_noe2") || DeviceTypeStr == TEXT("pico noe2"))
		{
			DeviceType = EDeviceType::Pico_Noe2;
		}
		else if (DeviceTypeStr == TEXT("pico_noe3") || DeviceTypeStr == TEXT("pico noe3"))
		{
			DeviceType = EDeviceType::Pico_Noe3;
		}
		else if (DeviceTypeStr == TEXT("vive_gun") || DeviceTypeStr == TEXT("vive gun"))
		{
			DeviceType = EDeviceType::Vive_Gun;
		}
		else if (DeviceTypeStr == TEXT("pp_gun") || DeviceTypeStr == TEXT("pp gun"))
		{
			DeviceType = EDeviceType::PP_Gun;
		}
	}

	return DeviceType;
}