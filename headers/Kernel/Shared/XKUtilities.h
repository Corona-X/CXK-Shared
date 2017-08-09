/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* XKUtilities.h - Basic Kernel Utilities                          */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* beeselmane - 4.14.2017  -  9:15 PM PST                          */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/

#ifndef __KERNEL_XKUTILITIES__
#define __KERNEL_XKUTILITIES__ 1

#include <Corona-X.h>
#include <System/OSCompilerMacros.h>
#include <System/OSTypes.h>

#if !kCXAssemblyCode

typedef struct {
    UInt32 date;
    UInt64 nanosecond;
} XKTime;

OSShared XKTime XKTimeGetCurrent(void);

#endif /* kCXAssemblyCode */

#endif /* !defined(__KERNEL_XKUTILITIES__) */

/*

 // Copy until character
 void    *memccpy(void *restrict, const void *restrict, int, size_t);
 // Locate byte in string
 void    *memchr(const void *, int, size_t);
 
 */
