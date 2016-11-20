#include <Kernel/XKAssembly.h>

.section kXKCodeSectionName
.align   kXKNaturalAlignment

XKDeclareFunction(XKProcessorMSRRead):
    movq %rdi, %rcx
    rdmsr
    shlq $32, %rdx
    orq %rdx, %rax
    ret

XKDeclareFunction(XKProcessorMSRWrite):
    movq $0x0, %rax
    ret

.align kXKNaturalAlignment

XKDeclareFunction(XKProcessorGetBasicState):
    pushfq
    movq %rax, 0x00(%rdi)
    movq %rbx, 0x08(%rdi)
    movq %rcx, 0x10(%rdi)
    movq %rdx, 0x18(%rdi)
    movq %r8,  0x20(%rdi)
    movq %r9,  0x28(%rdi)
    movq %r10, 0x30(%rdi)
    movq %r11, 0x38(%rdi)
    movq %r12, 0x40(%rdi)
    movq %r13, 0x48(%rdi)
    movq %r14, 0x50(%rdi)
    movq %r15, 0x58(%rdi)
    movq %rsi, 0x60(%rdi)
    movq %rdi, 0x68(%rdi)
    movq %rbp, 0x70(%rdi)
    popq %rsi

    movq (%rsp), %rax
    decq %rax
    addq $8, %rsp
    movq %rsp, 0x78(%rdi)
    movq %rax, 0x80(%rdi)
    subq $8, %rsp

    movq %rsi, 0x88(%rdi)
    movw %cs,  0x90(%rdi)
    movw %ds,  0x92(%rdi)
    movw %ss,  0x94(%rdi)
    movw %es,  0x96(%rdi)
    movw %fs,  0x98(%rdi)
    movw %gs,  0x9A(%rdi)
    ret

.align kXKNaturalAlignment

XKDeclareFunction(XKProcessorGetSystemState):
    movq %cr0, %rax
    movq %cr2, %rcx
    movq %cr3, %rdx
    movq %cr4, %r8
    movq %cr8, %r9
    movq %rax, 0x00(%rdi)
    movq %rcx, 0x08(%rdi)
    movq %rdx, 0x10(%rdi)
    movq %rsi, 0x18(%rdi)
    movq %r8,  0x20(%rdi)
    movq %r9,  0x28(%rdi)
    sgdt 0x30(%rdi)
    sidt 0x40(%rdi)
    sldt 0x50(%rdi)
    str  0x52(%rdi)
    ret

XKDeclareFunction(XKProcessorGetDebugState):
    movq %dr0, %rax
    movq %dr1, %rcx
    movq %dr2, %rdx
    movq %dr3, %rsi
    movq %dr6, %r8
    movq %dr7, %r9
    movq %rax, 0x00(%rdi)
    movq %rcx, 0x08(%rdi)
    movq %rdx, 0x10(%rdi)
    movq %rsi, 0x18(%rdi)
    movq %r8,  0x20(%rdi)
    movq %r9,  0x28(%rdi)
    ret
