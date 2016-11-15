#include <Kernel/CXKMemory.h>

static const UInt64 kCXKMask01_32 = 0x0000000100000001;
static const UInt64 kCXKMask80_32 = 0x8000000080000000;

static const UInt64 kCXKMask01_16 = 0x0001000100010001;
static const UInt64 kCXKMask80_16 = 0x8000800080008000;

static const UInt64 kCXKMask01_8  = 0x0101010101010101;
static const UInt64 kCXKMask80_8  = 0x8080808080808080;

#define test(p, l) (((*p) - kCXKMask01_ ## l) & ((~(*p)) & kCXKMask80_ ## l))

#define find(s, b, o)                                                                   \
    do {                                                                                \
        if (!(*(string + o)))                                                           \
            return (OSLength)((s - b) + o);                                             \
    } while (0)

#define CXKStringLength(s, l, ce)                                                       \
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

OSLength CXKStringSize32(UInt32 *utf32)
{
    CXKStringLength(utf32, 32, {
        find(string, utf32, 0);

        return (OSLength)((string - utf32) + 1);
    });
}

OSLength CXKStringSize16(UInt16 *utf16)
{
    CXKStringLength(utf16, 16, {
        find(string, utf16, 0);
        find(string, utf16, 1);
        find(string, utf16, 2);

        return (OSLength)((string - utf16) + 3);
    });
}

OSLength CXKStringSize8(UInt8 *utf8)
{
    CXKStringLength(utf8, 8, {
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
