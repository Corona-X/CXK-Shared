/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* XKPageTable.h - Native page table structure*/
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* beeselmane - 1.8.2017   - 1:15 PM EST                           */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/

#ifndef __KERNEL_SHARED_XKPAGETABLE__
#define __KERNEL_SHARED_XKPAGETABLE__ 1

#include <Corona-X.h>
#include <System/OSTypes.h>

#if kCXArchIA
    #if !kCXAssemblyCode

    //

    #endif /* Assembly */
#else /* ARM */
    #warning Unsupported Architecture
#endif /* Architecture */

#endif /* !defined(__KERNEL_SHARED_XKPAGETABLE__) */
