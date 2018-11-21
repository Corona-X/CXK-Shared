/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* XKDebugLog.h - Major upgrade to the logging system for debug    */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* beeselmane - 4.14.2017  -  6:15 PM PST                          */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/

#ifndef __KERNEL_XKDEBUGLOG__
#define __KERNEL_XKDEBUGLOG__ 1

#include <Corona-X.h>
#include <System/OSCompilerMacros.h>
#include <System/OSTypes.h>
#include <System/OSUID.h>
#include <Kernel/Shared/XKLog.h>

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

// Largest size supported by UEFI for the monitor
// Always RGBX or BGRX 32-bit pixel format
typedef struct {
    UInt32 height;
    UInt32 width;
    UInt32 *framebuffer;
    OSSize framebufferSize;
    OSCount pixelCount;
    bool isBGRX;
} XKGraphicsContext;

typedef struct {
    UInt32 height;
    UInt32 width;
    UInt8 *packedData;
    UInt32 *fontData;
} XKBitmapFont;

typedef struct {
    UInt32 y;
    UInt32 x;
} XKGraphicsPoint;

OSShared void XKGraphicsContextWriteCharacter(XKGraphicsContext *context, UInt8 character, XKGraphicsPoint location, XKBitmapFont *font, UInt32 color, UInt32 bgColor);

OSShared void XKAllocateLogBuffer(OSSize size);

OSExport XKBitmapFont *gXKBitmapFont8x16;
OSExport XKLogLevel gXKLogLevelCurrent;

#endif /* !defined(__KERNEL_XKDEBUGLOG__) */
