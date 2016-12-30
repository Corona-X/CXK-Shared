/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* OSCAR.h - Standard Structures for the CAR Archive Format        */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* beeselmane - 27.11.2015 - 8:00 PM EST                           */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* Note: This file uses the nonstandard prefix 'CA'                */
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

typedef OSEnum(UInt8, CAEntryType) {
    kCAEntryTypeDirectory                   = 0,
    kCAEntryTypeFile                        = 1,
    kCAEntryTypeSymbolicLink                = 2,
    kCAEntryTypeMeta                        = 0xFF
};

typedef OSEnum(UInt8, CAEntryFlags) {
    kCAEntryFlagUTF8                        = 0x00,
    kCAEntryFlagUTF16                       = 0x01,
    kCAEntryFlagUTF32                       = 0x02,
    kCAEntryFlagMetaHasData                 = 0x80
};

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= //

typedef OSEnum(UInt8, CAProcessorType) {
    kCARProcessorTypeX86                    = kCXProcessorTypeX86,
    kCARProcessorTypeX86_64                 = kCXProcessorTypeX86_64,
    kCARProcessorTypeARMv7                  = kCXProcessorTypeARMv7,
    kCARProcessorTypeARMv8                  = kCXProcessortypeARMv8
};

typedef OSEnum(UInt16, CASystemType) {
    kCARSystemTypeCoronaX                   = 0,
    kCARSystemTypeCorOS                     = 1,
    kCARSystemTypeXMobile                   = 2,
    kCARSystemTypeCMobile                   = 3
};

typedef OSEnum(UInt16, CASystemBuildType) {
    kCARSystemBuildTypeDebug                = 0,
    kCARSystemBuildTypeDevelopment          = 1,
    kCARSystemBuildTypeRelease              = 2,
    kCARSystemBuildTypeStable               = 3
};

typedef OSEnum(UInt16, CASystemDirectoryFlag) {
    kCARSystemDirectoryFlagCanEnumerate     = 0x0001,
    kCARSystemDirectoryFlagAllowsExecute    = 0x0002,
};

typedef OSEnum(UInt16, CASystemFileFlag) {
    kCARSystemFileFlagCanRead               = 0x0001,
    kCARSystemFileFlagCanUserRead           = 0x0002,
    kCARSystemFileCanExecute                = 0x0004,
    kCARSystemFileCanUserExecute            = 0x0010,
};

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= //

typedef struct {
    UInt8  magic[4];
    UInt8  version[4];
    UInt64 entryTableOffset;
    UInt64 dataSectionOffset;
    UInt32 checksum;
    UInt32 headerChecksum;
} CAHeaderS1;

typedef struct {
    UInt8  magic[4];
    UInt8  version[4];
    UInt64 entryTableOffset;
    UInt64 dataSectionOffset;
    UInt32 checksum;
    UInt32 headerChecksum;
    UInt64 dataModification;
    UInt64 archiveSignature;
} CAHeaderS2;

typedef struct {
    UInt8 type;
    UInt8 padding[3];
    UInt64 dataOffset;
    UInt64 dataSize;
    // OSUTF8Char *path;
} CAEntryS1;

typedef struct {
    UInt8 type;
    UInt8 flags;
    UInt16 padding;
    UInt64 dataOffset;
    UInt64 dataSize;
    // OSUTF8Char *path;
} CAEntryS2;

typedef struct {
    UInt8 entryCount;
    UInt8 padding[3];
} CADataModification;

typedef struct {
    UInt8 type;
    UInt8 padding[3];
    UInt64 startOffset;
    UInt64 runLength;
} CADataModificationInfo;

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= //

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

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= //

#endif /* !kCXAssemblyCode */

#endif /* !defined(__SYSTEM_OSCAR__) */
