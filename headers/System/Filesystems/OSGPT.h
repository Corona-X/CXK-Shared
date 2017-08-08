/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* OSGPT.h - Structures and Defines for GPT Headers                */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* beeselmane - 17.9.2016  - 3:15 PM EST                           */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/

#ifndef __SYSTEM_OSGPT__
#define __SYSTEM_OSGPT__ 1

#include <Corona-X.h>
#include <System/OSTypes.h>
#include <System/OSUID.h>

#if kCXLittleEndian
    #define kOSGPTSignature         0x5452415020494645
    #define kOSGPTSignatureReverse  0x4546492050415254
#else /* kCXBigEndian */
    #define kOSGPTSignature         0x4546492050415254
    #define kOSGPTSignatureReverse  0x5452415020494645
#endif /* endian */

#define kOSGPTRevision1             0x00010000
#define kOSGPTBasicDataFlagReadOnly (1 << 60)
#define kOSGPTBasicDataFlagHidden   (1 << 62)
#define kOSGPTBasicDataFlagNoAuto   (1 << 63)

#define kOSGPTUIDEFISystemPartition ((OSUIDIntelData){0xC12A7328, 0xF81F, 0x11D2, {0xBA, 0x4B, 0x00, 0xA0, 0xC9, 0x3E, 0xC9, 0x3B}})
#define kOSGPTUIDCXSystemPartition  ((OSUIDStandardData){0x27FCA2BA, 0xF3CB, 0x548D, 0x86, 0x19, {0x31, 0x9D, 0x49, 0x92, 0xA7, 0xF7}})
#define kOSGPTUIDCXDataPartition    ((OSUIDStandardData){0x516E7CB4, 0x6ECF, 0x11D6, 0x8F, 0xF8, {0x00, 0x02, 0x2D, 0x09, 0x71, 0x2B}})

#if !kCXAssemblyCode

typedef struct {
    UInt64 signature;
    UInt32 revision;
    UInt32 headerSize;
    UInt32 checksum;
    UInt32 reserved;
    
    UInt64 thisHeaderLBA;
    UInt64 otherHeaderLBA;
    
    UInt64 firstUsableLBA;
    UInt64 finalUsableLBA;
    
    OSUIDRawData diskUID;
    
    UInt64 partitionTableLBA;
    UInt32 partitionCount;
    UInt32 partitionEntrySize;
    UInt32 partitionTableChecksum;
    
    UInt8 padding[0];
} OSGPTHeader;

typedef OSEnum(UInt64, OSGPTPartitionFlags) {
    kOSGPTPartitionFlagSystemPartition = 0x0,
    kOSGPTPartitionFlagEFIIgnore       = 0x1,
    kOSGPTPartitionFlagLegacyBootable  = 0x2
};

typedef struct {
    OSUIDRawData typeUID;
    OSUIDRawData uid;
    UInt64 firstLBA;
    UInt64 finalLBA;
    OSGPTPartitionFlags attributes;
    UInt16 name[36];
    UInt8 reserved[0];
} OSGPTPartitionEntry;

typedef struct __attribute__((packed)) {
    UInt8 bootCode[440];
    UInt32 diskSignature;
    UInt16 unknown;
    
    struct {
        UInt8 bootIndicator;
        UInt8 firstLocation[3];
        UInt8 type;
        UInt8 finalLocation[3];
        UInt32 firstLBA;
        UInt32 blockCount;
    } bootRecords[4];
    
    UInt16 signature;
    UInt8 padding[0];
} OSGPTProtectiveMBR;

#endif /* !kCXAssemblyCode */

#endif /* !defined(__SYSTEM_OSGPT__) */
