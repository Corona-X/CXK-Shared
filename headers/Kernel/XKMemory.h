/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* XKMemory.h - Common Memory Operations                           */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* beeselmane - 18.9.2016  - 2:30 PM EST                           */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/

#ifndef __KERNEL_XKBUFFER__
#define __KERNEL_XKBUFFER__ 1

#include <Corona-X.h>
#include <System/OSCompilerMacros.h>
#include <System/OSTypes.h>

#if !kCXAssemblyCode

OSShared void XKBufferSetValue(OSBuffer buffer, UInt8 value);
OSShared void XKBufferCopy(OSAddress input, OSBuffer output);

OSShared void XKMemorySetValue(OSAddress address, OSSize size, UInt8 value);
OSShared void XKMemoryCopy(OSAddress input, OSAddress output, OSSize size);

OSShared SInt16 XKMemoryCompare(OSAddress first, OSAddress second, OSSize size);

OSShared SInt16 XKStringCompare8(UInt8 *first, UInt8 *second);

OSShared OSSize XKStringSize32(UInt32 *utf32);
OSShared OSSize XKStringSize16(UInt16 *utf16);
OSShared OSSize XKStringSize8(UInt8 *utf8);

#endif /* kCXAssemblyCode */

#endif /* !defined(__KERNEL_XKBUFFER__) */
