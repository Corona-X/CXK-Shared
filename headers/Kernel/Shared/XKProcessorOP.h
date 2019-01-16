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

typedef OSPackedStructure() {
    UInt16 limit;
    OSAddress base;
} XKSegmentDescriptor;

#define fence(type)                             \
    do {                                        \
        __asm__ volatile(#type "fence" : : : ); \
    } while (0)

#define lfence() fence(l)
#define sfence() fence(s)
#define mfence() fence(m)

// cpuid
// rdmsr
// wrmsr
// rdtsc
// l/s/mfence
// lgdt/sgdt
// lidt/sidt
// ltr/str
// invlpg
// swapgs
// hlt
// wbinvd

#define cpuid(c)                                \
    do  {                                       \
    __asm__ volatile("cpuid" :                  \
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

#define sgdt(gdtr)                              \
    do  {                                       \
        __asm__ volatile("sgdt %0" :            \
                         "=m" (*(gdtr)) : :     \
                        );                      \
    } while (0)

#define sidt(idtr)                              \
    do  {                                       \
        __asm__ volatile("sidt %0" :            \
                         "=m" (*(idtr)) : :     \
                        );                      \
    } while (0)

#define swapgs()                                \
    do {                                        \
        __asm__ volatile("swapgs" : : : );      \
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

/*        OSInline void XKProcessorSetGDTR(XKProcessorDescriptor *gdtr)
        {
            __asm__("sgdt %0" : "=m" (*gdtr));
        }

        OSInline void XKProcessorSetIDTR(XKProcessorDescriptor *idtr)
        {
            __asm__("sidt %0" : "=m" (*idtr));
        }*/

        /*OSInline void XKProcessorGetLDT(XKProcessorSegmentSelector ldt)
         {
         __asm__("sldt %0" : "=m" (ldt));
         }

         OSInline void XKProcessorGetTR(XKProcessorSegmentSelector tr)
         {
         __asm__("str %0" : "=m" (tr));
         }*/

#endif /* kCXAssemblyCode */
#endif /* Architecture */

#endif /* !defined(__KERNEL_SHARED_XKPROCESSOROP__) */
