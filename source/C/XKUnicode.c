#include <Kernel/C/XKUnicode.h>
#include <System/OSByteMacros.h>
#include <Kernel/Shared/XKSharedTarget.h>

#define kXKMask01_32        0x0000000100000001
#define kXKMask80_32        0x8000000080000000

#define kXKMask01_16        0x0001000100010001
#define kXKMask80_16        0x8000800080008000

#define kXKMask01_8         0x0101010101010101
#define kXKMask80_8         0x8080808080808080

#define kXKLengthError      0xFFFFFFFFFFFFFFFF

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

#pragma mark - Single character conversion

OSCount XKUTF8FromCodePoint(OSUnicodePoint point, OSUTF8Char *output, OSSize outputSize)
{
    #define next(o, b, p)               \
        b[o] = (p | 0x80) & 0xBF;       \
        p >>= 6

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

    if (OSExpect(point < 0x80)) {
        (*output) = (OSUTF8Char)point;
        return 1;
    } else if (point < 0x800) {
        entry(2, outputSize, point, output, {
            next(1, output, point);
        });
    } else if (point < 0x10000) {
        entry(3, outputSize, point, output, {
            next(2, output, point);
            next(1, output, point);
        });
    } else if (point < 0x200000) {
        entry(4, outputSize, point, output, {
            next(3, output, point);
            next(2, output, point);
            next(1, output, point);
        });
    } else {
        entry(5, outputSize, point, output, {
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

OSUnicodePoint XKUTF8ToCodePoint(const OSUTF8Char *input, OSSize inputSize, OSCount *used)
{
    if (!used) return kXKUTF32Error;

    // TODO: Make sure this works (make sure char is unsigned)
    OSCount count = kXKUTF8ExtraByteCount[*input];
    OSUnicodePoint point = 0;

    if (inputSize < count)
    {
        (*used) = inputSize;

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

OSCount XKUTF16FromCodePoint(OSUnicodePoint point, OSUTF16Char *output, OSSize outputSize)
{
    if (OSExpect(point < 0x10000)) {
        (*output) = point;

        return 1;
    } else if (outputSize >= 2) {
        point -= 0x10000;

        output[0] = (point >> 10)    + kXKSurrogateHighBegin;
        output[1] = (point & 0x3FFF) + kXKSurrogateLowBegin;

        return 2;
    } else {
        // Buffer is too small
        return 0;
    }
}

OSUnicodePoint XKUTF16ToCodePoint(const OSUTF16Char *input, OSSize inputSize, OSCount *used)
{
    OSUTF16Char first = (*input++);

    if (OSIsBetween(kXKSurrogateHighBegin, first, kXKSurrogateHighFinish)) {
        if (inputSize < 2) return kXKUTF32Error;

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

    OSUTF8Char *result = XKAllocate((utf8length + 1) * sizeof(OSUTF8Char));
    OSUTF8Char *end = result + utf8length;
    OSUTF8Char *utf8 = result;
    OSCount used;

    while (utf8 < end)
    {
        OSUnicodePoint codepoint = XKUTF16ToCodePoint(string, utf16length, &used);
        if (codepoint == kXKUTF32Error) goto failure;
        string += used;

        used = XKUTF8FromCodePoint(codepoint, utf8, utf8length);
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

    OSUTF8Char *result = XKAllocate((utf16length + 1) * sizeof(OSUTF8Char));
    OSUTF8Char *end = result + utf16length;
    OSUTF8Char *utf16 = result;
    OSCount used;

    while (utf16 < end)
    {
        OSUnicodePoint codepoint = XKUTF8ToCodePoint(string, utf8length, &used);
        if (codepoint == kXKUTF32Error) goto failure;
        string += used;

        used = XKUTF16FromCodePoint(codepoint, utf16, utf16length);
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

OSLength XKUTF8Length(const OSUTF8Char *rawString)
{
    UInt8 *utf8 = (UInt8 *)rawString;

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
