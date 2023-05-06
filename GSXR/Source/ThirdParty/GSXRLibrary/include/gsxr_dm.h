//
// Created by Admin on 2021/8/3.
//

#ifndef GSXR_DEV_LIB_GSXR_DM_H
#define GSXR_DEV_LIB_GSXR_DM_H
#include "gsxr_device.h"
#include "begin_code.h"
#ifdef __cplusplus
extern "C" {
#endif

//厂商信息
typedef struct GSXR_Manufacturer_Info {
    char    name[GSXR_COMMON_STRING_MAX_SIZE];           // 厂商名称
    char    site[GSXR_COMMON_STRING_MAX_SIZE];           // 厂商地址
    char    http[GSXR_COMMON_STRING_MAX_SIZE];           // 厂商官网
    char    phone[GSXR_COMMON_STRING_MAX_SIZE];          // 厂商电话
} GSXR_Manufacturer_Info;

//时间
typedef struct GSXR_TimeS {
    long        time;           //时间精确到毫秒
    uint32_t    year;           // 年
    uint32_t    month;          // 月
    uint32_t    day;            // 日
    uint32_t    hour;           // 时
    uint32_t    minute;         // 分
    uint32_t    second;         // 秒
} GSXR_TimeS;

//设备所使用的操作系统及版本号
typedef struct GSXR_Operation_Info {
    char    operation_system[GSXR_COMMON_STRING_MAX_SIZE];           // 操作系统
    char    version_number[GSXR_COMMON_STRING_MAX_SIZE];             // 版本号
} GSXR_Operation_Info;

//设备所使用的固件名称及版本号
typedef struct GSXR_Firmware_Info {
    char    firmware[GSXR_COMMON_STRING_MAX_SIZE];                  // 固件名称
    char    version_number[GSXR_COMMON_STRING_MAX_SIZE];            // 版本号
} GSXR_Firmware_Info;

//设备CPU/GPU信息
typedef struct GSXR_Device_CPU_GPU_Info {
    char    name[GSXR_COMMON_STRING_MAX_SIZE];            // 型号名称
    int     kernel_num;                                   // 核心数
} GSXR_Device_CPU_GPU_Info;

//分辨率
typedef struct GSXR_Resolution_Info {
    uint32_t                    width;                  // 屏幕宽度(pixel) 刘哥获取
    uint32_t                    height;
} GSXR_Resolution_Info;

//DM数据结构体基础信息
typedef struct GSXR_DM_Reported_Info {
    char                            sole_mark[GSXR_COMMON_STRING_MAX_SIZE];             // 设备唯一号
    char                            device_type[GSXR_COMMON_STRING_MAX_SIZE];           // 设备型号名
    char                            algorithm_version[GSXR_COMMON_STRING_MAX_SIZE];     // 厂商标识
    char                            platform[GSXR_COMMON_STRING_MAX_SIZE];              // 设备所使用的硬件芯片平台版本
    char                            versions_name[GSXR_COMMON_STRING_MAX_SIZE];         // 主版本名称
    char                            model[GSXR_COMMON_STRING_MAX_SIZE];                 // 主版本名称型号
    char                            sdk_version[GSXR_COMMON_STRING_MAX_SIZE];           // 设备所使用的的SDK版本号
    char                            ip_site[GSXR_COMMON_STRING_MAX_SIZE];               // 设备ip地址
    char                            mac_site[GSXR_COMMON_STRING_MAX_SIZE];              // 设备MAC地址
    char                            rom[GSXR_COMMON_STRING_MAX_SIZE];                   // 设备ROM信息(单位kb)
    char                            ram[GSXR_COMMON_STRING_MAX_SIZE];                   // 设备RAM信息(单位kb)
    GSXR_Resolution_Info            resolution;                                         // 屏幕分辨率
    GSXR_TimeS                      create_time;                                        // 设备创建时间
    GSXR_Operation_Info             operation_info;                                     // 设备操作系统及版本号信息
    GSXR_Firmware_Info              firmware_info;                                      // 设备所使用的固件名称及版本号
    GSXR_Manufacturer_Info          manufacturer_info;                                  // 厂商信息
    GSXR_Device_CPU_GPU_Info        cpu_info;                                           //设备CPU信息
    GSXR_Device_CPU_GPU_Info        gpu_info;                                           //设备GPU信息

} GSXR_DM_Reported_Info;

/**
 *  说明：
 *      获取设备DM数据结构基础信息
 *  参数：
 *       [in] runtime GSXR_Runtime 句柄, 由 GSXR_CreateRuntime 获取
 *      [out] batteryStatus 指向 GSXR_DM_Reported_Info 结构体之指针
 *  返回值：
 *      GSXR_Result_Success
 *      GSXR_Error_Operation_Failed
 *      GSXR_Error_Function_Unsupported
 *      GSXR_Error_Device_Disconnected
 *      GSXR_Error_Handle_Invalid
 *      GSXR_Error_Argument_Invalid
 *      GSXR_Error_Runtime_Unavailable
 *  备注：
 *      无
 *  所属：
 *      GSXR_API_设备信息类
 */
extern GSXR_EXPORT GSXR_Result GSXR_DMReportedInfo(
        GSXR_Runtime            runtime,
GSXR_DM_Reported_Info*     reported);

//应用信息
typedef struct GSXR_App_Info {
    char                app_name[GSXR_COMMON_STRING_MAX_SIZE];               // 应用名称
    char                package_name[GSXR_COMMON_STRING_MAX_SIZE];           // 包名
    long                begin_time_len;                                      // 启动时间以毫秒表示的Unix时间
    long                end_time_len;                                        // 关闭时间以毫秒表示的Unix时间
} GSXR_App_Info;

// 定位模式
typedef enum class GSXR_Device_Location_Mode : uint64_t{
    NOT                         = 0x00000000,        //无定位
    GPS                         = 0x00000001,        //卫星定位(GPS，北斗，伽利略，Glonass)
    BASE                        = 0x00000002,        //移动基站定位
    WIFI                        = 0x00000004,        //WiFi辅助定位
    AGPS                        = 0x00000008,        //AGPS定位
}GSXR_Device_Location_Mode;

// 经纬度
typedef struct GSXR_Device_Long_Lat {
    float      longitude;       // 经度
    float      latitude;        // 纬度
} GSXR_Device_Long_Lat;

// 采集码表信息 业务埋点信息
typedef struct GSXR_Buried_Point_Info {
    char                            stopwatch[GSXR_COMMON_STRING_MAX_SIZE];             // 码表信息
    char                            Buried_Point[GSXR_COMMON_STRING_MAX_SIZE];          // 埋点信息
} GSXR_Buried_Point_Info;

// 开始 结束信息
typedef struct GSXR_Start_End_Info {
    float           start;          // 开始
    float           end;            // 结束
    GSXR_TimeS     set_time;       // 时间
} GSXR_Start_End_Info;

//充电信息
typedef struct GSXR_Top_Electricity_Info {
    GSXR_TimeS         start;                           // 开始充电时间
    GSXR_TimeS         end;                             // 结束充电时间
    float              start_temperature;               // 开始充电温度
    float              end_temperature;                 // 结束充电温度
    float              start_voltage;                   // 开始充电电压
    float              end_voltage;                     // 结束充电电压
} GSXR_Top_Electricity_Info;



//采集信息
typedef struct GSXR_Timing_Gather_Info {
    float       value;                        //取值 cpu温度、ROM使用率、剩余电量、亮度
    GSXR_TimeS now_time;                     //当前时间
}GSXR_Timing_Gather_Info;

// 定时采集信息
typedef enum class GSXR_Timing_Gather : uint64_t {
    CPU_TEMPERATURE                             = 0x00000001,  //cpu温度警告 三分钟检测一次cpu温度，超过90摄氏度累计值+1，采集累计告警次数
    ROM_USAGE_RATE                              = 0x00000002,  //ROM使用率 没半小时采集一次当前ROM使用占比，采集当前时间和ROM使用占比
    ELECTRICITY_QUANTITY                        = 0x00000004,  //电量信息 每半小时采集一次当前电池剩余电量，采集当前时间和电池剩余电量
}GSXR_Timing_Gather;

//DM数据结构体使用信息
typedef struct GSXR_Dm_Reported_Use_Info{
    GSXR_App_Info                         app_info;          //设备上各应用启动次数及时长
    uint32_t                              location_mode;          //定位模式 gsxr_app_use_info_list_t的位掩码
    GSXR_Device_Long_Lat                  long_lat;               //经纬度
    GSXR_Buried_Point_Info                Buried_point_info;      //码表信息和业务埋点信息
    GSXR_Timing_Gather_Info               start_screen;           //点亮屏幕开始信息
    GSXR_Timing_Gather_Info               end_screen;             //点亮屏幕结束信息
    GSXR_Timing_Gather_Info               start_luminance_info;   //亮度开始调节时间及亮度值
    GSXR_Timing_Gather_Info               end_luminance_info;     //亮度结束调节时间及亮度值
    GSXR_Timing_Gather_Info               start_volume_info;      //音量开始调节时间及音量值
    GSXR_Timing_Gather_Info               end_volume_info;        //音量结束调节时间及音量值
    GSXR_Top_Electricity_Info             electricity_info;       //采集开始充电时间,结束充电时间,开始充电温度,结柬充电温度开始充电电压,结束充电电压开始充电|电池状态,结束充电电池状态,本次充电时长
}GSXR_Dm_Reported_Use_Info;




/**
 *  说明：
 *      获取指定设备DM数据
 *  参数：
 *       [in] runtime GSXR_Runtime 句柄, 由 GSXR_CreateRuntime 获取
 *       [in] deviceType 指定设备类型
 *       [in] timing_gather_Type 为GSXR_Timing_Gather的掩码指定请求数据类型
 *      [out] batteryStatus 指向 GSXR_DM_Reported_Info 结构体之指针
 *  返回值：
 *      GSXR_Result_Success
 *      GSXR_Error_Operation_Failed
 *      GSXR_Error_Function_Unsupported
 *      GSXR_Error_Device_Disconnected
 *      GSXR_Error_Handle_Invalid
 *      GSXR_Error_Argument_Invalid
 *      GSXR_Error_Runtime_Unavailable
 *  备注：
 *      无
 *  所属：
 *      GSXR_API_设备信息类
 */
extern GSXR_EXPORT GSXR_Result GSXR_GetDMData(
        GSXR_Runtime                 runtime,
        GSXR_DeviceType              deviceType,
        GSXR_Timing_Gather           timing_gather_Type,
        GSXR_Timing_Gather_Info*   timing_gather);

/**
 *  说明：
 *      设备DM信息事件回调函数, 当设备触发相关DM使用信息时回调
 *  参数：
 *       [in] dm_use DM使用信息
 *  返回值：
 *      无
 *  备注：
 *      当发生指定使用信息事件时，只需填写相关事件参数即可其他无用到参数需置0
 *  所属：
 *      GSXR_API_设备信息函数
 */
typedef void (*GSXR_DmEventCallbackFn)(GSXR_Dm_Reported_Use_Info dm_use);

/**
 *  说明：
 *      设置事件回调函数, 各设备于指定DM数据事件发生时回调
 *  参数：
 *       [in] runtime GSXR_Runtime 句柄, 由 GSXR_CreateRuntime 获取
 *       [in] callback 事件回调函数指针
 *
 *  返回值：
 *      GSXR_Result_Success
 *      GSXR_Error_Operation_Failed
 *      GSXR_Error_Function_Unsupported
 *      GSXR_Error_Device_Disconnected
 *      GSXR_Error_Handle_Invalid
 *      GSXR_Error_Argument_Invalid
 *      GSXR_Error_Runtime_Unavailable
 *  备注：
 *      当发生指定使用信息事件时，只需填写相关事件参数即可其他无用到参数需置0
 *
 *  所属：
 *      GSXR_API_设备信息类
 */
extern GSXR_EXPORT GSXR_Result GSXR_SetDmEventCallback(
        GSXR_Runtime                 runtime,
        GSXR_DmEventCallbackFn       callback);


#ifdef __cplusplus
} /* extern "C" */
#endif
#include "close_code.h"
#endif //GSXR_DEV_LIB_GSXR_DM_H
