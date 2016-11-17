/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* CXKMemoryIO.h - Functions for Reading and Writing IO Ports      */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* beeselmane - 18.9.2016  - 5:00 PM EST                           */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/

#ifndef __KERNEL_CXKMEMORYIO__
#define __KERNEL_CXKMEMORYIO__ 1

#include <Corona-X.h>
#include <System/OSCompilerMacros.h>
#include <System/OSTypes.h>

#if kCXArchIA
#if !kCXAssemblyCode

OSInline UInt8 CXKReadByte(OSAddress port)
{
    UInt8 value;
    __asm__ volatile("inb %w1, %b0" : "=a" (value) : "Nd" (port));
    return value;
}

OSInline UInt16 CXKReadWord(OSAddress port)
{
    UInt16 value;
    __asm__ volatile("inw %w1, %w0" : "=a" (value) : "Nd" (port));
    return value;
}

OSInline UInt32 CXKReadDoubleWord(OSAddress port)
{
    UInt32 value;
    __asm__ volatile("inl %w1, %0" : "=a" (value) : "Nd" (port));
    return value;
}

OSInline void CXKWriteByte(OSAddress port, UInt8 byte)
{
    __asm__ volatile("outb %b0, %w1" : : "a" (byte), "Nd" (port));
}

OSInline void CXKWriteWord(OSAddress port, UInt16 word)
{
    __asm__ volatile("outw %w0, %w1" : : "a" (word), "Nd" (port));
}

OSInline void CXKWriteDoubleWord(OSAddress port, UInt32 doubleword)
{
    __asm__ volatile("outl %0, %w1" : : "a" (doubleword), "Nd" (port));
}

#endif /* !kCXAssemblyCode */
#endif /* Architecture */

#endif /* !defined(__KERNEL_CXKMEMORYIO__) */
