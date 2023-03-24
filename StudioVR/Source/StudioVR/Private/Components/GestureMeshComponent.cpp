// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/GestureMeshComponent.h"
#include "StudioVR.h"


const FGestureBonePose& UGestureMeshComponent::GetGestureBonePose(EGestureBone GestureBone) const
{
	return GestureBonesMap.FindOrAdd(GestureBone);
}

void UGestureMeshComponent::SetGestureBonePose(EGestureBone GestureBone, const FGestureBonePose& GestureBonePose)
{
	FGestureBonePose& GestureBonePoseRef = GestureBonesMap.FindOrAdd(GestureBone);
	GestureBonePoseRef = GestureBonePose;
}

float UGestureMeshComponent::GetGrabAxis()
{
	return GrabAxis;
}

void UGestureMeshComponent::SetGrabAxis(float NewAxis)
{
	GrabAxis = NewAxis;
}

float UGestureMeshComponent::GetIndexAxis()
{
	return IndexAxis;
}

void UGestureMeshComponent::SetIndexAxis(float NewAxis)
{
	IndexAxis = NewAxis;
}

float UGestureMeshComponent::GetThumbAxis()
{
	return ThumbAxis;
}

void UGestureMeshComponent::SetThumbAxis(float NewAxis)
{
	ThumbAxis = NewAxis;
}