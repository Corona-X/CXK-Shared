/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* XKAssembly.h - Assembly Macros for Setup and Kernel             */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* beeselmane - 28.9.2016  - 4:00 PM EST                           */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/

#ifndef __KERNEL_XKASSEMBLY__
#define __KERNEL_XKASSEMBLY__ 1

#include <Corona-X.h>

#define kXKNaturalAlignment            8

#if kCXAssemblyCode

#if kCXTargetOSApple
    #define kXKCodeSectionName             __TEXT,__text
    #define XKDeclareFunction(f)           \
        .globl _ ## f;                      \
                                            \
    _ ## f

    #define XKLoadSymbol(n, r) movq _ ## n ## @GOTPCREL(%rip), r
    #define XKDeclareLocal(f)  _ ## f
    #define XKSymbol(f)        _ ## f
#elif kCXTargetOSLinux
    #define kXKCodeSectionName             .text
    #define XKDeclareFunction(f)           \
        .globl f;                           \
        .type f, @function;                 \
                                            \
        f

    #define XKLoadSymbol(n, r) movq n ## @GOTPCREL(%rip), r
    #define XKDeclareLocal(f)  f
    #define XKSymbol(f)        f
#endif /* Target OS */

#endif /* kCXAssemblyCode */

#endif /* !defined(__KERNEL_XKASSEMBLY__) */
