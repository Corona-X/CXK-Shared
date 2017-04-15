/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* XKLegacy.h - All legacy behaviour using I/O ports goes here     */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* beeselmane - 4.14.2017  -  6:15 PM PST                          */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/

#ifndef __KERNEL_XKLEGACY__
#define __KERNEL_XKLEGACY__ 1

#include <Corona-X.h>

#if kCXArchIA
    #define kXKDefaultPOSTPort 0x80

    #if kCXAssemblyCode
        #define XKSetPOSTCode(c)                \
            movb $(c), %al;                     \
            outb %al, $(kXKDefaultPOSTPort)
    #else /* !kCXAssemblyCode */
        #include <Kernel/XKPortIO.h>

        #define XKSetPOSTCode(c) XKWriteByte((OSAddress)kXKDefaultPOSTPort, ((XKPOSTCode)c))

        typedef UInt8 XKPOSTCode;

        OSInline UInt8 XKReadIOByte(OSAddress port)
        {
            UInt8 value;
            __asm__ volatile("inb %w1, %b0" : "=a" (value) : "Nd" (port));
            return value;
        }

        OSInline UInt16 XKReadIOWord(OSAddress port)
        {
            UInt16 value;
            __asm__ volatile("inw %w1, %w0" : "=a" (value) : "Nd" (port));
            return value;
        }

        OSInline UInt32 XKReadIODoubleWord(OSAddress port)
        {
            UInt32 value;
            __asm__ volatile("inl %w1, %0" : "=a" (value) : "Nd" (port));
            return value;
        }

        OSInline void XKWriteIOByte(OSAddress port, UInt8 byte)
        {
            __asm__ volatile("outb %b0, %w1" : : "a" (byte), "Nd" (port));
        }

        OSInline void XKWriteIOWord(OSAddress port, UInt16 word)
        {
            __asm__ volatile("outw %w0, %w1" : : "a" (word), "Nd" (port));
        }

        OSInline void XKWriteIODoubleWord(OSAddress port, UInt32 doubleword)
        {
            __asm__ volatile("outl %0, %w1" : : "a" (doubleword), "Nd" (port));
        }
    #endif /* Assembly Code */

#endif /* Architecture */

#endif /* !defined(__KERNEL_XKLEGACY__) */
