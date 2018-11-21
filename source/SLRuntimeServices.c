#include <SystemLoader/EFI/SLRuntimeServices.h>
#include <SystemLoader/EFI/SLSystemTable.h>

SLSystemTable *SLSystemTableGetCurrent(void)
{
    return gSLLoaderSystemTable;
}

SLRuntimeServices *SLRuntimeServicesGetCurrent(void)
{
    return SLSystemTableGetCurrent()->runtimeServices;
}

OSNoReturn void SLRuntimeServicesResetSystem(SLResetType type, SLStatus status, const OSUTF8Char *reason)
{
    SLRuntimeServicesGetCurrent()->resetSystem(type, status, 0, reason);
}
