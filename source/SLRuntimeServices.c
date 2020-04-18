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
    SLStatus code = SLRuntimeServicesGetCurrent()->resetSystem(type, status, 0, reason);

    // This shouldn't happen. If it does, we can look at %rax, %rbx, and %rcx to know why.
    __asm__ volatile ("ud2" : : "a" (code), "b" (type), "c" (status) : );
    __builtin_unreachable();
}
