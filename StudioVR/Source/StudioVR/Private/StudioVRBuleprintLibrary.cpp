// Fill out your copyright notice in the Description page of Project Settings.

#include "StudioVRBuleprintLibrary.h"


EVRDeviceType UStudioVRBuleprintLibrary::GetVRDeviceType()
{
	return GVRAdapter ? (EVRDeviceType)GVRAdapter->GetDeviceType() : EVRDeviceType::Defualt;
}

FVector UStudioVRBuleprintLibrary::GetVRCameraOffset()
{
	if (GVRAdapter != nullptr)
	{
		return GVRAdapter->GetCameraOffset();
	}

	return FVector::ZeroVector;
}

void UStudioVRBuleprintLibrary::StartHandGesture()
{

}

void UStudioVRBuleprintLibrary::StopHandGesture()
{

}

bool UStudioVRBuleprintLibrary::GetBonePositionAndRotation(EGestureHand GestureHand, EGestureBone GestureBone, FVector& OutPosition, FRotator& OutRotation)
{
	return false;
}