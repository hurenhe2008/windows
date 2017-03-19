#include "core_dump.h"
#include <Windows.h>
#include <DbgHelp.h>
#include <stdio.h>
#pragma comment(lib, "dbghelp.lib")

#ifndef NULL
#define NULL (void *)0
#endif 

#ifndef nullptr 
#define nullptr NULL
#endif

#define APP_NAME_LEN    50
static char s_app_name[APP_NAME_LEN + 1] = "tmp";  /* 1: '\0' */


static HANDLE create_dump_file()
{
    char filepath[MAX_PATH] = { 0 };
    char timenow[30] = { 0 };
    char *pos = nullptr;
    SYSTEMTIME sm;

    if (!GetModuleFileNameA(NULL, filepath, sizeof filepath)) {
        filepath[0] = '.';   /*current dir instead*/
    }
    else {
        pos = strrchr(filepath, '\\');
        if (!pos) return INVALID_HANDLE_VALUE;
        *pos = '\0';
    }
 
    GetLocalTime(&sm);
    sprintf_s(timenow, sizeof timenow, "_%04d-%02d-%02d_%02d-%02d-%02d-%03d",
        sm.wYear, sm.wMonth, sm.wDay, sm.wHour, sm.wMinute, sm.wSecond, 
        sm.wMilliseconds);

    strcat_s(filepath, sizeof filepath, "\\");
    strcat_s(filepath, sizeof filepath, s_app_name); /*app name*/
    strcat_s(filepath, sizeof filepath, timenow);    /*time*/
    strcat_s(filepath, sizeof filepath, ".dump");    /*suffix*/

    OutputDebugStringA("dump file path:");
    OutputDebugStringA(filepath);

    return CreateFileA(filepath, GENERIC_READ | GENERIC_WRITE, 0, NULL,
        CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
}

long __stdcall except_filter(struct _EXCEPTION_POINTERS *ep)
{
    HANDLE hfile = INVALID_HANDLE_VALUE;
    MINIDUMP_EXCEPTION_INFORMATION dumpinfo; 
    MINIDUMP_TYPE type = MiniDumpNormal;
    BOOL bret = FALSE;

    OutputDebugStringA("catch new exception!!!");
    if (INVALID_HANDLE_VALUE == (hfile = create_dump_file())) {
        OutputDebugStringA("create dump file failed!!!");
        return EXCEPTION_CONTINUE_SEARCH;
    }

    dumpinfo.ThreadId = GetCurrentThreadId();
    dumpinfo.ExceptionPointers = ep;
    dumpinfo.ClientPointers = TRUE;
    
    bret = MiniDumpWriteDump(GetCurrentProcess(),
        GetCurrentProcessId(), hfile, type,
        &dumpinfo, NULL, NULL);

    CloseHandle(hfile);

    return bret ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH;
}

long __stdcall except_filter_ex(unsigned excptcode, EXCEPTION_POINTERS *ep)
{
    /* not use excptcode */
    UNREFERENCED_PARAMETER(excptcode);

    return except_filter(ep);
}

void coredump_catch()
{
    SetUnhandledExceptionFilter(except_filter);
}

void coredump_catch_ex(const char *appname)
{
    int len = strlen(appname);
    int copylen = (len <= APP_NAME_LEN) ? len : APP_NAME_LEN;

    memcpy_s(s_app_name, copylen, appname, len);
    s_app_name[copylen] = '\0';

    SetUnhandledExceptionFilter(except_filter);
}