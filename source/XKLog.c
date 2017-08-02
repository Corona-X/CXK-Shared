#include <Kernel/XKLog.h>
#include <SystemLoader/EFI/SLBootServices.h>
#include <Kernel/XKUtilities.h>

OSAddress gXKLogBuffer = kOSNullPointer;
OSCount gXKLogBufferEntryCount = 0;
OSIndex gXKLogBufferIndex = 0;
OSSize gXKLogBufferSize = 0;

void XKAllocateLogBuffer(OSSize size)
{
    OSBuffer logBuffer = SLBootServicesAllocateAnyPages(size / kSLBootPageSize);

    if (OSBufferIsEmpty(logBuffer) || (logBuffer.size != size))
    {
        // This is effectively a fatal error.
        // It will only occur in a release build
        // If we have already encountered a
        // (likely) fatal error.

        OSFault();
    }

    gXKLogBuffer = logBuffer.address;
    gXKLogBufferSize = size;
}

#if kCXRelease

void XKBasicFormatString(OSUTF8Char *output, UInt16 *finalSize, OSSize maxSize, OSUTF8Char *format, OSVAList arguments)
{
    bool inFormat = false;
    (*finalSize) = 1;

    while ((*finalSize) <= maxSize && (*format))
    {
        OSUTF8Char c = (*format++);

        if (c == '%')
        {
            inFormat = true;
            continue;
        }

        if (inFormat) {
            switch (c)
            {
                case 'd': {
                    UInt64 number = OSVAGetNext(arguments, UInt64);
                    OSUTF8Char buffer[20];
                    OSIndex i = 0;

                    do {
                        UInt8 next = number % 0x10;
                        number /= 0x10;

                        if (next < 10) {
                            buffer[i++] = next + '0';
                        } else {
                            buffer[i++] = (next - 10) + 'A';
                        }
                    } while (number);

                    (*finalSize) += i;

                    if ((*finalSize) > maxSize)
                    {
                        // Out of space, can't do it

                        (*finalSize) = 0;
                        return;
                    }

                    for (i--; i >= 0; i--)
                        (*output++) = buffer[i];
                } break;
                case 's': {
                    OSUTF8Char *string = OSVAGetNext(arguments, OSAddress);

                    while (*string)
                    {
                        (*output++) = *string++;
                        (*finalSize)++;

                        if ((*finalSize) > maxSize)
                        {
                            // Out of space, can't do it

                            (*finalSize) = 0;
                            return;
                        }
                    }
                } break;
                default:
                    XKLog(kXKLogLevelError, "Invalid escape sequence (%d) used in release build!\n", (UInt64)c);
                    (*finalSize) = 0;
                    return;
            }
        } else {
            (*output++) = c;
            (*finalSize)++;
        }
    }
}

void XKLog(XKLogLevel level, const OSUTF8Char *format, ...)
{
    if (level < kXKLogLevelError)
    {
        XKLog(kXKLogLevelError, "Log level %d used in release build!\n", level);
        return;
    }

    if (gXKLogBuffer == kOSNullPointer)
        XKAllocateLogBuffer(0x1000);

    OSSize sizeLeft = gXKLogBufferSize - gXKLogBufferIndex;

    if (sizeLeft < (sizeof(XKLogEntry) + 1))
    {
        // The log buffer is full....
        // Again, we're probably dead anyway
        // at this point...

        OSFault();
    }

    UInt8 subsystem = 0;

    if (format[0] == '[' && format[1] && format[2])
    {
        subsystem = ((format[1] - '0') * 10) + (format[2] - '0');

        if (format[3] && format[4])
            format += 5;
    }

    XKLogEntry entry = {
        .level = level,
        .subsystem = subsystem,
        .timestamp = XKTimeGetCurrent(),
        .size = 0
    };

    XKMemoryCopy(&entry, gXKLogBuffer + gXKLogBufferIndex, sizeof(XKLogEntry));

    OSVAList arguments;
    OSVAStart(arguments, format);

    XKBasicFormatString(gXKLogBuffer + gXKLogBufferIndex + sizeof(XKLogEntry), &entry.size, sizeLeft - sizeof(XKLogEntry), format, arguments);

    if (entry.size)
    {
        entry.size += sizeof(XKLogEntry);
        gXKLogBufferIndex += entry.size;
        gXKLogBufferEntryCount++;
    }

    OSVAFinish(arguments);
}

#endif /* kCXRelease */
