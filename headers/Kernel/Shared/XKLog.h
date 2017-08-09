/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* XKLog.h - Basic logging subsystem for CXK                       */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* beeselmane - 1.8.2017   - 12:00 PM EST                          */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/

#ifndef __KERNEL_XKLOG__
#define __KERNEL_XKLOG__ 1

#include <Corona-X.h>
#include <System/OSCompilerMacros.h>
#include <System/OSTypes.h>
#include <Kernel/Shared/XKUtilities.h>

#define kXKLogSubsystemCore      "[00] "
#define kXKLogSubsystemMemory    "[01] "
#define kXKLogSubsystemScheduler "[02] "
#define kXKLogSubsystemLoader    "[30] "

typedef OSEnum(SInt8, XKLogLevel) {
    kXKLogLevelTooMuch = 0,
    kXKLogLevelVerbose = 1,
    kXKLogLevelInfo    = 2,
    kXKLogLevelDebug   = 3,
    kXKLogLevelWarning = 4,
    kXKLogLevelError   = 5,
    kXKLogLevelFatal   = 6
};

typedef struct {
    UInt8 level:3;
    UInt8 subsystem:5;
    XKTime timestamp;
    UInt16 size;
} XKLogEntry;

/**
 * This function logs a formatted string to all registered output
 *   consoles. Which consoles receive the message, as well as how
 *   the message is presented, is based upon the 'level' argument.
 *
 * @argument level:
 *   The level at which to log the formatted string.
 *   See #XKLogLevel for possible values.
 *
 * @argument format:
 *   The format string to log. Possible format characters and options
 *   are specified in the documenation for #XKPrintString
 */
OSShared void XKLog(XKLogLevel level, const OSUTF8Char *format, ...);

#endif /* !defined(__KERNEL_XKLOG__) */
