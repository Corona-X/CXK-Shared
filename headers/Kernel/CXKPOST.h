/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* CXKPOST.h - Accessing POST Codes                                */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* beeselmane - 21.9.2016  - 8:15 PM EST                           */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* beeselmane - 26.9.2016  - 10:45 PM EST                          */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/

#ifndef __KERNEL_CXKPOST__
#define __KERNEL_CXKPOST__ 1

#include <Corona-X.h>
#include <System/OSTypes.h>

#if kCXArchIA

#if kCXBuildDev
    #define kCXKDefaultPOSTPort 0x80

    #if kCXAssemblyCode
        #define CXKSetPOSTCode(c)               \
            movb $(c), %al;                     \
            outb %al, $(kCXKDefaultPOSTPort)
    #else /* !kCXAssemblyCode */
        #include <Kernel/CXKMemoryIO.h>

        #define CXKSetPOSTCode(c) CXKWriteByte((OSAddress)kCXKDefaultPOSTPort, ((CXKPOSTCode)c))

        typedef UInt8 CXKPOSTCode;
    #endif /* Assembly Code */
#else /* !kCXBuildDev */
    #if !kCXAssemblyCode
        typedef UInt8 CXKPOSTCode;
    #endif /* !kCXAssemblyCode */
#endif /* kCXBuildDev */

#endif /* Architecture */

#endif /* !defined(__KERNEL_CXKPOST__) */
