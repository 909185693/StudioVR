/**
 *  Declare GSXR Copyright here.
 */

#ifdef _begin_code_h
#error "Nested inclusion of begin_code.h"
#endif
#define _begin_code_h

#ifndef GSXR_EXPORT
# if defined(__GNUC__) && __GNUC__ >=4
#  define GSXR_EXPORT __attribute__ ((visibility("default")))
# else
#  define GSXR_EXPORT
# endif
#endif
