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

// XKStringFormatting.h

OSShared OSUTF8Char *XKUIDToString(const OSUIDIntelData *uid);
OSShared OSUTF8Char *XKBufferToString(const OSBuffer *buffer);

OSShared OSUTF8Char *XKNumberToString(SInt64 number, bool isSigned, UInt8 base, UInt8 padding, OSUTF8Char hexStart, OSLength *length);

OSShared OSUTF8Char *XKPrintToString(const OSUTF8Char *format, ...);
OSShared OSUTF8Char *XKPrintToStringFromList(const OSUTF8Char *format, OSVAList args);
OSShared void XKPrintString(const OSUTF8Char *format, ...);
OSShared void XKPrintStringFromList(const OSUTF8Char *format, OSVAList args);

OSShared void XKPrintSerial(const char *f, ...);

// XKLog.h

OSExport XKLogLevel gXKLogLevelCurrent;

#endif /* !defined(__KERNEL_XKDEBUGLOG__) */
