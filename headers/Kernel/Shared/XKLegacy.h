/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* XKLegacy.h - All legacy behaviour using I/O ports goes here     */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* beeselmane - 14.4.2017  -  6:15 PM PST                          */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/

#ifndef __KERNEL_XKLEGACY__
#define __KERNEL_XKLEGACY__ 1

#include <Corona-X.h>
#include <System/OSTypes.h>
#include <System/OSCompilerMacros.h>

#if kCXArchIA
    #define kXKSerialDefaultSpeed       1843200
    #define kXKSerialDefaultPort        0x03F8
    #define kXKSerialPortError          0xFFFF
    // Just a note: I chose the read error character
    // to be the form feed character, as I decided
    // that the form feed character is probably the
    // least useful 8-bit unicode/ASCII character (hah)
    #define kXKSerialReadError          0x0C

    #define kXKSerialWordLength5Bits    0x0
    #define kXKSerialWordLength6Bits    0x1
    #define kXKSerialWordLength7Bits    0x2
    #define kXKSerialWordLength8Bits    0x3

    #define kXKSerial1StopBit           0x0
    #define kXKSerial2StopBits          0x1

    #define kXKSerialNoParity           0x0
    #define kXKSerialOddParity          0x1
    #define kXKSerialEvenParity         0x3
    #define kXKSerialMarkParity         0x5
    #define kXKSerialSpaceParity        0x7

    #define kXKDefaultPOSTPort          0x80

    #if kCXAssemblyCode
        #define XKSetPOSTCode(c)                \
            movb $(c), %al;                     \
            outb %al, $(kXKDefaultPOSTPort)
    #else /* !kCXAssemblyCode */
        #define XKSetPOSTCode(c) XKWriteByte((OSAddress)kXKDefaultPOSTPort, ((XKPOSTCode)c))

        typedef UInt16 XKIOPort;
        typedef XKIOPort XKSerialPort;
        typedef UInt8 XKPOSTCode;

        /**
         * This function reads a byte from a given I/O port address.
         *
         * @argument port:
         *   The port address to read.
         *
         * @return the byte value received from reading the port.
         */
        OSInline UInt8 XKReadIOByte(XKIOPort port)
        {
            UInt8 value;
            __asm__ volatile("inb %w1, %b0" : "=a" (value) : "Nd" (port));
            return value;
        }

        /**
         * This function reads a word from a given I/O port address.
         *
         * @argument port:
         *   The port address to read.
         *
         * @return the word received from reading the port.
         */
        OSInline UInt16 XKReadIOWord(XKIOPort port)
        {
            UInt16 value;
            __asm__ volatile("inw %w1, %w0" : "=a" (value) : "Nd" (port));
            return value;
        }


        /**
         * This function reads a double word from a given I/O port address.
         *
         * @argument port:
         *   The port address to read.
         *
         * @return the double word received from reading the port.
         */
        OSInline UInt32 XKReadIODoubleWord(XKIOPort port)
        {
            UInt32 value;
            __asm__ volatile("inl %w1, %0" : "=a" (value) : "Nd" (port));
            return value;
        }

        /**
         * This function writes a byte to a given I/O port address.
         *
         * @argument port:
         *   The port address to write to.
         *
         * @argument byte:
         *   The byte to write to the give address.
         */
        OSInline void XKWriteIOByte(XKIOPort port, UInt8 byte)
        {
            __asm__ volatile("outb %b0, %w1" : : "a" (byte), "Nd" (port));
        }

        /**
         * This function writes a word to a given I/O port address.
         *
         * @argument port:
         *   The port address to write to.
         *
         * @argument word:
         *   The word to write to the give address.
         */
        OSInline void XKWriteIOWord(XKIOPort port, UInt16 word)
        {
            __asm__ volatile("outw %w0, %w1" : : "a" (word), "Nd" (port));
        }

        /**
         * This function writes a double word to a given I/O port address.
         *
         * @argument port:
         *   The port address to write to.
         *
         * @argument doubleWord:
         *   The double word to write to the give address.
         */
        OSInline void XKWriteIODoubleWord(XKIOPort port, UInt32 doubleWord)
        {
            __asm__ volatile("outl %0, %w1" : : "a" (doubleWord), "Nd" (port));
        }

        /**
         * This function initializes tests for a serial port at the given address.
         *   If there is a port found, it is reset to default values using the
         *   #XKSerialPortReset function and then certain port registers are setup
         *   for basic serial data transfer.
         *
         * @argument portBase:
         *   The base I/O port address for the prospective serial port.
         *
         * @return:
         *   On successful return, this function returns the address of the serial port
         *   If no serial port is found, this function returns a value of 'kXKSerialPortError'
         *
         * @implementation:
         *   This function operates as follows:
         *     1. Check if there is a valid serial port at this address by writing to the
         *          location where the scratch register would be located (base + 7) in a
         *          valid serial port and then reading the value back and ensuring they match.
         *          This is done twice. If it fails either time, 'kXKSerialPortError' is
         *          returned immediately.
         *     2. Reset the serial port by calling #XKSerialPortReset on the port given
         *     3. Setup the serial port for basic character sending.
         *     4. Return the serial port passed to the function
         */
        OSPrivate XKSerialPort XKSerialPortInit(OSAddress portBase);

        /**
         * This function resets a serial port to the default settings.
         *
         * @argument port;
         *   The base I/O port address for the serial port.
         *
         * @implementation:
         *   This function simply writes the default values
         *   to each register in the serial port.
         */
        OSPrivate void XKSerialPortReset(XKSerialPort port);

        /**
         * This function sets up the "line control" register in a given serial port.
         *   The line control register determines the size of each transferred character,
         *   how to send a parity checksum, if at all, and how many stop bits to send
         *   with each character transferred.
         *
         * @argument port:
         *   The base I/O port address for the serial port.
         *
         * @argument size:
         *   The character length for every character transferred. Set this to one of
         *   the following values for the given lengths:
         *     1. kXKSerialWordLength5Bits - 5 bits
         *     2. kXKSerialWordLength6Bits - 6 bits
         *     3. kXKSerialWordLength7Bits - 7 bits
         *     4. kXKSerialWordLength8Bits - 8 bits
         *
         * @argument parity:
         *   What type of parity checksum to send with each character, if any. Set this
         *   to one of the following values for the given type of parity:
         *     1. kXKSerialNoParity    - Don't send a parity checksum
         *     2. kXKSerialOddParity   - Force number of 1s per transmission to be odd.
         *     3. kXKSerialEvenParity  - Force number of 1s per transmission to be even.
         *     4. kXKSerialMarkParity  - Set the parity bit on every transmission.
         *     5. kXKSerialSpaceParity - Include but never set the parity bit.
         *
         * @argument stop:
         *   The number of stop bits to send with each transferred character. Set this
         *   to one of the following values for the given number of stop bits:
         *     1. kXKSerial1StopBit  - 1 stop bit
         *     2. kXKSerial2StopBits - 2 stop bits
         *
         * @implementation:
         *   This function simply builds the proper 8-bit value for the line control
         *   register given its current value, and writes it to the proper I/O port.
         */
        OSPrivate void XKSerialPortSetupLineControl(XKSerialPort port, UInt8 size, UInt8 parity, UInt8 stop);

        /**
         * This function sets the divisor for the serial baud rate on the given port.
         *   Basically, it sets how quickly the serial port transfers information as a
         *   fraction of its ~1.843200 MHz internal clock.
         *
         * @argument port:
         *   The base I/O port address for the serial port.
         *
         * @argument divisor:
         *   The factor by which to divide the internal clock
         *   (the baud rate is actually the (clock / 16) over this value)
         *
         * @implementation:
         *   This function writes the provided divisor to the first two I/O port registers
         *   in the serial port. It does this with the divisor latch-access bit (DLAB) bit
         *   in the line control register set. It enables and disables this bit as well.
         */
        OSPrivate void XKSerialPortSetBaudDivisor(XKSerialPort port, UInt16 divisor);

        /**
         * This function sets the baud rate for the given serial port directly. The baud rate
         * of a serial port is the effective transfer rate of information. It is specified
         * internally as a divisor of 1/16th the port's 1.843200 MHz internal clock.
         *
         * @argument port:
         *   The base I/O port address for the serial port.
         *
         * @argument rate:
         *   The baud rate which the serial port should operate at.
         *   This must be able to be specified as (183200 / 16) / n where is a whole number)
         *
         * @return:
         *   A successful return code (true) is returned only if a whole number divisor could
         *   be calculated from the provided rate.
         *
         * @implementation:
         *   This function begins by dividing (1843200 / 16), or 0x1C200, by the provided rate.
         *   It then calls #XKSerialPortSetBaudDivisor with the
         */
        OSPrivate bool XKSerialPortSetBaudRate(XKSerialPort port, UInt32 rate);

        /**
         * This function writes a single character to a serial port's transmit buffer. If there is
         *   already another pending transmission, this function will either wait for it to complete
         *   or it will return immediately (depending on the parameters passed)
         *
         * @argument port:
         *   The base I/O port address for the serial port.
         *
         * @argument character:
         *   The actual character to write. The number of bits of this field which will actually be
         *   transferred depends on the serial port's current word length setting.
         *   (See #XKSerialPortSetupLineControl)
         *
         * @arguemnt block:
         *   This argument specifies whether or not this function should wait for the port to be
         *   ready to transmit another character. If it is disabled, this function may not actually
         *   write the character to the transmit buffer
         *
         * @return:
         *    If block is set, this function will always return true. If not, this function will only
         *    return true if the character was actually written to the transmit buffer.
         *
         * @implementation:
         *   This function operates as follows:
         *     1. Check to see if the transmit buffer is ready. If it is write to it and return true.
         *     2. Check to see if we should wait on the transmit buffer. If we shouldn't, return false.
         *     3. Stall the processor for a small amount of time then jump back to step 1.
         */
        OSPrivate bool XKSerialWriteCharacter(XKSerialPort port, UInt8 character, bool block);

        /**
         * This function reads a single character from a serial port's incoming character buffer. If there
         *   is nothing ready, this function will either wait until a character is ready or it will return
         *   return immediately (depending on the parameters passed)
         *
         * @argument port:
         *   The base I/O port address for the serial port.
         *
         * @argument block:
         *   This argument specifies whether or not to wait for a character to be received. Passing a value of
         *   true means that this function should stall until a character is ready and passing a value of false
         *   specifies that this function should return immediately regardless of weather or not a character
         *   could be read.
         *
         * @return:
         *   If an error has been detected by the serial port, this function will return 'kXKSerialReadError'.
         *   If block is set and no error are detected, the character read from the serial port is returned.
         *   If block is not set, there are no transmission error detected, and no character is ready when
         *   this function is called, the value 0x00 is returned.
         *
         * @implementation:
         *   This function operates as follows:
         *     1. Check to see if the serial port has detected any transmission errors. If it has return
         *        'kXKSerialReadError' immediately.
         *     2. Check for a character in the incoming buffer. If there is one, return it.
         *     3. If block is not set, return 0x00. If block is set, go back to step 1.
         */
        OSPrivate UInt8 XKSerialReadCharacter(XKSerialPort port, bool block);

        /**
         * This function writes a UTF-8 encoded string to a provided serial port. The string is assumed to be
         * null-terminated. This function will stall the calling processor when writing each character to the
         * port if the port is not ready by the time the processor is (if the current CPU is faster than ~1.8
         * MHz, this will be the case, and given that 1.8 MHz CPUs are unsupported by CX, this will occur.
         *
         * @argument port:
         *   The base I/O port address for the serial port.
         *
         * @argument string:
         *   The address of the string to write to the port. Null terminated.
         *
         * @implementation:
         *   This function is implemented using #XKSerialWriteCharacter to write characters. The `block` parameter
         *   is always set to true. This function operates as follows:
         *     1. Load a byte from the string address. If this byte is 0 (the null terminator), return
         *     2. Increment the string pointer to point to the next character in the string.
         *     3. Print the character with a call to #XKSerialWriteCharacter.
         *     4. Go back to step 1.
         *
         */
        OSPrivate void XKSerialWriteString(XKSerialPort port, const OSUTF8Char *string);
    #endif /* Assembly Code */
#endif /* Architecture */

#endif /* !defined(__KERNEL_XKLEGACY__) */
