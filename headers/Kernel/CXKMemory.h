/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* CXKMemory.h - Common Memory Operations                          */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* beeselmane - 18.9.2016  - 2:30 PM EST                           */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* beeselmane - 18.9.2016  - 5:00 PM EST                           */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/

#ifndef __KERNEL_CXKBUFFER__
#define __KERNEL_CXKBUFFER__ 1

#include <Corona-X.h>
#include <System/OSTypes.h>
#include <System/OSCompilerMacros.h>

#if !kCXAssemblyCode

OSShared void CXKBufferSetValue(OSBuffer buffer, UInt8 value);
OSShared void CXKBufferCopy(OSAddress input, OSBuffer output);

OSShared void CXKMemorySetValue(OSAddress address, OSSize size, UInt8 value);
OSShared void CXKMemoryCopy(OSAddress input, OSAddress output, OSSize size);

OSShared SInt16 CXKMemoryCompare(OSAddress first, OSAddress second, OSSize size);

OSShared SInt16 CXKStringCompare8(UInt8 *first, UInt8 *second);

OSShared OSSize CXKStringSize32(UInt32 *utf32);
OSShared OSSize CXKStringSize16(UInt16 *utf16);
OSShared OSSize CXKStringSize8(UInt8 *utf8);

#endif /* kCXAssemblyCode */

#endif /* !defined(__KERNEL_CXKBUFFER__) */
