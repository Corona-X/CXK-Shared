/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* OSByteMacros.h - Macros for working with byte quantities        */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* beeselmane - 27.11.2015 - 12:00 AM EST                          */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/

#ifndef __SYSTEM_OSBYTEMACROS__
#define __SYSTEM_OSBYTEMACROS__ 1

#include <Corona-X.h>
#include <System/OSTypes.h>
#include <System/OSCompilerMacros.h>

#if !kCXAssemblyCode

#define sizeofb(type)               (sizeof(type) * 8)

#define OSIsBetweenEx(l, v, h)      (((l) <  (v)) && ((v) <  (h)))
#define OSIsBetween(l, v, h)        (((l) <= (v)) && ((v) <= (h)))
#define OSAlignUpward(p, s)         (((p) + ((s) - 1)) & (~((s) - 1)))
#define OSAlignDown(p, s)           ((p) & (~((s) - 1)))

#define OSSwap16(n)                 __builtin_bswap16(n)
#define OSSwap32(n)                 __builtin_bswap32(n)
#define OSSwap64(n)                 __builtin_bswap64(n)

#define OSSwapFromLittleEndian16(n) OSSwapLittle16(n)
#define OSSwapFromLittleEndian32(n) OSSwapLittle32(n)
#define OSSwapFromLittleEndian64(n) OSSwapLittle64(n)

#define OSSwapToLittleEndian16(n)   OSSwapLittle16(n)
#define OSSwapToLittleEndian32(n)   OSSwapLittle32(n)
#define OSSwapToLittleEndian64(n)   OSSwapLittle64(n)

#define OSSwapFromBigEndian16(n)    OSSwapLittle16(n)
#define OSSwapFromBigEndian32(n)    OSSwapLittle32(n)
#define OSSwapFromBigEndian64(n)    OSSwapLittle64(n)

#define OSSwapToBigEndian16(n)      OSSwapLittle16(n)
#define OSSwapToBigEndian32(n)      OSSwapLittle32(n)
#define OSSwapToBigEndian64(n)      OSSwapLittle64(n)

#if kCXLittleEndian
    #define OSSwapLittle16(n)       ((UInt16)(n))
    #define OSSwapLittle32(n)       ((UInt32)(n))
    #define OSSwapLittle64(n)       ((UInt64)(n))

    #define OSSwapBig16(n)          OSSwap16(n)
    #define OSSwapBig32(n)          OSSwap32(n)
    #define OSSwapBig64(n)          OSSwap64(n)
#else /* Big Endian */
    #define OSSwapLittle16(n)       OSSwap16(n)
    #define OSSwapLittle32(n)       OSSwap32(n)
    #define OSSwapLittle64(n)       OSSwap64(n)

    #define OSSwapBig16(n)          ((UInt16)(n))
    #define OSSwapBig32(n)          ((UInt32)(n))
    #define OSSwapBig64(n)          ((UInt32)(n))
#endif /* Endian */

#define OSROL8(n, s)  ((UInt8 )((((UInt8 )(n)) << ((UInt8)(s))) | (((UInt8 )(n)) >> (8  - ((UInt8)(s))))))
#define OSROL16(n, s) ((UInt16)((((UInt16)(n)) << ((UInt8)(s))) | (((UInt16)(n)) >> (16 - ((UInt8)(s))))))
#define OSROL32(n, s) ((UInt32)((((UInt32)(n)) << ((UInt8)(s))) | (((UInt32)(n)) >> (32 - ((UInt8)(s))))))
#define OSROL64(n, s) ((UInt64)((((UInt64)(n)) << ((UInt8)(s))) | (((UInt64)(n)) >> (64 - ((UInt8)(s))))))

#define OSROR8(n, s)  ((UInt8 )((((UInt8 )(n)) >> ((UInt8)(s))) | (((UInt8 )(n)) << (8  - ((UInt8)(s))))))
#define OSROR16(n, s) ((UInt16)((((UInt16)(n)) >> ((UInt8)(s))) | (((UInt16)(n)) << (16 - ((UInt8)(s))))))
#define OSROR32(n, s) ((UInt32)((((UInt32)(n)) >> ((UInt8)(s))) | (((UInt32)(n)) << (32 - ((UInt8)(s))))))
#define OSROR64(n, s) ((UInt64)((((UInt64)(n)) >> ((UInt8)(s))) | (((UInt64)(n)) << (64 - ((UInt8)(s))))))

#define OSRoundToPowerOf2(v) ((UInt64)OSIRoundToPowerOf2(((UInt64)(v))))
#define OSReverseBits8(b)    ((UInt8)OSIReverseBits8(((UInt8)(b))))

OSInline UInt8 OSIReverseBits8(UInt8 byte)
{
    OSAlign(32) static const UInt8 table[0x100] = {
        0x00, 0x80, 0x40, 0xC0, 0x20, 0xA0, 0x60, 0xE0,
        0x10, 0x90, 0x50, 0xD0, 0x30, 0xB0, 0x70, 0xF0,
        0x08, 0x88, 0x48, 0xC8, 0x28, 0xA8, 0x68, 0xE8,
        0x18, 0x98, 0x58, 0xD8, 0x38, 0xB8, 0x78, 0xF8,
        0x04, 0x84, 0x44, 0xC4, 0x24, 0xA4, 0x64, 0xE4,
        0x14, 0x94, 0x54, 0xD4, 0x34, 0xB4, 0x74, 0xF4,
        0x0C, 0x8C, 0x4C, 0xCC, 0x2C, 0xAC, 0x6C, 0xEC,
        0x1C, 0x9C, 0x5C, 0xDC, 0x3C, 0xBC, 0x7C, 0xFC,
        0x02, 0x82, 0x42, 0xC2, 0x22, 0xA2, 0x62, 0xE2,
        0x12, 0x92, 0x52, 0xD2, 0x32, 0xB2, 0x72, 0xF2,
        0x0A, 0x8A, 0x4A, 0xCA, 0x2A, 0xAA, 0x6A, 0xEA,
        0x1A, 0x9A, 0x5A, 0xDA, 0x3A, 0xBA, 0x7A, 0xFA,
        0x06, 0x86, 0x46, 0xC6, 0x26, 0xA6, 0x66, 0xE6,
        0x16, 0x96, 0x56, 0xD6, 0x36, 0xB6, 0x76, 0xF6,
        0x0E, 0x8E, 0x4E, 0xCE, 0x2E, 0xAE, 0x6E, 0xEE,
        0x1E, 0x9E, 0x5E, 0xDE, 0x3E, 0xBE, 0x7E, 0xFE,
        0x01, 0x81, 0x41, 0xC1, 0x21, 0xA1, 0x61, 0xE1,
        0x11, 0x91, 0x51, 0xD1, 0x31, 0xB1, 0x71, 0xF1,
        0x09, 0x89, 0x49, 0xC9, 0x29, 0xA9, 0x69, 0xE9,
        0x19, 0x99, 0x59, 0xD9, 0x39, 0xB9, 0x79, 0xF9,
        0x05, 0x85, 0x45, 0xC5, 0x25, 0xA5, 0x65, 0xE5,
        0x15, 0x95, 0x55, 0xD5, 0x35, 0xB5, 0x75, 0xF5,
        0x0D, 0x8D, 0x4D, 0xCD, 0x2D, 0xAD, 0x6D, 0xED,
        0x1D, 0x9D, 0x5D, 0xDD, 0x3D, 0xBD, 0x7D, 0xFD,
        0x03, 0x83, 0x43, 0xC3, 0x23, 0xA3, 0x63, 0xE3,
        0x13, 0x93, 0x53, 0xD3, 0x33, 0xB3, 0x73, 0xF3,
        0x0B, 0x8B, 0x4B, 0xCB, 0x2B, 0xAB, 0x6B, 0xEB,
        0x1B, 0x9B, 0x5B, 0xDB, 0x3B, 0xBB, 0x7B, 0xFB,
        0x07, 0x87, 0x47, 0xC7, 0x27, 0xA7, 0x67, 0xE7,
        0x17, 0x97, 0x57, 0xD7, 0x37, 0xB7, 0x77, 0xF7,
        0x0F, 0x8F, 0x4F, 0xCF, 0x2F, 0xAF, 0x6F, 0xEF,
        0x1F, 0x9F, 0x5F, 0xDF, 0x3F, 0xBF, 0x7F, 0xFF
    };

    return table[byte];
}

OSInline UInt64 OSIRoundToPowerOf2(UInt64 value)
{
    value |= value >> 1;
    value |= value >> 2;
    value |= value >> 4;
    value |= value >> 8;
    value |= value >> 16;
    value |= value >> 32;
    value++;

    return value;
}

#if kCXArchIA
    #undef OSROL8
    #define OSROL8(n, s) ((UInt8)OSIROL8(((UInt8)(n)), ((UInt8)(s))))

    OSInline UInt8 OSIROL8(UInt8 n, UInt8 s)
    {
        __asm__ ("rolb %1, %0"
                 : "+mq" (n)
                 : "cI"  (s));

        return n;
    }

    #undef OSROR8
    #define OSROR8(n, s) ((UInt8)OSIROR8(((UInt8)(n)), ((UInt8)(s))))

    OSInline UInt8 OSIROR8(UInt8 n, UInt8 s)
    {
        __asm__ ("rorb %1, %0"
                 : "+mq" (n)
                 : "cI"  (s));

        return n;
    }

    #undef OSROL16
    #define OSROL16(n, s) ((UInt16)OSIROL16(((UInt16)(n)), ((UInt8)(s))))

    OSInline UInt16 OSIROL16(UInt16 n, UInt8 s)
    {
        __asm__ ("rolw %1, %0"
                 : "+g" (n)
                 : "cI" (s));

        return n;
    }

    #undef OSROR16
    #define OSROR16(n, s) ((UInt16)OSIROR16(((UInt16)(n)), ((UInt8)(s))))

    OSInline UInt16 OSIROR16(UInt16 n, UInt8 s)
    {
        __asm__ ("rorw %1, %0"
                 : "+g" (n)
                 : "cI" (s));

        return n;
    }

    #undef OSROL32
    #define OSROL32(n, s) ((UInt32)OSIROL32(((UInt32)(n)), ((UInt8)(s))))

    OSInline UInt32 OSIROL32(UInt32 n, UInt8 s)
    {
        __asm__ ("roll %1, %0"
                 : "+g" (n)
                 : "cI" (s));

        return n;
    }

    #undef OSROR32
    #define OSROR32(n, s) ((UInt32)OSIROR32(((UInt32)(n)), ((UInt8)(s))))

    OSInline UInt32 OSIROR32(UInt32 n, UInt8 s)
    {
        __asm__ ("rorl %1, %0"
                 : "+g" (n)
                 : "cI" (s));

        return n;
    }

    #undef OSROL64
    #define OSROL64(n, s) ((UInt64)OSIROL64(((UInt64)(n)), ((UInt8)(s))))

    OSInline UInt64 OSIROL64(UInt64 n, UInt8 s)
    {
        __asm__ ("rolq %1, %0"
                 : "+g" (n)
                 : "cI" (s));

        return n;
    }

    #undef OSROR64
    #define OSROR64(n, s) ((UInt64)OSIROR64(((UInt64)(n)), ((UInt8)(s))))

    OSInline UInt64 OSIROR64(UInt64 n, UInt8 s)
    {
        __asm__ ("rorq %1, %0"
                 : "+g" (n)
                 : "cI" (s));

        return n;
    }
#endif /* Architecture */

#endif /* !kCXAssemblyCode */

#endif /* !defined(__SYSTEM_OSBYTEMACROS__) */
