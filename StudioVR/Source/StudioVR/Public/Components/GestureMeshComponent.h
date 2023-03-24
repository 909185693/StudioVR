// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "GestureMeshComponent.generated.h"

UENUM(BlueprintType)
enum class EGestureHand : uint8
{
	Left,
	Right
};

UENUM(BlueprintType)
enum class EGestureBone : uint8
{
	Bone_Root,
	Bone_Upperarm,
	Bone_Forearm,
	Bone_Hand_Wrist,
	Bone_Hand_Palm,
	Bone_Thumb_Joint1,	// 5
	Bone_Thumb_Joint2,
	Bone_Thumb_Joint3,
	Bone_Thumb_Tip,
	Bone_Index_Joint1,
	Bone_Index_Joint2,	// 10
	Bone_Index_Joint3,
	Bone_Index_Tip,
	Bone_Middle_Joint1,
	Bone_Middle_Joint2,
	Bone_Middle_Joint3,	// 15
	Bone_Middle_Tip,
	Bone_Ring_Joint1,
	Bone_Ring_Joint2,
	Bone_Ring_Joint3,
	Bone_Ring_Tip,		// 20
	Bone_Pinky_Joint1,
	Bone_Pinky_Joint2,
	Bone_Pinky_Joint3,
	Bone_Pinky_Tip,
};

USTRUCT(BlueprintType)
struct FGestureBonePose
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gesture)
	FVector Position;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gesture)
	FRotator Rotation;
};

/**
 * 手势模型组件
 */
UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent), ClassGroup = Mesh)
class STUDIOVR_API UGestureMeshComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gesture)
	EGestureHand GestureHand;

	UPROPERTY(BlueprintReadWrite, Category = Gesture)
	float GrabAxis;

	UPROPERTY(BlueprintReadWrite, Category = Gesture)
	float IndexAxis;

	UPROPERTY(BlueprintReadWrite, Category = Gesture)
	float ThumbAxis;

public:
	UFUNCTION(BlueprintCallable, Category = Gesture)
	const FGestureBonePose& GetGestureBonePose(EGestureBone GestureBone) const;

	UFUNCTION(BlueprintCallable, Category = Gesture)
	void SetGestureBonePose(EGestureBone GestureBone, const FGestureBonePose& GestureBonePose);

	UFUNCTION(BlueprintCallable, Category = Gesture)
	float GetGrabAxis();

	UFUNCTION(BlueprintCallable, Category = Gesture)
	void SetGrabAxis(float NewAxis);

	UFUNCTION(BlueprintCallable, Category = Gesture)
	float GetIndexAxis();

	UFUNCTION(BlueprintCallable, Category = Gesture)
	void SetIndexAxis(float NewAxis);

	UFUNCTION(BlueprintCallable, Category = Gesture)
	float GetThumbAxis();

	UFUNCTION(BlueprintCallable, Category = Gesture)
	void SetThumbAxis(float NewAxis);

private:
	UPROPERTY()
	mutable TMap<EGestureBone, FGestureBonePose> GestureBonesMap;
};
