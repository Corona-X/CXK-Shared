#include <Kernel/XKAssemblyCode.h>

.section kXKCodeSectionName
.align   kXKNaturalAlignment

XKDeclareFunction(XKProcessorReadID):
    pushq %rbx
    movl %esi, %eax
    cpuid
    movl %eax, 0x00(%rdi)
    movl %ebx, 0x04(%rdi)
    movl %ecx, 0x08(%rdi)
    movl %edx, 0x12(%rdi)
    popq %rbx
    ret

.align kXKNaturalAlignment

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
    movl %eax, %eax // Ensures top 32 bits of %rax are clear
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

    movq (%rsp), %rax
    movq %rax, 0x08(%rdi)

    movq %rsp, %rax
    movq %rax, 0x80(%rdi)
    movq %rbp, 0x88(%rdi)

    // Restore registers from before the call
    // (Even though this isn't necessary)
    movq 0x10(%rdi), %rax
    movq 0x70(%rdi), %rsi

    ret

.align kXKNaturalAlignment

XKDeclareFunction(XKProcessorGetSegmentState):
    movw %ds, 0x00(%rdi)
    movw %es, 0x02(%rdi)
    movw %fs, 0x04(%rdi)
    movw %gs, 0x06(%rdi)
    movw %cs, 0x08(%rdi)
    movw %ss, 0x0A(%rdi)

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
