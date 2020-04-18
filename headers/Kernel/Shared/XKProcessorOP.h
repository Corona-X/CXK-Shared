/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* XKProcessorOP.h - Definitions for CPU native instructions in C  */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* beeselmane - 12.1.2019  - 6:00 PM EST                           */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/

#ifndef __KERNEL_SHARED_XKPROCESSOROP__
#define __KERNEL_SHARED_XKPROCESSOROP__ 1

#include <Corona-X.h>
#include <System/OSTypes.h>

#if kCXArchIA
#if !kCXAssemblyCode

#define fence(type)                             \
    do {                                        \
        __asm__ volatile(#type "fence" : : : ); \
    } while (0)

#define lfence() fence(l)
#define sfence() fence(s)
#define mfence() fence(m)

// Note: This macro assumes e{a,b,c,d}x exist as variables in the local scope
#define cpuid(c)                                \
    do  {                                       \
        __asm__ volatile("cpuid" :              \
                     "=a" (eax), "=b" (ebx) ,   \
                     "=c" (ecx), "=d" (edx) :   \
                      "a" (c)                   \
                    );                          \
    } while (0)

#define XKExtendedCPUID(a, b, c, d)             \
    do  {                                       \
        __asm__ volatile("cpuid" :              \
                         "=a" (a), "=b" (b) ,   \
                         "=c" (c), "=d" (d) :   \
                          "a" (a),  "b" (b) ,   \
                          "c" (c),  "d" (d) :   \
                        );                      \
    } while (0)

#define rdtsc(tsc)                              \
    do {                                        \
        UInt32 hi, lo;                          \
                                                \
        __asm__ volatile("rdtsc" :              \
                         "=a" (lo), "=b" (hi)   \
                         : :                    \
                        );                      \
    } while (0)

#define sgdt(gdtr)                              \
    do  {                                       \
        __asm__ volatile("sgdt %0" :            \
                         "=m" (*(gdtr)) : :     \
                        );                      \
    } while (0)

#define lgdt(gdtr)                              \
    do  {                                       \
        __asm__ volatile("lgdt %0" : :          \
                         "m" (*(gdtr)) :        \
                        );                      \
    } while (0)

#define sidt(idtr)                              \
    do  {                                       \
        __asm__ volatile("sidt %0" :            \
                         "=m" (*(idtr)) : :     \
                        );                      \
    } while (0)

#define lidt(idtr)                              \
    do {                                        \
        __asm__ volatile("ligt %0" : :          \
                         "m" (*(idtr)) :        \
                        );                      \
    } while (0)

#define sldt(seg)                               \
    do {                                        \
        __asm__ volatile("sldt %0" :            \
                         "=rm" (seg) : :        \
                        );                      \
    } while (0)

#define lldt(seg)                               \
    do {                                        \
        __asm__ volatile ("lldt %0" : :         \
                          "rm" (seg) :          \
                         );                     \
    } while (0)

#define str(seg)                                \
    do {                                        \
        __asm__ volatile("str %0" :             \
                         "=rm" (seg) : :        \
                        );                      \
    } while (0)

#define ltr(seg)                                \
    do {                                        \
        __asm__ volatile("ltr %0" : :           \
                         "rm" (seg) :           \
                        );                      \
    } while (0)

#define swapgs()                                \
    do {                                        \
        __asm__ volatile("swapgs" : : : );      \
    } while (0)

#define wbinvd()                                \
    do {                                        \
        __asm__ volatile("wbinvd" : : : );      \
    } while (0)

#define invlpg(addr)                            \
    do {                                        \
        __asm__ volatile("invlpg (%0)" : :      \
                         "r" (addr) :           \
                         "memory"               \
                        );                      \
    } while (0)

#define cli()                                   \
    do {                                        \
        __asm__ volatile("cli" : : : );         \
    } while (0)

#define sti()                                   \
    do {                                        \
        __asm__ volatile("sti" : : : );         \
    } while (0)

#define hlt()                                   \
    do {                                        \
        __asm__ volatile("hlt" : : : );         \
    } while (0)

OSInline void wrmsr(UInt32 msr, UInt64 value)
{
    UInt32 high = value >> 32;
    UInt32 low = value & 0xFFFFFFFF;

    __asm__ volatile("wrmsr" : : "c" (msr), "a" (low), "d" (high) : );
}

OSInline UInt64 rdmsr(UInt32 msr)
{
    UInt32 high, low;

    __asm__ volatile("rdmsr" : "=a" (low), "=d" (high) : "c" (msr) : );

    return ((((UInt64)high) << 32) | low);
}

#endif /* kCXAssemblyCode */
#endif /* Architecture */

#endif /* !defined(__KERNEL_SHARED_XKPROCESSOROP__) */
