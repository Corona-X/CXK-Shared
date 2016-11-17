/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* OSEndian.h - Enumeration for System Endian                      */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* beeselmane - 27.11.2015 - 8:00 PM EST                           */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/

#ifndef __SYSTEM_OSENDIAN__
#define __SYSTEM_OSENDIAN__ 1

#include <Corona-X.h>
#include <System/OSCompilerMacros.h>
#include <System/OSTypes.h>

#if !kCXAssemblyCode

typedef CXEnum(UInt8, OSEndian) {
    kOSEndianLittle,
    kOSEndianBig
};

OSInline OSEndian OSEndianCurrent()
{
    if (kCXLittleEndian) return kOSEndianLittle;
    else                 return kOSEndianBig;
}

#endif /* !kCXAssemblyCode */

#endif /* !defined(__SYSTEM_OSENDIAN__) */
