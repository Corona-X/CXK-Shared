/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* XKProcessorState.h - CPU state related definitions (dynamic)    */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* beeselmane - 28.9.2016  - 3:30 PM EST                           */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/

#ifndef __KERNEL_SHARED_XKPROCESSORSTATE__
#define __KERNEL_SHARED_XKPROCESSORSTATE__ 1

#include <Corona-X.h>
#include <System/OSTypes.h>

#if kCXArchIA
#if !kCXAssemblyCode

typedef OSPackedStructure() {
    UInt64 rflags, rip;
    UInt64 rax, rbx, rcx, rdx;
    UInt64 r8,  r9,  r10, r11;
    UInt64 r12, r13, r14, r15;
    UInt64 rsi, rdi;
    UInt64 rsp, rbp;
} XKProcessorBasicState;

typedef OSPackedStructure() {
    UInt16 ds, es;
    UInt16 fs, gs;
    UInt16 cs, ss;
} XKProcessorSegmentState;

typedef OSPackedStructure() {
    UInt64 cr0, cr2, cr3;
    UInt64 cr4, cr8;
} XKProcessorControlState;

typedef OSPackedStructure() {
    UInt64 dr0, dr1, dr2;
    UInt64 dr3, dr6, dr7;
} XKProcessorDebugState;

// Note: rdi will always be the pointer passed in. This is by nature of the ABI.
OSShared void XKProcessorGetBasicState(XKProcessorBasicState *state);
OSShared void XKProcessorGetSegmentState(XKProcessorSegmentState *state);
OSShared void XKProcessorGetControlState(XKProcessorControlState *state);
OSShared void XKProcessorGetDebugState(XKProcessorDebugState *state);

#define XKProcessorGetRIP(pointer)                              \
    do {                                                        \
        __asm__ volatile("movq $1f, %0; 1:" : "=r" (pointer))   \
    } while (0)

#endif /* kCXAssemblyCode */
#endif /* Architecture */

#endif /* !defined(__KERNEL_SHARED_XKPROCESSORSTATE__) */
