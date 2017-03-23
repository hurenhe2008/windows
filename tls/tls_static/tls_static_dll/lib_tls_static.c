#include "lib_tls_static.h"
#include <Windows.h>

#ifndef NULL
#define NULL (void *)0
#endif 

#ifndef nullptr
#define nullptr NULL
#endif 

__declspec(thread) tls_info_t g_tls_info = {
    false, "thread tls data", 0, 0};

__declspec(thread) int g_tls_int; // = 1;

__declspec(thread) char *g_tls_ptr; // = nullptr;


tls_info_t* _cdecl get_tls_info()
{
    g_tls_info.binit = true;
    g_tls_info.tid = GetCurrentThreadId();
    g_tls_info.pid = GetCurrentProcessId();

    return &g_tls_info;
}

int _cdecl get_tls_int()
{
    return g_tls_int;
}

char* _cdecl get_tls_ptr()
{
    return g_tls_ptr;
}