#include <System/OSByteMacros.h>
#include <Kernel/XKShared.h>

static const OSUnicodePoint kXKErrorCodePoint = 0xFFFFFFFF;
static const UInt16 kXKSerialPort0 = 0x03F8;

static const OSUTF8Char kXKUTF8BitMasks[7] = {
    0,
    0b00000000,
    0b11000000,
    0b11100000,
    0b11110000,
    0b11111000,
    0b11111100
};

OSCount XKUTF8FromUTF16(OSUTF16Char *input, OSUTF8Char *output, OSCount count, OSCount *used)
{
    #define next(o, b, p)               \
        b[o] = (p | 0x80) & 0xBF;       \
        p >>= 6;                        \

    #define finish(o, p, c) o[0] = p | kXKUTF8BitMasks[c]
        
    #define entry(b, c, p, o, entries)  \
        do {                            \
            if (c < b) return c;        \
                                        \
            entries                     \
            finish(o, p, b);            \
                                        \
            return b;                   \
    } while (0)

    OSUTF16Char first = (*input++);
    (*used) = 0;

    if (OSIsBetween(kXKSurrogateHighBegin, first, kXKSurrogateHighFinish)) {
        (*used) = 1;

        if (!count) return 0;

        OSUTF16Char second = (*input);

        if (OSIsBetween(kXKSurrogateLowBegin, second, kXKSurrogateLowFinish)) {
            OSUnicodePoint codepoint = ((OSUnicodePoint)((first << 10) | second) + 0x10000);

            if (codepoint < 0x200000) {
                entry(4, count, codepoint, output, {
                    next(3, output, codepoint);
                    next(2, output, codepoint);
                    next(1, output, codepoint);
                });
            } else {
                entry(5, count, codepoint, output, {
                    next(4, output, codepoint);
                    next(3, output, codepoint);
                    next(2, output, codepoint);
                    next(1, output, codepoint);
                });
            }
        } else {
            return 0;
        }
    } else if (OSIsBetween(kXKSurrogateLowBegin, first, kXKSurrogateLowFinish)) {
        (*used) = 1;
        return 0;
    } else {
        if (first < 0x80) {
            (*output) = (OSUTF8Char)first;
            return 1;
        } else if (first < 0x800) {
            entry(2, count, first, output, {
                next(1, output, first);
            });
        } else {
            entry(3, count, first, output, {
                next(2, output, first);
                next(1, output, first);
            });
        }
    }

    #undef entry
    #undef finish
    #undef next
}

static OSLength XKSimpleStringLengthUTF16(OSUTF16Char *string)
{
    OSLength length = 0;
    
    while (*string)
    {
        string++;
        length++;
    }

    return length;
}

void XKSerial0OutputUTF8(OSUTF8Char character)
{
    if (character == '\n')
        XKSerial0OutputUTF8('\r');

    while (!(XKReadIOByte(kXKSerialPort0 + 5) & 0x20)) {
        // Do nothing
    }

    XKWriteIOByte(kXKSerialPort0, character);
}

OSInline void XKPrintSingle(OSUTF8Char character)
{
    XKSerial0OutputUTF8(character);
}

OSInline void XKPrintUTF8Single(OSUTF8Char character[7])
{
    UInt8 n = 0;

    while (character[n])
    {
        XKSerial0OutputUTF8(character[n]);
        n++;
    }
}

OSInline void XKPrintNumber(UInt64 number, UInt8 base, UInt8 hexBegin, UInt8 padding)
{
    OSUTF8Char  buffer[32];
    SInt8 i = 0;
    
    if (!number)
    {
        if (padding)
            for ( ; i < padding; i++)
                XKPrintSingle('0');
        
        return;
    }

    do {
        UInt8 next = number % base;
        number /= base;
        
        if (next < 10) {
            buffer[i++] = next + '0';
        } else if (next < 37) {
            buffer[i++] = (next - 10) + hexBegin;
        } else {
            buffer[i++] = '#';
        }
    } while (number);
    
    if (i < padding)
    {
        UInt8 zeroCount = padding - i;
        
        for (UInt8 j = 0; j < zeroCount; j++)
            XKPrintSingle('0');
    }
    
    for (i--; i >= 0; i--)
        XKPrintSingle(buffer[i]);
}

void XKPrintSerial(const char *f, ...)
{
    OSUTF8Char *format = (OSUTF8Char *)f;

    bool justEnteredEscapeCode = false;
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

    OSVAList args;
    OSVAStart(args, f);

    while (*format)
    {
        UInt8 character = *format++;

        if (!inEscapeCode && character != '%')
        {
            XKPrintSingle(character);
            
            continue;
        }

        if (!inEscapeCode)
        {
            justEnteredEscapeCode = true;
            inEscapeCode = true;
            
            continue;
        }

        if (justEnteredEscapeCode)
        {
            justEnteredEscapeCode = false;

            if (character == '%')
            {
                XKPrintSingle(character);
                inEscapeCode = false;

                continue;
            }
        }

        // Yes, I realize that this will allow an
        // infinite number of size flags to be
        // used, and that the last size flag given
        // will take precedence over all others.
        // This doesn't matter, however, it doesn't
        // actually expose anything that could be
        // exploited.
        switch (character)
        {
            case '0': {
                inPadding = true;
                padding = 0;
            } break;
            case 'h': {
                if (lastFlag == character) {
                    argSize = 1;
                } else {
                    lastFlag = character;
                    argSize = 2;
                }
            } break;
            case 'l': {
                if (lastFlag == character) {
                    argSize = 8;
                } else {
                    lastFlag = character;
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
                XKPrintSingle('0');
                XKPrintSingle('x');
                
                inEscapeCode = false;
                printNumber = true;
                hexBase = 'A';
                argSize = 8;
                base = 16;
            } break;
            case 'c': {
                UInt8 character = OSVAGetNext(args, UInt32);
                inEscapeCode = false;

                XKPrintSingle(character);
            } break;
            case 's': {
                inEscapeCode = false;
                OSUTF8Char *utf8string = OSVAGetNext(args, OSUTF8Char *);

                while (*utf8string)
                {
                    XKPrintSingle(*utf8string);
                    utf8string++;
                }
            } break;
            case 'S': {
                inEscapeCode = false;
                OSUTF16Char *utf16string = OSVAGetNext(args, OSUTF16Char *);
                OSCount charsLeft = XKSimpleStringLengthUTF16(utf16string);
                OSUTF16Char *end = utf16string + charsLeft;
                OSCount used, used8;
                OSUTF8Char utf8[7];

                while (utf16string < end)
                {
                    used8 = XKUTF8FromUTF16(utf16string, utf8, charsLeft, &used);
                    utf8[used8] = 0;

                    XKPrintSingle(utf8);
                    utf16string += used + 1;
                    charsLeft -= (used + 1);
                }
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
            if ('0' <= character && character <= '9') {
                padding *= 10;
                padding += (character - '0');
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

                XKPrintNumber(number, 16, 'A', argSize * 2);
                if (padding != 1) XKPrintSingle(' ');
                padding--;
            }

            hexDump = false;
            argSize = 4;
            padding = 1;
        }

        if (printNumber)
        {
            UInt64 number;

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
                SInt64 signedValue;
                
                switch (argSize)
                {
                    case 1:
                    case 2:
                    case 4:
                        signedValue = OSVAGetNext(args, SInt32);
                        break;
                    case 8:
                        signedValue = OSVAGetNext(args, SInt64);
                        break;
                    default: signedValue = 0;
                }
                
                if (signedValue < 0)
                {
                    XKPrintSingle('-');
                    signedValue *= -1;
                }

                printSign = false;
                number = signedValue;
            }

            XKPrintNumber(number, base, hexBase, padding);
            printNumber = false;
            padding = 1;
        }
    }

    OSVAFinish(args);
}
