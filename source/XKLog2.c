#include <Kernel/XKLog.h>
#include <Kernel/XKDebugLog.h>
#include <Kernel/XKSharedTarget.h>

extern void XKVideoConsoleSetColor(XKConsole *console, UInt32 color);

const OSUTF8Char *XKLogLevelToString(XKLogLevel level)
{
    switch (level)
    {
        case kXKLogLevelDebug:   return "[DEBUG]";
        case kXKLogLevelVerbose: return "[VERBOSE]";
        case kXKLogLevelInfo:    return "[INFO]";
        case kXKLogLevelWarning: return "[WARN]";
        case kXKLogLevelError:   return "[ERROR]";
        default: return "[???]";
    }
}

UInt32 XKLogLevelGetColor(XKLogLevel level)
{
    switch (level)
    {
        case kXKLogLevelDebug:   return 0x4C49F4;
        case kXKLogLevelVerbose: return 0x50ED7F;
        case kXKLogLevelInfo:    return 0x2BCEEF;
        case kXKLogLevelWarning: return 0xF449AD;
        case kXKLogLevelError:   return 0xC12424;
        default:                 return 0xFFFFFF;
    }
}

const OSUTF8Char *XKLogLevelGetEscapeSequence(XKLogLevel level)
{
    switch (level)
    {
        case kXKLogLevelDebug:   return "\e[32m";
        case kXKLogLevelVerbose: return "\e[36m";
        case kXKLogLevelInfo:    return "\e[35m";
        case kXKLogLevelWarning: return "\e[33m";
        case kXKLogLevelError:   return "\e[31m";
        default: return "\e[0m";
    }
}

void XKLogO(XKLogLevel level, const OSUTF8Char *format, ...)
{
    XKConsole *console = gXKConsoleFirst;
    gXKLogLevelCurrent = level;
    OSVAList args;

    OSVAStart(args, format);
    OSUTF8Char *message = XKPrintToStringFromList(format, args);
    OSVAFinish(args);

    while (console)
    {
        if ((console->type & kXKConsoleTypeOutputMask) && (console->level <= level))
        {
            switch (console->type)
            {
                case kXKConsoleTypeSerial: {
                    OSUTF8Char *line = XKPrintToString("%s%s", XKLogLevelGetEscapeSequence(level), message);
                    console->write(line, XKUTF8Length(line), console->context);
                    XKFree(line);
                } break;
                case kXKConsoleTypeVideo: {
                    XKVideoConsoleSetColor(console, XKLogLevelGetColor(level));
                    console->write(message, XKUTF8Length(message), console->context);
                } break;
                case kXKConsoleTypeMemory:
                case kXKConsoleTypeFile:
                default: {
                    OSUTF8Char *line = XKPrintToString("%s %s", XKLogLevelToString(level), message);
                    console->write(line, XKUTF8Length(line), console->context);
                    XKFree(line);
                } break;
            }
        }

        console = console->next;
    }

    XKFree(message);
}

XKTime XKTimeGetCurrent(void)
{
    XKTime time;
    time.date = 0;
    time.nanosecond = 0;
    return time;
}
