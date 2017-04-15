#include <Kernel/XKConsole.h>
#include <Kernel/XKSharedTarget.h>
#include <Kernel/XKUnicode.h>
#include <Kernel/XKMemory.h>

XKLogLevel gXKLogLevelCurrent = kXKLogLevelInfo;
XKConsole *gXKConsoleFirst = kOSNullPointer;

bool XKConsoleRegister(XKConsole *target)
{
    if (target->type & kXKConsoleTypeOutputMask) {
        if (!target->delete) return false;
        if (!target->write) return false;
    } if (target->type & kXKConsoleTypeInputMask) {
        if (!target->read) return false;
    }

    XKConsole *console = gXKConsoleFirst;
    target->next = kOSNullPointer;

    if (OSExpect(console)) {
        while (console->next)
            console = console->next;

        target->id = console->id + 1;
        console->next = target;
    } else {
        gXKConsoleFirst = target;
        target->id = 0;
    }

    return true;
}

bool XKConsoleRemove(const XKConsole *target)
{
    XKConsole *console = gXKConsoleFirst;
    XKConsole *previous = kOSNullPointer;

    while (console)
    {
        if (console == target)
        {
            if (OSExpect(previous))
            {
                previous->next = console;
                return true;
            }

            gXKConsoleFirst = console->next;
            return true;
        }

        previous = console;
        console = console->next;
    }
    
    return false;
}

XKConsole *XKConsoleRemoveByID(UInt8 id)
{
    XKConsole *console = gXKConsoleFirst;

    while (console)
    {
        if (console->id == id)
        {
            XKConsoleRemove(console);
            return console;
        }

        console = console->next;
    }

    return kOSNullPointer;
}

#pragma mark - Output Functions

void XKConsoleOutputString(const OSUTF8Char *string, OSLength length)
{
    XKConsole *console = gXKConsoleFirst;

    while (console)
    {
        if ((console->type & kXKConsoleTypeOutputMask) && (console->level <= gXKLogLevelCurrent))
            console->write(string, length, console->context);

        console = console->next;
    }
}

void XKConsoleDeleteCharacters(OSCount count)
{
    XKConsole *console = gXKConsoleFirst;

    while (console)
    {
        if (console->type & kXKConsoleTypeOutputMask)
            console->delete(count, console->context);

        console = console->next;
    }
}

#pragma mark - Input functions

OSUTF8Char *XKConsoleReadString(OSUnicodePoint terminator, OSLength *length)
{
    OSBuffer stringBuffer = SLAllocate(8);
    OSUTF8Char *string = stringBuffer.address;
    OSUTF8Char bytes[7];
    OSIndex i = 0;

    for ( ; ; )
    {
        XKConsole *console = gXKConsoleFirst;

        while (console)
        {
            if ((console->type & kXKConsoleTypeInputMask) && console->read)
            {
                OSUnicodePoint read = console->read(false, console->context);
                if (read == '\r') read = '\n';

                // Backspace or delete
                if (read == '\b' || read == 0x7F) {
                    if (i)
                    {
                        XKConsoleDeleteCharacters(1);
                        OSUTF8Char character;

                        do {
                            character = string[i];
                            string[i--] = 0;
                        }while ((character & 0xC0) == 0x80);
                    }
                } else if (read != kXKUTF32Error) {
                    OSCount byteCount = XKUTF8FromCodePoint(read, &OSBufferMake(bytes, 7));
                    XKConsoleOutputString(bytes, byteCount);

                    if (!byteCount)
                    {
                        XKFree(stringBuffer.address);
                        return kOSNullPointer;
                    }

                    if ((stringBuffer.size + byteCount) <= i)
                    {
                        stringBuffer = XKReallocate(stringBuffer.address, stringBuffer.size * 2);

                        if (OSBufferIsEmpty(stringBuffer))
                        {
                            XKFree(stringBuffer.address);
                            return kOSNullPointer;
                        }

                        string = stringBuffer.address;
                    }

                    if (read == terminator) {
                        string[i] = 0;

                        if (length) (*length) = i;
                        return string;
                    } else {
                        XKMemoryCopy(bytes, string + i, byteCount);
                        i += byteCount;
                    }
                }

                // Half a millisecond
                XKDelay(500);
            }

            console = console->next;
        }
    }
}

OSUnicodePoint XKConsoleWaitForInput(void)
{
    for ( ; ; )
    {
        XKConsole *console = gXKConsoleFirst;

        while (console)
        {
            if ((console->type & kXKConsoleTypeInputMask) && console->read)
            {
                OSUnicodePoint read = console->read(false, console->context);
                if (read != kXKUTF32Error) return read;

                // Half a millisecond
                XKDelay(500);
            }

            console = console->next;
        }
    }
}

#pragma mark - Other Functions

void XKConsoleSynchronize(void)
{
    XKConsole *console = gXKConsoleFirst;

    while (console)
    {
        if (console->synchronize)
            console->synchronize(console->context);

        console = console->next;
    }
}
