/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* CLMemory.h - Standard Memory Functions                          */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* beeselmane - 8.8.2017   - 5:00 PM EST                           */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/

#ifndef __KERNEL_C_CLMEMORY__
#define __KERNEL_C_CLMEMORY__ 1

#include <Corona-X.h>
#include <System/OSTypes.h>
#include <System/OSCompilerMacros.h>

OSShared void CLMemorySetValue(OSAddress address, OSSize size, UInt8 value);
OSShared void CLMemoryCopy(OSAddress input, OSAddress output, OSSize size);
OSShared SInt16 CLMemoryCompare(OSAddress first, OSAddress second, OSSize size);
OSShared void CLMemoryZero(OSAddress address, OSSize size);

// TODO: Write these functions
OSShared SInt64 CLStringCompare32(OSUTF32Char *first, UInt32 *second);
OSShared SInt32 CLStringCompare16(OSUTF16Char *first, UInt16 *second);
OSShared SInt16 CLStringCompare8 (OSUTF8Char  *first, UInt8  *second);

//#define CLMemorySetValue(address, size, value)  __builtin_memset(address, value, size)
//#define CLMemoryCopy(input, output, size)       __builtin_memcpy(output, input, size)
//#define CLMemoryCompare(first, second, size)    __builtin_memcmp(first, second, size)

//#define CLStringCompare8(first, second)         __builtin_strcmp(first, second)

#endif /* !defined(__KERNEL_C_CLMEMORY__) */
