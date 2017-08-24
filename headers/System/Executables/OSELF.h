/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* OSELF.h - C Declarations for the ELF Executable Format          */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* beeselmane - 11.9.2016  - 8:15 PM EST                           */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/

#ifndef __SYSTEM_EXECUTABLES_OSELF__
#define __SYSTEM_EXECUTABLES_OSELF__ 1

#include <Corona-X.h>
#include <System/OSTypes.h>

#if kCXLittleEndian
    #define kOSELFMagic                 0x464C457F
    #define kOSELFMagicReverse          0x7F454C46
#else /* kCXBigEndian */
    #define kOSELFMagic                 0x7F454C46
    #define kOSELFMagicReverse          0x464C457F
#endif /* endian */

#define kOSELFFileHeaderIs32Bit         1
#define kOSELFFileHeaderIs64Bit         2
#define kOSELFFileHeaderIsLittleEndian  1
#define kOSELFFileHeaderIsBigEndian     2
#define kOSELFFileHeaderVersion1        1

#define kOSELFSegmentHeaderMaskOSFlags  0x00FF0000
#define kOSELFSegmentHeaderMaskCPUFlags 0xFF000000

#define kOSELFSectionFlagWritable       0x00000001
#define kOSELFSectionFlagAllocated      0x00000002
#define kOSELFSectionFlagExecutable     0x00000004

#define kOSELFSectionHeaderMaskOSFlags  0x0F000000
#define kOSELFSectionHeaderMaskCPUFlags 0xF0000000

#define kOSELFSymbolScopeLocal          0x0
#define kOSELFSymbolScopeGlobal         0x1
#define kOSELFSymbolScopeWeakGlobal     0x2
#define kOSELFSymbolScopeOSSpecificMin  0xA
#define kOSELFSymbolScopeOSSpecificMax  0xC
#define kOSELFSymbolScopeCPUSpecificMin 0xD
#define kOSELFSymbolScopeCPUSpecificMax 0xF

#define kOSELFSymbolTypeUnspecified     0x0
#define kOSELFSymbolTypeDataObject      0x1
#define kOSELFSymbolTypeFunction        0x2
#define kOSELFSymbolTypeSection         0x3
#define kOSELFSymbolTypeSourceFile      0x4
#define kOSELFSymbolTypeOSSpecificMin   0xA
#define kOSELFSymbolTypeOSSpecificMax   0xC
#define kOSELFSymbolTypeCPUSpecificMin  0xD
#define kOSELFSymbolTypeCPUSpecificMax  0xF

#if !kCXAssemblyCode

#define OSELFSymbolInfoCreate(s, t)     (((s & 0xF) << 4) | (t & 0xF))
#define OSELFSymbolTypeFromInfo(i)      (i & 0xF)
#define OSELFSymbolScopeFromInfo(i)     (i >> 4)

typedef OSEnum(UInt8, OSELFABI) {
    kOSELFABISystemV            = 0x00,
    kOSELFABIHP_UX              = 0x01,
    kOSELFABINetBSD             = 0x02,
    kOSELFABILinux              = 0x03,
    kOSELFABISolaris            = 0x06,
    kOSELFABIAIX                = 0x07,
    kOSELFABIIRIX               = 0x08,
    kOSELFABIFreeBSD            = 0x09,
    kOSELFABIOpenBSD            = 0x0C,
    kOSELFABIOpenVMS            = 0x0D,
    kOSELFABINonStopKernel      = 0x0E,
    kOSELFABIAROS               = 0x0F,
    kOSELFABIFenixOS            = 0x10,
    kOSELFABICloudABI           = 0x11,
    kOSELFABISortix             = 0x53,
    kOSELFABIStandalone         = 0xFF
};

typedef OSEnum(UInt16, OSELFFileType) {
    kOSELFFileTypeNone          = 0x00,
    kOSELFFileTypeRelocatable   = 0x01,
    kOSELFFileTypeExecutable    = 0x02,
    kOSELFFileTypeShared        = 0x03,
    kOSELFFileTypeCore          = 0x04,
};

typedef OSEnum(UInt16, OSELFCPUType) {
    kOSELFCPUTypeGeneric        = 0x00,
    kOSELFCPUTypeSPARC          = 0x02,
    kOSELFCPUTypeX86            = 0x03,
    kOSELFCPUTypeMIPS           = 0x08,
    kOSELFCPUTypePowerPC        = 0x14,
    kOSELFCPUTypeARM            = 0x28,
    kOSELFCPUTypeSuperH         = 0x2A,
    kOSELFCPUTypeItanium        = 0x32,
    kOSELFCPUTypeX86_64         = 0x3E,
    kOSELFCPUTypeARM64          = 0xB7
};

typedef OSEnum(UInt32, OSELFSegmentType) {
    kOSELFSegmentTypeNull           = 0x00000000,
    kOSELFSegmentTypeLoad           = 0x00000001,
    kOSELFSegmentTypeDynamic        = 0x00000002,
    kOSELFSegmentTypeInterpreted    = 0x00000003,
    kOSELFSegmentTypeNote           = 0x00000004,
    kOSELFSegmentTypeSharedLibrary  = 0x00000005,
    kOSELFSegmentTypeSegmentHeader  = 0x00000006,
    kOSELFSegmentTypeOSSpecificMin  = 0x60000000,
    kOSELFSegmentTypeOSSpecificMax  = 0x6FFFFFFF,
    kOSELFSegmentTypeCPUSpecificMin = 0x70000000,
    kOSELFSegmentTypeCPUSpecificMax = 0x7FFFFFFF
};

typedef OSEnum(UInt32, OSELFSegmentFlags) {
    kOSELFSegmentFlagExecutable     = 0x1,
    kOSELFSegmentFlagWritable       = 0x2,
    kOSELFSegmentFlagReadable       = 0x4
};

typedef OSEnum(UInt32, OSELFSectionType) {
    kOSELFSectionTypeNull           = 0x0000,
    kOSELFSectionTypeProgramInfo    = 0x0001,
    kOSELFSectionTypeSymbolTable    = 0x0002,
    kOSELFSectionTypeStringTable    = 0x0003,
    kOSELFSectionTypeRelaRelocation = 0x0004,
    kOSELFSectionTypeSymbolHash     = 0x0005,
    kOSELFSectionTypeDynamicLink    = 0x0006,
    kOSELFSectionTypeNote           = 0x0007,
    kOSELFSectionTypeUninitialized  = 0x0008,
    kOSELFSectionTypeRelRelocation  = 0x0009,
    kOSELFSectionTypeSharedLibrary  = 0x000A,
    kOSELFSectionTypeDynamicSymbols = 0x000B,
    kOSELFSectionTypeReservedMin    = 0xFF00,
    kOSELFSectionTypeCPUSpecifcMin  = 0xFF00,
    kOSELFSectionTypeCPUSpecifcMax  = 0xFF1F,
    kOSELFSectionTypeAbsolute       = 0xFFF1,
    kOSELFSectionTypeCommonSymbols  = 0xFFF2,
    kOSELFSectionTypeReservedMax    = 0xFFFF
};

typedef OSPackedStructure() {
    UInt32 magic;
    UInt8  is64Bit;
    UInt8  endian;
    UInt8  iVersion;
    UInt8  abi;
    UInt8  abiVersion;
    UInt8  padding[7];
    UInt16 fileType;
    UInt16 machineType;
    UInt32 version;
    UInt64 entryPoint;
    UInt64 segmentHeaderOffset;
    UInt64 sectionHeaderOffset;
    UInt32 flags;
    UInt16 fileHeaderSize;
    UInt16 segmentHeaderSize;
    UInt16 segmentCount;
    UInt16 sectionHeaderSize;
    UInt16 sectionCount;
    UInt16 sectionNameSectionOffset;
} OSELFFileHeader;

typedef struct {
    UInt32 type;
    UInt32 flags;
    UInt64 offset;
    UInt64 virtualAddress;
    UInt64 physicalAddress;
    UInt64 sizeInFile;
    UInt64 sizeInMemory;
    UInt64 padding;
} OSELFSegmentHeader;

typedef struct {
    UInt32 name;
    UInt32 type;
    UInt64 flags;
    UInt64 virtualAddress;
    UInt64 offset;
    UInt64 size;
    UInt32 associatedSection;
    UInt32 info;
    UInt64 addressAlignment;
    UInt64 entrySize;
} OSELFSectionHeader;

typedef struct {
    UInt32 name;
    UInt8  attributes;
    UInt8  reserved;
    UInt16 sectionTableIndex;
    UInt64 value;
    UInt64 objectSize;
} OSELFSymbol;

#endif /* !kCXAssemblyCode */

#endif /* !defined(__SYSTEM_EXECUTABLES_OSELF__) */
