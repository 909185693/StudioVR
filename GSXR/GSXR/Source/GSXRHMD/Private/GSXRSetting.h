#pragma once
#include "GSXRSetting.generated.h"

UENUM()
namespace EApplicationType
{
	enum Type 
	{
		ApplicationType_VR		UMETA(DisplayName = "VR"),
		ApplicationType_AR		UMETA(DisplayName = "AR"),
		ApplicationType_MR		UMETA(DisplayName = "MR"),
		Other					UMETA(DisplayName = "Other"),
	};
}

UENUM()
namespace EFoveationLevel
{
	enum Type 
	{
		EFoveationLevel_System		UMETA(DisplayName = "System"),
		EFoveationLevel_High		UMETA(DisplayName = "High"),
		EFoveationLevel_Medium		UMETA(DisplayName = "Medium"),
		EFoveationLevel_Low			UMETA(DisplayName = "Low"),
	};
}

UCLASS(config = Engine, defaultconfig)
class GSXRHMD_API UGSXRSettings	: public UObject
{
	GENERATED_UCLASS_BODY()
public:
	UPROPERTY(Config, EditAnywhere, Category = HMD, Meta = (DisplayName = "ApplicationType"))
	TEnumAsByte<EApplicationType::Type> ApplicatinonType;

	UPROPERTY(Config, EditAnywhere, Category = Foveated, Meta = (DisplayName = "Enable Foveation"))
	bool bEnableFoveated;
	
	UPROPERTY(Config, EditAnywhere, Category = Foveated, Meta = (EditCondition = "bEnableFoveated",DisplayName = "Foveation Level"))
	TEnumAsByte<EFoveationLevel::Type> FoveationLevel;
	
	UPROPERTY(Config, EditAnywhere, Category = Foveated, Meta = (EditCondition = "bEnableFoveated",DisplayName = "Up Fov Percentage"))
	float Fov_Percentage_Up;

	UPROPERTY(Config, EditAnywhere, Category = Foveated, Meta = (EditCondition = "bEnableFoveated",DisplayName = "Down Fov Percentage"))
	float Fov_Percentage_Down;
	
	UPROPERTY(Config, EditAnywhere, Category = Foveated, Meta = (EditCondition = "bEnableFoveated",DisplayName = "Left Fov Percentage"))
	float Fov_Percentage_Left;

	UPROPERTY(Config, EditAnywhere, Category = Foveated, Meta = (EditCondition = "bEnableFoveated",DisplayName = "Right Fov Percentage"))
	float Fov_Percentage_Right;

	UPROPERTY(Config, VisibleAnywhere, Category = Version, Meta = (EditCondition = "Version", DisplayName = "Version Name"))
		FString Version;
	UPROPERTY(Config, VisibleAnywhere, Category = Version, Meta = (EditCondition = "Version", DisplayName = "Version Date"))
		FString Version_date;
	
	virtual void PostInitProperties() override;
	
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif // WITH_EDITOR
};
