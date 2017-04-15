#include <Kernel/XKConsole.h>
#include <Kernel/XKSharedTarget.h>
#include <Kernel/XKGraphics.h>
#include <Kernel/XKMemory.h>
#include <System/OSByteMacros.h>

typedef struct {
    XKConsole rootConsole;
    XKGraphicsContext *context;
    XKBitmapFont *selectedFont;
    XKGraphicsPoint cursor;
    UInt32 baseOffsetX;
    UInt32 baseOffsetY;
    UInt32 height;
    UInt32 width;
    UInt32 color;
    UInt32 backgroundColor;
    bool isBackground;
} XKVideoConsole;

void XKVideoConsoleSetColor(XKConsole *console, UInt32 color)
{
    XKVideoConsole *videoConsole = (XKVideoConsole *)console;

    if (videoConsole->context->isBGRX) {
        videoConsole->color = color & 0x00FF00;
        videoConsole->color |= (color >> 16) & 0xFF;
        videoConsole->color |= (color & 0xFF) << 16;
    } else {
        videoConsole->color = color;
    }
}

void XKVideoConsoleSanitizeCursor(XKVideoConsole *console)
{
    if (console->cursor.x >= console->width)
    {
        console->cursor.x = 0;
        console->cursor.y++;
    }

    if (console->cursor.y >= console->height)
    {
        UInt32 rows = (console->height - console->cursor.y) + 1;

        OSCount blankPixelCount = console->context->width * rows * console->selectedFont->height;
        OSAddress blankOffset = (console->context->framebuffer + console->context->pixelCount) - blankPixelCount;
        OSAddress framebufferOffset = console->context->framebuffer + blankPixelCount;
        OSSize pixelCount = console->context->pixelCount - blankPixelCount;

        XKMemoryCopy(framebufferOffset, console->context->framebuffer, pixelCount * sizeof(UInt32));
        XKMemorySetValue(blankOffset, blankPixelCount * sizeof(UInt32), 0x00);

        console->cursor.y -= rows;
    }
}

void XKVideoConsoleWriteCharacter(UInt8 character, XKVideoConsole *console)
{
    XKGraphicsPoint outputLocation;
    outputLocation.y = (console->cursor.y * console->selectedFont->height) + console->baseOffsetY;
    outputLocation.x = (console->cursor.x * console->selectedFont->width ) + console->baseOffsetX;

    switch (character)
    {
        case '\n': {
            console->cursor.x = 0;
            console->cursor.y++;
        } break;
        case '\r': {
            console->cursor.x = 0;
        } break;
        case '\b': {
            if (!console->cursor.x) {
                if (console->cursor.y)
                {
                    console->cursor.x = console->width - 1;
                    console->cursor.y--;
                }
            } else {
                console->cursor.x--;
            }
        } break;
        case '\t': {
            for (UInt8 i = 0; i < 4; i++)
                XKVideoConsoleWriteCharacter(' ', console);
        } return;
        default: {
            //SLDelayProcessor(5000, !SLBootServicesHaveTerminated());
            XKGraphicsContextWriteCharacter(console->context, character, outputLocation, console->selectedFont, console->color, console->backgroundColor);
            console->cursor.x++;
        } break;
    }

    XKVideoConsoleSanitizeCursor(console);
}

void XKVideoConsoleWrite(OSUTF8Char *string, OSSize size, XKVideoConsole *console)
{
    for (UInt8 i = 0; i < size; i++)
        XKVideoConsoleWriteCharacter(string[i], console);
}

void XKVideoConsoleDelete(OSCount count, XKVideoConsole *console)
{
    UInt8 *spaces = XKAllocate(count).address;
    XKMemorySetValue(spaces, count, '\b');
    XKVideoConsoleWrite(spaces, count, console);
    XKMemorySetValue(spaces, count, ' ');
    XKVideoConsoleWrite(spaces, count, console);
    XKMemorySetValue(spaces, count, '\b');
    XKVideoConsoleWrite(spaces, count, console);
    XKFree(spaces);
}

#if kCXBootloaderCode
    #include <SystemLoader/EFI/SLGraphics.h>
    #include <SystemLoader/SLConfigFile.h>

    void __XKVideoConsoleInitAll(void)
    {
        SLConfigFile *config = SLConfigGet();

        if (!config->dev.videoConsole.enabled)
            return;

        SLGraphicsOutput **screens = SLGraphicsOutputGetAll(kOSNullPointer);
        UInt8 count = 0;

        if (!screens)
        {
            XKLog(kXKLogLevelWarning, "No output screens found!\n");
            return;
        }

        while (screens)
        {
            SLGraphicsOutput *screen = (*screens++);
            count++;

            if (count > config->dev.videoConsole.maxScreenCount)
                break;

            XKGraphicsContext *context = SLGraphicsOutputGetContextWithMaxSize(screen, config->dev.videoConsole.maxScreenHeight, config->dev.videoConsole.maxScreenWidth);
            XKMemorySetValue(context->framebuffer, context->framebufferSize, 0x00);
            XKVideoConsole *console = XKAllocate(sizeof(XKVideoConsole)).address;

            console->selectedFont = gXKBitmapFont8x16;
            console->context = context;
            console->cursor = ((XKGraphicsPoint){0, 0});
            console->height = context->height / console->selectedFont->height;
            console->width  = context->width  / console->selectedFont->width;

            if (context->isBGRX) {
                UInt32 backgroundColor = config->dev.videoConsole.backgroundColor;
                console->backgroundColor = ((backgroundColor & 0xFF0000) >> 16);
                console->backgroundColor |= (backgroundColor & 0x00FF00);
                console->backgroundColor |= (backgroundColor & 0x0000FF) << 16;

                UInt32 foregroundColor = config->dev.videoConsole.foregroundColor;
                console->color = ((foregroundColor & 0xFF0000) >> 16);
                console->color |= (foregroundColor & 0x00FF00);
                console->color |= (foregroundColor & 0x0000FF) << 16;
            } else {
                console->backgroundColor = config->dev.videoConsole.backgroundColor;
                console->color = config->dev.videoConsole.foregroundColor;
            }

            console->rootConsole.id = 0xFF;
            console->rootConsole.context = console;
            console->rootConsole.level = kXKLogLevelInfo;
            console->rootConsole.type = kXKConsoleTypeVideo;
            console->rootConsole.write = XKVideoConsoleWrite;
            console->baseOffsetY =       (context->height % console->selectedFont->height) / 2;
            console->baseOffsetX =       (context->width  % console->selectedFont->width)  / 2;
            console->rootConsole.delete = XKVideoConsoleDelete;

            XKConsoleRegister(&console->rootConsole);
        }
    }
#endif /* kCXBootloaderCode */
