/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* XKShared.h - Include all Shared Library Header for the Kernel   */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* beeselmane - 20.11.2016 - 10:15 AM EST                          */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/

#ifndef __KERNEL_XKSHARED__
#define __KERNEL_XKSHARED__ 1

#include <Corona-X.h>

#include <System/OSTypes.h>
#include <Kernel/XKAssembly.h>
#include <Kernel/XKMemory.h>
#include <Kernel/XKPortIO.h>
#include <Kernel/XKPOST.h>
#include <Kernel/XKProcessorState.h>

#if kCXBuildDev
    // Debug logging function. Assumes serial port at default address
    // is already initialized. Only kernel shared logging function.
    OSShared void XKPrintString(const char *format, ...);
#else
    #define XKPrintString(...)
#endif

#endif /* !defined(__KERNEL_XKSHARED__) */
