#ifndef GSXR_PLANE_TRACKER_H_
#define GSXR_PLANE_TRACKER_H_

/**
 *  Declare GSXR Copyright here.
 */

#include "gsxr_xrfeature.h"
#include "begin_code.h"

#ifdef __cplusplus
extern "C" {
#endif

// 枚举平面特性之功能选项
typedef enum GSXR_PlaneOptions {
    GSXR_PlaneOptions_Horizontal_DownwardFacing = 0x00000001,
    GSXR_PlaneOptions_Horizontal_UpwardFacing   = 0x00000002,
    GSXR_PlaneOptions_Vertical                  = 0x00000004,
} GSXR_PlaneOptions;

// ------------------------------------
// GSXR_API_平面数据类
// ------------------------------------

// 枚举平面类型
typedef enum GSXR_PlaneType {
    GSXR_PlaneType_Invalid                      = 0,  // 无效的平面
    GSXR_PlaneType_Horizontal_DownwardFacing    = 1,  // 正面朝下的水平面, 如天花板
    GSXR_PlaneType_Horizontal_UpwardFacing      = 2,  // 正面朝上的水平面, 如地板,桌面
    GSXR_PlaneType_Vertical                     = 3,  // 垂直平面, 如墙面
} GSXR_PlaneType;

// 平面顶点数据结构体
typedef struct GSXR_PlaneVertexData {
    float   x;
    float   z;
} GSXR_PlaneVertexData;

// 平面数据结构体
typedef struct GSXR_PlaneData {
    void*                   next;           // 预留指针
    uint32_t                id;             // 平面识别号
    GSXR_PlaneType          type;           // 平面类型
    GSXR_Posef              center;         // 平面中心点
    float                   extentX;        // 以 center 为中心点的延伸平面 x 边长度, 单位为米
    float                   extentZ;        // 以 center 为中心点的延伸平面 z 边长度, 单位为米
    uint32_t                vertexCount;    // 平面顶点集合的点个数
    GSXR_PlaneVertexData*   vertices;       // 平面顶点集合, 以 center 为中心点围绕二维凸面多边形平面
} GSXR_PlaneData;

// 平面追踪数据结构体
typedef struct GSXR_PlaneTrackerData {
    void*                   next;           // 预留指针
    GSXR_Time               timestamp;      // 平面数据时间戳
    uint32_t                maxCount;       // 平面算法支持的最大平面个数
    uint32_t                planeCount;     // 平面集合的平面个数
    GSXR_PlaneData*         planes;         // 平面集合, 平面数据 0 <= id < planeCount
} GSXR_PlaneTrackerData;

/**
 *  说明：
 *      获得平面数据
 *  参数：
 *       [in] feature GSXR_XrFeature 句柄, 由 GSXR_StartXrFeature 获取
 *      [out] planeTracker 指向 GSXR_PlaneTrackerData 指针
 *  返回值：
 *      GSXR_Result_Success
 *      GSXR_Error_Operation_Failed
 *      GSXR_Error_Feature_Unsupported
 *      GSXR_Error_Handle_Invalid
 *      GSXR_Error_Argument_Invalid
 *      GSXR_Error_Out_Of_Memory
 *      GSXR_Error_Runtime_Unavailable
 *      GSXR_Error_Feature_Unavailable
 *  备注：
 *      planeTracker 指针指向的 GSXR_PlaneTrackerData 指针指向平面侦测特性的 GSXR_PlaneTrackerData 结构,
 *      应用程序用毕平面数据必须调用 GSXR_ReleasePlaneTrackerData 释放平面内存
 *  所属：
 *      GSXR_API_平面数据类
 */
extern GSXR_EXPORT GSXR_Result GSXR_AcquirePlaneTrackerData(
    GSXR_XrFeature              feature,
    GSXR_PlaneTrackerData**     planeTracker);

/**
 *  说明：
 *      释放平面数据
 *  参数：
 *       [in] feature GSXR_XrFeature 句柄, 由 GSXR_StartXrFeature 获取
 *      [out] planeTracker GSXR_PlaneTrackerData 指针, 由 GSXR_AcquirePlaneTrackerData 获取
 *  返回值：
 *      GSXR_Result_Success
 *      GSXR_Error_Operation_Failed
 *      GSXR_Error_Feature_Unsupported
 *      GSXR_Error_Handle_Invalid
 *      GSXR_Error_Argument_Invalid
 *      GSXR_Error_Call_Flow_Invalid
 *      GSXR_Error_Runtime_Unavailable
 *      GSXR_Error_Feature_Unavailable
 *  备注：
 *      无
 *  所属：
 *      GSXR_API_平面数据类
 */
extern GSXR_EXPORT GSXR_Result GSXR_ReleasePlaneTrackerData(
    GSXR_XrFeature              feature,
    GSXR_PlaneTrackerData*      planeTracker);

#ifdef __cplusplus
} /* extern "C" */
#endif

#include "close_code.h"

#endif /* GSXR_PLANE_TRACKER_H_ */
