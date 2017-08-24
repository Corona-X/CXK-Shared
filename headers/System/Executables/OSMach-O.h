/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* OSMach-O - C Declarations for the Mach-O Executable Format      */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* beeselmane - 8.8.2017   - 8:00 PM EST                           */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/

#ifndef __SYSTEM_EXECUTABLES_OSMACHO__
#define __SYSTEM_EXECUTABLES_OSMACHO__ 1

#include <Corona-X.h>
#include <System/OSTypes.h>


#if kCXLittleEndian
    #define kSOMachOMagic                           0xFEEDFACE
    #define kSOMachOMagicReverse                    0xCFFAEDFE
#else /* kCXBigEndian */
    #define kSOMachOMagic                           0xCFFAEDFE
    #define kSOMachOMagicReverse                    0xFEEDFACE
#endif /* endian */

#define kOSMachOHeaderFlagNoUndefinedSymbols        1

#define kOSMachOCommandSegment              0x01
#define kOSMachOCommandSymbolTable          0x02
#define kOSMachOCommandSymbolInfo           0x03
#define kOSMachOCommandThread               0x04
#define kOSMachOCommandUnixThread           0x05
#define kOSMachOCommandLoadFVM              0x06
#define kOSMachOCommandFVMID                0x07
#define kOSMachOCommandIdentity             0x08
#define kOSMachOCommandFVMFile              0x09
#define kOSMachOCommandPrepage              0x0A
#define kOSMachOCommandDynamicSymbolTable   0x0B
#define kOSMachOCommandLoadDynamicLibrary   0x0C
#define kOSMachOCommandDynamicLibraryID     0x0D
#define kOSMachOCommandLoadDynamicLinker    0x0E
#define kOSMachOCommandDynamicLinkerID      0x0F
#define kOSMachOCommandPreboundLibrary      0x10
#define kOSMachOCommandImageRountines       0x11
#define kOSMachOCommandSubFramework         0x12
#define kOSMachOCommandSubUmbrella          0x13
#define kOSMachOCommandSubClient            0x14
#define kOSMachOCommandSubLibrary           0x15
#define kOSMachOCommandTwoLevelHints        0x16
#define kOSMachOCommandPrebindChecksum      0x17
#define kOSMachOCommandLoadWeakLibrary      (0x18 | 0x80000000)
#define kOSMachOCommandSegment64            0x19
#define kOSMachOCommandImageRoutines64      0x1A
#define kOSMachOCommandUUID                 0x1B
#define kOSMachOCommandRuntimePath          0x1C
#define kOSMachOCommandCodeSignature        0x1D
#define kOSMachOCommandSegmentSplitInfo     0x1E
#define kOSMachOCommandReexportLibrary      (0x1F | 0x80000000)
#define kOSMachOCommandLazyLoadLibrary      0x20
#define kOSMachOCommandEncryption           0x21
#define kOSMachOCommandDyldInfo             0x22
#define kOSMachOCommandDyldInfoOnly         (0x22 | 0x80000000)
#define kOSMachOCommandLoadUpwardLibrary    (0x23 | 0x80000000)
#define kOSMachOCommandMinimumVersionOSX    0x24
#define kOSMachOCommandMinimumVersionIOS    0x25
#define kOSMachOCommandFunctionStarts       0x26
#define kOSMachOCommandDyldEnvironment      0x27
#define kOSMachOCommandMainFunction         (0x28 | 0x80000000)
#define kOSMachOCommandDataInCode           0x29
#define kOSMachOCommandSourceVersion        0x2A
#define kOSMachOCommandLibraryCodeSignDRS   0x2B
#define kOSMachOCommandEncryptionInfo       0x2C
#define kOSMachOCommandLinkerOptions        0x2D
#define kOSMachOCommandLinkerHint           0x2E
#define kOSMachOCommandMinimumVersionWatch  0x30

typedef OSEnum(UInt32, OSMachOFileType) {
    kOSMachOFileTypeObject          = 0x1,
    kOSMachOFileTypeExecutable      = 0x2,
    kOSMachOFileTypeFVMLibrary      = 0x3,
    kOSMachOFileTypeCore            = 0x4,
    kOSMachOFileTypePreloaded       = 0x5,
    kOSMachOFileTypeDynamicLibrary  = 0x6,
    kOSMachOFileTypeDynamicLinker   = 0x7,
    kOSMachOFileTypeBundle          = 0x8,
    kOSMachOFileTypeStubLibrary     = 0x9,
    kOSMachOFileTypeDebugSymbols    = 0xA,
    kOSMachOFileTypeKernelExtension = 0xB
};

typedef struct {
    UInt32 magic;
    UInt16 machineType;
    UInt16 machineSubtype;
    UInt32 fileType;
    UInt32 loadCommandCount;
    UInt32 loadCommandSize;
    UInt32 flags;
    UInt32 reserved;
} OSMachOHeader;

typedef struct {
    UInt32 command;
    UInt32 size;
} OSMachOLinkCommand;

typedef struct {
    UInt32 command;
    UInt32 commandSize;
    OSUTF8Char name[16];
    UInt64 virtualAddress;
    UInt64 virtualSize;
    UInt64 offset;
    UInt64 size;
    SInt32 maximumProtection;
    SInt32 initialProtection;
    UInt32 sectionCount;
    UInt32 flags;
} OSSegmentCommand;

#endif /* !defined(__SYSTEM_EXECUTABLES_OSMACHO__) */
