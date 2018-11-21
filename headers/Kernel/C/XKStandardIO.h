/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* XKStandardIO.h - Kernel C routines to print and read strings.   */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* beeselmane - 8.8.2017   - 5:00 PM EST                           */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/

#ifndef __KERNEL_C_XKSTANDARDIO__
#define __KERNEL_C_XKSTANDARDIO__ 1

#include <Corona-X.h>
#include <System/OSTypes.h>
#include <System/OSCompilerMacros.h>
#include <System/OSUID.h>

/**
 * This function converts a Unique-Identifir (UID) to a string.
 *   It accepts an intel-EFI type UID.
 *
 * @argument uid:
 *   The UID to convert
 *
 * @return:
 *   The string holding the converted uid. This string must
 *   be freed with a call to the free function in whichever
 *   environment the function was called in.
 */
OSShared OSUTF8Char *XKUIDToString(const OSUIDIntelData *uid);

/**
 * This function converts a number to string.
 *
 * @argument number:
 *   The number to convert (This can be unsigned or signed depending
 *   on the next parameter passed.
 *
 * @argument isSigned:
 *   Specifies whether or not the number to convert is a signed or
 *   unsigned integer.
 *
 * @argument base:
 *   The base system to convert the number to (usually 8, 10, or 16)
 *
 * @argument padding:
 *   The minimum number of charcters for the output string. The resulting
 *   string will be padded with zeros.
 *
 * @argument hexStart:
 *   The initial character to start with after the decimal digits 0-9 have
 *   been exhausted. Usually either 'a' or 'A'.
 *
 * @argument length:
 *   This optional pointer will be filled with the final length of the
 *   resulting string.
 *
 * @result:
 *   The string buffer created as a result of this function. This string must
 *   be freed with a call to the free function in whichever environment the
 *   function was called in.
 *
 */
OSShared OSUTF8Char *XKNumberToString(SInt64 number, bool isSigned, UInt8 base, UInt8 padding, OSUTF8Char hexStart, OSLength *length);

OSShared OSUTF8Char *XKWriteToString(const OSUTF8Char *format, ...);

OSShared OSUTF8Char *XKWriteToStringFromList(const OSUTF8Char *format, OSVAList args);

OSShared OSSize XKWriteToBuffer(const OSUTF8Char *buffer, OSSize bufferSize, const OSUTF8Char *format, ...);

OSShared OSSize XKWriteToBufferFromList(const OSUTF8Char *buffer, OSSize bufferSize, const OSUTF8Char *format, OSVAList args);

OSShared void XKPrintString(const OSUTF8Char *format, ...);

OSShared void XKPrintStringFromList(const OSUTF8Char *format, OSVAList args);

OSShared UInt64 XKNumberFromString(const OSUTF8Char *string, bool *isSigned, UInt64 defaultValue);

#endif /* !defined(__KERNEL_C_XKSTANDARDIO__) */
