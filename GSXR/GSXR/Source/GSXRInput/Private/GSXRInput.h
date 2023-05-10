#pragma once
#include "IHapticDevice.h"
#include "IInputDevice.h"
#include "GSXRInstance.h"
#include "InputCoreTypes.h"
#include "XRMotionControllerBase.h"
#include "GameFramework/InputSettings.h"

#define  MaxControllerCount 2

#define GSXR_INPUT_CONFIG_PATH		"GSXRInputConfig"
#define GSXR_INPUT_CONFIG_MANIFEST	"GSXR_Manifest.json"
#define GSXR_INPUT_CONFIG_GAMEPAD	"GSXR_GamePad.json"
#define GSXR_INPUT_CONFIG_TOUCH		"GSXR_Touch.json"
#define GSXR_INPUT_CONFIG_JOYSTICK	"GSXR_JoyStick.json"


struct FGSXRKey
{
	//GSXR Joystick
	static const FKey GSXR_Joystick_Left_X_Click ;
	static const FKey GSXR_Joystick_Left_X_Touch ;  
	static const FKey GSXR_Joystick_Left_Y_Click ;  
	static const FKey GSXR_Joystick_Left_Y_Touch ;
	static const FKey GSXR_Joystick_Left_Menu_Click ;
	static const FKey GSXR_Joystick_Left_Grip_Click ;
	static const FKey GSXR_Joystick_Left_Trigger_Click ;
	static const FKey GSXR_Joystick_Left_Trigger_Touch ;
	static const FKey GSXR_Joystick_Left_Thumbstick_Click ;
	static const FKey GSXR_Joystick_Left_Thumbstick_Touch ;
	static const FKey GSXR_Joystick_Left_ThumbstickUp_Click ;
	static const FKey GSXR_Joystick_Left_ThumbstickDown_Click ;
	static const FKey GSXR_Joystick_Left_ThumbstickLeft_Click ;
	static const FKey GSXR_Joystick_Left_ThumbstickRight_Click ;
		
	static const FKey GSXR_Joystick_Left_Grip_Axis ; 
	static const FKey GSXR_Joystick_Left_Trigger_Axis ; 
	static const FKey GSXR_Joystick_Left_ThumbstickX_Axis ;
	static const FKey GSXR_Joystick_Left_ThumbstickY_Axis ;
	
	static const FKey GSXR_Joystick_Right_A_Click ;
	static const FKey GSXR_Joystick_Right_A_Touch ;  
	static const FKey GSXR_Joystick_Right_B_Click ;  
	static const FKey GSXR_Joystick_Right_B_Touch ;
	static const FKey GSXR_Joystick_Right_System_Click ;
	static const FKey GSXR_Joystick_Right_Grip_Click ;
	static const FKey GSXR_Joystick_Right_Trigger_Click ;
	static const FKey GSXR_Joystick_Right_Trigger_Touch ;
	static const FKey GSXR_Joystick_Right_Thumbstick_Click ;
	static const FKey GSXR_Joystick_Right_Thumbstick_Touch ;
	static const FKey GSXR_Joystick_Right_ThumbstickUp_Click ;
	static const FKey GSXR_Joystick_Right_ThumbstickDown_Click ;
	static const FKey GSXR_Joystick_Right_ThumbstickLeft_Click ;
	static const FKey GSXR_Joystick_Right_ThumbstickRight_Click ;
		
	static const FKey GSXR_Joystick_Right_Grip_Axis ; 
	static const FKey GSXR_Joystick_Right_Trigger_Axis ; 
	static const FKey GSXR_Joystick_Right_ThumbstickX_Axis ;
	static const FKey GSXR_Joystick_Right_ThumbstickY_Axis ;

	//GSXR Touch
	static const FKey GSXR_Touch_Left_Trackpad_Click ;
	static const FKey GSXR_Touch_Left_Trackpad_Touch ;
	static const FKey GSXR_Touch_Left_Trigger_Click ;
	static const FKey GSXR_Touch_Left_Grip_Click ;
	static const FKey GSXR_Touch_Left_Menu_Click ;
	static const FKey GSXR_Touch_Left_System_Click ;
	static const FKey GSXR_Touch_Left_TrackpadUp_Click ;
	static const FKey GSXR_Touch_Left_TrackpadDown_Click ;
	static const FKey GSXR_Touch_Left_TrackpadLeft_Click ;
	static const FKey GSXR_Touch_Left_TrackpadRight_Click ;

	static const FKey GSXR_Touch_Left_TrackpadX_Axis ;
	static const FKey GSXR_Touch_Left_TrackpadY_Axis ;
	static const FKey GSXR_Touch_Left_Trigger_Axis ;

	static const FKey GSXR_Touch_Right_Trackpad_Click ;
	static const FKey GSXR_Touch_Right_Trackpad_Touch ;
	static const FKey GSXR_Touch_Right_Trigger_Click ;
	static const FKey GSXR_Touch_Right_Grip_Click ;
	static const FKey GSXR_Touch_Right_Menu_Click ;
	static const FKey GSXR_Touch_Right_System_Click ;
	static const FKey GSXR_Touch_Right_TrackpadUp_Click ;
	static const FKey GSXR_Touch_Right_TrackpadDown_Click ;
	static const FKey GSXR_Touch_Right_TrackpadLeft_Click ;
	static const FKey GSXR_Touch_Right_TrackpadRight_Click ;

	static const FKey GSXR_Touch_Right_TrackpadX_Axis ;
	static const FKey GSXR_Touch_Right_TrackpadY_Axis ;
	static const FKey GSXR_Touch_Right_Trigger_Axis ;


	//GSXR GamePad
	static const FKey GSXR_GamePad_A_Click ;
	static const FKey GSXR_GamePad_B_Click ;
	static const FKey GSXR_GamePad_X_Click ;
	static const FKey GSXR_GamePad_Y_Click ;
	static const FKey GSXR_GamePad_Start_Click ;
	static const FKey GSXR_GamePad_Back_Click ;
	static const FKey GSXR_GamePad_DpadDown_Click ;
	static const FKey GSXR_GamePad_DpadLeft_Click ;
	static const FKey GSXR_GamePad_DpadRight_Click ;
	static const FKey GSXR_GamePad_DpadUp_Click ;
	
	static const FKey GSXR_GamePad_LeftThumbstickDown_Click ;
	static const FKey GSXR_GamePad_LeftThumbstickLeft_Click ;
	static const FKey GSXR_GamePad_LeftThumbstickRight_Click ;
	static const FKey GSXR_GamePad_LeftThumbstickUp_Click ;
	static const FKey GSXR_GamePad_LeftThumbstick_Click ;
	static const FKey GSXR_GamePad_LeftShoulder_Click ;
	static const FKey GSXR_GamePad_LeftTrigger_Click ;
	
	static const FKey GSXR_GamePad_RightThumbstickDown_Click ;
	static const FKey GSXR_GamePad_RightThumbstickRight_Click ;
	static const FKey GSXR_GamePad_RightThumbstickLeft_Click ;
	static const FKey GSXR_GamePad_RightThumbstickUp_Click ;
	static const FKey GSXR_GamePad_RightThumbstick_Click ;
	static const FKey GSXR_GamePad_RightShoulder_Click ;
	static const FKey GSXR_GamePad_RightTrigger_Click ;

	static const FKey GSXR_GamePad_TriggerLeft_Axis ;
	static const FKey GSXR_GamePad_TriggerRight_Axis ;
	static const FKey GSXR_GamePad_LeftThumbstickX_Axis ;
	static const FKey GSXR_GamePad_LeftThumbstickY_Axis ;
	static const FKey GSXR_GamePad_RightThumbstickX_Axis ;
	static const FKey GSXR_GamePad_RightThumbstickY_Axis ;

	static const FKey GSXR_HMD_Enter_Click ;
	static const FKey GSXR_HMD_Back_Click ;
	static const FKey GSXR_HMD_VolumeUp_Click ;
	static const FKey GSXR_HMD_VolumeDown_Click ;
	static const FKey GSXR_HMD_Home_Click ;
	
};


struct FGSXRKeyNames
{
	typedef FName Type;
	
	//Left Controller
	static const FName GSXR_Joystick_Left_X_Click ;
	static const FName GSXR_Joystick_Left_X_Touch ;  
	static const FName GSXR_Joystick_Left_Y_Click ;  
	static const FName GSXR_Joystick_Left_Y_Touch ;
	static const FName GSXR_Joystick_Left_Menu_Click ;
	static const FName GSXR_Joystick_Left_Grip_Click ;
	static const FName GSXR_Joystick_Left_Trigger_Click ;
	static const FName GSXR_Joystick_Left_Trigger_Touch ;
	static const FName GSXR_Joystick_Left_Thumbstick_Click ;
	static const FName GSXR_Joystick_Left_Thumbstick_Touch ;
	static const FName GSXR_Joystick_Left_ThumbstickUp_Click ;
	static const FName GSXR_Joystick_Left_ThumbstickDown_Click ;
	static const FName GSXR_Joystick_Left_ThumbstickLeft_Click ;
	static const FName GSXR_Joystick_Left_ThumbstickRight_Click ;
		
	static const FName GSXR_Joystick_Left_Grip_Axis ; 
	static const FName GSXR_Joystick_Left_Trigger_Axis ; 
	static const FName GSXR_Joystick_Left_ThumbstickX_Axis ;
	static const FName GSXR_Joystick_Left_ThumbstickY_Axis ;
	
	//Right Controller
	static const FName GSXR_Joystick_Right_A_Click ;
	static const FName GSXR_Joystick_Right_A_Touch ;  
	static const FName GSXR_Joystick_Right_B_Click ;  
	static const FName GSXR_Joystick_Right_B_Touch ;
	static const FName GSXR_Joystick_Right_System_Click ;
	static const FName GSXR_Joystick_Right_Grip_Click ;
	static const FName GSXR_Joystick_Right_Trigger_Click ;
	static const FName GSXR_Joystick_Right_Trigger_Touch ;
	static const FName GSXR_Joystick_Right_Thumbstick_Click ;
	static const FName GSXR_Joystick_Right_Thumbstick_Touch ;
	static const FName GSXR_Joystick_Right_ThumbstickUp_Click ;
	static const FName GSXR_Joystick_Right_ThumbstickDown_Click ;
	static const FName GSXR_Joystick_Right_ThumbstickLeft_Click ;
	static const FName GSXR_Joystick_Right_ThumbstickRight_Click ;
		
	static const FName GSXR_Joystick_Right_Grip_Axis ; 
	static const FName GSXR_Joystick_Right_Trigger_Axis ; 
	static const FName GSXR_Joystick_Right_ThumbstickX_Axis ;
	static const FName GSXR_Joystick_Right_ThumbstickY_Axis ;


	//GSXR Touch
	static const FName GSXR_Touch_Left_Trackpad_Click ;
	static const FName GSXR_Touch_Left_Trackpad_Touch ;
	static const FName GSXR_Touch_Left_Trigger_Click ;
	static const FName GSXR_Touch_Left_Grip_Click ;
	static const FName GSXR_Touch_Left_Menu_Click ;
	static const FName GSXR_Touch_Left_System_Click ;
	static const FName GSXR_Touch_Left_TrackpadUp_Click ;
	static const FName GSXR_Touch_Left_TrackpadDown_Click ;
	static const FName GSXR_Touch_Left_TrackpadLeft_Click ;
	static const FName GSXR_Touch_Left_TrackpadRight_Click ;

	static const FName GSXR_Touch_Left_TrackpadX_Axis ;
	static const FName GSXR_Touch_Left_TrackpadY_Axis ;
	static const FName GSXR_Touch_Left_Trigger_Axis ;

	static const FName GSXR_Touch_Right_Trackpad_Click ;
	static const FName GSXR_Touch_Right_Trackpad_Touch ;
	static const FName GSXR_Touch_Right_Trigger_Click ;
	static const FName GSXR_Touch_Right_Grip_Click ;
	static const FName GSXR_Touch_Right_Menu_Click ;
	static const FName GSXR_Touch_Right_System_Click ;
	static const FName GSXR_Touch_Right_TrackpadUp_Click ;
	static const FName GSXR_Touch_Right_TrackpadDown_Click ;
	static const FName GSXR_Touch_Right_TrackpadLeft_Click ;
	static const FName GSXR_Touch_Right_TrackpadRight_Click ;

	static const FName GSXR_Touch_Right_TrackpadX_Axis ;
	static const FName GSXR_Touch_Right_TrackpadY_Axis ;
	static const FName GSXR_Touch_Right_Trigger_Axis ;


	//GSXR GamePad
	static const FName GSXR_GamePad_A_Click ;
	static const FName GSXR_GamePad_B_Click ;
	static const FName GSXR_GamePad_X_Click ;
	static const FName GSXR_GamePad_Y_Click ;
	static const FName GSXR_GamePad_Start_Click ;
	static const FName GSXR_GamePad_Back_Click ;
	static const FName GSXR_GamePad_DpadDown_Click ;
	static const FName GSXR_GamePad_DpadLeft_Click ;
	static const FName GSXR_GamePad_DpadRight_Click ;
	static const FName GSXR_GamePad_DpadUp_Click ;
	
	static const FName GSXR_GamePad_LeftThumbstickDown_Click ;
	static const FName GSXR_GamePad_LeftThumbstickLeft_Click ;
	static const FName GSXR_GamePad_LeftThumbstickRight_Click ;
	static const FName GSXR_GamePad_LeftThumbstickUp_Click ;
	static const FName GSXR_GamePad_RightThumbstickDown_Click ;
	static const FName GSXR_GamePad_RightThumbstickRight_Click ;
	static const FName GSXR_GamePad_RightThumbstickLeft_Click ;
	static const FName GSXR_GamePad_RightThumbstickUp_Click ;
	static const FName GSXR_GamePad_LeftThumbstick_Click ;
	static const FName GSXR_GamePad_RightThumbstick_Click ;
	static const FName GSXR_GamePad_LeftShoulder_Click ;
	static const FName GSXR_GamePad_RightShoulder_Click ;
	static const FName GSXR_GamePad_RightTrigger_Click ;
	static const FName GSXR_GamePad_LeftTrigger_Click ;


	static const FName GSXR_GamePad_TriggerLeft_Axis ;
	static const FName GSXR_GamePad_TriggerRight_Axis ;
	static const FName GSXR_GamePad_LeftThumbstickX_Axis ;
	static const FName GSXR_GamePad_LeftThumbstickY_Axis ;
	static const FName GSXR_GamePad_RightThumbstickX_Axis ;
	static const FName GSXR_GamePad_RightThumbstickY_Axis ;

	static const FName GSXR_HMD_Enter_Click ;
	static const FName GSXR_HMD_Back_Click ;
	static const FName GSXR_HMD_VolumeUp_Click ;
	static const FName GSXR_HMD_VolumeDown_Click ;
	static const FName GSXR_HMD_Home_Click ;
};

struct FGSXRHMDButtonState
{
	FName ButtonName;
	bool bChanged;
	bool bCurrentState;
	uint64_t InputMask;
#if USE_GSXRAPI
	GSXR_Hmd_InputId InputId;
#endif
	FGSXRHMDButtonState(
		FName InButtonName
#if USE_GSXRAPI
		,GSXR_Hmd_InputId InInputId
#endif
		):
	ButtonName(InButtonName)
	,bChanged(false)
	,bCurrentState(false)
#if USE_GSXRAPI
	,InputId(InInputId)
#endif
	{
#if USE_GSXRAPI
		InputMask = GSXRMaskFromId(InputId);
#endif
	}
};

enum class EGSXRActionType
{
	Boolean,
	Vector1,
	Vector2,
};

struct FGSXRActionSet
{
	FString ActionSetName;
	FString Usage;
#if USE_GSXRAPI
	GSXR_XrActionSetHandle_t ActionSetHandle;
#endif
};

struct FGSXRInputAction
{
	FString				ActionPath;
	FName				ActionName;	
	EGSXRActionType		ActionType;
	FName				KeyX;						
	FName				KeyY;						
	FName				KeyZ;
	FVector				Value;
	FString				StringPath;	
	bool				bState;						
	bool				bRequirement;
	bool				bIsRepeat;
	float				LastUpdated;
	FString				ControllerType;	
	FString				ButtonActionType;
	FString				ButtonPrivateName;
	FString				ButtonMode;
	FString				ButtonPath;
#if USE_GSXRAPI	
	GSXR_XrActionHandle_t	ActionHandle;
	GSXR_XrActionSetHandle_t ActionSetHandle;
#endif	
};


class FGSXRHMD;
class FGSXRInput :public IInputDevice,public FXRMotionControllerBase,public IHapticDevice,public TSharedFromThis<FGSXRInput>
{
public:
	FGSXRInput(const TSharedRef< FGenericApplicationMessageHandler >& InMessageHandler);
	virtual ~FGSXRInput() override;

	// IInputDevice overrides
	virtual void Tick( float DeltaTime ) override;
	virtual void SendControllerEvents() override;
	virtual void SetMessageHandler( const TSharedRef< FGenericApplicationMessageHandler >& InMessageHandler ) override;
	virtual bool Exec( UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar ) override;
	virtual void SetChannelValue( int32 ControllerId, FForceFeedbackChannelType ChannelType, float Value ) override;
	virtual void SetChannelValues( int32 ControllerId, const FForceFeedbackValues& Values ) override;

	// IMotionController overrides
	virtual FName GetMotionControllerDeviceTypeName() const override;
	virtual bool GetControllerOrientationAndPosition( const int32 ControllerIndex, const EControllerHand DeviceHand, FRotator& OutOrientation, FVector& OutPosition, float WorldToMetersScale ) const override;
	virtual ETrackingStatus GetControllerTrackingStatus(const int32 ControllerIndex, const EControllerHand DeviceHand) const override;

	// IHapticDevice overrides
	virtual IHapticDevice* GetHapticDevice() override { return this; }
	virtual void SetHapticFeedbackValues(int32 ControllerId, int32 Hand, const FHapticFeedbackValues& Values) override;

	virtual void GetHapticFrequencyRange(float& MinFrequency, float& MaxFrequency) const override;
	virtual float GetHapticAmplitudeScale() const override;
	FString GetManifestPath();
	
	void OnDeviceStateChange();

private:
	TSharedRef<FGenericApplicationMessageHandler> MessageHandler;
	void ApplicationPauseDelegate();
	void ApplicationResumeDelegate();
	void UpdateConnectState();
	void AddKeyToEngine();
	void ProcessActionEvents();
	void ProcessHMDButtonEvents();

	void GenerateManifestFile(const UInputSettings* InputSettings);
	void GenerateActionBinding(const UInputSettings* InputSettings,FString ControllerType);
	void GenerateActionMappings(const UInputSettings* InputSettings);
	TArray<FGSXRInputAction>	Gsxr_ActionMappings;
	TArray<FGSXRInputAction>	ActionEvents;
	TArray<FGSXRActionSet>		ActionSets;
	UGSXRInstance*				Gsxr_Instance;
	bool bLeftControllerState;
	bool bRightControllerState;
	TArray<FGSXRHMDButtonState> HMDButtons;
};
