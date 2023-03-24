// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Components/GestureMeshComponent.h"
#include "AnimInstance_Hand.generated.h"

/**
 * 
 */
UCLASS()
class STUDIOVR_API UAnimInstance_Hand : public UAnimInstance
{
	GENERATED_UCLASS_BODY()

	// Native init override point
	virtual void NativeInitializeAnimation() override;

	// Native update override point
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hand)
	float GribAxis;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hand)
	float IndexAxis;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hand)
	float ThumbAxis;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hand)
	float BoneBlendSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hand)
	FGestureBonePose RootPose;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hand)
	FGestureBonePose UpperarmPose;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hand)
	FGestureBonePose ForearmPose;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hand)
	FGestureBonePose HandWristPose;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hand)
	FGestureBonePose HandPalmPose;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hand)
	FGestureBonePose ThumbJoint1Pose;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hand)
	FGestureBonePose ThumbJoint2Pose;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hand)
	FGestureBonePose ThumbJoint3Pose;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hand)
	FGestureBonePose ThumbTipPose;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hand)
	FGestureBonePose IndexJoint1Pose;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hand)
	FGestureBonePose IndexJoint2Pose;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hand)
	FGestureBonePose IndexJoint3Pose;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hand)
	FGestureBonePose IndexTipPose;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hand)
	FGestureBonePose MiddleJoint1Pose;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hand)
	FGestureBonePose MiddleJoint2Pose;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hand)
	FGestureBonePose MiddleJoint3Pose;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hand)
	FGestureBonePose MiddleTipPose;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hand)
	FGestureBonePose RingJoint1Pose;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hand)
	FGestureBonePose RingJoint2Pose;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hand)
	FGestureBonePose RingJoint3Pose;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hand)
	FGestureBonePose RingTipPose;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hand)
	FGestureBonePose PinkyJoint1Pose;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hand)
	FGestureBonePose PinkyJoint2Pose;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hand)
	FGestureBonePose PinkyJoint3Pose;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hand)
	FGestureBonePose PinkyTipPose;
};
