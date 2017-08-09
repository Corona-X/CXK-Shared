/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* XKMemory.h - Standard Memory Functions                          */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* beeselmane - 8.8.2017   - 5:00 PM EST                           */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/

#ifndef __KERNEL_C_XKMEMORY__
#define __KERNEL_C_XKMEMORY__ 1

#include <Corona-X.h>
#include <System/OSTypes.h>
#include <System/OSCompilerMacros.h>

OSShared void XKMemorySetValue(OSAddress address, OSSize size, UInt8 value);
OSShared void XKMemoryCopy(OSAddress input, OSAddress output, OSSize size);
OSShared SInt16 XKMemoryCompare(OSAddress first, OSAddress second, OSSize size);

// TODO: Write these functions
OSShared SInt64 XKStringCompare32(UInt32 *first, UInt32 *second);
OSShared SInt32 XKStringCompare16(UInt16 *first, UInt16 *second);
OSShared SInt16 XKStringCompare8 (UInt8  *first, UInt8  *second);

//#define XKMemorySetValue(address, size, value)  __builtin_memset(address, value, size)
//#define XKMemoryCopy(input, output, size)       __builtin_memcpy(output, input, size)
//#define XKMemoryCompare(first, second, size)    __builtin_memcmp(first, second, size)

//#define XKStringCompare8(first, second)         __builtin_strcmp(first, second)

#endif /* !defined(__KERNEL_C_XKMEMORY__) */
