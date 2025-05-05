#include "debug.h"
#include <nitro.h>
#include <string.h>

int Desmume_Log(const char *fmt, ...)
{
    char s[1024];

    va_list va;
    va_start(va, fmt);
    vsprintf(s, fmt, va);
    va_end(va);

    ConsoleLog(s);

    return 0;
}

static ConsoleLog(char *s)
{
    // 0xFC is a reserved system-interrupt code on DeSmuME which logs the
    // contents of the string buffer in r0 to the emulator console.
    asm("swi 0xFC");
}