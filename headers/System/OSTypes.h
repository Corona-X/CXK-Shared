/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* OSTypes.h - System Types for all of the Corona-X System         */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* beeselmane - 27.11.2015 - 8:00 PM EST                           */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* beeselmane - 27.9.2016  - 1:30 AM EST                          */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* Note: This file does not follow standard prefix rules           */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/

#ifndef __SYSTEM_OSTYPES__
#define __SYSTEM_OSTYPES__ 1

#include <Corona-X.h>

#if !kCXAssemblyCode

#if kCXLanguageCXX
    #ifdef kCXLanguageGNUC
        #define kOSNullValue        0
    #else /* No GNU extensions */
        #define kOSNullValue        __null
    #endif /* Has GNU extensions */
#else /* Regular C */
    #define kOSNullValue            ((OSAddress)0x00000000)
#endif /* C/C++ */

#define kOSNullPointer              kOSNullValue
#define NULL                        kOSNullValue

#if kCXCompilerGCCLike
    #if defined(__SIZEOF_INT128__) && (__SIZEOF_INT128__ == 16)
        #define kOSInt128Available  1

        typedef unsigned __int128   UInt128;
        typedef   signed __int128   SInt128;
    #else /* unsupported */
        #define kOSInt128Available 0
    #endif /* __int128 support */

    #define OSVAStart               __builtin_va_start
    #define OSVAGetNext             __builtin_va_arg
    #define OSVACopy                __builtin_va_copy
    #define OSVAFinish              __builtin_va_end

    typedef __builtin_va_list       OSVAList;
#else /* other compiler */
    #define kOSInt128Available      0

    typedef void *OSVAList;
#endif /* clang or gcc */

#if kCXABI64Bit
    typedef unsigned long       UInt64;
    typedef unsigned int        UInt32;
    typedef unsigned short      UInt16;
    typedef unsigned char       UInt8;

    typedef   signed long       SInt64;
    typedef   signed int        SInt32;
    typedef   signed short      SInt16;
    typedef   signed char       SInt8;

    typedef UInt64              UIntN;
    typedef SInt64              SIntN;
#else /* kCXABI32Bit */
    typedef unsigned long long  UInt64;
    typedef unsigned long int   UInt32;
    typedef unsigned short      UInt16;
    typedef unsigned char       UInt8;

    typedef   signed long long  SInt64;
    typedef   signed long int   SInt32;
    typedef   signed short      SInt16;
    typedef   signed char       SInt8;

    typedef UInt32              UIntN;
    typedef SInt32              SIntN;
#endif /* ABI Version */

#if !kCXLanguageCXX
    #define true                1
    #define false               0

    typedef _Bool               bool;
#endif /* !C++ */

#define kOSSizeError            (OSSize)(~((OSSize)0))
#define kOSUTF8Error            (UInt8)(((UInt8)0))
#define kOSUTF16Error           (UInt16)(~((UInt16)0))
#define kOSUTF32Error           (UInt32)(~((UInt32)0))

typedef bool                    OSBoolean;
typedef double                  OSFloat64;
typedef float                   OSFloat32;
typedef SIntN                   OSInteger;
typedef UIntN                   OSPointerValue;
typedef void                   *OSAddress;
typedef void                   *OSFuncPtr;
typedef void                   *OSPointer;
typedef SInt64                  OSOffset;
typedef OSOffset                OSIndex;
typedef UInt64                  OSSize;
typedef OSSize                  OSLength;
typedef OSSize                  OSCount;

typedef UInt32                  OSUnicodePoint;
typedef UInt32                  OSUTF32Char;
typedef UInt16                  OSUTF16Char;
typedef UInt8                   OSUTF8Char;

typedef struct {
    OSAddress address;
    OSSize    size;
} OSBuffer;

#define kOSBufferEmpty      ((OSBuffer){kOSNullPointer, 0})
#define OSBufferIsEmpty(b)  (!b.size)
#define OSBufferMake(a, s)  ((OSBuffer){((OSAddress)(a)), ((OSSize)(s))})

#define __CXDeclareEnumWithName(type, name)     type name; enum
#define __CXDeclareEnumWithoutName(type)        enum
#define __CXDeclareEnum(a, b, function, ...)    function
#define CXEnum(...) __CXDeclareEnum(__VA_ARGS__, __CXDeclareEnumWithName, __CXDeclareEnumWithoutName)(__VA_ARGS__)

#endif /* !kCXAssemblyCode */

#define CXReflectMacro(...)                     __VA_ARGS__

#endif /* !defined(__SYSTEM_OSTYPES__) */
