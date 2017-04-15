#include <Kernel/XKStringFormatting.h>
#include <Kernel/XKSharedTarget.h>
#include <Kernel/XKConsole.h>
#include <Kernel/XKUnicode.h>
#include <Kernel/XKMemory.h>

#pragma mark - Utility Functions

OSUTF8Char *XKUIDToString(const OSUIDIntelData *uid)
{
    return XKPrintToString("%08X-%04X-%04X-%02X%02X%02X%02X%02X%02X%02X%02X",
                           uid->group0, uid->group1, uid->group2,
                           uid->group3[0], uid->group3[1], uid->group3[2], uid->group3[3],
                           uid->group3[4], uid->group3[5], uid->group3[6], uid->group3[7]);
}

OSUTF8Char *XKBufferToString(const OSBuffer *buffer)
{
    return XKPrintToString("{%u bytes @ %p}", buffer->size, buffer->address);
}

void XKPrintToStringCount(OSUTF8Char *string, OSLength length, OSLength *previous)
{
    (*previous) += length;
}

void XKPrintToStringPrint(OSUTF8Char *newChars, OSLength length, OSUTF8Char **string)
{
    XKMemoryCopy(newChars, (*string), length);
    (*string) += length;
}

#pragma mark - Number Conversion

void XKNumberToStringConverter(SInt64 n, bool isSigned, UInt8 base, UInt8 padding, UInt8 hexStart, bool shouldPrint, OSLength *length, OSUTF8Char **string)
{
    if (!shouldPrint && !string)
        return;

    bool prependNegative = false;
    OSUTF8Char buffer[32];
    UInt8 offset = 0;
    UInt64 number;
    SInt8 i = 0;

    if (isSigned) {
        if (n < 0) {
            prependNegative = true;
            number = -n;
        } else {
            number = n;
        }
    } else {
        // This hack makes the CPU read
        // n as an unsigned integer
        number = *((UInt64 *)&n);
    }

    if (!number)
    {
        if (!shouldPrint && length) (*length) = padding;

        if (padding) {
            if (!shouldPrint) {
                (*string) = XKAllocate(padding + 1).address;
                XKMemorySetValue((*string), padding, '0');
                (*string)[padding] = 0;
            } else {
                OSUTF8Char *zeros = XKAllocate(padding).address;
                XKMemorySetValue(zeros, padding, '0');
                XKConsoleOutputString(zeros, padding);
                XKFree(zeros);
            }
        } else {
            if (!shouldPrint)
            {
                (*string) = XKAllocate(1).address;
                (*string)[0] = 0;
            }
        }

        return;
    }

    do {
        UInt8 next = number % base;
        number /= base;
        
        if (next < 10) {
            buffer[i++] = next + '0';
        } else if (next < 37) {
            buffer[i++] = (next - 10) + hexStart;
        } else {
            buffer[i++] = '#';
        }
    } while (number);

    if (i < padding) {
        UInt8 zeroCount = (padding - i);

        if (!shouldPrint) {
            if (length) (*length) = padding;

            (*string) = XKAllocate(padding + 1).address;
            (*string)[padding] = 0;

            if (prependNegative) {
                XKMemorySetValue((*string) + 1, zeroCount - 1, '0');
                (*string)[0] = '-';
            } else {
                XKMemorySetValue((*string), zeroCount, '0');
            }

            offset = zeroCount;
        } else {
            if (prependNegative)
            {
                XKConsoleOutputString((OSUTF8Char *)"-", 1);
                zeroCount--;
            }

            OSUTF8Char *zeros = XKAllocate(zeroCount).address;
            XKMemorySetValue(zeros, zeroCount, '0');
            XKConsoleOutputString(zeros, zeroCount);
            XKFree(zeros);
        }
    } else {
        if (prependNegative) {
            if (!shouldPrint) {
                if (length) (*length) = (i + 1);

                (*string) = XKAllocate(i + 2).address;
                (*string)[i + 1] = 0;
                (*string)[0] = '-';
                offset = 1;
            } else {
                XKConsoleOutputString((OSUTF8Char *)"-", 1);
            }
        } else {
            if (!shouldPrint)
            {
                if (length) (*length) = i;

                (*string) = XKAllocate(i + 1).address;
                (*string)[i] = 0;
                offset = 0;
            }
        }
    }

    if (!shouldPrint) {
        for (i--; i >= 0; i--)
            string[offset++] = buffer[i];
    } else {
        OSUTF8Char finalBuffer[30];
        UInt8 toCopy = i;
        offset = 0;

        for (i--; i >= 0; i--)
            finalBuffer[offset++] = buffer[i];

        XKConsoleOutputString(finalBuffer, toCopy);
    }
}

OSUTF8Char *XKNumberToString(SInt64 number, bool isSigned, UInt8 base, UInt8 padding, OSUTF8Char hexStart, OSLength *length)
{
    OSUTF8Char *string;

    XKNumberToStringConverter(number, isSigned, base, padding, hexStart, false, length, &string);

    return string;
}

#pragma mark - String Functions

OSUTF8Char *XKPrintToString(const OSUTF8Char *format, ...)
{
    OSVAList args;
    OSVAStart(args, format);
    OSUTF8Char *result = XKPrintToStringFromList(format, args);
    OSVAFinish(args);

    return result;
}

OSUTF8Char *XKPrintToStringFromList(const OSUTF8Char *format, OSVAList args)
{
    OSVAList *copy = kOSNullPointer;
    OSVACopy(copy, args);

    OSLength length = 0;
    XKConsole *firstConsole = gXKConsoleFirst;

    XKConsole lengthConsole;
    lengthConsole.type = kXKConsoleTypeOutputMask | 0;
    lengthConsole.next = kOSNullPointer;
    lengthConsole.context = &length;
    lengthConsole.level = -3;
    lengthConsole.write = XKPrintToStringCount;

    gXKConsoleFirst = &lengthConsole;
    XKPrintStringFromList(format, copy);
    ///OSVAFinish(copy);

    OSUTF8Char *string = XKAllocate(length + 1).address;
    OSUTF8Char *pointer = string;

    XKConsole printConsole;
    printConsole.type = kXKConsoleTypeOutputMask | 0;
    printConsole.next = kOSNullPointer;
    printConsole.context = &pointer;
    printConsole.level = -3;
    printConsole.write = XKPrintToStringPrint;

    gXKConsoleFirst = &printConsole;
    XKPrintStringFromList(format, args);
    OSVAFinish(copy);

    string[length] = 0;
    gXKConsoleFirst = firstConsole;
    return string;
}

void XKPrintString(const OSUTF8Char *format, ...)
{
    OSVAList args;
    OSVAStart(args, format);
    XKPrintStringFromList(format, args);
    OSVAFinish(args);
}

void XKPrintStringFromList(const OSUTF8Char *format, OSVAList args)
{
    bool justEnteredEscapeCode = false;
    bool specialPadding = false;
    bool inEscapeCode = false;
    bool printNumber = false;
    bool printSign = false;
    bool inPadding = false;
    bool hexDump = false;

    UInt8 hexBase = 'A';
    UInt8 lastFlag = 0;
    UInt8 argSize = 4;
    UInt8 padding = 1;
    UInt8 base = 10;

    OSCount charsToPrint = 0;
    OSIndex i = -1;

    while (*format)
    {
        UInt8 c = (*format++);
        i++;

        if (!inEscapeCode)
        {
            if (c == '%'){
                justEnteredEscapeCode = true;
                inEscapeCode = true;
            } else {
                charsToPrint++;
            }

            continue;
        }

        if (justEnteredEscapeCode)
        {
            OSUTF8Char *source = format - (charsToPrint + 2);
            justEnteredEscapeCode = false;

            if (c == '%')
            {
                inEscapeCode = false;
                charsToPrint++;
            }

            XKConsoleOutputString(source, charsToPrint);
            charsToPrint = 0;

            if (c == '%') continue;
        }

        // Yes, I realize that this will allow an
        // infinite number of size flags to be
        // used, and that the last size flag given
        // will take precedence over all others.
        // This doesn't matter, however, it doesn't
        // actually expose anything that could be
        // exploited.
        switch (c)
        {
            case '0': {
                specialPadding = true;
                inPadding = true;
                padding = 0;
            } break;
            case 'h': {
                if (lastFlag == c) {
                    argSize = 1;
                } else {
                    lastFlag = c;
                    argSize = 2;
                }
            } break;
            case 'l': {
                if (lastFlag == c) {
                    argSize = 8;
                } else {
                    lastFlag = c;
                    argSize = 4;
                }
            } break;
            case 'z': {
                argSize = 8;
            } break;
                // Extension to print binary
                // numbers of length n
            case 'b': {
                inEscapeCode = false;
                printNumber = true;
                base = 2;
            } break;
            case 'o': {
                inEscapeCode = false;
                printNumber = true;
                base = 8;
            } break;
            case 'u': {
                inEscapeCode = false;
                printNumber = true;
                printSign = false;
                base = 10;
            } break;
            case 'd': {
                inEscapeCode = false;
                printNumber = true;
                printSign = true;
                base = 10;
            } break;
            case 'x': {
                inEscapeCode = false;
                printNumber = true;
                hexBase = 'a';
                base = 16;
            } break;
            case 'X': {
                inEscapeCode = false;
                printNumber = true;
                hexBase = 'A';
                base = 16;
            } break;
            case 'p': {
                XKConsoleOutputString((OSUTF8Char *)"0x", 2);

                inEscapeCode = false;
                printNumber = true;
                hexBase = 'A';
                argSize = 8;
                base = 16;
            } break;
            case 'c': {
                UInt8 character = OSVAGetNext(args, UInt32);
                inEscapeCode = false;

                XKConsoleOutputString(&character, 1);
            } break;
            case 's': {
                inEscapeCode = false;

                OSUTF8Char *utf8string = OSVAGetNext(args, OSUTF8Char *);

                if (specialPadding) {
                    XKConsoleOutputString(utf8string, padding);
                    specialPadding = false;
                    padding = 1;
                } else {
                    XKConsoleOutputString(utf8string, XKUTF8Length(utf8string));
                }
            } break;
            case 'S': {
                inEscapeCode = false;

                OSUTF16Char *utf16string = OSVAGetNext(args, OSUTF16Char *);
                OSUTF8Char *utf8string = XKUTF16ToUTF8(utf16string);

                if (!utf8string)
                {
                    XKConsoleOutputString((OSUTF8Char *)"<error>", 7);

                    break;
                }

                if (specialPadding) {
                    XKConsoleOutputString(utf8string, padding);
                    specialPadding = false;
                    padding = 1;
                } else {
                    XKConsoleOutputString(utf8string, XKUTF8Length(utf8string));
                }

                XKFree(utf8string);
            } break;
                // Extension to print a hex string
                // from a given address
                //
                // The length of the string is
                // specified by the padding,
                // and the number of characters
                // in between spaces is specified
                // by the length modifiers used
                //
                // Because of the implementation
                // of the padding, the maximum
                // amount of data which can be
                // dumped is 255 pieces of the
                // given data size. This gives
                // an overall maximum of 2040
                // bytes with a padding of 255
                // and a 64-bit separator size
            case 'H': {
                inEscapeCode = false;
                hexDump = true;
            } break;
        }

        if (inPadding)
        {
            if ('0' <= c && c <= '9') {
                padding *= 10;
                padding += (c - '0');
            } else {
                inPadding = false;
            }
        }

        if (hexDump)
        {
            OSAddress argument = OSVAGetNext(args, OSAddress);
            UInt64 number = 0;

            while (padding)
            {
                #define entry(l) {                  \
                    UInt ## l *arg = argument;      \
                    number = (*arg);                \
                    argument += sizeof(UInt ## l);  \
                } break

                switch (argSize)
                {
                    case 1: entry(8);
                    case 2: entry(16);
                    case 4: entry(32);
                    case 8: entry(64);
                }

                #undef entry

                XKNumberToStringConverter(number, false, 16, (argSize * 2), 'A', true, kOSNullPointer, kOSNullPointer);

                if (padding != 1)
                    XKConsoleOutputString((OSUTF8Char *)" ", 1);

                padding--;
            }

            specialPadding = false;
            hexDump = false;
            lastFlag = 0;
            argSize = 4;
            padding = 1;
        }

        if (printNumber)
        {
            SInt64 number;

            if (!printSign) {
                switch (argSize)
                {
                    case 1:
                    case 2:
                    case 4:
                        number = OSVAGetNext(args, UInt32);
                        break;
                    case 8:
                        number = OSVAGetNext(args, UInt64);
                        break;
                    default: number = 0;
                }
            } else {
                switch (argSize)
                {
                    case 1:
                    case 2:
                    case 4:
                        number = OSVAGetNext(args, SInt32);
                        break;
                    case 8:
                        number = OSVAGetNext(args, SInt64);
                        break;
                    default: number = 0;
                }
            }

            XKNumberToStringConverter(number, printSign, base, padding, hexBase, true, kOSNullPointer, kOSNullPointer);

            specialPadding = false;
            printNumber = false;
            printSign = false;
            lastFlag = 0;
            padding = 1;
        }
    }

    if (charsToPrint)
    {
        OSUTF8Char *source = (format - charsToPrint);
        XKConsoleOutputString(source, charsToPrint);
    }
}
