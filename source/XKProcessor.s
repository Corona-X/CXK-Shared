#include <Kernel/XKAssemblyCode.h>

.section kXKCodeSectionName
.align   kXKNaturalAlignment

XKDeclareFunction(XKProcessorMSRRead):
    movq %rdi, %rcx
    rdmsr
    shlq $32, %rdx
    orq %rdx, %rax
    ret

.align kXKNaturalAlignment

XKDeclareFunction(XKProcessorMSRWrite):
    movq %rdi, %rcx
    movq %rsi, %rdx
    movq %rsi, %rax
    andq $0xFFFFFFFF, %rax
    shrq $32, %rdx
    wrmsr
    ret

.align kXKNaturalAlignment

XKDeclareFunction(XKProcessorGetBasicState):
    pushfq
    movq %rax, 0x10(%rdi)
    movq %rbx, 0x18(%rdi)
    movq %rcx, 0x20(%rdi)
    movq %rdx, 0x28(%rdi)
    movq %r8,  0x30(%rdi)
    movq %r9,  0x38(%rdi)
    movq %r10, 0x40(%rdi)
    movq %r11, 0x48(%rdi)
    movq %r12, 0x50(%rdi)
    movq %r13, 0x58(%rdi)
    movq %r14, 0x60(%rdi)
    movq %r15, 0x68(%rdi)

    movq %rsi, 0x70(%rdi)
    movq %rdi, 0x78(%rdi)

    popq %rsi
    movq %rsi, 0x00(%rdi)

    // This is the return address for our function.
    popq %rsi
    movq %rsi, 0x08(%rdi)

    movq (%rsp), %rax
    movq %rax, 0x80(%rdi)
    movq %rbp, 0x88(%rdi)

    // Restore registers from before the call
    // (Even though this isn't necessary)
    movq 0x10(%rdi), %rax
    movq 0x70(%rdi), %rsi

    ret

.align kXKNaturalAlignment

XKDeclareFunction(XKProcessorGetSegmentState):
    movq %ds, 0x00(%rdi)
    movq %es, 0x00(%rdi)
    movq %fs, 0x00(%rdi)
    movq %gs, 0x00(%rdi)
    movq %cs, 0x00(%rdi)
    movq %ss, 0x00(%rdi)

    ret

.align kXKNaturalAlignment

XKDeclareFunction(XKProcessorGetControlState):
    movq %cr0, %rsi
    movq %cr2, %rdx
    movq %cr3, %rcx
    movq %cr4, %r8
    movq %cr8, %r9

    movq %rsi, 0x00(%rdi)
    movq %rdx, 0x08(%rdi)
    movq %rcx, 0x10(%rdi)
    movq %r8,  0x18(%rdi)
    movq %r9,  0x20(%rdi)

    ret

.align kXKNaturalAlignment

XKDeclareFunction(XKProcessorGetDebugState):
    movq %dr0, %rsi
    movq %dr1, %rdx
    movq %dr2, %rcx
    movq %dr3, %r8
    movq %dr6, %r9
    movq %dr7, %r11

    movq %rsi, 0x00(%rdi)
    movq %rdx, 0x08(%rdi)
    movq %rcx, 0x10(%rdi)
    movq %r8,  0x18(%rdi)
    movq %r9,  0x20(%rdi)
    movq %r11, 0x28(%rdi)

    ret
