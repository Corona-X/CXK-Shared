/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* XKPOST.h - Accessing POST Codes                                 */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* beeselmane - 21.9.2016  - 8:15 PM EST                           */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/

#ifndef __KERNEL_XKPOST__
#define __KERNEL_XKPOST__ 1

#include <Corona-X.h>
#include <System/OSTypes.h>

#if kCXArchIA

#if kCXBuildDev
    #define kXKDefaultPOSTPort 0x80

    #if kCXAssemblyCode
        #define XKSetPOSTCode(c)               \
            movb $(c), %al;                     \
            outb %al, $(kXKDefaultPOSTPort)
    #else /* !kCXAssemblyCode */
        #include <Kernel/XKPortIO.h>

        #define XKSetPOSTCode(c) XKWriteByte((OSAddress)kXKDefaultPOSTPort, ((XKPOSTCode)c))

        typedef UInt8 XKPOSTCode;
    #endif /* Assembly Code */
#else /* !kCXBuildDev */
    #if !kCXAssemblyCode
        typedef UInt8 XKPOSTCode;
    #endif /* !kCXAssemblyCode */
#endif /* kCXBuildDev */

#endif /* Architecture */

#endif /* !defined(__KERNEL_XKPOST__) */
