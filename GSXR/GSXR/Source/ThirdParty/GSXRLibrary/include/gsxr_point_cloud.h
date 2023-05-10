#ifndef GSXR_POINT_CLOUD_H_
#define GSXR_POINT_CLOUD_H_

/**
 *  Declare GSXR Copyright here.
 */

#include "gsxr_xrfeature.h"
#include "begin_code.h"

#ifdef __cplusplus
extern "C" {
#endif

// 枚举点云特性之功能选项
typedef enum GSXR_PointCloudOptions {
    GSXR_PointCloudOptions_Reserved     = 0xFFFFFFFF,
} GSXR_PointCloudOptions;

// ------------------------------------
// GSXR_API_点云数据类
// ------------------------------------

// 点数据结构体
typedef struct GSXR_PointData {
    void*               next;           // 预留指针
    uint32_t            id;             // 点数据识别号
    GSXR_Vector3f       position;       // 位置
} GSXR_PointData;

// 点云数据结构体
typedef struct GSXR_PointCloudData {
    void*               next;           // 预留指针
    GSXR_Time           timestamp;      // 点云数据时间戳
    uint32_t            maxCount;       // 点云算法支持的最大点个数
    uint32_t            pointCount;     // 点集合的点个数
    GSXR_PointData*     points;         // 点集合, 点数据识别号 0 <= id < pointCount
} GSXR_PointCloudData;

/**
 *  说明：
 *      获得点云数据
 *  参数：
 *       [in] feature GSXR_XrFeature 句柄, 由 GSXR_StartXrFeature 获取
 *      [out] pointCloud 指向 GSXR_PointCloudData 指针
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
 *      pointCloud 指针指向的 GSXR_PointCloudData 指针指向点云特性的 GSXR_PointCloudData 结构,
 *      应用程序用毕点云数据必须调用 GSXR_ReleasePointCloudData 释放点云内存
 *  所属：
 *      GSXR_API_点云数据类
 */
extern GSXR_EXPORT GSXR_Result GSXR_AcquirePointCloudData(
    GSXR_XrFeature          feature,
    GSXR_PointCloudData**   pointCloud);

/**
 *  说明：
 *      释放点云数据
 *  参数：
 *       [in] feature GSXR_XrFeature 句柄, 由 GSXR_StartXrFeature 获取
 *      [out] pointCloud GSXR_PointCloudData 指针, 由 GSXR_AcquirePointCloudData 获取
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
 *      GSXR_API_点云数据类
 */
extern GSXR_EXPORT GSXR_Result GSXR_ReleasePointCloudData(
    GSXR_XrFeature          feature,
    GSXR_PointCloudData*    pointCloud);

#ifdef __cplusplus
} /* extern "C" */
#endif

#include "close_code.h"

#endif /* GSXR_POINT_CLOUD_H_ */
