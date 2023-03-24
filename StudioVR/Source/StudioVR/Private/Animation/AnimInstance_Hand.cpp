// Fill out your copyright notice in the Description page of Project Settings.

#include "Animation/AnimInstance_Hand.h"


UAnimInstance_Hand::UAnimInstance_Hand(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, BoneBlendSpeed(10.f)
{

}

void UAnimInstance_Hand::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

const FGestureBonePose BonePoseInterpTo(const FGestureBonePose& Current, const FGestureBonePose& Target, float DeltaTime, float InterpSpeed)
{
	FGestureBonePose NewPose = Current;
	NewPose.Position = FMath::VInterpTo(Current.Position, Target.Position, DeltaTime, InterpSpeed);
	NewPose.Rotation = FMath::RInterpTo(Current.Rotation, Target.Rotation, DeltaTime, InterpSpeed);

	return NewPose;
}

void UAnimInstance_Hand::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	UGestureMeshComponent* GestureMesh = Cast<UGestureMeshComponent>(GetOwningComponent());
	if (GestureMesh != nullptr)
	{
		GribAxis = FMath::FInterpTo(GribAxis, GestureMesh->GetGrabAxis(), DeltaSeconds, BoneBlendSpeed);
		IndexAxis = FMath::FInterpTo(IndexAxis, GestureMesh->GetIndexAxis(), DeltaSeconds, BoneBlendSpeed);
		ThumbAxis = FMath::FInterpTo(ThumbAxis, GestureMesh->GetThumbAxis(), DeltaSeconds, BoneBlendSpeed);

		RootPose = BonePoseInterpTo(RootPose, GestureMesh->GetGestureBonePose(EGestureBone::Bone_Root), DeltaSeconds, BoneBlendSpeed);
		UpperarmPose = BonePoseInterpTo(UpperarmPose, GestureMesh->GetGestureBonePose(EGestureBone::Bone_Upperarm), DeltaSeconds, BoneBlendSpeed);
		ForearmPose = BonePoseInterpTo(ForearmPose, GestureMesh->GetGestureBonePose(EGestureBone::Bone_Forearm), DeltaSeconds, BoneBlendSpeed);
		HandWristPose = BonePoseInterpTo(HandWristPose, GestureMesh->GetGestureBonePose(EGestureBone::Bone_Hand_Wrist), DeltaSeconds, BoneBlendSpeed);
		HandPalmPose = BonePoseInterpTo(HandPalmPose, GestureMesh->GetGestureBonePose(EGestureBone::Bone_Hand_Palm), DeltaSeconds, BoneBlendSpeed);
		ThumbJoint1Pose = BonePoseInterpTo(ThumbJoint1Pose, GestureMesh->GetGestureBonePose(EGestureBone::Bone_Thumb_Joint1), DeltaSeconds, BoneBlendSpeed);
		ThumbJoint2Pose = BonePoseInterpTo(ThumbJoint2Pose, GestureMesh->GetGestureBonePose(EGestureBone::Bone_Thumb_Joint2), DeltaSeconds, BoneBlendSpeed);
		ThumbJoint3Pose = BonePoseInterpTo(ThumbJoint3Pose, GestureMesh->GetGestureBonePose(EGestureBone::Bone_Thumb_Joint3), DeltaSeconds, BoneBlendSpeed);
		ThumbTipPose = BonePoseInterpTo(ThumbTipPose, GestureMesh->GetGestureBonePose(EGestureBone::Bone_Thumb_Tip), DeltaSeconds, BoneBlendSpeed);
		IndexJoint1Pose = BonePoseInterpTo(IndexJoint1Pose, GestureMesh->GetGestureBonePose(EGestureBone::Bone_Index_Joint1), DeltaSeconds, BoneBlendSpeed);
		IndexJoint2Pose = BonePoseInterpTo(IndexJoint2Pose, GestureMesh->GetGestureBonePose(EGestureBone::Bone_Index_Joint2), DeltaSeconds, BoneBlendSpeed);
		IndexJoint3Pose = BonePoseInterpTo(IndexJoint3Pose, GestureMesh->GetGestureBonePose(EGestureBone::Bone_Index_Joint3), DeltaSeconds, BoneBlendSpeed);
		IndexTipPose = BonePoseInterpTo(IndexTipPose, GestureMesh->GetGestureBonePose(EGestureBone::Bone_Index_Tip), DeltaSeconds, BoneBlendSpeed);
		MiddleJoint1Pose = BonePoseInterpTo(MiddleJoint1Pose, GestureMesh->GetGestureBonePose(EGestureBone::Bone_Middle_Joint1), DeltaSeconds, BoneBlendSpeed);
		MiddleJoint2Pose = BonePoseInterpTo(MiddleJoint2Pose, GestureMesh->GetGestureBonePose(EGestureBone::Bone_Middle_Joint2), DeltaSeconds, BoneBlendSpeed);
		MiddleJoint3Pose = BonePoseInterpTo(MiddleJoint3Pose, GestureMesh->GetGestureBonePose(EGestureBone::Bone_Middle_Joint3), DeltaSeconds, BoneBlendSpeed);
		MiddleTipPose = BonePoseInterpTo(MiddleTipPose, GestureMesh->GetGestureBonePose(EGestureBone::Bone_Index_Tip), DeltaSeconds, BoneBlendSpeed);
		RingJoint1Pose = BonePoseInterpTo(RingJoint1Pose, GestureMesh->GetGestureBonePose(EGestureBone::Bone_Ring_Joint1), DeltaSeconds, BoneBlendSpeed);
		RingJoint2Pose = BonePoseInterpTo(RingJoint2Pose, GestureMesh->GetGestureBonePose(EGestureBone::Bone_Ring_Joint2), DeltaSeconds, BoneBlendSpeed);
		RingJoint3Pose = BonePoseInterpTo(RingJoint3Pose, GestureMesh->GetGestureBonePose(EGestureBone::Bone_Ring_Joint3), DeltaSeconds, BoneBlendSpeed);
		RingTipPose = BonePoseInterpTo(RingTipPose, GestureMesh->GetGestureBonePose(EGestureBone::Bone_Ring_Tip), DeltaSeconds, BoneBlendSpeed);
		PinkyJoint1Pose = BonePoseInterpTo(PinkyJoint1Pose, GestureMesh->GetGestureBonePose(EGestureBone::Bone_Pinky_Joint1), DeltaSeconds, BoneBlendSpeed);
		PinkyJoint2Pose = BonePoseInterpTo(PinkyJoint2Pose, GestureMesh->GetGestureBonePose(EGestureBone::Bone_Pinky_Joint2), DeltaSeconds, BoneBlendSpeed);
		PinkyJoint3Pose = BonePoseInterpTo(PinkyJoint3Pose, GestureMesh->GetGestureBonePose(EGestureBone::Bone_Pinky_Joint3), DeltaSeconds, BoneBlendSpeed);
		PinkyTipPose = BonePoseInterpTo(PinkyTipPose, GestureMesh->GetGestureBonePose(EGestureBone::Bone_Pinky_Tip), DeltaSeconds, BoneBlendSpeed);
	}
}

