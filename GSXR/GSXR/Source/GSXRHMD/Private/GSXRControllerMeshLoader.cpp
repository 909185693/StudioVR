// Fill out your copyright notice in the Description page of Project Settings.


#include "GSXRControllerMeshLoader.h"
#include "KismetProceduralMeshLibrary.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Modules/ModuleManager.h"




void UGSXRControllerMeshLoader::loadmesh(int32 LODIndex, bool bCreateCollision)
{
	if (staticmesh != nullptr)
	{
		UStaticMesh* StaticMesh = staticmesh;
		//// MESH DATA
		int32 NumSections = StaticMesh->GetNumSections(LODIndex);
		for (int32 SectionIndex = 0; SectionIndex < NumSections; SectionIndex++)
		{
			// Buffers for copying geom data
			TArray<FVector> Vertices;
			TArray<int32> Triangles;
			TArray<FVector> Normals;
			TArray<FVector2D> UVs;
			TArray<FVector2D> UVs1;
			TArray<FVector2D> UVs2;
			TArray<FVector2D> UVs3;
			TArray<FProcMeshTangent> Tangents;
			// Get geom data from static mesh
			UKismetProceduralMeshLibrary::GetSectionFromStaticMesh(StaticMesh, LODIndex, SectionIndex, Vertices, Triangles, Normals, UVs, Tangents);

			// Create section using data
			TArray<FLinearColor> DummyColors;
			CreateMeshSection_LinearColor(SectionIndex, Vertices, Triangles, Normals, UVs, UVs1, UVs2, UVs3, DummyColors, Tangents, bCreateCollision);
		}

		//// SIMPLE COLLISION

		// Clear any existing collision hulls
		ClearCollisionConvexMeshes();

		if (StaticMesh->BodySetup != nullptr)
		{
			// Iterate over all convex hulls on static mesh..
			const int32 NumConvex = StaticMesh->BodySetup->AggGeom.ConvexElems.Num();
			for (int ConvexIndex = 0; ConvexIndex < NumConvex; ConvexIndex++)
			{
				// Copy convex verts to ProcMesh
				FKConvexElem& MeshConvex = StaticMesh->BodySetup->AggGeom.ConvexElems[ConvexIndex];
				AddCollisionConvexMesh(MeshConvex.VertexData);
			}
		}
	}
}

UTexture2D* UGSXRControllerMeshLoader::GetTexture2DTest()
{

	TArray<uint8> Data;
	Data.Reserve(20 * 20 * 4);

	for (int i = 0; i < 20 * 20 * 4; i++)
	{
		Data.Insert(63, i);
		i++;
		Data.Insert(72, i);
		i++;
		Data.Insert(204, i);
		i++;
		Data.Insert(255, i);
	}

	if (!MyTexture2D)
	{
		MyTexture2D = UTexture2D::CreateTransient(20, 20, EPixelFormat::PF_R8G8B8A8);
	}

	MyTexture2D->UpdateResource();

	Regions.SrcX = 0;
	Regions.SrcY = 0;
	Regions.DestX = 0;
	Regions.DestY = 0;
	Regions.Width = 20;
	Regions.Height = 20;

	MyTexture2D->UpdateTextureRegions(0, 1, &Regions, 20 * 4, 4, Data.GetData());

	return MyTexture2D;

}

#if WITH_EDITOR
void UGSXRControllerMeshLoader::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	switch (hand)
	{
	case Hand::LeftHand:
		ControllerMeshPath = TEXT("StaticMesh'/GSXR/M1/Meshes/LCtrl.LCtrl'");
		ControllerMaterialPath = TEXT("Material'/GSXR/M1/Materials/LCtrl_Mat.LCtrl_Mat'");
		break;
	case Hand::RightHand:
		ControllerMeshPath = TEXT("StaticMesh'/GSXR/M1/Meshes/RCtrl.RCtrl'");
		ControllerMaterialPath = TEXT("Material'/GSXR/M1/Materials/RCtrl_Mat.RCtrl_Mat'");
		break;
	default:
		ControllerMeshPath = TEXT("");
		ControllerMaterialPath = TEXT("");
		break;
	}
	if (Oldhand != hand)
	{
		Oldhand = hand;
		staticmesh = LoadObject<UStaticMesh>(NULL, *ControllerMeshPath);
		Material = LoadObject<UMaterial>(NULL, *ControllerMaterialPath);
		if (staticmesh&&Material)
		{
			// mesh = valid path
			loadmesh();
			DynamicMaterial = UMaterialInstanceDynamic::Create(Material, GEngine->GetWorld());
			SetMaterial(0, DynamicMaterial);

		}
		else
		{
			ClearAllMeshSections();
		}
	}
}

#endif

void UGSXRControllerMeshLoader::BeginPlay()
{
	Super::BeginPlay();

#if USE_GSXRAPI	
	Gsxrinstance = UGSXRInstance::GetInstance();
	GetHmdDevice();
	Gsxrinstance->GetSupportedDeviceType(&SupportDeviceFlags);
	Gsxrinstance->ControllerModelTextureLoadComplete.AddDynamic(this, &UGSXRControllerMeshLoader::LoadTexture);
	GSXR_Flags64 XrFeatureType;
	Gsxrinstance->GetSupportedXrFeatures(XrFeatureType);
	if ((GSXR_XrFeatureType::GSXR_XrFeatureType_ControllerModel&XrFeatureType) == GSXR_XrFeatureType::GSXR_XrFeatureType_ControllerModel)
	{

		return;
	}
	else
#endif
	{
		UE_LOG(LogTemp, Log, TEXT("GetSupportedXrFeatures not support"));
		switch (hand)
		{
		case Hand::LeftHand:
			ControllerMeshPath = TEXT("StaticMesh'/GSXR/M1/Meshes/LCtrl.LCtrl'");
			ControllerMaterialPath = TEXT("Material'/GSXR/M1/Materials/LCtrl_Mat.LCtrl_Mat'");
#if USE_GSXRAPI			
			Gsxrinstance->GetSupportedDeviceInputType(GSXR_DeviceType::GSXR_DeviceType_Controller_Left, &DeviceType);
			if (GSXR_Device_InputType::GSXR_Touch == DeviceType)
			{
				UE_LOG(LogTemp, Log, TEXT("GetSupportedXrFeatures CV1 left"));
				ControllerMeshPath = TEXT("StaticMesh'/GSXR/CV1Pro/Meshes/nolo_controller.nolo_controller'");
				ControllerMaterialPath = TEXT("Material'/GSXR/CV1Pro/Materials/C1_Mat.C1_Mat'");
			}
#endif
			break;
		case Hand::RightHand:
			ControllerMeshPath = TEXT("StaticMesh'/GSXR/M1/Meshes/RCtrl.RCtrl'");
			ControllerMaterialPath = TEXT("Material'/GSXR/M1/Materials/RCtrl_Mat.RCtrl_Mat'");
#if USE_GSXRAPI

			Gsxrinstance->GetSupportedDeviceInputType(GSXR_DeviceType::GSXR_DeviceType_Controller_Left, &DeviceType);
			if (GSXR_Device_InputType::GSXR_Touch == DeviceType)
			{
				UE_LOG(LogTemp, Log, TEXT("GetSupportedXrFeatures CV1 right"));
				ControllerMeshPath = TEXT("StaticMesh'/GSXR/CV1Pro/Meshes/nolo_controller.nolo_controller'");
				ControllerMaterialPath = TEXT("Material'/GSXR/CV1Pro/Materials/C1_Mat.C1_Mat'");
			}
#endif
			break;
		default:
			ControllerMeshPath = TEXT("");
			ControllerMaterialPath = TEXT("");
			break;
		}
		staticmesh = LoadObject<UStaticMesh>(NULL, *ControllerMeshPath);
		Material = LoadObject<UMaterial>(NULL, *ControllerMaterialPath);
		if (staticmesh&&Material)
		{
			loadmesh();
			DynamicMaterial = UMaterialInstanceDynamic::Create(Material, this);
			SetMaterial(0, DynamicMaterial);
		}
		else
		{
			ClearAllMeshSections();
		}

	}
}






bool UGSXRControllerMeshLoader::GetControlleMeshData()
{
	//#if USE_GSXRAPI
	//	if (HmdDevice->controllerModelSize == 1)				
	//		switch (hand)
	//		{
	//		case Hand::LeftHand:
	//			
	//			if (SupportDeviceFlags&GSXRMaskFromId(GSXR_DeviceType::GSXR_DeviceType_Controller_Left))
	//			{
	//				UE_LOG(LogTemp, Log, TEXT("GetControlleMeshData GSXR_DeviceType_Controller_Left "));
	//				FindMeshInfo(0);
	//				CreateMeshSection_LinearColor(0,
	//					meshInfo.Vertices,
	//					meshInfo.Triangles,
	//					meshInfo.Normals,
	//					meshInfo.UV0,
	//					meshInfo.UV1,
	//					meshInfo.UV2,
	//					meshInfo.UV3,
	//					meshInfo.VertexColors,
	//					meshInfo.Tangents,
	//					false);
	//			}
	//			else 
	//			{ 
	//				ClearAllMeshSections(); 
	//			}
	//			
	//			break;
	//		case Hand::RightHand:
	//			if (SupportDeviceFlags&GSXRMaskFromId(GSXR_DeviceType::GSXR_DeviceType_Controller_Right))
	//			{
	//				UE_LOG(LogTemp, Log, TEXT("GetControlleMeshData GSXR_DeviceType_Controller_Right "));
	//				FindMeshInfo(0);
	//				CreateMeshSection_LinearColor(0,
	//					meshInfo.Vertices,
	//					meshInfo.Triangles,
	//					meshInfo.Normals,
	//					meshInfo.UV0,
	//					meshInfo.UV1,
	//					meshInfo.UV2,
	//					meshInfo.UV3,
	//					meshInfo.VertexColors,
	//					meshInfo.Tangents,
	//					false);
	//			}
	//			else
	//			{
	//				ClearAllMeshSections();
	//			}
	//			break;
	//		default:
	//
	//			break;
	//		}
	//		return true;
	//	}
	//	else if (HmdDevice->controllerModelSize == 2)
	//	{
	//		switch (hand)
	//		{
	//		case Hand::LeftHand:
	//			FindMeshInfo(1);
	//			CreateMeshSection_LinearColor(0,
	//				meshInfo.Vertices,
	//				meshInfo.Triangles,
	//				meshInfo.Normals,
	//				meshInfo.UV0,
	//				meshInfo.UV1,
	//				meshInfo.UV2,
	//				meshInfo.UV3,
	//				meshInfo.VertexColors,
	//				meshInfo.Tangents,
	//				false);
	//			break;
	//		case Hand::RightHand:
	//			FindMeshInfo(0);
	//			CreateMeshSection_LinearColor(0,
	//				meshInfo.Vertices,
	//				meshInfo.Triangles,
	//				meshInfo.Normals,
	//				meshInfo.UV0,
	//				meshInfo.UV1,
	//				meshInfo.UV2,
	//				meshInfo.UV3,
	//				meshInfo.VertexColors,
	//				meshInfo.Tangents,
	//				false);
	//			break;
	//		default:
	//
	//			break;
	//		}
	//		return true;
	//
	//	}
	//#endif
	return false;
}

bool UGSXRControllerMeshLoader::GetHmdDevice()
{
	if (GEngine->XRSystem.IsValid() && GEngine->XRSystem->GetSystemName() == FName("GSXRHMD"))
	{

		HmdDevice = static_cast<FGSXRHMD*>(GEngine->XRSystem.Get());
		return true;
	}
	else
	{
		return false;
	}

}

UTexture2D* UGSXRControllerMeshLoader::GetTexture2DFromBitmap(GSXR_TextureBitmap Bitmap)
{

	UTexture2D* Texture2D = NULL;
	Texture2D = UTexture2D::CreateTransient(Bitmap.width, Bitmap.height, PF_R8G8B8A8);
	Texture2D->UpdateResource();
	Regions.SrcX = 0;
	Regions.SrcY = 0;
	Regions.DestX = 0;
	Regions.DestY = 0;
	Regions.Width = Bitmap.width;
	Regions.Height = Bitmap.height;
	Texture2D->UpdateTextureRegions(0, 1, &Regions, Bitmap.width * 4, 4, Bitmap.buffer);
	return Texture2D;

}

void UGSXRControllerMeshLoader::SetMeshMaterial(UTexture2D* Texture)
{
	switch (hand)
	{
	case Hand::LeftHand:
		ControllerMaterialPath = TEXT("Material'/GSXR/M1/Materials/LCtrl_Mat.LCtrl_Mat'");
		break;
	case Hand::RightHand:
		ControllerMaterialPath = TEXT("Material'/GSXR/M1/Materials/RCtrl_Mat.RCtrl_Mat'");
		break;
	default:
		ControllerMaterialPath = TEXT("");
		break;
	}
	Material = LoadObject<UMaterial>(NULL, *ControllerMaterialPath);
	if (Material)
	{
		DynamicMaterial = UMaterialInstanceDynamic::Create(Material, this);
		if (Texture != nullptr)
		{
			DynamicMaterial->SetTextureParameterValue(FName("Texture"), Texture);
		}
		SetMaterial(0, DynamicMaterial);
	}

}


void UGSXRControllerMeshLoader::FindMeshInfo(int32 HandType)
{
	GSXR_VertexBuffer vertices = HmdDevice->ControllerModelData[HandType].components[0].vertices;
	GSXR_VertexBuffer normals = HmdDevice->ControllerModelData[HandType].components[0].normals;
	GSXR_VertexBuffer texCoords = HmdDevice->ControllerModelData[HandType].components[0].texCoords;
	GSXR_IndexBuffer   indices = HmdDevice->ControllerModelData[HandType].components[0].indices;



	/*float  localMatrix[16];
	for (uint32 i = 0; i < 16; ++i)
	{
		localMatrix[i] = HmdDevice->ControllerModelData->components[HandType].localMatrix[i];
	}	*/
	meshInfo.Triangles.Reset();
	meshInfo.Vertices.Reset();
	meshInfo.Normals.Reset();
	meshInfo.UV0.Reset();
	//FMatrix tempMatrix;

	//tempMatrix.M[0][0] = localMatrix[0]; tempMatrix.M[0][1] = localMatrix[1]; tempMatrix.M[0][2] = localMatrix[2]; tempMatrix.M[0][3] = localMatrix[3];
	//tempMatrix.M[1][0] = localMatrix[4]; tempMatrix.M[1][1] = localMatrix[5]; tempMatrix.M[1][2] = localMatrix[6]; tempMatrix.M[1][3] = localMatrix[7];
	//tempMatrix.M[2][0] = localMatrix[8]; tempMatrix.M[2][1] = localMatrix[9]; tempMatrix.M[2][2] = localMatrix[10]; tempMatrix.M[2][3] = localMatrix[11];
	//tempMatrix.M[3][0] = localMatrix[12]; tempMatrix.M[3][1] = localMatrix[13]; tempMatrix.M[3][2] = localMatrix[14]; tempMatrix.M[3][3] = localMatrix[15];
	//meshInfo.RelativeTransform = FTransform(tempMatrix);
	for (uint32 i = 0; i < indices.size; ++i)
	{
		meshInfo.Triangles.Add(indices.buffer[i]);
	}
	uint32 VerticesNum = vertices.size / vertices.dimension;
	for (uint32 j = 0; j < VerticesNum; ++j)
	{
		FVector vertex = FVector(
			vertices.buffer[j * 3 + 2],
			-vertices.buffer[j * 3],
			vertices.buffer[j * 3 + 1]);
		meshInfo.Vertices.Add(vertex * 100);
		//Normal		
		FVector normal = FVector(
			normals.buffer[j * 3 + 2],
			-normals.buffer[j * 3],
			normals.buffer[j * 3 + 1]);
		meshInfo.Normals.Add(normal);
		// UV0 Coordinates - inconsistent coordinates		
		FVector2D uv = FVector2D(texCoords.buffer[j * 2], -texCoords.buffer[j * 2 + 1]);
		meshInfo.UV0.Add(uv);

	}
}

void UGSXRControllerMeshLoader::LoadTexture()
{
#if USE_GSXRAPI		 
	UE_LOG(LogTemp, Log, TEXT("AcquireControllerModelData"));
	Gsxrinstance->AcquireControllerModelData(HmdDevice->feature, &HmdDevice->ControllerModelData, &HmdDevice->controllerModelSize);
	//GetControlleMeshData();

	if (HmdDevice->controllerModelSize == 1)
	{

		switch (hand)
		{
		case Hand::LeftHand:
			if (SupportDeviceFlags&GSXRMaskFromId(GSXR_DeviceType::GSXR_DeviceType_Controller_Left))
			{
				UE_LOG(LogTemp, Log, TEXT("GetControlleMeshData GSXR_DeviceType_Controller_Left "));
				FindMeshInfo(0);
				CreateMeshSection_LinearColor(0,
					meshInfo.Vertices,
					meshInfo.Triangles,
					meshInfo.Normals,
					meshInfo.UV0,
					meshInfo.UV1,
					meshInfo.UV2,
					meshInfo.UV3,
					meshInfo.VertexColors,
					meshInfo.Tangents,
					false);
				if (HmdDevice->ControllerModelData[0].textures != nullptr)
				{
					MyTexture2D = GetTexture2DFromBitmap(HmdDevice->ControllerModelData[0].textures[0]);
					SetMeshMaterial(MyTexture2D);
				}
				else
				{
					UE_LOG(LogTemp, Log, TEXT("LeftLoadTexture faild no Texture to find"));
				}
			}
			else
			{
				ClearAllMeshSections();
			}
			break;
		case Hand::RightHand:

			if (SupportDeviceFlags&GSXRMaskFromId(GSXR_DeviceType::GSXR_DeviceType_Controller_Right))
			{
				UE_LOG(LogTemp, Log, TEXT("GetControlleMeshData GSXR_DeviceType_Controller_Right "));
				FindMeshInfo(0);
				CreateMeshSection_LinearColor(0,
					meshInfo.Vertices,
					meshInfo.Triangles,
					meshInfo.Normals,
					meshInfo.UV0,
					meshInfo.UV1,
					meshInfo.UV2,
					meshInfo.UV3,
					meshInfo.VertexColors,
					meshInfo.Tangents,
					false);
				if (HmdDevice->ControllerModelData[0].textures != nullptr)
				{
					MyTexture2D = GetTexture2DFromBitmap(HmdDevice->ControllerModelData[0].textures[0]);
					SetMeshMaterial(MyTexture2D);
				}
				else
				{
					UE_LOG(LogTemp, Log, TEXT("RightLoadTexture faild no Texture to find"));
				}
			}
			else
			{
				ClearAllMeshSections();
			}
			break;
		default:

			break;
		}
		return;
	}
	else if (HmdDevice->controllerModelSize == 2)
	{
		switch (hand)
		{

		case Hand::LeftHand:
			FindMeshInfo(1);
			CreateMeshSection_LinearColor(0,
				meshInfo.Vertices,
				meshInfo.Triangles,
				meshInfo.Normals,
				meshInfo.UV0,
				meshInfo.UV1,
				meshInfo.UV2,
				meshInfo.UV3,
				meshInfo.VertexColors,
				meshInfo.Tangents,
				false);
			MyTexture2D = GetTexture2DFromBitmap(HmdDevice->ControllerModelData[1].textures[0]);
			SetMeshMaterial(MyTexture2D);
			break;
		case Hand::RightHand:
			FindMeshInfo(0);
			CreateMeshSection_LinearColor(0,
				meshInfo.Vertices,
				meshInfo.Triangles,
				meshInfo.Normals,
				meshInfo.UV0,
				meshInfo.UV1,
				meshInfo.UV2,
				meshInfo.UV3,
				meshInfo.VertexColors,
				meshInfo.Tangents,
				false);
			MyTexture2D = GetTexture2DFromBitmap(HmdDevice->ControllerModelData[0].textures[0]); ;
			SetMeshMaterial(MyTexture2D);
			break;
		default:

			break;
		}
		return;

	}


#endif
}
