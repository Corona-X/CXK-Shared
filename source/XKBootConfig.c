#include <Kernel/Shared/XKBootConfig.h>
#include <Kernel/C/CLStandardIO.h>
#include <Kernel/C/CLMemory.h>

bool XKBootConfigGetBool(XKBootConfig *config, const OSUTF8Char *key, bool defaultValue)
{
    for (OSIndex i = 0; i < (OSIndex)(config->entryCount * 2); i += 2)
        if (!CLStringCompare8(key, config->entries[i]))
            return !CLStringCompare8((OSUTF8Char *)"yes", config->entries[i + 1]);

    return defaultValue;
}

SInt64 XKBootConfigGetNumber(XKBootConfig *config, const OSUTF8Char *key, SInt64 defaultValue)
{
    for (OSIndex i = 0; i < (OSIndex)(config->entryCount * 2); i += 2)
    {
        if (!CLStringCompare8(key, config->entries[i]))
        {
            bool isSigned;

            UInt64 value = CLNumberFromString(config->entries[i + 1], &isSigned, (defaultValue < 0) ? -defaultValue : +defaultValue);

            if (isSigned)
                return -((SInt64)value);
            else
                return +((SInt64)value);
        }
    }

    return defaultValue;
}

const OSUTF8Char *XKBootConfigGetString(XKBootConfig *config, const OSUTF8Char *key, const OSUTF8Char *defaultValue)
{
    for (OSIndex i = 0; i < (OSIndex)(config->entryCount * 2); i += 2)
        if (!CLStringCompare8(key, config->entries[i]))
            return config->entries[i + 1];

    return defaultValue;
}
