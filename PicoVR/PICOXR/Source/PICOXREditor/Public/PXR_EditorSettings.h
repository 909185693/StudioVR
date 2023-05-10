//Unreal® Engine, Copyright 1998 – 2023, Epic Games, Inc. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PXR_EditorSettings.generated.h"

/**
 * 
 */
UCLASS(config = Editor, defaultconfig)
class PICOXREDITOR_API UPICOXREditorSettings : public UObject
{
	GENERATED_UCLASS_BODY()
public:
	/** Whether or not to show the PICOXR Input settings toolbar button */
	UPROPERTY(config, EditAnywhere, Category = "PICOXR Editor Settings")
	bool bShowPICOXRInputToolbarButton;
};
