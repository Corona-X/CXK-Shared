/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* XKProcessorState.h - CPU state related operations               */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* beeselmane - 28.9.2016  - 3:30 PM EST                           */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/

#ifndef __KERNEL_XKPROCESSOR__
#define __KERNEL_XKPROCESSOR__ 1

#include <Corona-X.h>
#include <System/OSTypes.h>

#if kCXArchIA
#if !kCXAssemblyCode

typedef OSPackedStructure() {
    UInt16 limit;
    OSAddress base;
} XKProcessorDescriptor;

typedef OSPackedStructure() {
    UInt64 rflags, rip;
    UInt64 rax, rbx, rcx, rdx;
    UInt64 r8,  r9,  r10, r11;
    UInt64 r12, r13, r14, r15;
    UInt64 rsi, rdi;
    UInt64 rsp, rbp;
} XKProcessorBasicState;

typedef OSPackedStructure() {
    UInt64 ds, es;
    UInt64 fs, gs;
    UInt64 cs, ss;
} XKProcessorSegmentState;

typedef OSPackedStructure() {
    UInt64 cr0, cr2, cr3;
    UInt64 cr4, cr8;
} XKProcessorControlState;

typedef OSPackedStructure() {
    UInt64 dr0, dr1, dr2;
    UInt64 dr3, dr6, dr7;
} XKProcessorDebugState;

typedef UInt64 XKProcessorMSR;

OSShared XKProcessorMSR XKProcessorMSRRead(UInt32 id);
OSShared void XKProcessorMSRWrite(UInt32 id, XKProcessorMSR value);

// Note: rdi will always be the pointer passed in. This is by nature of the ABI.
OSShared void XKProcessorGetBasicState(XKProcessorBasicState *state);
OSShared void XKProcessorGetSegmentState(XKProcessorSegmentState *state);
OSShared void XKProcessorGetControlState(XKProcessorControlState *state);
OSShared void XKProcessorGetDebugState(XKProcessorDebugState *state);

OSInline void XKProcessorGetGDTR(XKProcessorDescriptor *gdtr)
{
    __asm__("sgdt %0" : "=m" (*gdtr));
}

OSInline void XKProcessorGetIDTR(XKProcessorDescriptor *idtr)
{
    __asm__("sidt %0" : "=m" (*idtr));
}

/*OSInline void XKProcessorGetLDT(XKProcessorSegmentSelector ldt)
{
    __asm__("sldt %0" : "=m" (ldt));
}

OSInline void XKProcessorGetTR(XKProcessorSegmentSelector tr)
{
    __asm__("str %0" : "=m" (tr));
}*/

OSShared UInt64 XKProcessorGetSpeed(void);

#define XKProcessorLoadRIP(pointer)                             \
    do {                                                        \
        __asm__ volatile("movq $1f, %0; 1:" : "=r" (pointer))   \
    } while (0)

#else /* kCXAssemblyCode */

#define XKProcessorPushAll      \
    pushq %rsp;                 \
    pushq %rbp;                 \
    pushq %rax;                 \
    pushq %rbx;                 \
    pushq %rcx;                 \
    pushq %rdx;                 \
    pushq %rsi;                 \
    pushq %rdi

#define XKProcessorPopAll       \
    popq %rdi;                  \
    popq %rsi;                  \
    popq %rdx;                  \
    popq %rcx;                  \
    popq %rbx;                  \
    popq %rax;                  \
    popq %rbp;                  \
    popq %rsp

#endif /* kCXAssemblyCode */
#endif /* Architecture */

#endif /* !defined(__KERNEL_XKPROCESSOR__) */
