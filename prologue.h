/*
 * BRLTTY - A background process providing access to the console screen (when in
 *          text mode) for a blind person using a refreshable braille display.
 *
 * Copyright (C) 1995-2011 by The BRLTTY Developers.
 *
 * BRLTTY comes with ABSOLUTELY NO WARRANTY.
 *
 * This is free software, placed under the terms of the
 * GNU General Public License, as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option) any
 * later version. Please see the file LICENSE-GPL for details.
 *
 * Web Page: http://mielke.cc/brltty/
 *
 * This software is maintained by Dave Mielke <dave@mielke.cc>.
 */

#ifndef BRLTTY_INCLUDED_PROLOGUE
#define BRLTTY_INCLUDED_PROLOGUE

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define PACKAGE_COPYRIGHT "Copyright (C) 1995-2011 by The BRLTTY Developers."
#define PACKAGE_URL "http://mielke.cc/brltty/"

#define _CONCATENATE(a,b) a##b
#define CONCATENATE(a,b) _CONCATENATE(a,b)

#define _STRINGIFY(a) #a
#define STRINGIFY(a) _STRINGIFY(a)

#define MIN(a, b)  (((a) < (b))? (a): (b)) 
#define MAX(a, b)  (((a) > (b))? (a): (b)) 

#define ARRAY_COUNT(array) (sizeof((array)) / sizeof((array)[0]))
#define ARRAY_SIZE(pointer, count) ((count) * sizeof(*(pointer)))

#if defined(__CYGWIN32__) || defined(__MINGW32__)
#define WINDOWS

#include <w32api.h>
#define WINVER WindowsXP

#ifdef __MINGW32__
#ifndef __USE_W32_SOCKETS
#define __USE_W32_SOCKETS
#endif /* __USE_W32_SOCKETS */
#endif /* __MINGW32__ */

#include <windows.h>
#endif /* WINDOWS */

#ifdef __MINGW32__
#include <_mingw.h>
#endif /* __MINGW32__ */

/*
 * The (poorly named) macro "interface" is unfortunately defined within
 * Windows headers. Fortunately, though, it's also only ever used within
 * them so it's safe to undefine it here.
 */
#ifdef interface
#undef interface
#endif /* interface */

#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <inttypes.h>

#ifdef __MINGW32__
typedef HANDLE FileDescriptor;
#define INVALID_FILE_DESCRIPTOR INVALID_HANDLE_VALUE
#define PRIfd "p"
#define closeFileDescriptor(fd) CloseHandle(fd)

typedef SOCKET SocketDescriptor;
#define INVALID_SOCKET_DESCRIPTOR -1
#define PRIsd "d"
#define closeSocketDescriptor(sd) closesocket(sd)
#else /* __MINGW32__ */
typedef int FileDescriptor;
#define INVALID_FILE_DESCRIPTOR -1
#define PRIfd "d"
#define closeFileDescriptor(fd) close(fd)

typedef int SocketDescriptor;
#define INVALID_SOCKET_DESCRIPTOR -1
#define PRIsd "d"
#define closeSocketDescriptor(sd) close(sd)
#endif /* __MINGW32__ */

#ifdef WINDOWS
#define getSystemError() GetLastError()

#ifdef __CYGWIN32__
#include <sys/cygwin.h>

#define getSocketError() errno
#define setErrno(error) errno = cygwin_internal(CW_GET_ERRNO_FROM_WINERROR, (error))
#else /* __CYGWIN32__ */
#define getSocketError() WSAGetLastError()
#define setErrno(error) errno = win_toErrno((error))

#ifndef WIN_ERRNO_STORAGE_CLASS
#define WIN_ERRNO_STORAGE_CLASS
extern
#endif /* WIN_ERRNO_STORAGE_CLASS */
WIN_ERRNO_STORAGE_CLASS int win_toErrno (DWORD error);
#endif /* __CYGWIN32__ */

#else /* WINDOWS */
#define getSystemError() errno
#define getSocketError() errno

#define setErrno(error)
#endif /* WINDOWS */

#define setSystemErrno() setErrno(getSystemError())
#define setSocketErrno() setErrno(getSocketError())

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#ifdef HAVE_WCHAR_H 
#include <wchar.h>
#include <wctype.h>

#define WC_C(wc) L##wc
#define WS_C(ws) L##ws
#define PRIwc "lc"
#define PRIws "ls"
#define iswLatin1(wc) ((wc) < 0X100)
#else /* HAVE_WCHAR_H */
#include <ctype.h>
#include <string.h>

#define wchar_t char
#define wint_t int
#define WEOF EOF

#define wmemchr memchr
#define wmemcmp memcmp
#define wmemcpy memcpy
#define wmemmove memmove
#define wmemset memset

#define wcscasecmp strcasecmp
#define wcsncasecmp strncasecmp
#define wcscat strcat
#define wcsncat strncat
#define wcscmp strcmp
#define wcsncmp strncmp
#define wcscpy strcpy
#define wcsncpy strncpy
#define wcslen strlen
#define wcsnlen strnlen

#define wcschr strchr
#define wcscoll strcoll
#define wcscspn strcspn
#define wcsdup strdup
#define wcspbrk strpbrk
#define wcsrchr strrchr
#define wcsspn strspn
#define wcsstr strstr
#define wcstok strtok
#define wcswcs strstr
#define wcsxfrm strxfrm

#define wcstol strtol
#define wcstoll strtoll

#define iswalnum isalnum
#define iswalpha isalpha
#define iswblank isblank
#define iswcntrl iscntrl
#define iswdigit isdigit
#define iswgraph isgraph
#define iswlower islower
#define iswprint isprint
#define iswpunct ispunct
#define iswspace isspace
#define iswupper isupper
#define iswxdigit isxdigit

#define towlower tolower
#define towupper toupper

#define swprintf snprintf
#define vswprintf vsnprintf

#define WC_C(wc) wc
#define WS_C(ws) ws
#define PRIwc "c"
#define PRIws "s"
#define iswLatin1(wc) (1)
#endif /* HAVE_WCHAR_H */

#ifndef WRITABLE_DIRECTORY
#define WRITABLE_DIRECTORY ""
#endif /* WRITABLE_DIRECTORY */

#ifndef PACKED
#ifdef HAVE_ATTRIBUTE_PACKED
#define PACKED __attribute__((packed))
#else /* HAVE_ATTRIBUTE_PACKED */
#define PACKED
#endif /* HAVE_ATTRIBUTE_PACKED */
#endif /* PACKED */

#ifdef HAVE_ATTRIBUTE_FORMAT_PRINTF
#define PRINTF(fmt,var) __attribute__((format(__printf__, fmt, var)))
#else /* HAVE_ATTRIBUTE_FORMAT_PRINTF */
#define PRINTF(fmt,var)
#endif /* HAVE_ATTRIBUTE_FORMAT_PRINTF */

#ifdef HAVE_ATTRIBUTE_UNUSED
#define UNUSED __attribute__((unused))
#else /* HAVE_ATTRIBUTE_UNUSED */
#define UNUSED
#endif /* HAVE_ATTRIBUTE_UNUSED */

#ifdef ENABLE_I18N_SUPPORT
#include <libintl.h>
#else /* ENABLE_I18N_SUPPORT */
#define gettext(string) (string)
#define ngettext(singular, plural, count) (((count) == 1)? (singular): (plural))
#endif /* ENABLE_I18N_SUPPORT */
#define strtext(string) string

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* BRLTTY_INCLUDED_PROLOGUE */
