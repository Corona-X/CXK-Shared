#include <Kernel/C/CLStandardIO.h>

// This only works for bases 0, 8, and 16
UInt64 CLNumberFromString(const OSUTF8Char *string, bool *isSigned, UInt64 defaultValue)
{
    UInt64 value = 0;
    UInt8 base = 10;
    OSIndex i = 0;

    if (!string || !isSigned)
        return defaultValue;

    if (*string == '-')
        (*isSigned) = true;

    if (*string == '+' || *string == '-')
        i++;

    if (!string[i])
        return defaultValue;

    if (string[i] == '0')
    {
        switch (string[i + 1])
        {
            case 0: return 0;
            case 'x':
                base = 16;
                i++;
                break;
            default: base = 8; break;
        }

        i++;
    }

    while (string[i] != 0)
    {
        UInt8 digit;

        if ('0' <= string[i] && string[i] <= '9') {
            digit = string[i] - '0';
        } else if (base >= 11 && 'A' <= string[i] && string[i] <= 'Z') {
            digit = (string[i] - 'A') + 10;
        } else if (base >= 11 && 'a' <= string[i] && string[i] <= 'z') {
            digit = (string[i] - 'a') + 10;
        } else {
            // Invalid character...
            return value;
        }

        value *= base;
        value += digit;

        i++;
    }

    return value;
}
