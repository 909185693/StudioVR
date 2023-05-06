// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GSXRInstance.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GSXRInputFunctionLibrary.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EGSXR_ControllerType :uint8
{	
	LeftHand    UMETA(DisplayName = "LeftHand"),
	RightHand   UMETA(DisplayName = "RightHand"),
	
};

UCLASS()
class GSXRINPUT_API UGSXRInputFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
		/**
	* Get controller linear And angular velocity.
	* @param ControllerType     (In) The controller type(LeftHand/RightHand).
	* @param LinearVelocity    (Out) Controller linear velocity.
	* @param AngularVelocity   (Out) Controller angular velocity.
	*/
		UFUNCTION(BlueprintCallable, Category = "GSXR|GSXRInput")
		static bool GetControllerLinearAndAngularVelocity(EGSXR_ControllerType ControllerType, FVector &LinearVelocity, FVector &AngularVelocity, float WorldScale=100.0f);
};
