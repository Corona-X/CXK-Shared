/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* XKSharedTarget.h - Macros for different shared library targets  */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* beeselmane - 1.8.2017   - 1:15 PM EST                           */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/

#ifndef __KERNEL_SHAREDTARGET__
#define __KERNEL_SHAREDTARGET__ 1

#include <Corona-X.h>

#if kCXBootloaderCode
    #include <SystemLoader/SLMemoryAllocator.h>
    #include <SystemLoader/EFI/SLBootServices.h>
    #include <SystemLoader/SLLibrary.h>

    #define XKAllocate      SLAllocate
    #define XKReallocate    SLReallocate
    #define XKFree          SLFree
    #define XKDelay(t)      SLDelayProcessor(t, !SLBootServicesHaveTerminated())
#else /* Kernel */

#endif /* Targets */

#endif /* !defined(__KERNEL_SHAREDTARGET__) */
