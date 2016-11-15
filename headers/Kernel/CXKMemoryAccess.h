/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* CXKMemoryAccess.h - CPU Memory Access Structures                */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* beeselmane - 2.10.2016  - 6:45 PM EST                           */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* beeselmane - 2.10.2016  - 6:45 PM EST                           */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/

#ifndef __KERNEL_CXKMEMORYACCESS__
#define __KERNEL_CXKMEMORYACCESS__ 1

typedef struct {
    UInt64 limit0:16;
    UInt64 base0:24;
    UInt64 type:4;
    UInt64 isCodeOrData:1;
    UInt64 priviledgeLevel:2;
    UInt64 isPresent:1;
    UInt64 limit1:4;
    UInt64 empty:1;
    UInt64 is64bit:1;
    UInt64 codeSize:1;
    UInt64 granularity;
    UInt64 base1:8;
} CXKProcessorSegmentDescriptor;

typedef struct {
    UInt16 index:13;
    UInt16 useLocalTable:1;
    UInt16 priviledgeLevel:1;
} CXKProcessorSegmentSelector;

#endif /* !defined(__KERNEL_CXKMEMORYACCESS__) */
