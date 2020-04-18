/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* XKBootConfig.h - Functions for reading kernel configs           */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* beeselmane - 20.11.2018 -  5:00 PM TP                           */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/

#ifndef __KERNEL_SHARED_XKBOOTCONFIG__
#define __KERNEL_SHARED_XKBOOTCONFIG__ 1

#include <Corona-X.h>
#include <System/OSCompilerMacros.h>
#include <System/OSTypes.h>

#if !kCXAssemblyCode

// A note on the format of this structure:
// The `entries` field is a pointer to a list of (key, value) pairs.
// Each (key, value) pair is simply comprised of two pointers.
// This structure is a `dumb` dictionary, it simply stores strings for keys.
typedef struct {
    OSCount entryCount;
    OSAddress *entries;
} XKBootConfig;

#if kCXKernelCode || kCXBootloaderCode
    OSShared bool XKBootConfigGetBool(XKBootConfig *config, const OSUTF8Char *key, bool defaultValue);
    OSShared SInt64 XKBootConfigGetNumber(XKBootConfig *config, const OSUTF8Char *key, SInt64 defaultValue);
    OSShared const OSUTF8Char *XKBootConfigGetString(XKBootConfig *config, const OSUTF8Char *key, const OSUTF8Char *defaultValue);

    OSExport XKBootConfig *gXKBootConfig;
#endif /* kCKKernelCode || kCKBootloaderCode */

#endif /* kCXAssemblyCode */

#endif /* !defined(__KERNEL_SHARED_XKBOOTCONFIG__) */
