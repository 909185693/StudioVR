// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Components/GestureMeshComponent.h"
#include "StudioVRPrivate.h"
#include "StudioVRBuleprintLibrary.generated.h"


UENUM(BlueprintType)
enum class EVRDeviceType : uint8
{
	Defualt = 0 /** EDeviceType::Defualt */,

	Oculus_Quest = EDeviceType::Oculus_Quest,
	Oculus_Quest2 = EDeviceType::Oculus_Quest2,

	HTC_Vive = EDeviceType::HTC_Vive,
	HTC_Vive_Cosmos = EDeviceType::HTC_Vive_Cosmos,
	HTC_Vive_Focus = EDeviceType::HTC_Vive_Focus,
	HTC_Vive_Focus_Plus = EDeviceType::HTC_Vive_Focus_Plus,

	Pico_G2 = EDeviceType::Pico_G2,
	Pico_G2_4K = EDeviceType::Pico_G2_4K,
	Pico_Noe = EDeviceType::Pico_Noe,
	Pico_Noe2 = EDeviceType::Pico_Noe2,
	Pico_Noe3 = EDeviceType::Pico_Noe3,

	Vive_Gun = EDeviceType::Vive_Gun,
	PP_Gun = EDeviceType::PP_Gun,

	Nolo = EDeviceType::Nolo
};

UCLASS()
class UStudioVRBuleprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "StudioVR")
	static EVRDeviceType GetVRDeviceType();

	UFUNCTION(BlueprintCallable, Category = "StudioVR")
	static FVector GetVRCameraOffset();

	UFUNCTION(BlueprintCallable, Category = "StudioVR|Gesture", meta = (ToolTip = "To enable the Hand Gesture component."))
	static void StartHandGesture();

	UFUNCTION(BlueprintCallable, Category = "StudioVR|Gesture", meta = (ToolTip = "To disable the Hand Gesture component."))
	static void StopHandGesture();

	UFUNCTION(BlueprintCallable, Category = "StudioVR|Gesture", meta = (ToolTip = "If the Hand Tracking component is available, this API is used for getting the bone poses."))
	static bool GetBonePositionAndRotation(EGestureHand GestureHand, EGestureBone GestureBone, FVector& OutPosition, FRotator& OutRotation);


};