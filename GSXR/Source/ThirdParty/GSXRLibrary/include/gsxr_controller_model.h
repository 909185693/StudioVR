#ifndef GSXR_CONTROLLER_MODEL_H_
#define GSXR_CONTROLLER_MODEL_H_

/**
 *  Declare GSXR Copyright here.
 */

#include "gsxr_xrfeature.h"
#include "begin_code.h"

#ifdef __cplusplus
extern "C" {
#endif

// 枚举手柄模型特性之功能选项
typedef enum GSXR_ControllerModelOptions {
    GSXR_ControllerModelOptions_Reserved     = 0xFFFFFFFF,
} GSXR_ControllerModelOptions;

// ------------------------------------
// GSXR_API_手柄模型类
// ------------------------------------

// 顶点缓存结构体
typedef struct GSXR_VertexBuffer {
    void*       next;       // 预留指针
    float*      buffer;     // 顶点位置
    uint32_t    size;       // 顶点缓存大小
    uint32_t    dimension;  // 顶点维度, 2 为平面 (buffer 内容为 x, y, x, y, ...), 3 为立体 (buffer 内容为 x, y, z, x, y, z, ...)
} GSXR_VertexBuffer;

// 图元缓存结构体
typedef struct GSXR_IndexBuffer {
    void*       next;       // 预留指针
    uint32_t*   buffer;     // 几何图元的顶点索引
    uint32_t    size;       // 几何图元缓存大小
    uint32_t    type;       // 几何图元类型及组成顶点数, 1 为点 (buffer 以一组索引为单位), 2 为线 (buffer 以两组索引为单位), 3 为三角形 (buffer 以三组索引为单位)
} GSXR_IndexBuffer;

// 部件资讯结构体
typedef struct GSXR_ComponentInfo {
    void*               next;                               // 预留指针
    char                name[GSXR_COMMON_STRING_MAX_SIZE];  // 部件名称
    GSXR_VertexBuffer   vertices;                           // 部件顶点位置资讯 (顶点数目同 normals 及 texCoords, dimension 为 3)
    GSXR_VertexBuffer   normals;                            // 部件顶点法线资讯 (顶点数目同 vertices 及 texCoords, dimension 为 3)
    GSXR_VertexBuffer   texCoords;                          // 部件纹理位置资讯 (顶点数目同 vertices 及 normals, dimension 为 2)
    GSXR_IndexBuffer    indices;                            // 部件图元平面的顶点索引资讯
    int32_t             texIndex;                           // 部件纹理于纹理缓存数组的索引值
    float               localMatrix[16];                    // 部件模型原点相对于整体模型原点的本地端相对位姿
    GSXR_Bool32         defaultDraw;                        // 是否预设绘制此部件
} GSXR_ComponentInfo;

// 纹理 Bitmap 缓存结构体
typedef struct GSXR_TextureBitmap {
    void*           next;           // 预留指针
    uint8_t*        buffer;         // bitmap 缓存
    uint32_t        width;          // 纹理宽度, 像素
    uint32_t        height;         // 纹理高度, 像素
    int32_t         format;         // 纹理像素格式 (预设支持 RGBA_8888 格式, format 数值为 1)
    uint32_t        stride;         // 每一个 row 的位组大小 (bytes)
} GSXR_TextureBitmap;

// 手柄模型数据结构体
typedef struct GSXR_ControllerModelData {
    void*                   next;                               // 预留指针
    char                    name[GSXR_COMMON_STRING_MAX_SIZE];  // 手柄名称
    uint32_t                componentCount;                     // 部件数目
    GSXR_ComponentInfo*     components;                         // 部件资讯数组
    uint32_t                textureCount;                       // 纹理数目
    GSXR_TextureBitmap*     textures;                           // 纹理缓存数组
} GSXR_ControllerModelData;

/**
 *  说明：
 *      获得手柄模型数据
 *  参数：
 *       [in] feature GSXR_XrFeature 句柄, 由 GSXR_StartXrFeature 获取
 *      [out] controllerModel 指向 GSXR_ControllerModelData 指针
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
 *      controllerModel 指针指向的 GSXR_ControllerModelData 指针指向 Runtime 的 GSXR_ControllerModelData 结构,
 *      应用程序用毕手柄模型数据必须调用 GSXR_ReleaseControllerModelData 释放内存
 *  所属：
 *      GSXR_API_手柄模型类
 */
extern GSXR_EXPORT GSXR_Result GSXR_AcquireControllerModelData(
    GSXR_XrFeature                  feature,
    GSXR_ControllerModelData**      controllerModel,
    uint32_t*                       controllerModelSize);

/**
 *  说明：
 *      释放手柄模型数据
 *  参数：
 *       [in] feature GSXR_XrFeature 句柄, 由 GSXR_StartXrFeature 获取
 *      [out] controllerModel GSXR_ControllerModelData 指针, 由 GSXR_AcquireControllerModelData 获取
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
 *      GSXR_API_手柄模型类
 */
extern GSXR_EXPORT void GSXR_ReleaseControllerModelData(
    GSXR_XrFeature                  feature,
    GSXR_ControllerModelData*       controllerModel);


#ifdef __cplusplus
} /* extern "C" */
#endif

#include "close_code.h"

#endif /* GSXR_CONTROLLER_MODEL_H_ */
