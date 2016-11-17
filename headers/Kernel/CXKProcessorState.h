/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* CXKProcessorState.h - Functions get Retrieving Processor State  */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* beeselmane - 28.9.2016  - 3:30 PM EST                           */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/

#ifndef __KERNEL_CXKPROCESSOR__
#define __KERNEL_CXKPROCESSOR__ 1

#include <Corona-X.h>
#include <System/OSTypes.h>

#if kCXArchIA
#if !kCXAssemblyCode

typedef struct {
    UInt16 limit;
    OSAddress base;
    UInt16 pad[3];
} CXKProcessorGDTR;

typedef struct {
    UInt16 limit;
    OSAddress base;
    UInt16 pad[3];
} CXKProcessorIDTR;

typedef struct {
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
} CXKProcessorBasicState;

typedef struct {
    UInt64 cr0, cr2;
    UInt64 cr3, cr4, cr8;
    CXKProcessorGDTR gdtr;
    CXKProcessorIDTR idtr;
    UInt16 ldtr;
    UInt16 tr;
    UInt32 padding;
} CXKProcessorSystemState;

typedef struct {
    UInt64 dr0, dr1, dr2;
    UInt64 dr3, dr6, dr7;
} CXKProcessorDebugState;

typedef UInt64 CXKProcessorMSR;

OSShared CXKProcessorMSR CXKProcessorMSRRead(UInt32 id);
OSShared void CXKProcessorMSRWrite(UInt32 id, CXKProcessorMSR value);

// Note: rdi will always be the pointer passed in
OSShared void CXKProcessorGetBasicState(CXKProcessorBasicState *state);
OSShared void CXKProcessorGetSystemState(CXKProcessorSystemState *state);
OSShared void CXKProcessorGetDebugState(CXKProcessorDebugState *state);

/*OSInline void CXKProcessorGetGDTR(CXKProcessorGDTR gdt)
{
    __asm__("sgdt %0" : "=m" (gdt));
}

OSInline void CXKProcessorGetLDT(CXKProcessorSegmentSelector ldt)
{
    __asm__("sldt %0" : "=m" (ldt));
}

OSInline void CXKProcessorGetTR(CXKProcessorSegmentSelector tr)
{
    __asm__("str %0" : "=m" (tr));
}

OSInline void CXKProcessorGetIDTR(CXKProcessorIDTR idt)
{
    __asm__("sidt %0" : "=m" (idt));
}*/

#define CXKProcessorLoadRIP(pointer)                            \
    do {                                                        \
        __asm__ volatile("movq $1f, %0; 1:" : "=r" (pointer))   \
    } while (0)

#else /* kCXAssemblyCode */

#define CXKProcessorPushAll     \
    pushq %rsp;                 \
    pushq %rbp;                 \
    pushq %rax;                 \
    pushq %rbx;                 \
    pushq %rcx;                 \
    pushq %rdx;                 \
    pushq %rsi;                 \
    pushq %rdi

#define CXKProcessorPopAll      \
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

#endif /* !defined(__KERNEL_CXKPROCESSOR__) */
