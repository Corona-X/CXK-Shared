/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* XKBootConfig - Basic Kernel Utilities                          */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* beeselmane - 4.14.2017 -  9:15 PM PST                          */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/

#ifndef __KERNEL_SHARED_XKBOOTCONFIG__
#define __KERNEL_SHARED_XKBOOTCONFIG__ 1

#include <Corona-X.h>
#include <System/OSCompilerMacros.h>
#include <System/OSTypes.h>

#if !kCXAssemblyCode

typedef struct {
    OSCount entryCount;
    OSAddress *entries;
} XKBootConfig;

#if kCXKernelCode || kCXBootloaderCode
    OSShared SInt64 XKBootConfigGetNumber(XKBootConfig *config, const OSUTF8Char *key, SInt64 defaultValue);
    OSShared bool XKBootConfigGetBool(XKBootConfig *config, const OSUTF8Char *key, bool defaultValue);

    OSExport XKBootConfig *gXKBootConfig;
#endif /* kCKKernelCode || kCKBootloaderCode */

#endif /* kCXAssemblyCode */

#endif /* !defined(__KERNEL_SHARED_XKBOOTCONFIG__) */
