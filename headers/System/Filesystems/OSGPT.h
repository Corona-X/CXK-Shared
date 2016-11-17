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

#define kOSGPTRevision1             0x00000100
#define kOSGPTBasicDataFlagReadOnly (1 << 60)
#define kOSGPTBasicDataFlagHidden   (1 << 62)
#define kOSGPTBasicDataFlagNoAuto   (1 << 63)

#define kOSGPTUIDEFISystemPartition ((UInt128)0x28732AC11FF8D211BA4B00A0C93EC93B)
#define kOSGPTUIDCXSystemPartition  ((UInt128)0xF491CA00C31C4642860D78E6033A322F)

#if !kCXAssemblyCode

typedef OSEnum(UInt64, OSGPTPartitionFlags) {
    kOSGPTPartitionFlagSystemPartition = 0x0,
    kOSGPTPartitionFlagEFIIgnore       = 0x1,
    kOSGPTPartitionFlagLegacyBootable  = 0x2
};

typedef struct {
    UInt64 signiture;
    UInt32 revision;
    UInt32 headerSize;
    UInt32 headerChecksum;
    UInt32 reserved;
    UInt64 thisHeaderGPT;
    UInt64 backupHeaderGPT;
    UInt64 firstPartitionLBA;
    UInt64 lastPartitionLBA;
    OSUID  diskID;
    UInt64 paritionTableLBA;
    UInt32 partitionCount;
    UInt32 eachPartitionSize;
    UInt32 partitionChecksum;
} OSGPTHeader;

typedef struct {
    OSUID  partitionType;
    OSUID  partitionID;
    UInt64 firstLBA;
    UInt64 finalLBA;
    OSGPTPartitionFlags attributes;
    UInt16 partitionName[36];
} OSGPTPartitionEntry;

#endif /* !kCXAssemblyCode */

#endif /* !defined(__SYSTEM_OSGPT__) */
