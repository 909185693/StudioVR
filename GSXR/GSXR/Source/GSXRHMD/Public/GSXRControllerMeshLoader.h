// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "GSXRHMD.h"
#include "GSXRInstance.h"
#include "RHI.h"
#include "GSXRControllerMeshLoader.generated.h"

/**
 *
 */
UENUM(BlueprintType)
enum class Hand : uint8
{
	None UMETA(DisplayName = "None"),
	LeftHand UMETA(DisplayName = "LeftHand"),
	RightHand UMETA(DisplayName = "RightHand"),


};

USTRUCT(BlueprintType)
struct FMeshInfo
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ReturnedData")
		TArray<FVector> Vertices;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ReturnedData")
		/** Vertices index */
		TArray<int32> Triangles;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ReturnedData")
		TArray<FVector> Normals;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ReturnedData")
		TArray<FVector2D> UV0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ReturnedData")
		TArray<FVector2D> UV1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ReturnedData")
		TArray<FVector2D> UV2;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ReturnedData")
		TArray<FVector2D> UV3;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ReturnedData")
		TArray<FLinearColor> VertexColors;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ReturnedData")
		TArray<FProcMeshTangent> Tangents;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ReturnedData")
		FTransform RelativeTransform;
};


UCLASS(Blueprintable, ClassGroup = (Rendering, Common), hidecategories = (Object, Activation, "Components|Activation"), ShowCategories = (Mobility), editinlinenew, meta = (BlueprintSpawnableComponent))
class GSXRHMD_API UGSXRControllerMeshLoader : public UProceduralMeshComponent
{
	GENERATED_BODY()
public:

	void loadmesh(int32 LODIndex = 0, bool bCreateCollision = false);
	UPROPERTY(EditDefaultsOnly, Category = Hand, Meta = (DisplayName = "Hand"))
		Hand hand;
	UPROPERTY(EditAnywhere, Category = "Custom")
		UStaticMesh* staticmesh;
	UPROPERTY(BlueprintReadWrite, Category = "Custom")
		UTexture2D* MyTexture2D;
	UFUNCTION(BlueprintCallable)
		UTexture2D* GetTexture2DTest();
#if WITH_EDITOR		
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	void BeginPlay() override;






private:
	void FindMeshInfo(int32 HandType);
	UFUNCTION()
		void LoadTexture();
	bool GetControlleMeshData();
	bool GetHmdDevice();
	UTexture2D* GetTexture2DFromBitmap(GSXR_TextureBitmap Bitmap);
	void SetMeshMaterial(UTexture2D* Texture);
	UPROPERTY()
		FString ControllerMeshPath;
	UPROPERTY()
		FString ControllerMaterialPath;
	GSXR_Flags64 DeviceType;
	UPROPERTY()
		UMaterial* Material;
	UPROPERTY()
		UMaterialInstanceDynamic* DynamicMaterial;
	FUpdateTextureRegion2D Regions;
	UPROPERTY()
		Hand Oldhand = Hand::None;
	FGSXRHMD* HmdDevice;
	GSXR_Flags64 SupportDeviceFlags;
	FMeshInfo meshInfo;
	UGSXRInstance* Gsxrinstance;
};
