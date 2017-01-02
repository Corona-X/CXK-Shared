/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* OSCompilerMacros.h - Macros for basic compiler functions        */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* beeselmane - 27.11.2015 - 8:00 PM EST                           */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/

#ifndef __SYSTEM_OSCOMPILERMACROS__
#define __SYSTEM_OSCOMPILERMACROS__ 1

#include <Corona-X.h>

#if !kCXAssemblyCode

#if kCXLanguageCXX
    #define OSStartCBlock       extern "C" {
    #define OSFinishCBlock      }
#else /* Regular C */
    #define OSStartCBlock
    #define OSFinishCBlock
#endif /* C/C++ */

#if kCXCompilerMSC
    #if kCXLanguageCXX
        #define OSExport        extern "C" __declspec(dllimport)
    #else /* Regular C */
        #define OSExport        extern __declspec(dllimport)
    #endif /* C/C++ */

    #define OSInline            static __inline
    #define OSPrivate           extern
    #define OSInitializer       OSExport

    #define OSExpect(x)         ((x) == 1)
#else /* Clang or GCC */
    #define OSExpect(x)         __builtin_expect(!!(x), 1)

    #define OSExport            extern
    #define OSFormattedPrint    __attribute__((format(printf, 1, 2)))
    #define OSPrivate           __attribute__((visibility("hidden")))
    #define OSInitializer       __attribute__((constructor))
    #define OSWeakFunction      __attribute__((weak))
    #define OSNoReturn          __attribute__((noreturn))
    #define OSNotInline         __attribute__((noinline))
    #define OSEndCode()         __builtin_unreachable()
    #define OSAlias(s)          __attribute__((alias(#s)))
    #define OSAlign(n)          __attribute__((aligned(n)))
    #define OSStandardABI       __attribute__((sysv_abi))
    #define OSFault()           __builtin_trap()

    #if kCXGNUCVersion >= 4
        #define OSInline        static __inline __attribute__((always_inline))
    #elif kCXCompilerGCCLike
        #define OSInline        static __inline__
    #elif kCXLanguageCXX
        #define OSInline        static inline
    #endif

    #define OSPackedStructure   __attribute__((__packed__))
    #define OSAligned(n)        __attricute__((aligned(n)))
#endif /* Compiler */

#define OSStringValue(x)        #x
#define OSShared                OSExport OSStandardABI

/// TODO: Define Availability Macros

#endif /* !kCXAssemblyCode */

#endif /* !defined(__SYSTEM_OSCOMPILERMACROS__) */
