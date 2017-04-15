#include <Kernel/XKSerial.h>
#include <Kernel/XKShared.h>

void XKSerialConsoleWrite(OSUTF8Char *string, OSSize size, XKSerialPort port)
{
    for (OSIndex i = 0; i < size; i++)
    {
        if (string[i] == '\n')
        {
            XKSerialWriteString(port, (UInt8 *)"\r\n");
            continue;
        }

        XKSerialWriteCharacter(port, string[i], true);
    }
}

OSUnicodePoint XKSerialConsoleRead(bool wait, XKSerialPort port)
{
    OSUnicodePoint character = XKSerialReadCharacter(port, wait);

    if (character == kXKSerialReadError)
    {
        XKLog(kXKLogLevelWarning, "Detected serial data transmission error!\n");
        character = kXKUTF32Error;
    }

    if (!character) character = kXKUTF32Error;
    return character;
}

void XKSerialConsoleDelete(OSCount count, XKSerialPort port)
{
    for (OSCount i = 0; i < count; i++)
        XKSerialWriteString(port, (UInt8 *)"\b \b");
}

#if kCXBootloaderCode
    #include <SystemLoader/SLConfigFile.h>

    void __XKSerialConsoleInitAll(void)
    {
        SLConfigFile *config = SLConfigGet();

        if (!config->dev.serialConsole.enabled)
            return;


        for (OSIndex i = 0; i < config->dev.serialConsole.portCount; i++)
        {
            XKSerialPort port = XKSerialPortInit(config->dev.serialConsole.ports[i]);

            if (port == kXKSerialPortError)
            {
                XKPrintString("Error Loading Serial Port at '0x%04X'\n", config->dev.serialConsole.ports[i]);
                continue;
            }

            XKSerialPortSetupLineControl(port, config->dev.serialConsole.worldLength, config->dev.serialConsole.parityType, config->dev.serialConsole.stopBits);
            XKSerialPortSetBaudRate(port, config->dev.serialConsole.baudRate);

            XKConsole *console = XKAllocate(sizeof(XKConsole)).address;
            console->id = 0xFF;
            console->context = port;
            console->level = kXKLogLevelDebug;
            console->type = kXKConsoleTypeSerial;
            console->write = XKSerialConsoleWrite;
            console->read = XKSerialConsoleRead;
            console->delete = XKSerialConsoleDelete;
            XKConsoleRegister(console);
        }
    }
#endif /* kCXBootloaderCode */
