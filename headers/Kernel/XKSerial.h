/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* XKSerial.h - Kernel shared Serial Driver                        */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* beeselmane - 9.10.2016  - 12:00 AM EST                          */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/

#ifndef __KERNEL_XKSERIAL__
#define __KERNEL_XKSERIAL__ 1

#include <Corona-X.h>
#include <System/OSTypes.h>

#if kCXBuildDev
    #define kXKSerialAssumedClockSpeed  1843200
    #define kXKSerialPortError          0xFFFF
    // Just a note: I chose the read error character
    // to be the form feed character, as I decided
    // that the form feed character is probably the
    // least useful 8-bit unicode/ASCII character
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

    #if kCXBootloaderCode && !kCXAssemblyCode
        typedef UInt16 XKSerialPort;

        /**
         * This function initializes tests for a serial port at the given address.
         *   If there is a port found, it is reset to default values using the
         *   @XKSerialPortReset function and then certain port registers are setup
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
         *          returned immidietely.
         *     2. Reset the serial port by calling @XKSerialPortReset on the port given
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
         *     to each register in the serial port.
         */
        OSPrivate void XKSerialPortReset(XKSerialPort port);

        /**
         * This function setups up the "line control" register in a given serial port.
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
         *   This function simply builds the proper 8-bit value for the line control register
         *     given its current value, and writes it to the proper I/O port.
         *
         */
        OSPrivate void XKSerialPortSetupLineControl(XKSerialPort port, UInt8 size, UInt8 parity, UInt8 stop);

        OSPrivate void XKSerialPortSetBaudDivisor(XKSerialPort port, UInt16 divisor);

        OSPrivate void XKSerialPortSetBaudRate(XKSerialPort port, UInt32 rate);

        OSPrivate bool XKSerialWriteCharacter(XKSerialPort port, UInt8 character, bool block);

        OSPrivate UInt8 XKSerialReadCharacter(XKSerialPort port, bool block);

        OSPrivate void XKSerialWriteString(XKSerialPort port, UInt8 *string);
    #endif /* kCXBootloaderCode && !kCXAssemblyCode */
#endif /* kCXBuildDev */

#endif /* !defined(__KERNEL_XKSERIAL__) */
