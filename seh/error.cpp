#include "error.h"
#include <Windows.h>
#include <stdio.h>

void output_win_error()
{
    DWORD ret = GetLastError();

#ifndef USE_SYSTEM_ALLOC_BUFF
    char tmpbuf[1024 + 1] = { 0 };

    FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM,
        nullptr, ret, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        tmpbuf, sizeof(tmpbuf) - 1, nullptr);

    printf("error string: %s\n", tmpbuf);
#else
    LPVOID tmpbuf = nullptr;

    FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
        nullptr, ret, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPSTR)&tmpbuf, 0, nullptr);

    printf("error string: %s\n", (char *)tmpbuf);

    LocalFree(tmpbuf);
#endif
}
