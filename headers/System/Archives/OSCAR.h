/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* OSCAR.h - Standard Structures for the CAR Archive Format        */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* beeselmane - 27.11.2015 - 8:00 PM EST                           */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* Note: This file uses the nonstandard prefix 'CAR'               */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/

#ifndef __SYSTEM_OSCAR__
#define __SYSTEM_OSCAR__ 1

#include <Corona-X.h>
#include <System/OSTypes.h>
#include <System/Executables/OSCX.h>

#if !kCXAssemblyCode

#define kCARHeaderMagic                     ((UInt8 [4]){'C', 'A', 'R', '\0'})
#define kCARHeaderVersionS1                 ((UInt8 [4]){'X', '.', 'F', '1'})
#define kCARHeaderVersionS2                 ((UInt8 [4]){'X', '.', 'F', '2'})
#define kCARHeaderVersionBootX              ((UInt8 [4]){'X', '.', 'F', 'b'})
#define kCARHeaderVersionSystem             ((UInt8 [4]){'X', '.', 'F', 's'})
#define kCARHeaderBootXLockValue            0xFFFF

typedef CXEnum(OSProcessorType, CARProcessorType) {
    kCARProcessorTypeX86                    = kCXProcessorTypeX86,
    kCARProcessorTypeX86_64                 = kCXProcessorTypeX86_64,
    kCARProcessorTypeARMv7                  = kCXProcessorTypeARMv7,
    kCARProcessorTypeARMv8                  = kCXProcessortypeARMv8
};

typedef CXEnum(UInt8, CARTOCEntryType) {
    kCARTOCEntryTypeDirectory               = 0,
    kCARTOCEntryTypeFile                    = 1,
    kCARTOCEntryTypeSymbolicLink            = 2
};

typedef CXEnum(UInt16, CARSystemType) {
    kCARSystemTypeCoronaX                   = 0,
    kCARSystemTypeCorOS                     = 1,
    kCARSystemTypeXMobile                   = 2,
    kCARSystemTypeCMobile                   = 3
};

typedef CXEnum(UInt16, CARSystemBuildType) {
    kCARSystemBuildTypeDebug                = 0,
    kCARSystemBuildTypeDevelopment          = 1,
    kCARSystemBuildTypeRelease              = 2,
    kCARSystemBuildTypeStable               = 3
};

typedef CXEnum(UInt16, CARSystemDirectoryFlag) {
    kCARSystemDirectoryFlagCanEnumerate     = 0x0001,
    kCARSystemDirectoryFlagAllowsExecute    = 0x0002,
};

typedef CXEnum(UInt16, CARSystemFileFlag) {
    kCARSystemFileFlagCanRead               = 0x0001,
    kCARSystemFileFlagCanUserRead           = 0x0002,
    kCARSystemFileCanExecute                = 0x0004,
    kCARSystemFileCanUserExecute            = 0x0010,
};

typedef struct CARHeaderS1 {
    UInt8  magic[4];
    UInt8  version[4];
    UInt32 tocOffset;
    UInt64 stringTableOffset;
    UInt64 dataSectionOffset;
    UInt32 checksum;
} CARHeaderS1;

typedef struct CARHeaderS2 {
    UInt8  magic[4];
    UInt8  version[4];
    UInt32 tocOffset;
    UInt64 stringTableOffset;
    UInt64 dataSectionOffset;
    UInt32 checksum;
    UInt64 dataModification;
    UInt64 archiveSignature;
} CARHeaderS2;

typedef struct CARTOCEntry {
    UInt32 nameOffset;
    UInt8 entryType;
    UInt64 dataOffset;
    UInt64 dataSize;
} CARTOCEntry;

typedef struct CARDataModification {
    struct CAREncryptionInfo {
        UInt8 cipher;
        UInt64 startOffset;
        UInt64 runLength;
        UInt16 padding;
        UInt8 reserved;
    } encryption;
    struct CARCompressionInfo {
        UInt8 algorithm;
        UInt64 startOffset;
        UInt64 runLength;
        UInt16 padding;
        UInt8 reserved;
    } compression;
} CARDataModification;

typedef struct CARHeaderBootX {
    UInt8  magic[4];
    UInt8  version[4];
    UInt16 processorType;
    UInt32 bootID;
    UInt64 stringTableOffset;
    UInt64 dataSectionOffset;
    UInt32 checksum;
    UInt16 kernelEntry;
    UInt16 bootConfigEntry;
    UInt16 systemServerEntry;
    UInt16 ioServerEntry;
    UInt16 faultServerEntry;
    UInt16 extensionDirectory;
    UInt16 lock;
} CARHeaderBootX;

typedef struct CARHeaderSystem {
    UInt8  magic[4];
    UInt8  version[4];

    struct CARSystemVersionInternal {
        UInt64 type:2;
        UInt64 majorVersion:7;
        UInt64 revision:5;
        UInt64 buildType:2;
        UInt64 buildID:48;
    } systemVersion;

    UInt32 tocOffset;
    UInt64 stringTableOffset;
    UInt64 dataSectionOffset;
    UInt32 checksum;
    UInt64 dataModification;
    UInt64 archiveSignature;
} CARHeaderSystem;

typedef struct CARSystemTOCDirectoryEntry {
    UInt32 nameOffset;
    UInt16 parentDirectory;
    UInt32 subfileCount;
    UInt16 entryCount;
    UInt16 nextDirectoryEntry;
    UInt16 flags;
    UInt64 padding;
} CARSystemTOCDirectoryEntry;

typedef struct CARSystemTOCFileEntry {
    UInt32 nameOffset;
    UInt16 parentDirectory;
    UInt8 entryType;
    UInt64 dataOffset;
    UInt64 dataSize;
    UInt8 flags;
} CARSystemTOCFileEntry;

typedef struct CARSystemVersionInternal CARSystemVersionInternal;
typedef struct CARCompressionInfo CARCompressionInfo;
typedef struct CAREncryptionInfo CAREncryptionInfo;

#endif /* !kCXAssemblyCode */

#endif /* !defined(__SYSTEM_OSCAR__) */
