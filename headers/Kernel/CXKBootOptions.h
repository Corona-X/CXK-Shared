/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* CXKBootOptions.h - Structure of first argument passed to kernel */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* beeselmane - 24.9.2016  - 1:15 PM EST                           */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/

#ifndef __KERNEL_CXKBOOTOPTIONS__
#define __KERNEL_CXKBOOTOPTIONS__ 1

#include <Corona-X.h>
#include <Kernel/CXKPOST.h>
#include <System/OSTypes.h>

#define kCXKMemoryMapEntryTypeReserved              0x0
#define kCXKMemoryMapEntryTypeOSLoaderCode          0x1
#define kCXKMemoryMapEntryTypeOSLoaderData          0x2
#define kCXKMemoryMapEntryTypeBootServiceCode       0x3
#define kCXKMemoryMapEntryTypeBootServiceData       0x4
#define kCXKMemoryMapEntryTypeRuntimeServiceCode    0x5
#define kCXKMemoryMapEntryTypeRuntimeServiceData    0x6
#define kCXKMemoryMapEntryTypeFree                  0x7
#define kCXKMemoryMapEntryTypeUnusable              0x8
#define kCXKMemoryMapEntryTypeACPIReclaimable       0x9
#define kCXKMemoryMapEntryTypeACPINVS               0xA
#define kCXKMemoryMapEntryTypeMappedIO              0xB
#define kCXKMemoryMapEntryTypeMappedIOPorts         0xC
#define kCXKMemoryMapEntryTypePALCode               0xD
#define kCXKMemoryMapEntryTypePersistant            0xE
#define kCXKMemoryMapEntryTypeMax                   0xF

#if !kCXAssemblyCode

typedef struct {
    UInt32              entryType;
    UInt32              padding0;
    OSAddress           physicalAddress;
    OSAddress           virtualAddress;
    UInt64              pageCount;
    UInt64              attributes;
    UInt64              padding1;
} CXKMemoryMapEntry;

typedef struct {
    UIntN               key;
    OSCount             entryCount;
    CXKMemoryMapEntry  *entries;
} CXKMemoryMap;

typedef struct {
    UInt16              serialPort0;
    CXKPOSTCode         setupCode;
    CXKPOSTCode         kernelCode;
    OSAddress           kernelAddress;
    UInt8               enableRelocation;
} CXKBootDevOptions;

typedef struct {
    OSAddress           loaderBase;
    CXKMemoryMap        memoryMap;
    CXKBootDevOptions   devOptions;
} CXKBootOptions;

#endif /* !defined(kCXAssemblyCode) */

#endif /* !defined(__KERNEL_CXKBOOTOPTIONS__) */
