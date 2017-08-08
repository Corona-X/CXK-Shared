/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* XKDebugLog.h - Major upgrade to the logging system for debug    */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* beeselmane - 4.14.2017  -  6:15 PM PST                          */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/

#ifndef __KERNEL_XKDEBUGLOG__
#define __KERNEL_XKDEBUGLOG__ 1

#include <Corona-X.h>
#include <System/OSCompilerMacros.h>
#include <System/OSTypes.h>
#include <System/OSUID.h>
#include <Kernel/XKLog.h>

#define kXKUTF32Error               0xFFFFFFFF
#define kXKUTF16Error               0xFFFF
#define kXKUTF8Error                0xFF

#define kXKSurrogateHighBegin       0xD800
#define kXKSurrogateHighFinish      0xDBFF
#define kXKSurrogateLowBegin        0xDC00
#define kXKSurrogateLowFinish       0xDFFF

#define kXKConsoleTypeOutputMask    (1 << 7)
#define kXKConsoleTypeInputMask     (1 << 6)
#define kXKConsoleBiDirectionalMask (kXKConsoleTypeOutputMask    | kXKConsoleTypeInputMask)

#define kXKConsoleTypeSerial        (kXKConsoleBiDirectionalMask | 1)

#define kXKConsoleTypeVideo         (kXKConsoleTypeOutputMask    | 1)
#define kXKConsoleTypeMemory        (kXKConsoleTypeOutputMask    | 2)
#define kXKConsoleTypeFile          (kXKConsoleTypeOutputMask    | 3)

#define kXKConsoleTypeKeyboard      (kXKConsoleTypeInputMask     | 1)
#define kXKConsoleTypeEFIkeyboard   (kXKConsoleTypeInputMask     | 2)

/**
 * This function converts a unicode codepoint into a string of UTF-8 bytes.
 *   It is NOT guarenteed to work for characters greater than U+10FFFF, as this
 *   is the highest possible codepoint for standard UTF-8.
 *
 * @argument point:
 *   The codepoint to be converted to UTF-8
 *
 * @argument output:
 *   The buffer in which to place the resulting UTF-8. The 'size' field in this
 *   buffer specifies how many characters may be placed into it. If more bytes
 *   are needed, this function will fail.
 *
 * @return:
 *   This function returns the number of characters placed into the buffer,
 *   or 0 in the case of an error. The possible errors are that the codepoint
 *   given cannot be encoded in the UTF-8 format or that the buffer is too small
 *   to hold all the required bytes. If the buffer is not large enough, no bytes
 *   will be written to it on return.
 */
OSShared OSCount XKUTF8FromCodePoint(OSUnicodePoint point, const OSBuffer *output);

/**
 * This function converts a string of UTF-8 characters into a single code point.
 *   This method will work on 1-6 characters to produce a codepoint, although the
 *   resulting value is not guarenteed to be a valid Unicode point.
 *
 * @argument input:
 *   The buffer from which to read the input bytes. The 'size' field in this buffer
 *   specifies how many characters are in the input buffer. If a full codepoint
 *   cannot be extracted from this buffer, this function will fail.
 *
 * @argument used:
 *   This is a pointer used to communicate how many bytes from the input buffer were
 *   used to create the output codepoint. This pointer must not be null. It will be
 *   set to the number of bytes used when the function returns. If the function
 *   fails, it will be set to the size of the input buffer.
 *
 * @return:
 *   This function returns either a codepoint which was constructed from the first
 *   bytes in the buffer or a value of 'kXKUTF32Error' if an error occurred. This
 *   function will error if there are not enough bytes in the input buffer, the used
 *   pointer is null, or if the input buffer contains malformed UTF-8.
 */
OSShared OSUnicodePoint XKUTF8ToCodePoint(const OSBuffer *input, OSCount *used);

/**
 * This function converts a unicode codepoint into a string of UTF-16 words.
 *   It is NOT guarenteed to work for characters greater than U+10FFFF, as this
 *   is the highest possible codepoint for standard UTF-16.
 *
 * @argument point:
 *   The codepoint to be converted to UTF-16
 *
 * @argument output:
 *   The buffer in which to place the resulting UTF-16. The 'size' field in this
 *   buffer specifies how many characters may be placed into it. If more space
 *   is needed, this function will fail.
 *
 * @return:
 *   This function returns the number of characters placed into the buffer,
 *   or 0 in the case of an error. The possible errors are that the codepoint
 *   given cannot be encoded in the UTF-16 format or that the buffer is too
 *   small to hold all the required bytes. If the buffer is not large enough,
 *   no bytes will be written to it on return.
 */
OSShared OSCount XKUTF16FromCodePoint(OSUnicodePoint point, const OSBuffer *output);

/**
 * This function converts a string of UTF-16 characters into a single code point.
 *   This method will work on 1-6 characters to produce a codepoint, although the
 *   resulting value is not guarenteed to be a valid Unicode point.
 *
 * @argument input:
 *   The buffer from which to read the input characters. The 'size' field in this
 *   buffer specifies how many characters are in the input buffer. If a full
 *   codepoint cannot be extracted from this buffer, this function will fail.
 *
 * @argument used:
 *   This is a pointer used to communicate how many characters from the input buffer
 *   were used to create the output codepoint. This pointer must not be null. It wil
 *   be set to the number of bytes used when the function returns. If the function
 *   fails, it will be set to the size of the input buffer.
 *
 * @return:
 *   This function returns either a codepoint which was constructed from the first
 *   characters in the buffer or a value of 'kXKUTF32Error' if an error occurred.
 *   This function will error if there are not enough characters in the input buffer,
 *   the used pointer is null, or if the input buffer contains malformed UTF-8.
 */
OSShared OSUnicodePoint XKUTF16ToCodePoint(const OSBuffer *input, OSCount *used);

/**
 * This function gets the number of characters required to encode a UTF-16 encoded
 *   string in UTF-8.
 *
 * @argument string:
 *   The UTF-16 encoded string.
 *
 * @return:
 *   The number of bytes required to re-encode the string on success, or every bit
 *     set in the result for error.
 */
OSShared OSLength XKUTF16LengthInUTF8(const OSUTF16Char *string);

/**
 * This function gets the number of characters required to encode a UTF-8 encoded
 *   string in UTF-16.
 *
 * @argument string:
 *   The UTF-8 encoded string.
 *
 * @return:
 *   The number of bytes required to re-encode the string on success, or every bit
 *     set in the result for error.
 */
OSShared OSLength XKUTF8LengthInUTF16(const OSUTF8Char *string);

/**
 * This function converts a provided UTF-16 encoded string into
 *   an equivalent string encoded in UTF-8. The returned string
 *   must be free'd with the proper function (whatever the
 *   current definition of XKFree is in XKSharedTarget.h)
 *
 * @argument string:
 *   The UTF-16 encoded string to convert.
 *
 * @return:
 *   On failure, a null pointer.
 *   On successs, a string allocated with XKAllocate encoded with
 *   the UTF-8 encoding. Must be freed with XKFree (note:
 *   XKAllocate and XKFree are macros define in XKSharedTarget.h)
 */
OSShared OSUTF8Char *XKUTF16ToUTF8(const OSUTF16Char *string);

/**
 * This function converts a provided UTF-8 encoded string into
 *   an equivalent string encoded in UTF-16. The returned string
 *   must be free'd with the proper function (whatever the
 *   current definition of XKFree is in XKSharedTarget.h)
 *
 * @argument string:
 *   The UTF-8 encoded string to convert.
 *
 * @return:
 *   On failure, a null pointer.
 *   On successs, a string allocated with XKAllocate encoded with
 *   the UTF-16 encoding. Must be freed with XKFree (note:
 *   XKAllocate and XKFree are macros define in XKSharedTarget.h)
 */
OSShared OSUTF16Char *XKUTF8ToUTF16(const OSUTF8Char *string);

/**
 * This function gets the number of characters in the given
 *   UTF-32 encoded string.
 *
 * @argument string:
 *   The UTF-32 encoded string.
 *
 * @return:
 *   The number of characters in the string.
 */
OSShared OSLength XKUTF32Length(const OSUTF32Char *string);

/**
 * This function gets the number of characters in the given
 *   UTF-16 encoded string.
 *
 * @argument string:
 *   The UTF-16 encoded string.
 *
 * @return:
 *   The number of characters in the string.
 */
OSShared OSLength XKUTF16Length(const OSUTF16Char *string);

/**
 * This function gets the number of characters in the given
 *   UTF-8 encoded string.
 *
 * @argument string:
 *   The UTF-8 encoded string.
 *
 * @return:
 *   The number of characters in the string.
 */
OSShared OSLength XKUTF8Length(const OSUTF8Char *string);

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
 * This function converts a buffer into a string.
 *
 * @argument buffer:
 *   This buffer to convert
 *
 * @return:
 *   The string holding the converted buffer. This string must
 *   be freed with a call to the free function in whichever
 *   environment the function was called in.
 */
OSShared OSUTF8Char *XKBufferToString(const OSBuffer *buffer);

/**
 * This func converts a number to string.
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
OSShared void XKPrintMessage(const OSUTF8Char *format, ...);
OSShared void XKPrintMessageFromList(const OSUTF8Char *format, OSVAList args);

typedef struct __XKConsole {
    UInt8 type;
    UInt8 id;
    
    struct __XKConsole *next;
    OSAddress context;
    XKLogLevel level;
    
    // For output consoles
    void (*write)(const OSUTF8Char *string, OSLength length, OSAddress context);
    void (*delete)(OSCount count, OSAddress context);
    
    // For input consoles
    OSUnicodePoint (*read)(bool wait, OSAddress context);
    
    // Optional methods
    void (*destroy)(OSAddress context);
    void (*synchronize)(OSAddress context);
} XKConsole;

OSShared bool XKConsoleRegister(XKConsole *console);
OSShared bool XKConsoleRemove(const XKConsole *target);
OSShared XKConsole *XKConsoleRemoveByID(UInt8 id);
OSShared void XKConsoleSynchronize(void);

// For output consoles
OSShared void XKConsoleOutputString(const OSUTF8Char *string, OSLength length);
OSShared void XKConsoleDeleteCharacters(OSCount count);

// For input consoles
OSShared OSUTF8Char *XKConsoleReadString(OSUnicodePoint terminator, OSLength *length);
OSShared OSUnicodePoint XKConsoleWaitForInput(void);

OSExport XKConsole *gXKConsoleFirst;

// Largest size supported by UEFI for the monitor
// Always RGBX or BGRX 32-bit pixel format
typedef struct {
    UInt32 height;
    UInt32 width;
    UInt32 *framebuffer;
    OSSize framebufferSize;
    OSCount pixelCount;
    bool isBGRX;
} XKGraphicsContext;

typedef struct {
    UInt32 height;
    UInt32 width;
    UInt8 *packedData;
    UInt32 *fontData;
} XKBitmapFont;

typedef struct {
    UInt32 y;
    UInt32 x;
} XKGraphicsPoint;

OSShared void XKGraphicsContextWriteCharacter(XKGraphicsContext *context, UInt8 character, XKGraphicsPoint location, XKBitmapFont *font, UInt32 color, UInt32 bgColor);

OSShared void XKAllocateLogBuffer(OSSize size);

OSExport XKBitmapFont *gXKBitmapFont8x16;
OSExport XKLogLevel gXKLogLevelCurrent;

#endif /* !defined(__KERNEL_XKDEBUGLOG__) */
