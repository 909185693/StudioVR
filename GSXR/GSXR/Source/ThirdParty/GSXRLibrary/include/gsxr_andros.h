#ifndef GSXR_ANDROS_H_
#define GSXR_ANDROS_H_

/**
 *  Declare GSXR Copyright here.
 */

#include "gsxr_types.h"
#include "begin_code.h"

#ifdef __cplusplus
extern "C" {
#endif

// ------------------------------------
// GSXR_API_移动系统特化类
// ------------------------------------

// 移动系统特化资讯结构体
typedef struct GSXR_AndrosSystemBinding {
    void*   next;                   // 预留指针
    void*   androsAppVM;            // 指向 JNI JavaVM 结构体
    void*   androsAppActivity;      // Activity 的 jobject 参考
} GSXR_AndrosSystemBinding;

#define GSXR_EGL_EXTENSION_MAX_SIZE 16

typedef int64_t GSXR_EglExtension;

// OpenGL ES 特化资讯结构体
typedef struct GSXR_AndrosOpenGLESBinding {
    void*                   next;                                       // 预留指针
    void*                   eglDisplay;                                 // OpenGL ES EGLDisplay
    void*                   eglContext;                                 // OpenGL ES EGLContext
    uint32_t                eglExtensionsCount;                         // 有效的 EGL Extension 数组个数, 通常为 0, 最大为 16
    GSXR_EglExtension       eglExtensions[GSXR_EGL_EXTENSION_MAX_SIZE]; // 设置应用需求的 EGL Extension 定义数值
} GSXR_AndrosOpenGLESBinding;

#ifdef __cplusplus
} /* extern "C" */
#endif

#include "close_code.h"

#endif /* GSXR_ANDROS_H_ */
