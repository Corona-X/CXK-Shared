/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* XKUtilities.h - Basic Kernel Utilities                          */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/
/* beeselmane - 4.14.2017  -  9:15 PM PST                          */
/**=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=**/

#ifndef __KERNEL_XKUTILITIES__
#define __KERNEL_XKUTILITIES__ 1

#include <Corona-X.h>
#include <System/OSCompilerMacros.h>
#include <System/OSTypes.h>

#if !kCXAssemblyCode

OSShared void XKMemorySetValue(OSAddress address, OSSize size, UInt8 value);
OSShared void XKMemoryCopy(OSAddress input, OSAddress output, OSSize size);

OSShared SInt16 XKMemoryCompare(OSAddress first, OSAddress second, OSSize size);

OSShared SInt16 XKStringCompare8(UInt8 *first, UInt8 *second);

OSShared UInt64 XKGetProcessorSpeed(void);

typedef struct {
    UInt32 date;
    UInt64 nanosecond;
} XKTime;

OSShared XKTime XKTimeGetCurrent(void);

#endif /* kCXAssemblyCode */

#endif /* !defined(__KERNEL_XKUTILITIES__) */

/*
 
 int    ffs(int);
 int    strcasecmp(const char *, const char *);
 int    strcasecmp_l(const char *, const char *, locale_t);
 int    strncasecmp(const char *, const char *, size_t);
 int    strncasecmp_l(const char *, const char *, size_t, locale_t);
 
 // Copy until character
 void    *memccpy(void *restrict, const void *restrict, int, size_t);
 // Locate byte in string
 void    *memchr(const void *, int, size_t);
 // Memory Compare
 int      memcmp(const void *, const void *, size_t);
 // Copy Memory
 void    *memcpy(void *restrict, const void *restrict, size_t);
 // Move Memory
 void    *memmove(void *, const void *, size_t);
 // Set Memory
 void    *memset(void *, int, size_t);
 
 
 char    *stpcpy(char *restrict, const char *restrict);
 char    *stpncpy(char *restrict, const char *restrict, size_t);
 char    *strcat(char *restrict, const char *restrict);
 char    *strchr(const char *, int);
 int      strcmp(const char *, const char *);
 int      strcoll(const char *, const char *);
 int      strcoll_l(const char *, const char *, locale_t);
 char    *strcpy(char *restrict, const char *restrict);
 size_t   strcspn(const char *, const char *);
 char    *strdup(const char *);
 char    *strerror(int);
 char    *strerror_l(int, locale_t);
 int      strerror_r(int, char *, size_t);
 size_t   strlen(const char *);
 char    *strncat(char *restrict, const char *restrict, size_t);
 int      strncmp(const char *, const char *, size_t);
 char    *strncpy(char *restrict, const char *restrict, size_t);
 char    *strndup(const char *, size_t);
 size_t   strnlen(const char *, size_t);
 char    *strpbrk(const char *, const char *);
 char    *strrchr(const char *, int);
 char    *strsignal(int);
 size_t   strspn(const char *, const char *);
 char    *strstr(const char *, const char *);
 char    *strtok(char *restrict, const char *restrict);
 char    *strtok_r(char *restrict, const char *restrict, char **restrict);
 size_t   strxfrm(char *restrict, const char *restrict, size_t);
 size_t   strxfrm_l(char *restrict, const char *restrict, size_t, locale_t);
 
 */
