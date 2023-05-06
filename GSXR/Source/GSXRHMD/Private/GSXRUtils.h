#pragma once
#include "CoreMinimal.h"

class FGSXRUtils 
{
public:
	static FQuat ConvertXRQuatToUnrealQuat(FQuat InQuat);

	static FVector ConvertXRVectorToUnrealVector(FVector InVector, float Scale);

	static FQuat ConvertUnrealQuatToXRQuat(FQuat InQuat);

	static FVector ConvertUnrealVectorToXRVector(FVector InVector, float Scale);

};

inline FQuat FGSXRUtils::ConvertXRQuatToUnrealQuat(FQuat InQuat)
{
	return FQuat(-InQuat.Z, InQuat.X, InQuat.Y, -InQuat.W);
}

inline FVector FGSXRUtils::ConvertXRVectorToUnrealVector(FVector InVector, float Scale)
{
	return FVector(-InVector.Z * Scale, InVector.X * Scale, InVector.Y * Scale);
}

inline FQuat FGSXRUtils::ConvertUnrealQuatToXRQuat(FQuat InQuat)
{
	return  FQuat{ InQuat.Y, InQuat.Z, -InQuat.X, -InQuat.W };
}

inline FVector FGSXRUtils::ConvertUnrealVectorToXRVector(FVector InVector, float Scale)
{
	if (InVector.IsZero())
		return FVector{ 0.0f, 0.0f, 0.0f };

	return FVector{ InVector.Y / Scale, InVector.Z / Scale, -InVector.X / Scale };
}
