#include "GSXRInput.h"
#include "IXRTrackingSystem.h"
#include "CoreMinimal.h"
#include "Engine/Engine.h"
#include "Features/IModularFeatures.h"
#include "GameFramework/InputSettings.h"
#include "GenericPlatform/IInputInterface.h"
#include "HAL/FileManager.h"
#include "HAL/FileManagerGeneric.h"
#include "Misc/ConfigCacheIni.h"
#include "Misc/CoreDelegates.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Misc/TypeContainer.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"
#if PLATFORM_ANDROID
#include "AndroidRuntimeSettings.h"
#endif

#define LOCTEXT_NAMESPACE "GSXRInput"
DEFINE_LOG_CATEGORY_STATIC(GSXR_Unreal_Input, Log, All);

const FKey  FGSXRKey::GSXR_Joystick_Left_X_Click("GSXR_Joystick_Left_X_Click");
const FKey  FGSXRKey::GSXR_Joystick_Left_X_Touch("GSXR_Joystick_Left_X_Touch");
const FKey  FGSXRKey::GSXR_Joystick_Left_Y_Click("GSXR_Joystick_Left_Y_Click");
const FKey  FGSXRKey::GSXR_Joystick_Left_Y_Touch("GSXR_Joystick_Left_Y_Touch");
const FKey  FGSXRKey::GSXR_Joystick_Left_Menu_Click("GSXR_Joystick_Left_Menu_Click");
const FKey  FGSXRKey::GSXR_Joystick_Left_Grip_Click("GSXR_Joystick_Left_Grip_Click");
const FKey  FGSXRKey::GSXR_Joystick_Left_Trigger_Click("GSXR_Joystick_Left_Trigger_Click");
const FKey  FGSXRKey::GSXR_Joystick_Left_Trigger_Touch("GSXR_Joystick_Left_Trigger_Touch");
const FKey  FGSXRKey::GSXR_Joystick_Left_Thumbstick_Click("GSXR_Joystick_Left_Thumbstick_Click");
const FKey  FGSXRKey::GSXR_Joystick_Left_Thumbstick_Touch("GSXR_Joystick_Left_Thumbstick_Touch");
const FKey  FGSXRKey::GSXR_Joystick_Left_ThumbstickUp_Click("GSXR_Joystick_Left_ThumbstickUp_Click");
const FKey  FGSXRKey::GSXR_Joystick_Left_ThumbstickDown_Click("GSXR_Joystick_Left_ThumbstickDown_Click");
const FKey  FGSXRKey::GSXR_Joystick_Left_ThumbstickLeft_Click("GSXR_Joystick_Left_ThumbstickLeft_Click");
const FKey  FGSXRKey::GSXR_Joystick_Left_ThumbstickRight_Click("GSXR_Joystick_Left_ThumbstickRight_Click");

const FKey  FGSXRKey::GSXR_Joystick_Left_Grip_Axis("GSXR_Joystick_Left_Grip_Axis");
const FKey  FGSXRKey::GSXR_Joystick_Left_Trigger_Axis("GSXR_Joystick_Left_Trigger_Axis");
const FKey  FGSXRKey::GSXR_Joystick_Left_ThumbstickX_Axis("GSXR_Joystick_Left_ThumbstickX_Axis");
const FKey  FGSXRKey::GSXR_Joystick_Left_ThumbstickY_Axis("GSXR_Joystick_Left_ThumbstickY_Axis");

const FKey  FGSXRKey::GSXR_Joystick_Right_A_Click("GSXR_Joystick_Right_A_Click");
const FKey  FGSXRKey::GSXR_Joystick_Right_A_Touch("GSXR_Joystick_Right_A_Touch");
const FKey  FGSXRKey::GSXR_Joystick_Right_B_Click("GSXR_Joystick_Right_B_Click");
const FKey  FGSXRKey::GSXR_Joystick_Right_B_Touch("GSXR_Joystick_Right_B_Touch");
const FKey  FGSXRKey::GSXR_Joystick_Right_System_Click("GSXR_Joystick_Right_System_Click");
const FKey  FGSXRKey::GSXR_Joystick_Right_Grip_Click("GSXR_Joystick_Right_Grip_Click");
const FKey  FGSXRKey::GSXR_Joystick_Right_Trigger_Click("GSXR_Joystick_Right_Trigger_Click");
const FKey  FGSXRKey::GSXR_Joystick_Right_Trigger_Touch("GSXR_Joystick_Right_Trigger_Touch");
const FKey  FGSXRKey::GSXR_Joystick_Right_Thumbstick_Click("GSXR_Joystick_Right_Thumbstick_Click");
const FKey  FGSXRKey::GSXR_Joystick_Right_Thumbstick_Touch("GSXR_Joystick_Right_Thumbstick_Touch");
const FKey  FGSXRKey::GSXR_Joystick_Right_ThumbstickUp_Click("GSXR_Joystick_Right_ThumbstickUp_Click");
const FKey  FGSXRKey::GSXR_Joystick_Right_ThumbstickDown_Click("GSXR_Joystick_Right_ThumbstickDown_Click");
const FKey  FGSXRKey::GSXR_Joystick_Right_ThumbstickLeft_Click("GSXR_Joystick_Right_ThumbstickLeft_Click");
const FKey  FGSXRKey::GSXR_Joystick_Right_ThumbstickRight_Click("GSXR_Joystick_Right_ThumbstickRight_Click");

const FKey  FGSXRKey::GSXR_Joystick_Right_Grip_Axis("GSXR_Joystick_Right_Grip_Axis");
const FKey  FGSXRKey::GSXR_Joystick_Right_Trigger_Axis("GSXR_Joystick_Right_Trigger_Axis");
const FKey  FGSXRKey::GSXR_Joystick_Right_ThumbstickX_Axis("GSXR_Joystick_Right_ThumbstickX_Axis");
const FKey  FGSXRKey::GSXR_Joystick_Right_ThumbstickY_Axis("GSXR_Joystick_Right_ThumbstickY_Axis");

const FKey  FGSXRKey::GSXR_Touch_Left_Trackpad_Click("GSXR_Touch_Left_Trackpad_Click");
const FKey  FGSXRKey::GSXR_Touch_Left_Trackpad_Touch("GSXR_Touch_Left_Trackpad_Touch");
const FKey  FGSXRKey::GSXR_Touch_Left_Trigger_Click("GSXR_Touch_Left_Trigger_Click");
const FKey  FGSXRKey::GSXR_Touch_Left_Grip_Click("GSXR_Touch_Left_Grip_Click");
const FKey  FGSXRKey::GSXR_Touch_Left_Menu_Click("GSXR_Touch_Left_Menu_Click");
const FKey  FGSXRKey::GSXR_Touch_Left_System_Click("GSXR_Touch_Left_System_Click");
const FKey  FGSXRKey::GSXR_Touch_Left_TrackpadUp_Click("GSXR_Touch_Left_TrackpadUp_Click");
const FKey  FGSXRKey::GSXR_Touch_Left_TrackpadDown_Click("GSXR_Touch_Left_TrackpadDown_Click");
const FKey  FGSXRKey::GSXR_Touch_Left_TrackpadLeft_Click("GSXR_Touch_Left_TrackpadLeft_Click");
const FKey  FGSXRKey::GSXR_Touch_Left_TrackpadRight_Click("GSXR_Touch_Left_TrackpadRight_Click");

const FKey  FGSXRKey::GSXR_Touch_Left_TrackpadX_Axis("GSXR_Touch_Left_TrackpadX_Axis");
const FKey  FGSXRKey::GSXR_Touch_Left_TrackpadY_Axis("GSXR_Touch_Left_TrackpadY_Axis");
const FKey  FGSXRKey::GSXR_Touch_Left_Trigger_Axis("GSXR_Touch_Left_Trigger_Axis");


const FKey  FGSXRKey::GSXR_Touch_Right_Trackpad_Click("GSXR_Touch_Right_Trackpad_Click");
const FKey  FGSXRKey::GSXR_Touch_Right_Trackpad_Touch("GSXR_Touch_Right_Trackpad_Touch");
const FKey  FGSXRKey::GSXR_Touch_Right_Trigger_Click("GSXR_Touch_Right_Trigger_Click");
const FKey  FGSXRKey::GSXR_Touch_Right_Grip_Click("GSXR_Touch_Right_Grip_Click");
const FKey  FGSXRKey::GSXR_Touch_Right_Menu_Click("GSXR_Touch_Right_Menu_Click");
const FKey  FGSXRKey::GSXR_Touch_Right_System_Click("GSXR_Touch_Right_System_Click");
const FKey  FGSXRKey::GSXR_Touch_Right_TrackpadUp_Click("GSXR_Touch_Right_TrackpadUp_Click");
const FKey  FGSXRKey::GSXR_Touch_Right_TrackpadDown_Click("GSXR_Touch_Right_TrackpadDown_Click");
const FKey  FGSXRKey::GSXR_Touch_Right_TrackpadLeft_Click("GSXR_Touch_Right_TrackpadLeft_Click");
const FKey  FGSXRKey::GSXR_Touch_Right_TrackpadRight_Click("GSXR_Touch_Right_TrackpadRight_Click");
const FKey  FGSXRKey::GSXR_Touch_Right_TrackpadX_Axis("GSXR_Touch_Right_TrackpadX_Axis");
const FKey  FGSXRKey::GSXR_Touch_Right_TrackpadY_Axis("GSXR_Touch_Right_TrackpadY_Axis");
const FKey  FGSXRKey::GSXR_Touch_Right_Trigger_Axis("GSXR_Touch_Right_Trigger_Axis");

//GSXR GamePad
const FKey  FGSXRKey::GSXR_GamePad_A_Click("GSXR_GamePad_A_Click");
const FKey  FGSXRKey::GSXR_GamePad_B_Click("GSXR_GamePad_B_Click");
const FKey  FGSXRKey::GSXR_GamePad_Back_Click("GSXR_GamePad_Back_Click");
const FKey  FGSXRKey::GSXR_GamePad_DpadDown_Click("GSXR_GamePad_DpadDown_Click");
const FKey  FGSXRKey::GSXR_GamePad_DpadLeft_Click("GSXR_GamePad_DpadLeft_Click");
const FKey  FGSXRKey::GSXR_GamePad_DpadRight_Click("GSXR_GamePad_DpadRight_Click");
const FKey  FGSXRKey::GSXR_GamePad_DpadUp_Click("GSXR_GamePad_DpadUp_Click");
const FKey  FGSXRKey::GSXR_GamePad_LeftThumbstick_Click("GSXR_GamePad_LeftThumbstick_Click");
const FKey  FGSXRKey::GSXR_GamePad_RightThumbstick_Click("GSXR_GamePad_RightThumbstick_Click");
const FKey  FGSXRKey::GSXR_GamePad_LeftShoulder_Click("GSXR_GamePad_LeftShoulder_Click");
const FKey  FGSXRKey::GSXR_GamePad_RightShoulder_Click("GSXR_GamePad_RightShoulder_Click");
const FKey  FGSXRKey::GSXR_GamePad_X_Click("GSXR_GamePad_X_Click");
const FKey  FGSXRKey::GSXR_GamePad_Y_Click("GSXR_GamePad_Y_Click");
const FKey  FGSXRKey::GSXR_GamePad_Start_Click("GSXR_GamePad_Start_Click");
const FKey  FGSXRKey::GSXR_GamePad_LeftTrigger_Click("GSXR_GamePad_LeftTrigger_Click");
const FKey  FGSXRKey::GSXR_GamePad_RightTrigger_Click("GSXR_GamePad_RightTrigger_Click");

const FKey  FGSXRKey::GSXR_GamePad_TriggerLeft_Axis("GSXR_GamePad_TriggerLeft_Axis");
const FKey  FGSXRKey::GSXR_GamePad_TriggerRight_Axis("GSXR_GamePad_TriggerRight_Axis");
const FKey  FGSXRKey::GSXR_GamePad_LeftThumbstickDown_Click("GSXR_GamePad_LeftThumbstickDown_Click");
const FKey  FGSXRKey::GSXR_GamePad_LeftThumbstickLeft_Click("GSXR_GamePad_LeftThumbstickLeft_Click");
const FKey  FGSXRKey::GSXR_GamePad_LeftThumbstickRight_Click("GSXR_GamePad_LeftThumbstickRight_Click");
const FKey  FGSXRKey::GSXR_GamePad_LeftThumbstickUp_Click("GSXR_GamePad_LeftThumbstickUp_Click");
const FKey  FGSXRKey::GSXR_GamePad_RightThumbstickDown_Click("GSXR_GamePad_RightThumbstickDown_Click");
const FKey  FGSXRKey::GSXR_GamePad_RightThumbstickRight_Click("GSXR_GamePad_RightThumbstickRight_Click");
const FKey  FGSXRKey::GSXR_GamePad_RightThumbstickLeft_Click("GSXR_GamePad_RightThumbstickLeft_Click");
const FKey  FGSXRKey::GSXR_GamePad_RightThumbstickUp_Click("GSXR_GamePad_RightThumbstickUp_Click");
const FKey  FGSXRKey::GSXR_GamePad_LeftThumbstickX_Axis("GSXR_GamePad_LeftThumbstickX_Axis");
const FKey  FGSXRKey::GSXR_GamePad_LeftThumbstickY_Axis("GSXR_GamePad_LeftThumbstickY_Axis");
const FKey  FGSXRKey::GSXR_GamePad_RightThumbstickX_Axis("GSXR_GamePad_RightThumbstickX_Axis");
const FKey  FGSXRKey::GSXR_GamePad_RightThumbstickY_Axis("GSXR_GamePad_RightThumbstickY_Axis");

const FKey  FGSXRKey::GSXR_HMD_Enter_Click("GSXR_HMD_Enter_Click");
const FKey  FGSXRKey::GSXR_HMD_Back_Click("GSXR_HMD_Back_Click");
const FKey  FGSXRKey::GSXR_HMD_VolumeUp_Click("GSXR_HMD_VolumeUp_Click");
const FKey  FGSXRKey::GSXR_HMD_VolumeDown_Click("GSXR_HMD_VolumeDown_Click");
const FKey  FGSXRKey::GSXR_HMD_Home_Click("GSXR_HMD_Home_Click");


const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_Joystick_Left_X_Click(" GSXR_Joystick_Left_X_Click ");
const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_Joystick_Left_X_Touch(" GSXR_Joystick_Left_X_Touch ");
const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_Joystick_Left_Y_Click(" GSXR_Joystick_Left_Y_Click ");
const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_Joystick_Left_Y_Touch(" GSXR_Joystick_Left_Y_Touch ");
const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_Joystick_Left_Menu_Click(" GSXR_Joystick_Left_Menu_Click ");
const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_Joystick_Left_Grip_Click(" GSXR_Joystick_Left_Grip_Click ");
const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_Joystick_Left_Trigger_Click(" GSXR_Joystick_Left_Trigger_Click ");
const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_Joystick_Left_Trigger_Touch(" GSXR_Joystick_Left_Trigger_Touch ");
const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_Joystick_Left_ThumbstickUp_Click(" GSXR_Joystick_Left_ThumbstickUp_Click ");
const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_Joystick_Left_Thumbstick_Click(" GSXR_Joystick_Left_Thumbstick_Click ");
const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_Joystick_Left_Thumbstick_Touch(" GSXR_Joystick_Left_Thumbstick_Touch ");
const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_Joystick_Left_ThumbstickDown_Click(" GSXR_Joystick_Left_ThumbstickDown_Click ");
const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_Joystick_Left_ThumbstickLeft_Click(" GSXR_Joystick_Left_ThumbstickLeft_Click ");
const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_Joystick_Left_ThumbstickRight_Click(" GSXR_Joystick_Left_ThumbstickRight_Click ");

const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_Joystick_Left_Grip_Axis(" GSXR_Joystick_Left_Grip_Axis ");
const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_Joystick_Left_Trigger_Axis(" GSXR_Joystick_Left_Trigger_Axis ");
const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_Joystick_Left_ThumbstickX_Axis(" GSXR_Joystick_Left_ThumbstickX_Axis ");
const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_Joystick_Left_ThumbstickY_Axis(" GSXR_Joystick_Left_ThumbstickY_Axis ");

const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_Joystick_Right_A_Click(" GSXR_Joystick_Right_A_Click ");
const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_Joystick_Right_A_Touch(" GSXR_Joystick_Right_A_Touch ");
const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_Joystick_Right_B_Click(" GSXR_Joystick_Right_B_Click ");
const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_Joystick_Right_B_Touch(" GSXR_Joystick_Right_B_Touch ");
const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_Joystick_Right_System_Click(" GSXR_Joystick_Right_System_Click ");
const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_Joystick_Right_Grip_Click(" GSXR_Joystick_Right_Grip_Click ");
const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_Joystick_Right_Trigger_Click(" GSXR_Joystick_Right_Trigger_Click ");
const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_Joystick_Right_Trigger_Touch(" GSXR_Joystick_Right_Trigger_Touch ");
const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_Joystick_Right_Thumbstick_Click(" GSXR_Joystick_Right_Thumbstick_Click ");
const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_Joystick_Right_Thumbstick_Touch(" GSXR_Joystick_Right_Thumbstick_Touch ");
const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_Joystick_Right_ThumbstickUp_Click(" GSXR_Joystick_Right_ThumbstickUp_Click ");
const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_Joystick_Right_ThumbstickDown_Click(" GSXR_Joystick_Right_ThumbstickDown_Click ");
const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_Joystick_Right_ThumbstickLeft_Click(" GSXR_Joystick_Right_ThumbstickLeft_Click ");
const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_Joystick_Right_ThumbstickRight_Click(" GSXR_Joystick_Right_ThumbstickRight_Click ");

const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_Joystick_Right_Grip_Axis(" GSXR_Joystick_Right_Grip_Axis ");
const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_Joystick_Right_Trigger_Axis(" GSXR_Joystick_Right_Trigger_Axis ");
const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_Joystick_Right_ThumbstickX_Axis(" GSXR_Joystick_Right_ThumbstickX_Axis ");
const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_Joystick_Right_ThumbstickY_Axis(" GSXR_Joystick_Right_ThumbstickY_Axis ");

const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_Touch_Left_Trackpad_Click("GSXR_Touch_Left_Trackpad_Click");
const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_Touch_Left_Trackpad_Touch("GSXR_Touch_Left_Trackpad_Touch");
const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_Touch_Left_Trigger_Click("GSXR_Touch_Left_Trigger_Click");
const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_Touch_Left_Grip_Click("GSXR_Touch_Left_Grip_Click");
const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_Touch_Left_Menu_Click("GSXR_Touch_Left_Menu_Click");
const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_Touch_Left_System_Click("GSXR_Touch_Left_System_Click");
const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_Touch_Left_TrackpadX_Axis("GSXR_Touch_Left_TrackpadX_Axis");
const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_Touch_Left_TrackpadY_Axis("GSXR_Touch_Left_TrackpadY_Axis");
const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_Touch_Left_Trigger_Axis("GSXR_Touch_Left_Trigger_Axis");


const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_Touch_Right_Trackpad_Click("GSXR_Touch_Right_Trackpad_Click");
const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_Touch_Right_Trackpad_Touch("GSXR_Touch_Right_Trackpad_Touch");
const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_Touch_Right_Trigger_Click("GSXR_Touch_Right_Trigger_Click");
const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_Touch_Right_Grip_Click("GSXR_Touch_Right_Grip_Click");
const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_Touch_Right_Menu_Click("GSXR_Touch_Right_Menu_Click");
const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_Touch_Right_System_Click("GSXR_Touch_Right_System_Click");
const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_Touch_Right_TrackpadX_Axis("GSXR_Touch_Right_TrackpadX_Axis");
const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_Touch_Right_TrackpadY_Axis("GSXR_Touch_Right_TrackpadY_Axis");
const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_Touch_Right_Trigger_Axis("GSXR_Touch_Right_Trigger_Axis");

//GSXR GamePad
const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_GamePad_A_Click("GSXR_GamePad_A_Click");
const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_GamePad_B_Click("GSXR_GamePad_B_Click");
const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_GamePad_Back_Click("GSXR_GamePad_Back_Click");
const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_GamePad_DpadDown_Click("GSXR_GamePad_DpadDown_Click");
const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_GamePad_DpadLeft_Click("GSXR_GamePad_DpadLeft_Click");
const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_GamePad_DpadRight_Click("GSXR_GamePad_DpadRight_Click");
const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_GamePad_DpadUp_Click("GSXR_GamePad_DpadUp_Click");
const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_GamePad_LeftThumbstick_Click("GSXR_GamePad_LeftThumbstick_Click");
const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_GamePad_RightThumbstick_Click("GSXR_GamePad_RightThumbstick_Click");
const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_GamePad_LeftShoulder_Click("GSXR_GamePad_LeftShoulder_Click");
const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_GamePad_RightShoulder_Click("GSXR_GamePad_RightShoulder_Click");
const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_GamePad_X_Click("GSXR_GamePad_X_Click");
const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_GamePad_Y_Click("GSXR_GamePad_Y_Click");
const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_GamePad_Start_Click("GSXR_GamePad_Start_Click");
const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_GamePad_LeftTrigger_Click("GSXR_GamePad_LeftTrigger_Click");
const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_GamePad_RightTrigger_Click("GSXR_GamePad_RightTrigger_Click");

const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_GamePad_TriggerLeft_Axis("GSXR_GamePad_TriggerLeft_Axis");
const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_GamePad_TriggerRight_Axis("GSXR_GamePad_TriggerRight_Axis");
const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_GamePad_LeftThumbstickDown_Click("GSXR_GamePad_LeftThumbstickDown_Click");
const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_GamePad_LeftThumbstickLeft_Click("GSXR_GamePad_LeftThumbstickLeft_Click");
const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_GamePad_LeftThumbstickRight_Click("GSXR_GamePad_LeftThumbstickRight_Click");
const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_GamePad_LeftThumbstickUp_Click("GSXR_GamePad_LeftThumbstickUp_Click");
const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_GamePad_RightThumbstickDown_Click("GSXR_GamePad_RightThumbstickDown_Click");
const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_GamePad_RightThumbstickRight_Click("GSXR_GamePad_RightThumbstickRight_Click");
const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_GamePad_RightThumbstickLeft_Click("GSXR_GamePad_RightThumbstickLeft_Click");
const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_GamePad_RightThumbstickUp_Click("GSXR_GamePad_RightThumbstickUp_Click");
const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_GamePad_LeftThumbstickX_Axis("GSXR_GamePad_LeftThumbstickX_Axis");
const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_GamePad_LeftThumbstickY_Axis("GSXR_GamePad_LeftThumbstickY_Axis");
const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_GamePad_RightThumbstickX_Axis("GSXR_GamePad_RightThumbstickX_Axis");
const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_GamePad_RightThumbstickY_Axis("GSXR_GamePad_RightThumbstickY_Axis");

const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_HMD_Enter_Click("GSXR_HMD_Enter_Click");
const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_HMD_Back_Click("GSXR_HMD_Back_Click");
const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_HMD_VolumeUp_Click("GSXR_HMD_VolumeUp_Click");
const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_HMD_VolumeDown_Click("GSXR_HMD_VolumeDown_Click");
const FGSXRKeyNames::Type  FGSXRKeyNames::GSXR_HMD_Home_Click("GSXR_HMD_Home_Click");

FGSXRInput::FGSXRInput(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler)
	:MessageHandler(InMessageHandler)
{
	IModularFeatures::Get().RegisterModularFeature(GetModularFeatureName(), this);
	AddKeyToEngine();
	const UInputSettings* EngineInputSettings = GetDefault<UInputSettings>();
	Gsxr_Instance = UGSXRInstance::GetInstance();
	FGSXRActionSet ActionSet;
	ActionSet.ActionSetName = "/actions/default";
	ActionSet.Usage = "leftright";
	ActionSets.Add(ActionSet);
	GenerateManifestFile(EngineInputSettings);
	GenerateActionMappings(EngineInputSettings);
	GenerateActionBinding(EngineInputSettings, "JoyStick");
	GenerateActionBinding(EngineInputSettings, "GamePad");
	GenerateActionBinding(EngineInputSettings, "Touch");

#if USE_GSXRAPI
	Gsxr_Instance->SetXrActionManifestPath(GetManifestPath().Replace(TEXT("../"), TEXT("")) / GSXR_INPUT_CONFIG_MANIFEST);

	for (auto& ActionSetInner : ActionSets)
	{
		Gsxr_Instance->GetXrActionSetHandle(ActionSetInner.ActionSetName, ActionSetInner.ActionSetHandle);
	}

	for (auto& Action : ActionEvents)
	{
		Gsxr_Instance->GetXrActionHandle(Action.ActionPath, Action.ActionHandle);
	}
	GSXR_XrInputValueHandle_t LeftValueHandle;
	GSXR_XrInputValueHandle_t RightValueHandle;
	GSXR_XrInputValueHandle_t GamePadInputHandle;
	Gsxr_Instance->GetXrInputSourceHandle("/user/hand/left", LeftValueHandle);
	Gsxr_Instance->GetXrInputSourceHandle("/user/hand/right", RightValueHandle);
	Gsxr_Instance->GetXrInputSourceHandle("/user/gamepad", GamePadInputHandle);
	HMDButtons.Empty();
	HMDButtons.Add(FGSXRHMDButtonState(FGSXRKeyNames::GSXR_HMD_Enter_Click, GSXR_Hmd_InputId::GSXR_Hmd_InputId_Enter));
	HMDButtons.Add(FGSXRHMDButtonState(FGSXRKeyNames::GSXR_HMD_Back_Click, GSXR_Hmd_InputId::GSXR_Hmd_InputId_Back));
	HMDButtons.Add(FGSXRHMDButtonState(FGSXRKeyNames::GSXR_HMD_VolumeUp_Click, GSXR_Hmd_InputId::GSXR_Hmd_InputId_Volume_Up));
	HMDButtons.Add(FGSXRHMDButtonState(FGSXRKeyNames::GSXR_HMD_VolumeDown_Click, GSXR_Hmd_InputId::GSXR_Hmd_InputId_Volume_Down));
	HMDButtons.Add(FGSXRHMDButtonState(FGSXRKeyNames::GSXR_HMD_Home_Click, GSXR_Hmd_InputId::GSXR_Hmd_InputId_Home));
#endif
	FCoreDelegates::ApplicationWillEnterBackgroundDelegate.AddRaw(this, &FGSXRInput::ApplicationPauseDelegate);
	FCoreDelegates::ApplicationHasEnteredForegroundDelegate.AddRaw(this, &FGSXRInput::ApplicationResumeDelegate);
	UpdateConnectState();
}

FGSXRInput::~FGSXRInput()
{
	IModularFeatures::Get().UnregisterModularFeature(GetModularFeatureName(), this);
}

void FGSXRInput::Tick(float DeltaTime)
{
}

void FGSXRInput::SendControllerEvents()
{
	ProcessActionEvents();
	ProcessHMDButtonEvents();
}

void FGSXRInput::SetMessageHandler(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler)
{
	MessageHandler = InMessageHandler;
}

bool FGSXRInput::Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar)
{
	return false;
}

void FGSXRInput::SetChannelValue(int32 ControllerId, FForceFeedbackChannelType ChannelType, float Value)
{
	if (ChannelType != FForceFeedbackChannelType::LEFT_LARGE && ChannelType != FForceFeedbackChannelType::RIGHT_LARGE)
	{
		return;
	}
	const int32 Hand = (ChannelType == FForceFeedbackChannelType::LEFT_LARGE) ? 0 : 1;
#if USE_GSXRAPI
	Gsxr_Instance->TriggerVibration(Hand, true, 50, 100, Value > 1 ? 1 : Value);
#endif

}

void FGSXRInput::SetChannelValues(int32 ControllerId, const FForceFeedbackValues& values)
{
#if USE_GSXRAPI
	if (values.LeftLarge > 0)
	{
		Gsxr_Instance->TriggerVibration(0, true, 50, 100.f, (values.LeftLarge) > 1 ? 1 : values.LeftLarge);
	}
	if (values.RightLarge > 0)
	{
		Gsxr_Instance->TriggerVibration(1, true, 50, 100.f, (values.LeftLarge) > 1 ? 1 : values.RightLarge);
	}
#endif
}

FName FGSXRInput::GetMotionControllerDeviceTypeName() const
{
	return FName(TEXT("GSXRInput"));
}

bool FGSXRInput::GetControllerOrientationAndPosition(const int32 ControllerIndex, const EControllerHand DeviceHand,
	FRotator& OutOrientation, FVector& OutPosition, float WorldToMetersScale) const
{
#if USE_GSXRAPI
	if (bLeftControllerState && DeviceHand == EControllerHand::Left)
	{
		Gsxr_Instance->GetPoseState(GSXR_DeviceType_Controller_Left, WorldToMetersScale, OutPosition, OutOrientation);
		return true;
	}
	else if (bRightControllerState && DeviceHand == EControllerHand::Right)
	{
		Gsxr_Instance->GetPoseState(GSXR_DeviceType_Controller_Right, WorldToMetersScale, OutPosition, OutOrientation);
		return true;
	}
#endif
	return false;
}

ETrackingStatus FGSXRInput::GetControllerTrackingStatus(const int32 ControllerIndex,
	const EControllerHand DeviceHand) const
{
	if (ControllerIndex == 0 && (DeviceHand == EControllerHand::Left || DeviceHand == EControllerHand::Right || DeviceHand == EControllerHand::AnyHand))
	{
		return ETrackingStatus::Tracked;
	}

	return ETrackingStatus::NotTracked;
}

void FGSXRInput::SetHapticFeedbackValues(int32 ControllerId, int32 Hand, const FHapticFeedbackValues& Values)
{
	if (Hand != (int32)EControllerHand::Left && Hand != (int32)EControllerHand::Right)
	{
		return;
	}
#if USE_GSXRAPI
	float Intensity = Values.Amplitude > 1 ? 1 : Values.Amplitude;
	Gsxr_Instance->TriggerVibration(Hand, true, 50, (Values.Frequency > 0.0f) ? Values.Frequency * 100 : 0.0f, Intensity);
#endif
}

void FGSXRInput::GetHapticFrequencyRange(float& MinFrequency, float& MaxFrequency) const
{
	MinFrequency = 0.0f;
	MaxFrequency = 1.0f;
}

float FGSXRInput::GetHapticAmplitudeScale() const
{
	return 1.f;
}

extern FString GExternalFilePath;
FString FGSXRInput::GetManifestPath()
{
	FString ManifestPath = FPaths::ProjectConfigDir() / GSXR_INPUT_CONFIG_PATH;
	FString RootPath = "";
#if PLATFORM_ANDROID
	TArray<FString> Folders;
	GExternalFilePath.ParseIntoArray(Folders, TEXT("/"));
	for (FString Folder : Folders)
	{
		RootPath += FString("/..");
	}
	auto AndroidRuntimeSettings = GetDefault<UAndroidRuntimeSettings>();
	ManifestPath = FPaths::Combine(RootPath, TEXT("/sdcard/Android/data/"), AndroidRuntimeSettings->PackageName, TEXT("files"), TEXT(GSXR_INPUT_CONFIG_PATH));
#endif
	return ManifestPath;
}

void FGSXRInput::OnDeviceStateChange()
{
	UpdateConnectState();
}

void FGSXRInput::ApplicationPauseDelegate()
{

}

void FGSXRInput::ApplicationResumeDelegate()
{
	UpdateConnectState();
}

void FGSXRInput::UpdateConnectState()
{
#if USE_GSXRAPI
	Gsxr_Instance->GetDeviceConnectState(GSXR_DeviceType_Controller_Left, bLeftControllerState);
	Gsxr_Instance->GetDeviceConnectState(GSXR_DeviceType_Controller_Right, bRightControllerState);
#endif
}

#if ENGINE_MINOR_VERSION >25
#define FloatAxis Axis1D
#else
#define FloatAxis FloatAxis
#endif
void FGSXRInput::AddKeyToEngine()
{
	EKeys::AddMenuCategoryDisplayInfo("GSXRJoystick", LOCTEXT("GSXRJoystickSubCategory", "GSXR Joystic"), TEXT("GraphEditor.PadEvent_16x"));

	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_Joystick_Left_X_Click, LOCTEXT("GSXR_Joystick_Left_X_Click", "GSXRJoystick(L) X Click"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "GSXRJoystick"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_Joystick_Left_X_Touch, LOCTEXT("GSXR_Joystick_Left_X_Touch", "GSXRJoystick(L) X Touch"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "GSXRJoystick"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_Joystick_Left_Y_Click, LOCTEXT("GSXR_Joystick_Left_Y_Click", "GSXRJoystick(L) Y Click"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "GSXRJoystick"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_Joystick_Left_Y_Touch, LOCTEXT("GSXR_Joystick_Left_Y_Touch", "GSXRJoystick(L) Y Touch"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "GSXRJoystick"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_Joystick_Left_Grip_Click, LOCTEXT("GSXR_Joystick_Left_Grip_Click", "GSXRJoystick(L) Grip Click"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "GSXRJoystick"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_Joystick_Left_Menu_Click, LOCTEXT("GSXR_Joystick_Left_Menu_Click", "GSXRJoystick(L) Menu Click"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "GSXRJoystick"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_Joystick_Left_Trigger_Touch, LOCTEXT("GSXR_Joystick_Left_Trigger_Touch", "GSXRJoystick(L) Trigger Touch"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "GSXRJoystick"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_Joystick_Left_Trigger_Click, LOCTEXT("GSXR_Joystick_Left_Trigger_Click", "GSXRJoystick(L) Trigger Click"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "GSXRJoystick"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_Joystick_Left_Thumbstick_Click, LOCTEXT("GSXR_Joystick_Left_Thumbstick_Click", "GSXRJoystick(L) Thumbstick Click"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "GSXRJoystick"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_Joystick_Left_Thumbstick_Touch, LOCTEXT("GSXR_Joystick_Left_Thumbstick_Touch", "GSXRJoystick(L) Thumbstick Touch"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "GSXRJoystick"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_Joystick_Left_ThumbstickUp_Click, LOCTEXT("GSXR_Joystick_Left_ThumbstickUp_Click", "GSXRJoystick(L) ThumbstickUp Click"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "GSXRJoystick"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_Joystick_Left_ThumbstickDown_Click, LOCTEXT("GSXR_Joystick_Left_ThumbstickDown_Click", "GSXRJoystick(L) ThumbstickDown Click"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "GSXRJoystick"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_Joystick_Left_ThumbstickLeft_Click, LOCTEXT("GSXR_Joystick_Left_ThumbstickLeft_Click", "GSXRJoystick(L) ThumbstickLeft Click"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "GSXRJoystick"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_Joystick_Left_ThumbstickRight_Click, LOCTEXT("GSXR_Joystick_Left_ThumbstickRight_Click", "GSXRJoystick(L) ThumbstickRight Click"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "GSXRJoystick"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_Joystick_Left_Grip_Axis, LOCTEXT("GSXR_Joystick_Left_Grip_Axis", "GSXRJoystick(L) Grip Axis"), FKeyDetails::GamepadKey | FKeyDetails::FloatAxis | FKeyDetails::NotBlueprintBindableKey, "GSXRJoystick"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_Joystick_Left_Trigger_Axis, LOCTEXT("GSXR_Joystick_Left_Trigger_Axis", "GSXRJoystick(L) Trigger Axis"), FKeyDetails::GamepadKey | FKeyDetails::FloatAxis | FKeyDetails::NotBlueprintBindableKey, "GSXRJoystick"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_Joystick_Left_ThumbstickX_Axis, LOCTEXT("GSXR_Joystick_Left_ThumbstickX_Axis", "GSXRJoystick(L) ThumbstickX Axis"), FKeyDetails::GamepadKey | FKeyDetails::FloatAxis | FKeyDetails::NotBlueprintBindableKey, "GSXRJoystick"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_Joystick_Left_ThumbstickY_Axis, LOCTEXT("GSXR_Joystick_Left_ThumbstickY_Axis", "GSXRJoystick(L) ThumbstickY Axis"), FKeyDetails::GamepadKey | FKeyDetails::FloatAxis | FKeyDetails::NotBlueprintBindableKey, "GSXRJoystick"));


	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_Joystick_Right_A_Click, LOCTEXT("GSXR_Joystick_Right_A_Click", "GSXRJoystick(R) A Click"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "GSXRJoystick"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_Joystick_Right_A_Touch, LOCTEXT("GSXR_Joystick_Right_A_Touch", "GSXRJoystick(R) A Touch"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "GSXRJoystick"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_Joystick_Right_B_Click, LOCTEXT("GSXR_Joystick_Right_B_Click", "GSXRJoystick(R) B Click"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "GSXRJoystick"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_Joystick_Right_B_Touch, LOCTEXT("GSXR_Joystick_Right_B_Touch", "GSXRJoystick(R) B Touch"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "GSXRJoystick"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_Joystick_Right_Grip_Click, LOCTEXT("GSXR_Joystick_Right_Grip_Click", "GSXRJoystick(R) Grip Click"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "GSXRJoystick"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_Joystick_Right_System_Click, LOCTEXT("GSXR_Joystick_Right_Menu_Click", "GSXRJoystick(R) System Click"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "GSXRJoystick"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_Joystick_Right_Trigger_Touch, LOCTEXT("GSXR_Joystick_Right_Trigger_Touch", "GSXRJoystick(R) Trigger Touch"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "GSXRJoystick"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_Joystick_Right_Trigger_Click, LOCTEXT("GSXR_Joystick_Right_Trigger_Click", "GSXRJoystick(R) Trigger Click"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "GSXRJoystick"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_Joystick_Right_Thumbstick_Click, LOCTEXT("GSXR_Joystick_Right_Thumbstick_Click", "GSXRJoystick(R) Thumbstick Click"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "GSXRJoystick"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_Joystick_Right_Thumbstick_Touch, LOCTEXT("GSXR_Joystick_Right_Thumbstick_Touch", "GSXRJoystick(R) Thumbstick Touch"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "GSXRJoystick"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_Joystick_Right_ThumbstickUp_Click, LOCTEXT("GSXR_Joystick_Right_ThumbstickUp_Click", "GSXRJoystick(R) ThumbstickUp Click"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "GSXRJoystick"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_Joystick_Right_ThumbstickDown_Click, LOCTEXT("GSXR_Joystick_Right_ThumbstickDown_Click", "GSXRJoystick(R) ThumbstickDown Click"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "GSXRJoystick"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_Joystick_Right_ThumbstickLeft_Click, LOCTEXT("GSXR_Joystick_Right_ThumbstickLeft_Click", "GSXRJoystick(R) ThumbstickLeft Click"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "GSXRJoystick"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_Joystick_Right_ThumbstickRight_Click, LOCTEXT("GSXR_Joystick_Right_ThumbstickRight_Click", "GSXRJoystick(R) ThumbstickRight Click"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "GSXRJoystick"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_Joystick_Right_Grip_Axis, LOCTEXT("GSXR_Joystick_Right_Grip_Axis", "GSXRJoystick(R) Grip Axis"), FKeyDetails::GamepadKey | FKeyDetails::FloatAxis | FKeyDetails::NotBlueprintBindableKey, "GSXRJoystick"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_Joystick_Right_Trigger_Axis, LOCTEXT("GSXR_Joystick_Right_Trigger_Axis", "GSXRJoystick(R) Trigger Axis"), FKeyDetails::GamepadKey | FKeyDetails::FloatAxis | FKeyDetails::NotBlueprintBindableKey, "GSXRJoystick"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_Joystick_Right_ThumbstickX_Axis, LOCTEXT("GSXR_Joystick_Right_ThumbstickX_Axis", "GSXRJoystick(R) ThumbstickX Axis"), FKeyDetails::GamepadKey | FKeyDetails::FloatAxis | FKeyDetails::NotBlueprintBindableKey, "GSXRJoystick"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_Joystick_Right_ThumbstickY_Axis, LOCTEXT("GSXR_Joystick_Right_ThumbstickY_Axis", "GSXRJoystick(R) ThumbstickY Axis"), FKeyDetails::GamepadKey | FKeyDetails::FloatAxis | FKeyDetails::NotBlueprintBindableKey, "GSXRJoystick"));

	EKeys::AddMenuCategoryDisplayInfo("GSXRTouch", LOCTEXT("GSXRTouchSubCategory", "GSXR Touch"), TEXT("GraphEditor.PadEvent_16x"));

	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_Touch_Left_Trackpad_Click, LOCTEXT("GSXR_Touch_Left_Trackpad_Click", "GSXRTouch(L) Trackpad Click"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "GSXRTouch"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_Touch_Left_Trackpad_Touch, LOCTEXT("GSXR_Touch_Left_Trackpad_Touch", "GSXRTouch(L) Trackpad Touch"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "GSXRTouch"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_Touch_Left_Trigger_Click, LOCTEXT("GSXR_Touch_Left_Trigger_Click", "GSXRTouch(L) Trigger Click"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "GSXRTouch"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_Touch_Left_Grip_Click, LOCTEXT("GSXR_Touch_Left_Grip_Click", "GSXRTouch(L) Grip Click"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "GSXRTouch"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_Touch_Left_Menu_Click, LOCTEXT("GSXR_Touch_Left_Menu_Click", "GSXRTouch(L) Menu Click"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "GSXRTouch"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_Touch_Left_System_Click, LOCTEXT("GSXR_Touch_Left_System_Click", "GSXRTouch(L) System Click"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "GSXRTouch"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_Touch_Left_TrackpadX_Axis, LOCTEXT("GSXR_Touch_Left_TrackpadX_Axis", "GSXRTouch(L) TrackpadX Axis"), FKeyDetails::GamepadKey | FKeyDetails::FloatAxis | FKeyDetails::NotBlueprintBindableKey, "GSXRTouch"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_Touch_Left_TrackpadY_Axis, LOCTEXT("GSXR_Touch_Left_TrackpadY_Axis", "GSXRTouch(L) TrackpadY Axis"), FKeyDetails::GamepadKey | FKeyDetails::FloatAxis | FKeyDetails::NotBlueprintBindableKey, "GSXRTouch"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_Touch_Left_Trigger_Axis, LOCTEXT("GSXR_Touch_Left_Trigger_Axis", "GSXRTouch(L) Trigger Axis"), FKeyDetails::GamepadKey | FKeyDetails::FloatAxis | FKeyDetails::NotBlueprintBindableKey, "GSXRTouch"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_Touch_Left_TrackpadUp_Click, LOCTEXT("GSXR_Touch_Left_TrackpadUp_Click", "GSXRTouch(L) TrackpadUp Click"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "GSXRTouch"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_Touch_Left_TrackpadDown_Click, LOCTEXT("GSXR_Touch_Left_TrackpadDown_Click", "GSXRTouch(L) TrackpadDown Click"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "GSXRTouch"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_Touch_Left_TrackpadLeft_Click, LOCTEXT("GSXR_Touch_Left_TrackpadLeft_Click", "GSXRTouch(L) TrackpadLeft Click"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "GSXRTouch"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_Touch_Left_TrackpadRight_Click, LOCTEXT("GSXR_Touch_Left_TrackpadRight_Click", "GSXRTouch(L) TrackpadRight Click"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "GSXRTouch"));



	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_Touch_Right_Trackpad_Click, LOCTEXT("GSXR_Touch_Right_Trackpad_Click", "GSXRTouch(R) Trackpad Click"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "GSXRTouch"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_Touch_Right_Trackpad_Touch, LOCTEXT("GSXR_Touch_Right_Trackpad_Touch", "GSXRTouch(R) Trackpad Touch"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "GSXRTouch"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_Touch_Right_Trigger_Click, LOCTEXT("GSXR_Touch_Right_Trigger_Click", "GSXRTouch(R) Trigger Click"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "GSXRTouch"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_Touch_Right_Grip_Click, LOCTEXT("GSXR_Touch_Right_Grip_Click", "GSXRTouch(R) Grip Click"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "GSXRTouch"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_Touch_Right_Menu_Click, LOCTEXT("GSXR_Touch_Right_Menu_Click", "GSXRTouch(R) Menu Click"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "GSXRTouch"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_Touch_Right_System_Click, LOCTEXT("GSXR_Touch_Right_System_Click", "GSXRTouch(R) System Click"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "GSXRTouch"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_Touch_Right_TrackpadX_Axis, LOCTEXT("GSXR_Touch_Right_TrackpadX_Axis", "GSXRTouch(R) TrackpadX Axis"), FKeyDetails::GamepadKey | FKeyDetails::FloatAxis | FKeyDetails::NotBlueprintBindableKey, "GSXRTouch"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_Touch_Right_TrackpadY_Axis, LOCTEXT("GSXR_Touch_Right_TrackpadY_Axis", "GSXRTouch(R) TrackpadY Axis"), FKeyDetails::GamepadKey | FKeyDetails::FloatAxis | FKeyDetails::NotBlueprintBindableKey, "GSXRTouch"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_Touch_Right_Trigger_Axis, LOCTEXT("GSXR_Touch_Right_Trigger_Axis", "GSXRTouch(R) Trigger Axis"), FKeyDetails::GamepadKey | FKeyDetails::FloatAxis | FKeyDetails::NotBlueprintBindableKey, "GSXRTouch"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_Touch_Right_TrackpadUp_Click, LOCTEXT("GSXR_Touch_Right_TrackpadUp_Click", "GSXRTouch(R) TrackpadUp Click"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "GSXRTouch"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_Touch_Right_TrackpadDown_Click, LOCTEXT("GSXR_Touch_Right_TrackpadDown_Click", "GSXRTouch(R) TrackpadDown Click"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "GSXRTouch"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_Touch_Right_TrackpadLeft_Click, LOCTEXT("GSXR_Touch_Right_TrackpadLeft_Click", "GSXRTouch(R) TrackpadLeft Click"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "GSXRTouch"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_Touch_Right_TrackpadRight_Click, LOCTEXT("GSXR_Touch_Right_TrackpadRight_Click", "GSXRTouch(R) TrackpadRight Click"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "GSXRTouch"));



	EKeys::AddMenuCategoryDisplayInfo("GSXRGamePad", LOCTEXT("GSXRGamePadSubCategory", "GSXR GamePad"), TEXT("GraphEditor.PadEvent_16x"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_GamePad_A_Click, LOCTEXT("GSXR_GamePad_A_Click", "GSXRGamePad A Click"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "GSXRGamePad"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_GamePad_B_Click, LOCTEXT("GSXR_GamePad_B_Click", "GSXRGamePad B Click"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "GSXRGamePad"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_GamePad_Back_Click, LOCTEXT("GSXR_GamePad_Back_Click", "GSXRGamePad Back Click"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "GSXRGamePad"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_GamePad_DpadDown_Click, LOCTEXT("GSXR_GamePad_DpadDown_Click", "GSXRGamePad DpadDown Click"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "GSXRGamePad"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_GamePad_DpadLeft_Click, LOCTEXT("GSXR_GamePad_DpadLeft_Click", "GSXRGamePad DpadLeft Click"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "GSXRGamePad"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_GamePad_DpadRight_Click, LOCTEXT("GSXR_GamePad_DpadRight_Click", "GSXRGamePad DpadRight Click"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "GSXRGamePad"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_GamePad_DpadUp_Click, LOCTEXT("GSXR_GamePad_DpadUp_Click", "GSXRGamePad DpadUp Click"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "GSXRGamePad"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_GamePad_LeftThumbstick_Click, LOCTEXT("GSXR_GamePad_LeftThumbstick_Click", "GSXRGamePad LeftThumbstick Click"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "GSXRGamePad"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_GamePad_RightThumbstick_Click, LOCTEXT("GSXR_GamePad_RightThumbstick_Click", "GSXRGamePad RightThumbstick Click"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "GSXRGamePad"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_GamePad_LeftShoulder_Click, LOCTEXT("GSXR_GamePad_LeftShoulder_Click", "GSXRGamePad LeftShoulder Click"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "GSXRGamePad"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_GamePad_RightShoulder_Click, LOCTEXT("GSXR_GamePad_RightShoulder_Click", "GSXRGamePad RightShoulder Click"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "GSXRGamePad"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_GamePad_X_Click, LOCTEXT("GSXR_GamePad_X_Click", "GSXRGamePad X Click"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "GSXRGamePad"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_GamePad_Y_Click, LOCTEXT("GSXR_GamePad_Y_Click", "GSXRGamePad Y Click"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "GSXRGamePad"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_GamePad_Start_Click, LOCTEXT("GSXR_GamePad_Start_Click", "GSXRGamePad Start Click"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "GSXRGamePad"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_GamePad_LeftThumbstickDown_Click, LOCTEXT("GSXR_GamePad_LeftThumbstickDown_Click", "GSXRGamePad LeftThumbstickDown Click"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "GSXRGamePad"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_GamePad_LeftThumbstickLeft_Click, LOCTEXT("GSXR_GamePad_LeftThumbstickLeft_Click", "GSXRGamePad LeftThumbstickLeft Click"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "GSXRGamePad"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_GamePad_LeftThumbstickRight_Click, LOCTEXT("GSXR_GamePad_LeftThumbstickRight_Click", "GSXRGamePad LeftThumbstickRight Click"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "GSXRGamePad"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_GamePad_LeftThumbstickUp_Click, LOCTEXT("GSXR_GamePad_LeftThumbstickUp_Click", "GSXRGamePad LeftThumbstickUp Click"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "GSXRGamePad"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_GamePad_RightThumbstickDown_Click, LOCTEXT("GSXR_GamePad_RightThumbstickDown_Click", "GSXRGamePad RightThumbstickDown Click"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "GSXRGamePad"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_GamePad_RightThumbstickRight_Click, LOCTEXT("GSXR_GamePad_RightThumbstickRight_Click", "GSXRGamePad RightThumbstickRight Click"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "GSXRGamePad"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_GamePad_RightThumbstickLeft_Click, LOCTEXT("GSXR_GamePad_RightThumbstickLeft_Click", "GSXRGamePad RightThumbstickLeft Click"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "GSXRGamePad"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_GamePad_RightThumbstickUp_Click, LOCTEXT("GSXR_GamePad_RightThumbstickUp_Click", "GSXRGamePad RightThumbstickUp Click"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "GSXRGamePad"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_GamePad_LeftTrigger_Click, LOCTEXT("GSXR_GamePad_LeftTrigger_Click", "GSXRGamePad LefTrigger Click"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "GSXRGamePad"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_GamePad_RightTrigger_Click, LOCTEXT("GSXR_GamePad_RightTrigger_Click", "GSXRGamePad RightTrigger Click"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "GSXRGamePad"));

	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_GamePad_TriggerLeft_Axis, LOCTEXT("GSXR_GamePad_TriggerLeft_Axis", "GSXRGamePad TriggerLeft Axis"), FKeyDetails::GamepadKey | FKeyDetails::FloatAxis | FKeyDetails::NotBlueprintBindableKey, "GSXRGamePad"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_GamePad_TriggerRight_Axis, LOCTEXT("GSXR_GamePad_TriggerRight_Axis", "GSXRGamePad TriggerRight Axis"), FKeyDetails::GamepadKey | FKeyDetails::FloatAxis | FKeyDetails::NotBlueprintBindableKey, "GSXRGamePad"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_GamePad_LeftThumbstickX_Axis, LOCTEXT("GSXR_GamePad_LeftThumbstickX_Axis", "GSXRGamePad LeftThumbstickX Axis"), FKeyDetails::GamepadKey | FKeyDetails::FloatAxis | FKeyDetails::NotBlueprintBindableKey, "GSXRGamePad"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_GamePad_LeftThumbstickY_Axis, LOCTEXT("GSXR_GamePad_LeftThumbstickY_Axis", "GSXRGamePad LeftThumbstickY Axis"), FKeyDetails::GamepadKey | FKeyDetails::FloatAxis | FKeyDetails::NotBlueprintBindableKey, "GSXRGamePad"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_GamePad_RightThumbstickX_Axis, LOCTEXT("GSXR_GamePad_RightThumbstickX_Axis", "GSXRGamePad RightThumbstickX Axis"), FKeyDetails::GamepadKey | FKeyDetails::FloatAxis | FKeyDetails::NotBlueprintBindableKey, "GSXRGamePad"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_GamePad_RightThumbstickY_Axis, LOCTEXT("GSXR_GamePad_RightThumbstickY_Axis", "GSXRGamePad RightThumbstickY Axis"), FKeyDetails::GamepadKey | FKeyDetails::FloatAxis | FKeyDetails::NotBlueprintBindableKey, "GSXRGamePad"));

	EKeys::AddMenuCategoryDisplayInfo("GSXRHMD", LOCTEXT("GSXRHMDSubCategory", "GSXR HMD"), TEXT("GraphEditor.PadEvent_16x"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_HMD_Enter_Click, LOCTEXT("GSXR_HMD_Enter_Click", "GSXR HMD Enter Click"), FKeyDetails::GamepadKey, "GSXRHMD"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_HMD_Back_Click, LOCTEXT("GSXR_HMD_Back_Click", "GSXR HMD Back Click"), FKeyDetails::GamepadKey, "GSXRHMD"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_HMD_VolumeUp_Click, LOCTEXT("GSXR_HMD_VolumeUp_Click", "GSXR HMD VolumeUp Click"), FKeyDetails::GamepadKey, "GSXRHMD"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_HMD_VolumeDown_Click, LOCTEXT("GSXR_HMD_VolumeDown_Click", "GSXR HMD VolumeDown Click"), FKeyDetails::GamepadKey, "GSXRHMD"));
	EKeys::AddKey(FKeyDetails(FGSXRKey::GSXR_HMD_Home_Click, LOCTEXT("GSXR_HMD_Home_Click", "GSXR HMD Home Click"), FKeyDetails::GamepadKey, "GSXRHMD"));
}

void FGSXRInput::ProcessActionEvents()
{
#if USE_GSXRAPI
	if (!(bLeftControllerState || bRightControllerState))
	{
		return;
	}

	for (auto ActionSet : ActionSets)
	{
		GSXR_XrActiveActionSet ActiveActionSet;
		ActiveActionSet.ulActionSet = ActionSet.ActionSetHandle;
		ActiveActionSet.ulRestrictedToDevice = 0;
		ActiveActionSet.nPriorty = 0;
		Gsxr_Instance->UpdateXrActionState(ActiveActionSet, sizeof(ActiveActionSet), 1);
	}

	for (auto& Action : ActionEvents)
	{
		if (Action.ActionType == EGSXRActionType::Boolean)
		{
			GSXR_XrInputDigitalActionData DigitalActionData;
			Gsxr_Instance->GetXrActionDigitalActionData(Action.ActionHandle, DigitalActionData, sizeof(DigitalActionData), 0);
			if (DigitalActionData.bState && DigitalActionData.bChanged)
			{
				MessageHandler->OnControllerButtonPressed(Action.KeyX, 0, false);
				UE_LOG(LogTemp, Log, TEXT("keyname :%s"), *Action.KeyX.ToString());
			}
			else if (!DigitalActionData.bState && DigitalActionData.bChanged)
			{
				MessageHandler->OnControllerButtonReleased(Action.KeyX, 0, false);
			}
		}
		else if (Action.ActionType == EGSXRActionType::Vector1)
		{
			GSXR_XrInputAnalogActionData AnalogActionData;
			Gsxr_Instance->GetXrAnalogActionData(Action.ActionHandle, AnalogActionData, sizeof(AnalogActionData), 0);
			if (Action.ButtonPrivateName.EndsWith("y"))
			{
				MessageHandler->OnControllerAnalog(Action.KeyX, 0, AnalogActionData.y);
			}
			else
			{
				MessageHandler->OnControllerAnalog(Action.KeyX, 0, AnalogActionData.x);
			}

		}
	}
#endif	
}

void FGSXRInput::ProcessHMDButtonEvents()
{
#if USE_GSXRAPI
	GSXR_Flags64 Clicks;
	Gsxr_Instance->GetInputClickStates(GSXR_DeviceType_HMD_VR, GSXR_Hmd, Clicks);

	for (auto& Button : HMDButtons)
	{
		Button.bChanged = (Clicks & Button.InputMask) != Button.bCurrentState;
		Button.bCurrentState = Clicks & Button.InputMask;		
	}
	for (auto Button : HMDButtons)
	{
		if (Button.bChanged)
		{
			UE_LOG(LogTemp, Log, TEXT("Print HMD Button State:Clicks :%d"), Clicks);
			UE_LOG(LogTemp, Log, TEXT("Print HMD Button State keyname: %s Click"), *Button.ButtonName.ToString());
			if (Button.bCurrentState)
			{
				MessageHandler->OnControllerButtonPressed(Button.ButtonName, 0, false);
			}
			else
			{
				MessageHandler->OnControllerButtonReleased(Button.ButtonName, 0, false);
			}
		}
	}
#endif
}

void FGSXRInput::GenerateManifestFile(const UInputSettings* InputSettings)
{
	const FString ManifestPath = GetManifestPath() / GSXR_INPUT_CONFIG_MANIFEST;
	TArray<FName>ActionNames;
	InputSettings->GetActionNames(ActionNames);
	TArray<TSharedPtr<FJsonValue>> ActionJsonValues;
	TSharedRef<FJsonObject> LocalizationJsonObject = MakeShareable(new FJsonObject());
	LocalizationJsonObject->SetStringField("language_tag", "en_us");
	for (auto ActionName : ActionNames)
	{
		TSharedRef<FJsonObject> ActionJsonObject = MakeShareable(new FJsonObject());
		ActionJsonObject->SetStringField("name", "/actions/default/in/" + ActionName.ToString());
		ActionJsonObject->SetStringField("type", "boolean");
		ActionJsonValues.Add(MakeShareable(new FJsonValueObject(ActionJsonObject)));
		LocalizationJsonObject->SetStringField("/actions/default/in/" + ActionName.ToString(), ActionName.ToString());
	}

	TArray<FName>AxisNames;
	InputSettings->GetAxisNames(AxisNames);
	for (auto AxisName : AxisNames)
	{
		TSharedRef<FJsonObject> AxisJsonObject = MakeShareable(new FJsonObject());
		AxisJsonObject->SetStringField("name", "/actions/default/in/" + AxisName.ToString());
		AxisJsonObject->SetStringField("type", "vector1");
		ActionJsonValues.Add(MakeShareable(new FJsonValueObject(AxisJsonObject)));
		LocalizationJsonObject->SetStringField("/actions/default/in/" + AxisName.ToString(), AxisName.ToString());
	}
	TSharedRef<FJsonObject> AppManifestObject = MakeShareable(new FJsonObject());
	AppManifestObject->SetStringField("source", FString::Printf(TEXT("UE")));
	AppManifestObject->SetArrayField("actions", ActionJsonValues);
	AppManifestObject->SetObjectField("localization", LocalizationJsonObject);

	TArray<TSharedPtr<FJsonValue>> DefaultBindingsJsonValues;
	TSharedRef<FJsonObject> DefaultBindingJsonObject = MakeShareable(new FJsonObject());
	DefaultBindingJsonObject->SetStringField("controller_type", "GSXR_JoyStick");
	DefaultBindingJsonObject->SetStringField("binding_url", GSXR_INPUT_CONFIG_JOYSTICK);
	DefaultBindingsJsonValues.Add(MakeShareable(new FJsonValueObject(DefaultBindingJsonObject)));
	DefaultBindingJsonObject = MakeShareable(new FJsonObject());
	DefaultBindingJsonObject->SetStringField("controller_type", "GSXR_Touch");
	DefaultBindingJsonObject->SetStringField("binding_url", GSXR_INPUT_CONFIG_TOUCH);
	DefaultBindingsJsonValues.Add(MakeShareable(new FJsonValueObject(DefaultBindingJsonObject)));
	DefaultBindingJsonObject = MakeShareable(new FJsonObject());
	DefaultBindingJsonObject->SetStringField("controller_type", "GSXR_GamePad");
	DefaultBindingJsonObject->SetStringField("binding_url", GSXR_INPUT_CONFIG_GAMEPAD);
	DefaultBindingsJsonValues.Add(MakeShareable(new FJsonValueObject(DefaultBindingJsonObject)));
	AppManifestObject->SetArrayField("default_bindings", DefaultBindingsJsonValues);

	//ActionSet


	TArray<TSharedPtr<FJsonValue>> ActionSetsValues;
	for (auto ActionSet : ActionSets)
	{
		TSharedRef<FJsonObject> ActionSetJsonObject = MakeShareable(new FJsonObject());
		ActionSetJsonObject->SetStringField("name", ActionSet.ActionSetName);
		ActionSetJsonObject->SetStringField("usage", ActionSet.Usage);
		ActionSetsValues.Add(MakeShareable(new FJsonValueObject(ActionSetJsonObject)));
	}
	AppManifestObject->SetArrayField("action_sets", ActionSetsValues);

	//Save Json File
	FString AppManifestString;
	TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&AppManifestString);
	FJsonSerializer::Serialize(AppManifestObject, JsonWriter);
	IFileManager& FileManager = FFileManagerGeneric::Get();
	if (!FileManager.FileExists(*ManifestPath))
	{

		bool SaveResult = FFileHelper::SaveStringToFile(AppManifestString, *ManifestPath, FFileHelper::EEncodingOptions::ForceUTF8WithoutBOM);
		UE_LOG(GSXR_Unreal_Input, Warning, TEXT(" Save File %s Result %d"), *ManifestPath, SaveResult);
	}
	else
	{
		UE_LOG(GSXR_Unreal_Input, Warning, TEXT(" Manifest file is exist !"));
	}

}

void FGSXRInput::GenerateActionBinding(const UInputSettings* InputSettings, FString ControllerType)
{
	TSharedRef<FJsonObject> ActionBindingObject = MakeShareable(new FJsonObject());
	ActionBindingObject->SetStringField("name", "Default bindings for GSXR " + ControllerType + " controller.");
	ActionBindingObject->SetStringField("description", "Default mapping for " + ControllerType + " controllers into the main interface.");
	ActionBindingObject->SetStringField("controller_type", "GSXR_" + ControllerType);
	ActionBindingObject->SetStringField("last_edited_by", "UnrealEngine");
	TSharedRef<FJsonObject> BindingsObject = MakeShareable(new FJsonObject());
	TSharedRef<FJsonObject> ActionsMainObject = MakeShareable(new FJsonObject());
	TArray<TSharedPtr<FJsonValue>> ActionsSourceObjects;
	TArray<FString> AllActionButton;
	TArray<FString> AllAxisButton;
	for (auto ActionMapping : Gsxr_ActionMappings)
	{
		if (ActionMapping.ControllerType != ControllerType.ToLower())
		{
			continue;
		}

		TSharedRef<FJsonObject> ActionItemObject = MakeShareable(new FJsonObject());
		TSharedRef<FJsonObject> ActionItemActionObject = MakeShareable(new FJsonObject());
		TSharedRef<FJsonObject> ActionOutputObject = MakeShareable(new FJsonObject());
		ActionOutputObject->SetStringField("output", ActionMapping.ActionPath);
		if (ActionMapping.ActionType == EGSXRActionType::Vector1)
		{
			ActionItemActionObject->SetObjectField("position", ActionOutputObject);
		}
		else
		{
			ActionItemActionObject->SetObjectField(ActionMapping.ButtonActionType, ActionOutputObject);
		}
		ActionItemObject->SetStringField("mode", ActionMapping.ButtonMode);
		ActionItemObject->SetStringField("path", ActionMapping.ButtonPath);
		if (ActionMapping.ControllerType == "JoyStick")
		{
			if (ActionMapping.ButtonPrivateName.Contains("thumbstick") && ActionMapping.ButtonPrivateName.Len() > 11)
			{
				FString Type = ActionMapping.ButtonPrivateName.Right(ActionMapping.ButtonPrivateName.Len() - 10);
				TSharedRef<FJsonObject> SubModeJsonValue = MakeShareable(new FJsonObject());
				SubModeJsonValue->SetStringField("sub_mode", "click");
				ActionItemObject->SetStringField("mode", "dpad");
				ActionItemObject->SetObjectField("parameters", SubModeJsonValue);
				TSharedRef<FJsonObject> ActionItemActionOutputObject = MakeShareable(new FJsonObject());
				ActionItemActionObject->RemoveField("click");
				ActionItemActionOutputObject->SetStringField("output", ActionMapping.ActionPath);
				if (Type == "up")
				{
					ActionItemActionObject->SetObjectField("north", ActionItemActionOutputObject);
				}
				else if (Type == "down")
				{
					ActionItemActionObject->SetObjectField("south", ActionItemActionOutputObject);
				}
				else if (Type == "left")
				{
					ActionItemActionObject->SetObjectField("west", ActionItemActionOutputObject);
				}
				else if (Type == "right")
				{
					ActionItemActionObject->SetObjectField("east", ActionItemActionOutputObject);
				}
			}
		}
		else if (ActionMapping.ControllerType == "gamepad")
		{
			if (ActionMapping.ButtonPrivateName.Contains("leftthumbstick"))
			{
				ActionItemObject->SetStringField("path", "/user/gamepad/input/joystick_left");
				if (ActionMapping.ButtonPrivateName.Len() > 15)
				{
					FString Type = ActionMapping.ButtonPrivateName.Right(ActionMapping.ButtonPrivateName.Len() - 14);
					TSharedRef<FJsonObject> SubModeJsonValue = MakeShareable(new FJsonObject());
					SubModeJsonValue->SetStringField("sub_mode", "click");
					ActionItemObject->SetStringField("mode", "dpad");
					ActionItemObject->SetObjectField("parameters", SubModeJsonValue);
					TSharedRef<FJsonObject> ActionItemActionOutputObject = MakeShareable(new FJsonObject());
					ActionItemActionObject->RemoveField("click");
					ActionItemActionOutputObject->SetStringField("output", ActionMapping.ActionPath);

					if (Type == "up")
					{
						ActionItemActionObject->SetObjectField("north", ActionItemActionOutputObject);
					}
					else if (Type == "down")
					{
						ActionItemActionObject->SetObjectField("south", ActionItemActionOutputObject);
					}
					else if (Type == "left")
					{
						ActionItemActionObject->SetObjectField("west", ActionItemActionOutputObject);
					}
					else if (Type == "right")
					{
						ActionItemActionObject->SetObjectField("east", ActionItemActionOutputObject);
					}
				}
			}
			else if (ActionMapping.ButtonPrivateName.Contains("rightthumbstick"))
			{
				ActionItemObject->SetStringField("path", "/user/gamepad/input/joystick_right");
				FString Type = ActionMapping.ButtonPrivateName.Right(ActionMapping.ButtonPrivateName.Len() - 15);
				if (ActionMapping.ButtonPrivateName.Len() > 15)
				{
					TSharedRef<FJsonObject> SubModeJsonValue = MakeShareable(new FJsonObject());
					SubModeJsonValue->SetStringField("sub_mode", "click");
					ActionItemObject->SetStringField("mode", "dpad");
					ActionItemObject->SetObjectField("parameters", SubModeJsonValue);
					TSharedRef<FJsonObject> ActionItemActionOutputObject = MakeShareable(new FJsonObject());
					ActionItemActionObject->RemoveField("click");
					ActionItemActionOutputObject->SetStringField("output", ActionMapping.ActionPath);

					if (Type == "up")
					{
						ActionItemActionObject->SetObjectField("north", ActionItemActionOutputObject);
					}
					else if (Type == "down")
					{
						ActionItemActionObject->SetObjectField("south", ActionItemActionOutputObject);
					}
					else if (Type == "left")
					{
						ActionItemActionObject->SetObjectField("west", ActionItemActionOutputObject);
					}
					else if (Type == "right")
					{
						ActionItemActionObject->SetObjectField("east", ActionItemActionOutputObject);
					}
				}
			}
			else if (ActionMapping.ButtonPrivateName.Contains("dpad"))
			{
				FString Type = ActionMapping.ButtonPrivateName.Right(ActionMapping.ButtonPrivateName.Len() - 4);
				ActionItemObject->SetStringField("path", "/user/gamepad/input/dpad_" + Type);
			}
			else if (ActionMapping.ButtonPrivateName == "leftttigger")
			{
				ActionItemObject->SetStringField("path", "/user/gamepad/input/trigger_left");
			}
			else if (ActionMapping.ButtonPrivateName == "rightttigger")
			{
				ActionItemObject->SetStringField("path", "/user/gamepad/input/trigger_right");
			}
			else if (ActionMapping.ButtonPrivateName == "leftshoulder")
			{
				ActionItemObject->SetStringField("path", "/user/gamepad/input/shoulder_left");
			}
			else if (ActionMapping.ButtonPrivateName == "rightshoulder")
			{
				ActionItemObject->SetStringField("path", "/user/gamepad/input/shoulder_right");
			}
			else if (ActionMapping.ButtonPrivateName == "triggerleft")
			{
				ActionItemObject->SetStringField("path", "/user/gamepad/input/trigger_right");
			}
			else if (ActionMapping.ButtonPrivateName == "triggerright")
			{
				ActionItemObject->SetStringField("path", "/user/gamepad/input/trigger_right");
			}
		}
		else if (ActionMapping.ControllerType == "touch")
		{
			if (ActionMapping.ButtonPrivateName.Contains("trackpad") && ActionMapping.ButtonPrivateName.Len() > 9)
			{
				FString Type = ActionMapping.ButtonPrivateName.Right(ActionMapping.ButtonPrivateName.Len() - 8);
				TSharedRef<FJsonObject> SubModeJsonValue = MakeShareable(new FJsonObject());
				SubModeJsonValue->SetStringField("sub_mode", "click");
				ActionItemObject->SetStringField("mode", "dpad");
				ActionItemObject->SetObjectField("parameters", SubModeJsonValue);

				TSharedRef<FJsonObject> ActionItemActionOutputObject = MakeShareable(new FJsonObject());
				ActionItemActionObject->RemoveField("click");
				ActionItemActionOutputObject->SetStringField("output", ActionMapping.ActionPath);
				if (Type == "up")
				{
					ActionItemActionObject->SetObjectField("north", ActionItemActionOutputObject);
				}
				else if (Type == "down")
				{
					ActionItemActionObject->SetObjectField("south", ActionItemActionOutputObject);
				}
				else if (Type == "left")
				{
					ActionItemActionObject->SetObjectField("west", ActionItemActionOutputObject);
				}
				else if (Type == "right")
				{
					ActionItemActionObject->SetObjectField("east", ActionItemActionOutputObject);
				}
			}
		}
		ActionItemObject->SetObjectField("inputs", ActionItemActionObject);
		ActionsSourceObjects.Add(MakeShareable(new FJsonValueObject(ActionItemObject)));
	}
	ActionsMainObject->SetArrayField("sources", ActionsSourceObjects);
	BindingsObject->SetObjectField("/actions/default", ActionsMainObject);
	ActionBindingObject->SetObjectField("bindings", BindingsObject);

	FString ActionBindingString;
	FString ActionBindingPath;
	if (ControllerType == "Touch")
	{
		ActionBindingPath = GetManifestPath() / GSXR_INPUT_CONFIG_TOUCH;
	}
	else if (ControllerType == "JoyStick")
	{
		ActionBindingPath = GetManifestPath() / GSXR_INPUT_CONFIG_JOYSTICK;
	}
	else
	{
		ActionBindingPath = GetManifestPath() / GSXR_INPUT_CONFIG_GAMEPAD;
	}

	TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&ActionBindingString);
	FJsonSerializer::Serialize(ActionBindingObject, JsonWriter);
	IFileManager& FileManager = FFileManagerGeneric::Get();
	if (!FileManager.FileExists(*ActionBindingPath))
	{
		bool SaveResult = FFileHelper::SaveStringToFile(ActionBindingString, *ActionBindingPath, FFileHelper::EEncodingOptions::ForceUTF8WithoutBOM);
		UE_LOG(GSXR_Unreal_Input, Warning, TEXT(" Save File %s Result %d"), *ActionBindingPath, SaveResult);
	}
	else
	{
		UE_LOG(GSXR_Unreal_Input, Warning, TEXT(" ActionBinding file is exist !"));
	}


}

void FGSXRInput::GenerateActionMappings(const UInputSettings* InputSettings)
{
	TArray<FInputActionKeyMapping>ActionKeyMappings = InputSettings->GetActionMappings();
	Gsxr_ActionMappings.Empty();
	for (auto ActionKeyMapping : ActionKeyMappings)
	{
		if (!ActionKeyMapping.Key.ToString().Contains("GSXR") || ActionKeyMapping.Key.ToString().Contains("HMD"))
		{
			continue;
		}
		TArray<FString> KeyName;
		int32 ArrayLength = ActionKeyMapping.Key.ToString().ToLower().ParseIntoArray(KeyName, TEXT("_"));
		FString Hand;
		FString ButtonName;
		FString ButtonActionType;
		FString ControllerType;
		if (ArrayLength == 4)
		{
			Hand = "";
			ControllerType = KeyName[1];
			ButtonName = KeyName[2];
			ButtonActionType = KeyName[3];
		}
		else
		{
			ControllerType = KeyName[1];
			Hand = KeyName[2];
			ButtonName = KeyName[3];
			ButtonActionType = KeyName[4];
		}
		FGSXRInputAction InputAction;
		InputAction.ActionName = ActionKeyMapping.ActionName;
		InputAction.ActionType = EGSXRActionType::Boolean;
		InputAction.KeyX = ActionKeyMapping.Key.GetFName();
		InputAction.ControllerType = ControllerType;
		InputAction.ButtonActionType = ButtonActionType;
		InputAction.ButtonPrivateName = ButtonName;
		if (ControllerType == "gamepad")
		{
			InputAction.ButtonPath = "/user/gamepad/input/" + ButtonName;
		}
		else
		{
			if (ButtonName.Contains("thumbstick"))
			{
				InputAction.ButtonPath = "/user/hand/" + Hand + "/input/joystick";
			}
			else if (ButtonName.Contains("trackpad"))
			{
				InputAction.ButtonPath = "/user/hand/" + Hand + "/input/trackpad";
			}
			else if (ControllerType == "touch" && ButtonName.Contains("menu"))
			{
				InputAction.ButtonPath = "/user/hand/" + Hand + "/input/menu";
			}
			else
			{
				InputAction.ButtonPath = "/user/hand/" + Hand + "/input/" + ButtonName;
			}
		}
		InputAction.ActionPath = "/actions/default/in/" + InputAction.ActionName.ToString();

		if (ButtonName.Contains("thumbstick") || ButtonName.Contains("joystick"))
		{
			InputAction.ButtonMode = "joystick";
		}
		else if (ButtonName == "trigger" || ButtonName == "grip")
		{
			InputAction.ButtonMode = "trigger";
			if (ControllerType == "touch" && ButtonName == "grip")
			{
				InputAction.ButtonMode = "button";
			}
		}
		else if (ButtonName.Contains("trackpad"))
		{
			InputAction.ButtonMode = "trackpad";
		}
		else
		{
			InputAction.ButtonMode = "button";
		}
		Gsxr_ActionMappings.Add(InputAction);
	}

	TArray<FInputAxisKeyMapping>AxisKeyMappings = InputSettings->GetAxisMappings();
	for (auto AxisKeyMapping : AxisKeyMappings)
	{
		if (!AxisKeyMapping.Key.ToString().Contains("GSXR"))
		{
			continue;
		}
		TArray<FString> KeyName;
		int32 ArrayLength = AxisKeyMapping.Key.ToString().ToLower().ParseIntoArray(KeyName, TEXT("_"));
		FString Hand;
		FString ButtonName;
		FString ButtonActionType;
		FString ControllerType;
		if (ArrayLength == 4)
		{
			Hand = "";
			ControllerType = KeyName[1];
			ButtonName = KeyName[2];
			ButtonActionType = KeyName[3];
		}
		else
		{
			ControllerType = KeyName[1];
			Hand = KeyName[2];
			ButtonName = KeyName[3];
			ButtonActionType = KeyName[4];
		}

		FGSXRInputAction InputAction;
		InputAction.ActionName = AxisKeyMapping.AxisName;
		InputAction.ActionType = EGSXRActionType::Vector1;
		InputAction.KeyX = AxisKeyMapping.Key.GetFName();
		InputAction.ButtonActionType = ButtonActionType;
		InputAction.ControllerType = ControllerType;

		InputAction.ButtonPrivateName = ButtonName;
		if (ButtonName.Len() > 1 && (ButtonName.EndsWith("x") || ButtonName.EndsWith("y")))
		{
			if (ButtonName.Contains("thumbstick") && ButtonName.Len() == 11)
			{
				InputAction.ButtonPath = "/user/hand/" + Hand + "/input/joystick";
			}
			else
			{
				InputAction.ButtonPath = "/user/hand/" + Hand + "/input/" + ButtonName.Left(ButtonName.Len() - 1);
			}
		}
		else
		{
			InputAction.ButtonPath = "/user/hand/" + Hand + "/input/" + ButtonName;
		}
		InputAction.ActionPath = "/actions/default/in/" + AxisKeyMapping.AxisName.ToString();
		if (ButtonName.Contains("thumbstick") || ButtonName.Contains("joystick"))
		{
			InputAction.ButtonMode = "joystick";
		}
		else if (ButtonName == "trigger" || ButtonName == "grip")
		{
			InputAction.ButtonMode = "trigger";
		}
		else if (ButtonName.Contains("trackpad"))
		{
			InputAction.ButtonMode = "trackpad";
		}
		else
		{
			InputAction.ButtonMode = "button";
		}
		Gsxr_ActionMappings.Add(InputAction);
	}

	TMultiMap<FName, FName> KeyToActionsMap;
	ActionEvents.Empty();	
	bool bAlreadyExists = false;

	// Fill-in unique Action Events that will be processed per tick
	for (auto& InputAction : Gsxr_ActionMappings)
	{
		// Check if we've already got a similar action event as we won't need the flat action list for processing controller events
		bAlreadyExists = false;
		for (auto& ActionEvent : ActionEvents)
		{
			if (ActionEvent.ActionName == InputAction.ActionName)
			{
				bAlreadyExists = true;
				break;
			}
		}
		// Add unique action handles to action events
		if (!bAlreadyExists )
		{
		ActionEvents.Add(InputAction);
		UE_LOG(GSXR_Unreal_Input, Warning, TEXT(" InputAction.ActionName %s InputAction.keyname %s"), *InputAction.ActionName.ToString(), *InputAction.KeyX.ToString());
		}
		if (InputAction.KeyX != NAME_None)
		{
			KeyToActionsMap.AddUnique(InputAction.KeyX, InputAction.ActionName);
		}
	}

	// Find keys that trigger multiple actions
	TArray<FName> ActionKeys;
	KeyToActionsMap.GetKeys(ActionKeys);
	for (auto Key : ActionKeys)
	{
		TArray<FName> MappedActions;
		KeyToActionsMap.MultiFind(Key, MappedActions);
		if (MappedActions.Num() > 1)
		{
			// Remove key-value pairing for keys that trigger multiple actions in map
			KeyToActionsMap.Remove(Key);
		}
	}

	// Finalize keys that will be used to trigger actions per tick
	KeyToActionsMap.Shrink();
	for (auto& ActionEvent : ActionEvents)
	{
		// Skip actions with no key mapping (poses, haptics, etc)
		if (ActionEvent.KeyX != NAME_None)
		{
			// Check if this action is in the KeyToActionsMap map
			const FName* PriorityKey = KeyToActionsMap.FindKey(ActionEvent.ActionName);
			if (PriorityKey != nullptr)
			{
				// Set the key in the KeyToActionsMap as the default key that will be used to trigger actions
				ActionEvent.KeyX = *PriorityKey;
			}
		}

		//UE_LOG(LogSteamVRInputDevice, Warning, TEXT("Action [%s] mapped to key [%s]"), *ActionEvent.Name.ToString(), *ActionEvent.KeyX.ToString());
	}


}

#undef LOCTEXT_NAMESPACE
