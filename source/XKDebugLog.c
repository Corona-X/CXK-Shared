#include <Kernel/XKSharedTarget.h>
#include <System/OSByteMacros.h>
#include <Kernel/XKDebugLog.h>

#if 0

static const UInt64 kXKMask01_32 = 0x0000000100000001;
static const UInt64 kXKMask80_32 = 0x8000000080000000;

static const UInt64 kXKMask01_16 = 0x0001000100010001;
static const UInt64 kXKMask80_16 = 0x8000800080008000;

static const UInt64 kXKMask01_8  = 0x0101010101010101;
static const UInt64 kXKMask80_8  = 0x8080808080808080;

static const OSLength kXKLengthError = 0xFFFFFFFFFFFFFFFF;

static const UInt8 kXKUTF8ExtraByteCount[0x100] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5
};

static const UInt64 kXKUTF8Excess[6] = {
    0x0000000000,
    0x0000003040,
    0x00000E2080,
    0x00E2041040,
    0x00FA082080,
    0x3F82082080
};

static const OSUTF8Char kXKUTF8BitMasks[7] = {
    0,
    0b00000000,
    0b11000000,
    0b11100000,
    0b11110000,
    0b11111000,
    0b11111100
};

#pragma mark - Unicode Functions

OSCount XKUTF8FromCodePoint(OSUnicodePoint point, const OSBuffer *buffer)
{
    #define next(o, b, p)               \
        b[o] = (p | 0x80) & 0xBF;       \
        p >>= 6;                        \

    #define finish(o, p, c)             \
        o[0] = p | kXKUTF8BitMasks[c]
    
    #define entry(b, c, p, o, entries)  \
        do {                            \
            if (c < b) return 0;        \
                                        \
            entries                     \
            finish(o, p, b);            \
                                        \
            return b;                   \
        } while (0)

    OSUTF8Char *output = (OSUTF8Char *)buffer->address;
    OSCount count = buffer->size;

    if (OSExpect(point < 0x80)) {
        (*output) = (OSUTF8Char)point;
        return 1;
    } else if (point < 0x800) {
        entry(2, count, point, output, {
            next(1, output, point);
        });
    } else if (point < 0x10000) {
        entry(3, count, point, output, {
            next(2, output, point);
            next(1, output, point);
        });
    } else if (point < 0x200000) {
        entry(4, count, point, output, {
            next(3, output, point);
            next(2, output, point);
            next(1, output, point);
        });
    } else {
        entry(5, count, point, output, {
            next(4, output, point);
            next(3, output, point);
            next(2, output, point);
            next(1, output, point);
        });
    }

    #undef entry
    #undef finish
    #undef next
}

OSUnicodePoint XKUTF8ToCodePoint(const OSBuffer *buffer, OSCount *used)
{
    const OSUTF8Char *input = (OSUTF8Char *)buffer->address;
    if (!used) return kXKUTF32Error;

    OSCount count = kXKUTF8ExtraByteCount[*input];
    OSUnicodePoint point = 0;

    if (buffer->size < count)
    {
        (*used) = buffer->size;
        return kXKUTF32Error;
    }

    switch (count)
    {
        case 5:
            point += (*input++);
            point <<= 6;
        case 4:
            point += (*input++);
            point <<= 6;
        case 3:
            point += (*input++);
            point <<= 6;
        case 2:
            point += (*input++);
            point <<= 6;
        case 1:
            point += (*input++);
            point <<= 6;
        case 0:
            point += (*input++);
    }

    point -= kXKUTF8Excess[count];
    (*used) = (count + 1);

    return point;
}

OSCount XKUTF16FromCodePoint(OSUnicodePoint point, const OSBuffer *buffer)
{
    OSUTF16Char *output = (OSUTF16Char *)buffer->address;

    if (OSExpect(point < 0x10000)) {
        (*output) = point;

        return 1;
    } else if (buffer->size >= 2) {
        point -= 0x10000;

        output[0] = (point >> 10)    + kXKSurrogateHighBegin;
        output[1] = (point & 0x3FFF) + kXKSurrogateLowBegin;

        return 2;
    } else {
        // Buffer is too small
        return 0;
    }
}

OSUnicodePoint XKUTF16ToCodePoint(const OSBuffer *buffer, OSCount *used)
{
    const OSUTF16Char *input = (OSUTF16Char *)buffer->address;
    OSUTF16Char first = (*input++);

    if (OSIsBetween(kXKSurrogateHighBegin, first, kXKSurrogateHighFinish)) {
        if (buffer->size < 2) return kXKUTF32Error;

        OSUTF16Char second = (*input);
        (*used) = 2;

        if (OSIsBetween(kXKSurrogateLowBegin, second, kXKSurrogateLowFinish)) {
            return ((OSUnicodePoint)((first << 10) + second) - 0x35FDC00);
        } else {
            return kXKUTF32Error;
        }
    } else if (OSIsBetween(kXKSurrogateLowBegin, first, kXKSurrogateLowFinish)) {
        (*used) = 1;

        return kXKUTF32Error;
    } else {
        (*used) = 1;

        return ((OSUnicodePoint)first);
    }
}

// FIXME: This doesn't do what it should do
OSLength XKUTF16LengthInUTF8(const OSUTF16Char *string)
{
    OSLength stringLength = XKUTF16Length(string);
    OSSize size = 0;

    for (OSCount i = 0; i < stringLength; i++)
    {
        OSUTF16Char character = string[i];

        if (OSIsBetween(kXKSurrogateHighBegin, character, kXKSurrogateHighFinish)) {
            if (!((i++) < stringLength)) return kXKLengthError;
            character = string[i];
            
            if (OSIsBetween(kXKSurrogateLowBegin, character, kXKSurrogateLowFinish)) {
                size += 2;
            } else {
                return kXKLengthError;
            }
        } else if (OSIsBetween(kXKSurrogateLowBegin, character, kXKSurrogateLowFinish)) {
            return kXKLengthError;
        } else {
            size++;
        }
    }

    return size;
}

OSLength XKUTF8LengthInUTF16(const OSUTF8Char *string)
{
    OSLength stringLength = XKUTF8Length(string);
    OSSize size = 0;

    for (OSCount i = 0; i < stringLength; i++)
    {
        UInt8 extraBytes = kXKUTF8ExtraByteCount[string[i]];
        i += extraBytes;

        if (extraBytes && (i >= stringLength))
            return kXKLengthError;

        if (extraBytes >= 2) {
            size += 2;
        } else {
            size++;
        }
    }

    return size;
}

// FIXME: This is inefficient
OSUTF8Char *XKUTF16ToUTF8(const OSUTF16Char *string)
{
    OSLength utf8length = XKUTF16LengthInUTF8(string);
    OSLength utf16length = XKUTF16Length(string);

    if (utf16length == kXKLengthError) return kOSNullPointer;
    if (utf8length == kXKLengthError) return kOSNullPointer;

    OSUTF8Char *result = XKAllocate((utf8length + 1) * sizeof(OSUTF8Char)).address;
    OSUTF8Char *end = result + utf8length;
    OSUTF8Char *utf8 = result;
    OSCount used;

    while (utf8 < end)
    {
        OSUnicodePoint codepoint = XKUTF16ToCodePoint(&OSBufferMake(string, utf16length), &used);
        if (codepoint == kXKUTF32Error) goto failure;
        string += used;

        used = XKUTF8FromCodePoint(codepoint, &OSBufferMake(utf8, utf8length));
        if (!used) goto failure;
        utf8 += used;
    }

    (*utf8) = 0;
    return result;

failure:
    XKFree(result);

    return kOSNullPointer;
}

// FIXME: This is inefficient
OSUTF16Char *XKUTF8ToUTF16(const OSUTF8Char *string)
{
    OSLength utf16length = XKUTF8LengthInUTF16(string);
    OSLength utf8length = XKUTF8Length(string);

    if (utf16length == kXKLengthError) return kOSNullPointer;
    if (utf8length == kXKLengthError) return kOSNullPointer;

    OSUTF8Char *result = XKAllocate((utf16length + 1) * sizeof(OSUTF8Char)).address;
    OSUTF8Char *end = result + utf16length;
    OSUTF8Char *utf16 = result;
    OSCount used;

    while (utf16 < end)
    {
        OSUnicodePoint codepoint = XKUTF8ToCodePoint(&OSBufferMake(string, utf8length), &used);
        if (codepoint == kXKUTF32Error) goto failure;
        string += used;

        used = XKUTF16FromCodePoint(codepoint, &OSBufferMake(utf16, utf16length));
        if (!used) goto failure;
        utf16 += used;
    }

    (*utf16) = 0;
    return result;

failure:
    XKFree(result);

    return kOSNullPointer;
}

#define test(p, l) (((*p) - kXKMask01_ ## l) & ((~(*p)) & kXKMask80_ ## l))

#define find(s, b, o)                                                                   \
    do {                                                                                \
        if (!(*(string + o)))                                                           \
            return (OSLength)((s - b) + o);                                             \
    } while (0)

#define XKStringLength(s, l, ce)                                                        \
    do {                                                                                \
        UInt64 *aligned = (((UInt64)s) & (UInt64)(~7));                                 \
                                                                                        \
        if (test(aligned, l))                                                           \
        {                                                                               \
            for (UInt ## l *string = s; string < (UInt ## l *)(aligned + 1); string++)  \
                if (!(*string)) return (OSLength)(string - s);                          \
        }                                                                               \
                                                                                        \
        for (aligned++; ; aligned++)                                                    \
        {                                                                               \
            if (test(aligned, l))                                                       \
            {                                                                           \
                UInt ## l *string = aligned;                                            \
                                                                                        \
                ce                                                                      \
            }                                                                           \
        }                                                                               \
    } while (0)

OSLength XKUTF32Length(const OSUTF32Char *utf32)
{
    XKStringLength(utf32, 32, {
        find(string, utf32, 0);
        
        return (OSLength)((string - utf32) + 1);
    });
}

OSLength XKUTF16Length(const OSUTF16Char *utf16)
{
    XKStringLength(utf16, 16, {
        find(string, utf16, 0);
        find(string, utf16, 1);
        find(string, utf16, 2);
        
        return (OSLength)((string - utf16) + 3);
    });
}

OSLength XKUTF8Length(const OSUTF8Char *utf8)
{
    XKStringLength(utf8, 8, {
        find(string, utf8, 0);
        find(string, utf8, 1);
        find(string, utf8, 2);
        find(string, utf8, 3);
        find(string, utf8, 4);
        find(string, utf8, 5);
        find(string, utf8, 6);
        
        return (OSLength)((string - utf8) + 7);
    });
}

#undef XKStringLength
#undef find
#undef test

#pragma mark - Type to String Converters

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

#pragma mark - Utility Functions

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
    OSUTF8Char buffer[64];
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

XKLogLevel gXKLogLevelCurrent = kXKLogLevelInfo;
XKConsole *gXKConsoleFirst = kOSNullPointer;

#pragma mark - Console Registeration Functions

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

#pragma mark - Console Output Functions

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

#pragma mark - Console Input functions

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

#pragma mark - Other Console Functions

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

#pragma mark - Graphics Functions

#include "XKBitmapFontData.h"

XKBitmapFont *gXKBitmapFont8x16;

XKBitmapFont gXKBitmapFont8x16Raw = {
    .height = 16,
    .width  = 8,
};

void XKGraphicsContextWriteCharacter(XKGraphicsContext *context, UInt8 character, XKGraphicsPoint location, XKBitmapFont *font, UInt32 color, UInt32 bgColor)
{
    UInt32 *rowPointer = context->framebuffer + ((location.y * context->width) + location.x);
    UInt8 *characterData = font->packedData + (character * font->height);
    
    for (OSCount y = 0; y < font->height; y++)
    {
        UInt8 data = characterData[y];
        
        for (SInt8 x = (font->width - 1); x >= 0; x--)
        {
            UInt8 state = (data >> x) & 1;
            UInt32 fillValue = (state ? color : bgColor);
            
            rowPointer[x] = fillValue;
        }
        
        rowPointer += context->width;
    }
}

void __XKBitmapFontInitialize(void)
{
    gXKBitmapFont8x16 = &gXKBitmapFont8x16Raw;
    gXKBitmapFont8x16->packedData = __apple_xnu_font;
}

#pragma mark - Video Console (EFI Framebuffer)

typedef struct {
    XKConsole rootConsole;
    XKGraphicsContext *context;
    XKBitmapFont *selectedFont;
    XKGraphicsPoint cursor;
    UInt32 baseOffsetX;
    UInt32 baseOffsetY;
    UInt32 height;
    UInt32 width;
    UInt32 color;
    UInt32 backgroundColor;
    bool isBackground;
} XKVideoConsole;

void XKVideoConsoleSetColor(XKConsole *console, UInt32 color)
{
    XKVideoConsole *videoConsole = (XKVideoConsole *)console;
    
    if (videoConsole->context->isBGRX) {
        videoConsole->color = color & 0x00FF00;
        videoConsole->color |= (color >> 16) & 0xFF;
        videoConsole->color |= (color & 0xFF) << 16;
    } else {
        videoConsole->color = color;
    }
}

void XKVideoConsoleSanitizeCursor(XKVideoConsole *console)
{
    if (console->cursor.x >= console->width)
    {
        console->cursor.x = 0;
        console->cursor.y++;
    }
    
    if (console->cursor.y >= console->height)
    {
        UInt32 rows = (console->height - console->cursor.y) + 1;
        
        OSCount blankPixelCount = console->context->width * rows * console->selectedFont->height;
        OSAddress blankOffset = (console->context->framebuffer + console->context->pixelCount) - blankPixelCount;
        OSAddress framebufferOffset = console->context->framebuffer + blankPixelCount;
        OSSize pixelCount = console->context->pixelCount - blankPixelCount;
        
        XKMemoryCopy(framebufferOffset, console->context->framebuffer, pixelCount * sizeof(UInt32));
        XKMemorySetValue(blankOffset, blankPixelCount * sizeof(UInt32), 0x00);
        
        console->cursor.y -= rows;
    }
}

void XKVideoConsoleWriteCharacter(UInt8 character, XKVideoConsole *console)
{
    XKGraphicsPoint outputLocation;
    outputLocation.y = (console->cursor.y * console->selectedFont->height) + console->baseOffsetY;
    outputLocation.x = (console->cursor.x * console->selectedFont->width ) + console->baseOffsetX;
    
    switch (character)
    {
        case '\n': {
            console->cursor.x = 0;
            console->cursor.y++;
        } break;
        case '\r': {
            console->cursor.x = 0;
        } break;
        case '\b': {
            if (!console->cursor.x) {
                if (console->cursor.y)
                {
                    console->cursor.x = console->width - 1;
                    console->cursor.y--;
                }
            } else {
                console->cursor.x--;
            }
        } break;
        case '\t': {
            for (UInt8 i = 0; i < 4; i++)
                XKVideoConsoleWriteCharacter(' ', console);
        } return;
        default: {
            //SLDelayProcessor(5000, !SLBootServicesHaveTerminated());
            XKGraphicsContextWriteCharacter(console->context, character, outputLocation, console->selectedFont, console->color, console->backgroundColor);
            console->cursor.x++;
        } break;
    }
    
    XKVideoConsoleSanitizeCursor(console);
}

void XKVideoConsoleWrite(OSUTF8Char *string, OSSize size, XKVideoConsole *console)
{
    for (UInt8 i = 0; i < size; i++)
        XKVideoConsoleWriteCharacter(string[i], console);
}

void XKVideoConsoleDelete(OSCount count, XKVideoConsole *console)
{
    UInt8 *spaces = XKAllocate(count).address;
    XKMemorySetValue(spaces, count, '\b');
    XKVideoConsoleWrite(spaces, count, console);
    XKMemorySetValue(spaces, count, ' ');
    XKVideoConsoleWrite(spaces, count, console);
    XKMemorySetValue(spaces, count, '\b');
    XKVideoConsoleWrite(spaces, count, console);
    XKFree(spaces);
}

#if kCXBootloaderCode
#include <SystemLoader/EFI/SLGraphics.h>
#include <SystemLoader/SLConfigFile.h>

void __XKVideoConsoleInitAll(void)
{
    SLConfigFile *config = SLConfigGet();
    
    if (!config->dev.videoConsole.enabled)
        return;
    
    SLGraphicsOutput **screens = SLGraphicsOutputGetAll(kOSNullPointer);
    UInt8 count = 0;
    
    if (!screens)
    {
        XKLog(kXKLogLevelWarning, "No output screens found!\n");
        return;
    }
    
    while (screens)
    {
        SLGraphicsOutput *screen = (*screens++);
        count++;
        
        if (count > config->dev.videoConsole.maxScreenCount)
            break;
        
        XKGraphicsContext *context = SLGraphicsOutputGetContextWithMaxSize(screen, config->dev.videoConsole.maxScreenHeight, config->dev.videoConsole.maxScreenWidth);
        XKMemorySetValue(context->framebuffer, context->framebufferSize, 0x00);
        XKVideoConsole *console = XKAllocate(sizeof(XKVideoConsole)).address;
        
        console->selectedFont = gXKBitmapFont8x16;
        console->context = context;
        console->cursor = ((XKGraphicsPoint){0, 0});
        console->height = context->height / console->selectedFont->height;
        console->width  = context->width  / console->selectedFont->width;
        
        if (context->isBGRX) {
            UInt32 backgroundColor = config->dev.videoConsole.backgroundColor;
            console->backgroundColor = ((backgroundColor & 0xFF0000) >> 16);
            console->backgroundColor |= (backgroundColor & 0x00FF00);
            console->backgroundColor |= (backgroundColor & 0x0000FF) << 16;
            
            UInt32 foregroundColor = config->dev.videoConsole.foregroundColor;
            console->color = ((foregroundColor & 0xFF0000) >> 16);
            console->color |= (foregroundColor & 0x00FF00);
            console->color |= (foregroundColor & 0x0000FF) << 16;
        } else {
            console->backgroundColor = config->dev.videoConsole.backgroundColor;
            console->color = config->dev.videoConsole.foregroundColor;
        }
        
        console->rootConsole.id = 0xFF;
        console->rootConsole.context = console;
        console->rootConsole.level = kXKLogLevelInfo;
        console->rootConsole.type = kXKConsoleTypeVideo;
        console->rootConsole.write = XKVideoConsoleWrite;
        console->baseOffsetY =       (context->height % console->selectedFont->height) / 2;
        console->baseOffsetX =       (context->width  % console->selectedFont->width)  / 2;
        console->rootConsole.delete = XKVideoConsoleDelete;
        
        XKConsoleRegister(&console->rootConsole);
    }
}
#endif /* kCXBootloaderCode */

#pragma mark - Serial Console (Legacy)

void XKSerialConsoleWrite(OSUTF8Char *string, OSSize size, XKSerialPort port)
{
    for (OSIndex i = 0; i < size; i++)
    {
        if (string[i] == '\n')
        {
            XKSerialWriteString(port, (UInt8 *)"\r\n");
            continue;
        }
        
        XKSerialWriteCharacter(port, string[i], true);
    }
}

OSUnicodePoint XKSerialConsoleRead(bool wait, XKSerialPort port)
{
    OSUnicodePoint character = XKSerialReadCharacter(port, wait);
    
    if (character == kXKSerialReadError)
    {
        XKLog(kXKLogLevelWarning, "Detected serial data transmission error!\n");
        character = kXKUTF32Error;
    }
    
    if (!character) character = kXKUTF32Error;
    return character;
}

void XKSerialConsoleDelete(OSCount count, XKSerialPort port)
{
    for (OSCount i = 0; i < count; i++)
        XKSerialWriteString(port, (UInt8 *)"\b \b");
}

#if kCXBootloaderCode
#include <SystemLoader/SLConfigFile.h>

void __XKSerialConsoleInitAll(void)
{
    SLConfigFile *config = SLConfigGet();
    
    if (!config->dev.serialConsole.enabled)
        return;
    
    
    for (OSIndex i = 0; i < config->dev.serialConsole.portCount; i++)
    {
        XKSerialPort port = XKSerialPortInit(config->dev.serialConsole.ports[i]);
        
        if (port == kXKSerialPortError)
        {
            XKPrintString("Error Loading Serial Port at '0x%04X'\n", config->dev.serialConsole.ports[i]);
            continue;
        }
        
        XKSerialPortSetupLineControl(port, config->dev.serialConsole.worldLength, config->dev.serialConsole.parityType, config->dev.serialConsole.stopBits);
        XKSerialPortSetBaudRate(port, config->dev.serialConsole.baudRate);
        
        XKConsole *console = XKAllocate(sizeof(XKConsole)).address;
        console->id = 0xFF;
        console->context = port;
        console->level = kXKLogLevelDebug;
        console->type = kXKConsoleTypeSerial;
        console->write = XKSerialConsoleWrite;
        console->read = XKSerialConsoleRead;
        console->delete = XKSerialConsoleDelete;
        XKConsoleRegister(console);
    }
}
#endif /* kCXBootloaderCode */

#pragma mark - EFI Input Console

#if kCXBootloaderCode
#include <SystemLoader/EFI/SLBootServices.h>
#include <SystemLoader/SLMemoryAllocator.h>
#include <SystemLoader/SLConfigFile.h>
#include <SystemLoader/SLLibrary.h>

static UInt8 gXKEFIInputConsoleID = 0xFF;

OSUnicodePoint XKEFIInputConsoleRead(bool wait, SLSystemTable *context)
{
    SLStatus status;
    SLKeyPress key;
    
    do {
        status = context->stdin->readKey(gSLLoaderSystemTable->stdin, &key);
    } while (wait && status == kSLStatusNotReady);
    
    return (key.keycode ? key.keycode : kXKUTF32Error);
}

void XKInputConsoleTerminateEFI(void)
{
    XKConsole *console = XKConsoleRemoveByID(gXKEFIInputConsoleID);
    XKFree(console);
}

void __XKInputConsoleInitAllEFI(void)
{
    SLBootServicesRegisterTerminationFunction(XKInputConsoleTerminateEFI, kOSNullPointer);
    SLSystemTable *systemTable = SLSystemTableGetCurrent();
    systemTable->stdin->reset(systemTable->stdin, false);
    
    XKConsole *console = XKAllocate(sizeof(XKConsole)).address;
    console->id = 0xFF;
    console->context = systemTable;
    console->read = XKEFIInputConsoleRead;
    XKConsoleRegister(console);
}
#endif /* kCXBootloaderCode */

#pragma mark - Logging Functions

void XKLog(XKLogLevel level, const OSUTF8Char *format, ...)
{
    //
}

#endif
