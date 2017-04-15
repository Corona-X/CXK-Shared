/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* XKConsole.h - Input and Output console subsystem for CXK        */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* beeselmane - 1.8.2017   - 12:30 PM EST                          */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/

#ifndef __KERNEL_XKCONSOLE__
#define __KERNEL_XKCONSOLE__ 1

#include <Corona-X.h>
#include <System/OSCompilerMacros.h>
#include <System/OSTypes.h>
#include <Kernel/XKLog.h>

#define kXKConsoleTypeOutputMask    (1 << 7)
#define kXKConsoleTypeInputMask     (1 << 6)
#define kXKConsoleBiDirectionalMask (kXKConsoleTypeOutputMask    | kXKConsoleTypeInputMask)

#define kXKConsoleTypeSerial        (kXKConsoleBiDirectionalMask | 1)

#define kXKConsoleTypeVideo         (kXKConsoleTypeOutputMask    | 1)
#define kXKConsoleTypeMemory        (kXKConsoleTypeOutputMask    | 2)
#define kXKConsoleTypeFile          (kXKConsoleTypeOutputMask    | 3)

#define kXKConsoleTypeKeyboard      (kXKConsoleTypeInputMask     | 1)
#define kXKConsoleTypeEFIkeyboard   (kXKConsoleTypeInputMask     | 2)

typedef struct __XKConsole {
    UInt8 type;
    UInt8 id;
    
    struct __XKConsole *next;
    OSAddress context;
    XKLogLevel level;

    // For output consoles
    void (*write)(const OSUTF8Char *string, OSLength length, OSAddress context);
    void (*delete)(OSCount count, OSAddress context);

    // For input consoles
    OSUnicodePoint (*read)(bool wait, OSAddress context);

    // Optional methods
    void (*destroy)(OSAddress context);
    void (*synchronize)(OSAddress context);
} XKConsole;

OSShared bool XKConsoleRegister(XKConsole *console);
OSShared bool XKConsoleRemove(const XKConsole *target);
OSShared XKConsole *XKConsoleRemoveByID(UInt8 id);
OSShared void XKConsoleSynchronize(void);

// For output consoles
OSShared void XKConsoleOutputString(const OSUTF8Char *string, OSLength length);
OSShared void XKConsoleDeleteCharacters(OSCount count);

// For input consoles
OSShared OSUTF8Char *XKConsoleReadString(OSUnicodePoint terminator, OSLength *length);
OSShared OSUnicodePoint XKConsoleWaitForInput(void);

OSExport XKConsole *gXKConsoleFirst;

#endif /* !defined(__KERNEL_XKCONSOLE__) */
