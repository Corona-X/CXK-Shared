/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* XKGraphics - Framebuffer structures for Video Console           */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* beeselmane - 1.8.2017   - 6:30 PM EST                           */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/

#ifndef __KERNEL_XKGRAPHICS__
#define __KERNEL_XKGRAPHICS__ 1

#include <Corona-X.h>
#include <System/OSCompilerMacros.h>
#include <System/OSTypes.h>

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

OSExport XKBitmapFont *gXKBitmapFont8x16;

#endif /* !defined(__KERNEL_XKGRAPHICS__) */
