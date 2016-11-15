/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* OSUID.h - Definition of Types able to store Unique Identifiers  */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* beeselmane - 17.9.2016  - 2:00 PM EST                           */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* beeselmane - 17.9.2016  - 2:00 PM EST                           */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/

#ifndef __SYSTEM_OSUID__
#define __SYSTEM_OSUID__ 1

#include <Corona-X.h>
#include <System/OSTypes.h>

#if !kCXAssemblyCode

typedef struct {
    UInt8 byte0,  byte1,  byte2,  byte3,
          byte4,  byte5,  byte6,  byte7,
          byte8,  byte9,  byte10, byte11,
          byte12, byte13, byte14, byte15;
} OSUIDByteData;

typedef struct {
    UInt32 group0;
    UInt16 group1;
    UInt16 group2;
    UInt64 group3;
} OSUIDGroupedData;

typedef struct {
    UInt32 group0;
    UInt16 group1;
    UInt16 group2;

    struct {
        UInt8 byte0, byte1,
              byte2, byte3,
              byte4, byte5,
              byte6, byte7;
    } group3;
} OSUIDPrintableData;

typedef struct {
    UInt32 group0;
    UInt16 group1;
    UInt16 group2;
    UInt8  group3[8];
} OSUIDIntelData;

typedef UInt8 OSUIDRawData[16];
typedef UInt128 OSUID;

#endif /* AkCXAssemblyCode */

#endif /* !defined(__SYSTEM_OSUID__) */