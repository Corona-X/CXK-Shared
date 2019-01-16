#include <Kernel/Shared/XKBootConfig.h>
#include <Kernel/C/XKStandardIO.h>
#include <Kernel/C/XKMemory.h>

bool XKBootConfigGetBool(XKBootConfig *config, const OSUTF8Char *key, bool defaultValue)
{
    for (OSIndex i = 0; i < (OSIndex)(config->entryCount * 2); i += 2)
        if (!XKStringCompare8(key, config->entries[i]))
            return !XKStringCompare8((OSUTF8Char *)"yes", config->entries[i + 1]);

    return defaultValue;
}

SInt64 XKBootConfigGetNumber(XKBootConfig *config, const OSUTF8Char *key, SInt64 defaultValue)
{
    for (OSIndex i = 0; i < (OSIndex)(config->entryCount * 2); i += 2)
    {
        if (!XKStringCompare8(key, config->entries[i]))
        {
            bool isSigned;

            UInt64 value = XKNumberFromString(config->entries[i + 1], &isSigned, (defaultValue < 0) ? -defaultValue : +defaultValue);

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
        if (!XKStringCompare8(key, config->entries[i]))
            return config->entries[i + 1];

    return defaultValue;
}
