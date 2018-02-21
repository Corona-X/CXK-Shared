/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* OSMach-O - C Declarations for the Mach-O Executable Format      */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* beeselmane - 8.8.2017   - 8:00 PM EST                           */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/

#ifndef __SYSTEM_EXECUTABLES_OSMO__
#define __SYSTEM_EXECUTABLES_OSMO__ 1

#include <Corona-X.h>
#include <System/OSTypes.h>

#if kCXLittleEndian
    #define kOSMOMagic                  0xFEEDFACF
    #define kOSMOMagicReverse           0xCFFAEDFE
#else /* kCXBigEndian */
    #define kOSMOMagic                  0xCFFAEDFE
    #define kOSMOMagicReverse           0xFEEDFACF
#endif /* endian */

#if kCXArchIA
    #define OSMOThreadStateNative       OSMOThreadStateX86
#elif kCXArchARM
    #define OSMOThreadStateNative       OSMOThreadStateARM
#else
    #error Unknown Architecture
#endif

#define kOSMOFlagNoUndefinedSymbols     0x0000001
#define kOSMOFlagIncrementalLink        0x0000002
#define kOSMOFlagDynamicLinkerInput     0x0000004
#define kOSMOFlagBindAtLoad             0x0000008
#define kOSMOFlagPrebound               0x0000010
#define kOSMOFlagSegmentsSplit          0x0000020
#define kOSMOFlagLazyInit               0x0000040
#define kOSMOFlagTwoLevelNames          0x0000080
#define kOSMOFlagForceFlat              0x0000100
#define kOSMOFlagNoMultipleDefinitions  0x0000200
#define kOSMOFlagDontFixPrebinding      0x0000400
#define kOSMOFlagPrebindable            0x0000800
#define kOSMOFlagModulesPrebound        0x0001000
#define kOSMOFlagSymbolSubsections      0x0002000
#define kOSMOFlagCanocialized           0x0004000
#define kOSMOFlagWeakSymbols            0x0008000
#define kOSMOFlagUsesWeakSymbols        0x0010000
#define kOSMOFlagExecutableStack        0x0020000
#define kOSMOFlagRootSafe               0x0040000
#define kOSMOFlagSetuidSafe             0x0080000
#define kOSMOFlagNoReexports            0x0100000
#define kOSMOFlagPositionIndependant    0x0200000
#define kOSMOFlagStrippable             0x0400000
#define kOSMOFlagContainsThreadLocals   0x0800000
#define kOSMOFlagHeapExecutionDisabled  0x1000000
#define kOSMOFlagAppExtensionSafe       0x2000000

#define kOSMOCommandSegment             0x01
#define kOSMOCommandSymbolTable         0x02
#define kOSMOCommandSymbolInfo          0x03
#define kOSMOCommandThread              0x04
#define kOSMOCommandUnixThread          0x05
#define kOSMOCommandLoadFVM             0x06
#define kOSMOCommandFVMID               0x07
#define kOSMOCommandIdentity            0x08
#define kOSMOCommandFVMFile             0x09
#define kOSMOCommandPrepage             0x0A
#define kOSMOCommandDynamicSymbolTable  0x0B
#define kOSMOCommandLoadDynamicLibrary  0x0C
#define kOSMOCommandDynamicLibraryID    0x0D
#define kOSMOCommandLoadDynamicLinker   0x0E
#define kOSMOCommandDynamicLinkerID     0x0F
#define kOSMOCommandPreboundLibrary     0x10
#define kOSMOCommandImageRountines      0x11
#define kOSMOCommandSubFramework        0x12
#define kOSMOCommandSubUmbrella         0x13
#define kOSMOCommandSubClient           0x14
#define kOSMOCommandSubLibrary          0x15
#define kOSMOCommandTwoLevelHints       0x16
#define kOSMOCommandPrebindChecksum     0x17
#define kOSMOCommandLoadWeakLibrary     (0x18 | 0x80000000)
#define kOSMOCommandSegment64           0x19
#define kOSMOCommandImageRoutines64     0x1A
#define kOSMOCommandUUID                0x1B
#define kOSMOCommandRuntimePath         0x1C
#define kOSMOCommandCodeSignature       0x1D
#define kOSMOCommandSegmentSplitInfo    0x1E
#define kOSMOCommandReexportLibrary     (0x1F | 0x80000000)
#define kOSMOCommandLazyLoadLibrary     0x20
#define kOSMOCommandEncryption          0x21
#define kOSMOCommandDyldInfo            0x22
#define kOSMOCommandDyldInfoOnly        (0x22 | 0x80000000)
#define kOSMOCommandLoadUpwardLibrary   (0x23 | 0x80000000)
#define kOSMOCommandMinimumVersionOSX   0x24
#define kOSMOCommandMinimumVersionIOS   0x25
#define kOSMOCommandFunctionStarts      0x26
#define kOSMOCommandDyldEnvironment     0x27
#define kOSMOCommandMainFunction        (0x28 | 0x80000000)
#define kOSMOCommandDataInCode          0x29
#define kOSMOCommandSourceVersion       0x2A
#define kOSMOCommandLibraryCodeSignDRS  0x2B
#define kOSMOCommandEncryptionInfo      0x2C
#define kOSMOCommandLinkerOptions       0x2D
#define kOSMOCommandLinkerHint          0x2E
#define kOSMOCommandMinimumVersionWatch 0x30
#define kOSMOCommandNote                0x31
#define kOSMOCommandBuildVersion        0x32

#define kOSMOSegmentFlagHighVM          0x1
#define kOSMOSegmentFlagFVM             0x2
#define kOSMOSegmentFlagNoRelocation    0x4
#define kOSMOSegmentFlagProtected       0x8

#define kOSMOSymbolFlagSymbolicDebug    0xE0
#define kOSMOSymbolFlagPrivateExternal  0x10
#define kOSMOSymbolFlagType             0x0E
#define kOSMOSymbolFlagExternal         0x01

#define kOSMOSymbolTypeUndefined        0x0
#define kOSMOSymbolTypeAbsolute         0x2
#define kOSMOSymbolTypeInSection        0xE
#define kOSMOSymbolTypePrebound         0xC
#define kOSMOSymbolTypeIndirect         0xA

#define kOSMOx86ThreadState32           1
#define kOSMOx86FloatState32            2
#define kOSMOx86ExceptionState32        3
#define kOSMOx86ThreadState64           4
#define kOSMOx86FloatState64            5
#define kOSMOx86ExceptionState64        6
#define kOSMOx86ThreadState             7
#define kOSMOx86FloatState              8
#define kOSMOx86ExceptionState          9
#define kOSMOx86DebugState32            10
#define kOSMOx86DebugState64            11
#define kOSMOx86DebugState              12
#define kOSMOx86ThreadStateNone         13
#define kOSMOx86AVX32                   16
#define kOSMOx86AVX64                   17
#define kOSMOx86AVX                     18
#define kOSMOx86AVX512_32               19
#define kOSMOx86AVX512_64               20
#define kOSMOx86AVX512                  21

typedef OSEnum(UInt32, OSMOFileType) {
    kOSMOFileTypeObject          = 0x1,
    kOSMOFileTypeExecutable      = 0x2,
    kOSMOFileTypeFVMLibrary      = 0x3,
    kOSMOFileTypeCore            = 0x4,
    kOSMOFileTypePreloaded       = 0x5,
    kOSMOFileTypeDynamicLibrary  = 0x6,
    kOSMOFileTypeDynamicLinker   = 0x7,
    kOSMOFileTypeBundle          = 0x8,
    kOSMOFileTypeStubLibrary     = 0x9,
    kOSMOFileTypeDebugSymbols    = 0xA,
    kOSMOFileTypeKernelExtension = 0xB
};

typedef struct {
    UInt32 magic;
    UInt32 machineType;
    UInt32 machineSubtype;
    UInt32 fileType;
    UInt32 loadCommandCount;
    UInt32 loadCommandSize;
    UInt32 flags;
    UInt32 reserved;
} OSMOHeader;

typedef struct {
    UInt32 command;
    UInt32 size;
} OSMOLoadCommand;

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
} OSMOSegmentCommand;

typedef struct {
    OSUTF8Char name[16];
    OSUTF8Char segmentName[16];
    UInt64 address;
    UInt64 size;
    UInt32 offset;
    UInt32 alignment;
    UInt32 relocationOffset;
    UInt32 relocationEntryCount;
    UInt32 flags;
    UInt32 reserved0;
    UInt32 reserved1;
    UInt32 reserved2;
} OSMOSectionCommand;

typedef struct {
    UInt32 command;
    UInt32 size;
    UInt32 symbolTableOffset;
    UInt32 symbolCount;
    UInt32 stringTableOffset;
    UInt32 stringCount;
} OSMOSymbolTableCommand;

typedef struct {
    UInt32 command;
    UInt32 size;
    UInt32 type;
    UInt32 count;

    // This part is architecture dependant
} OSMOThreadCommand;

typedef struct {
    UInt64 rax, rbx, rcx, rdx;
    UInt64 rdi, rsi, rbp, rsp;
    UInt64 r8,  r9,  r10, r11;
    UInt64 r12, r13, r14, r15;
    UInt64 rip, rflags;
    UInt64 cs, fs, gs;
} OSMOThreadStateX86;

typedef struct {
    UInt64 x[29];
    UInt64 fp, lr;
    UInt64 sp, pc;
    UInt64 cpsr;
    UInt32 padding;
} OSMOThreadStateARM;

typedef struct {
    UInt32 nameOffset;
    UInt8 type;
    UInt8 section;
    UInt16 description;
    UInt64 value;
} OSMOSymbolEntry;

#endif /* !defined(__SYSTEM_EXECUTABLES_OSMO__) */
