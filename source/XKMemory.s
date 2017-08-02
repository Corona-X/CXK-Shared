#include <Kernel/XKAssemblyCode.h>

.section kXKCodeSectionName
.align   kXKNaturalAlignment

XKDeclareFunction(memset):
    xchgq %rsi, %r8
    movb %cl, %sil

XKDeclareFunction(XKMemorySetValue):
    movb %dl, %al
    movq %rsi, %rcx
    cld

    cmpq $0x0F, %rcx
    jle 1f

    movzbq %al, %rsi
    movabsq $0x0101010101010101, %rax
    imulq %rsi, %rax

    movq %rdi, %rdx
    negq %rdx
    andq $7, %rdx
    movq %rcx, %r8
    subq %rdx, %r8
    movq %rdx, %rcx
    rep stosb

    movq %r8, %rcx
    shrq $3, %rcx
    rep stosq

    movq %r8, %rcx
    andq $7, %rcx

    1:
        rep stosb
        ret

XKDeclareFunction(XKMemoryCopy):
    xchgq %rsi, %rdi
    cld

    movq %rdx, %rcx
    shrq $3, %rcx
    rep movsq

    movq %rdx, %rcx
    andq $7, %rcx
    rep movsb

    ret

XKDeclareFunction(XKMemoryCompare):
    movq %rdx, %rcx
    cld

    shrq $3, %rcx
    repe cmpsq
    jne 1f

    movq %rdx, %rcx
    andq $7, %rcx
    repe cmpsb
    jne 2f

    xorw %ax, %ax
    ret

    1:
        movl $8, %ecx
        subq %rcx, %rdi
        subq %rcx, %rsi
        repe cmpsb

    2:
        xorw %ax, %ax
        movb -1(%rdi), %al
        xorw %dx, %dx
        movb -1(%rsi), %dl
        subw %dx, %ax
        ret

XKDeclareFunction(XKStringCompare8):
    1:
        testb $7, %dil
        je 2f

        movb (%rdi), %al
        movb (%rsi), %dl
        testb %al, %al
        je 5f

        incq %rdi
        incq %rsi
        cmpb %al, %dl
        je 1b

        jmp 5f

    2:
        testb $7, %sil
        jne 4f
        movabsq $0x0101010101010101, %r8
        movabsq $0x8080808080808080, %r9
        subq $8, %rdi
        subq $8, %rsi

        .align kXKNaturalAlignment
    3:
        movq 8(%rdi), %rax
        movq 8(%rsi), %rdx
        addq $8, %rdi
        addq $8, %rsi
        cmpq %rax, %rdx
        jne 4f

        subq %r8, %rdx
        notq %rax
        andq %rax, %rdx
        testq %r9, %rdx
        je 4f

        .align kXKNaturalAlignment
    4:
        movb (%rdi), %al
        movb (%rsi), %dl
        incq %rdi
        incq %rsi
        testb %al, %al
        je 5f

        cmpb %al, %dl
        je 4b

    5:
        movzbq %al, %rax
        movzbq %dl, %rdx
        subq %rdx, %rax
        ret
