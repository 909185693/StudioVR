#include "GSXRSetting.h"

#include "NetworkMessage.h"


UGSXRSettings::UGSXRSettings(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	,ApplicatinonType(EApplicationType::ApplicationType_VR)
	,bEnableFoveated(false)
	,FoveationLevel(EFoveationLevel::EFoveationLevel_System)
	,Fov_Percentage_Up(0.2f)
	,Fov_Percentage_Down(0.2f)
	,Fov_Percentage_Left(0.2f)
	,Fov_Percentage_Right(0.2f)
	, Version("1.1.1")
	, Version_date("2021.11.3")
{
	
}

void UGSXRSettings::PostInitProperties()
{
	Super::PostInitProperties();
}

#if WITH_EDITOR
void UGSXRSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	Super::UpdateDefaultConfigFile();
}
#endif