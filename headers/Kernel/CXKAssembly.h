/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* CXKAssembly.h - Assembly Macros for Setup and Kernel            */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* beeselmane - 28.9.2016  - 4:00 PM EST                           */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/

#ifndef __KERNEL_CXKPROCESSOR__
#define __KERNEL_CXKPROCESSOR__ 1

#include <Corona-X.h>

#if kCXAssemblyCode

#define kCXKNaturalAlignment            8

#if kCXTargetOSApple
    #define kCXKCodeSectionName             __TEXT,__text
    #define CXKDeclareFunction(f)           \
        .globl _ ## f;                      \
                                            \
    _ ## f

    #define CXKLoadSymbol(n, r) movq _ ## n ## @GOTPCREL(%rip), r
    #define CXKDeclareLocal(f)  _ ## f
    #define CXKFunction(f)      _ ## f
    #define CXKSymbol(f)        _ ## f
#elif kCXTargetOSLinux
    #define kCXKCodeSectionName             .text
    #define CXKDeclareFunction(f)           \
        .globl f;                           \
        .type f, @function;                 \
                                            \
        f

    #define CXKDeclareLocal(f)  f
    #define CXKLoadSymbol(n, r) movq n ## @GOTPCREL(%rip), r
    #define CXKFunction(f)      f
    #define CXKSymbol(f)        f
#endif /* Target OS */

#endif /* kCXAssemblyCode */

#endif /* !defined(__KERNEL_CXKPROCESSOR__) */
