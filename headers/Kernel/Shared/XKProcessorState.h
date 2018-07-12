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
    UInt16 pad[3];
} XKProcessorGDTR;

typedef OSPackedStructure() {
    UInt16 limit;
    OSAddress base;
    UInt16 pad[3];
} XKProcessorIDTR;

typedef OSPackedStructure() {
    UInt64 rax, rbx;
    UInt64 rcx, rdx;
    UInt64 r8,  r9,  r10, r11;
    UInt64 r12, r13, r14, r15;
    UInt64 rsi, rdi;
    UInt64 rbp;
    UInt64 rsp;
    UInt64 rip;
    UInt64 rflags;
    UInt16 cs, ds, ss;
    UInt16 es, fs, gs;
    UInt32 padding;
} XKProcessorBasicState;

typedef OSPackedStructure() {
    UInt64 cr0, cr2;
    UInt64 cr3, cr4, cr8;
    XKProcessorGDTR gdtr;
    XKProcessorIDTR idtr;
    UInt16 ldtr;
    UInt16 tr;
    UInt32 padding;
} XKProcessorSystemState;

typedef struct {
    UInt64 dr0, dr1, dr2;
    UInt64 dr3, dr6, dr7;
} XKProcessorDebugState;

typedef UInt64 XKProcessorMSR;

OSShared XKProcessorMSR XKProcessorMSRRead(UInt32 id);
OSShared void XKProcessorMSRWrite(UInt32 id, XKProcessorMSR value);

// Note: rdi will always be the pointer passed in
OSShared void XKProcessorGetBasicState(XKProcessorBasicState *state);
OSShared void XKProcessorGetSystemState(XKProcessorSystemState *state);
OSShared void XKProcessorGetDebugState(XKProcessorDebugState *state);

OSInline void XKProcessorGetGDTR(XKProcessorGDTR gdt)
{
    __asm__("sgdt %0" : "=m" (gdt));
}

/*OSInline void XKProcessorGetLDT(XKProcessorSegmentSelector ldt)
{
    __asm__("sldt %0" : "=m" (ldt));
}

OSInline void XKProcessorGetTR(XKProcessorSegmentSelector tr)
{
    __asm__("str %0" : "=m" (tr));
}*/

OSInline void XKProcessorGetIDTR(XKProcessorIDTR idt)
{
    __asm__("sidt %0" : "=m" (idt));
}

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
