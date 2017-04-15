#include <Kernel/XKConsole.h>
#include <Kernel/XKSharedTarget.h>
#include <Kernel/XKUnicode.h>

#if kCXBootloaderCode
    #include <SystemLoader/EFI/SLBootServices.h>
    #include <SystemLoader/SLMemoryAllocator.h>
    #include <SystemLoader/SLConfigFile.h>
    #include <SystemLoader/SLLibrary.h>

    static UInt8 gXKEFIInputConsoleID = 0xFF;

    OSUnicodePoint XKEFIInputConsoleRead(bool wait, SLSystemTable *context)
    {
        SLStatus status;
        SLKeyPress key;

        do {
            status = context->stdin->readKey(gSLLoaderSystemTable->stdin, &key);
        } while (wait && status == kSLStatusNotReady);

        return (key.keycode ? key.keycode : kXKUTF32Error);
    }

    void XKInputConsoleTerminateEFI(void)
    {
        XKConsole *console = XKConsoleRemoveByID(gXKEFIInputConsoleID);
        XKFree(console);
    }

    void __XKInputConsoleInitAllEFI(void)
    {
        SLBootServicesRegisterTerminationFunction(XKInputConsoleTerminateEFI, kOSNullPointer);
        SLSystemTable *systemTable = SLSystemTableGetCurrent();
        systemTable->stdin->reset(systemTable->stdin, false);

        XKConsole *console = XKAllocate(sizeof(XKConsole)).address;
        console->id = 0xFF;
        console->context = systemTable;
        console->read = XKEFIInputConsoleRead;
        XKConsoleRegister(console);
    }
#endif /* kCXBootloaderCode */
