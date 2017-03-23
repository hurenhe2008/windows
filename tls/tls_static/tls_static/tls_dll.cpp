#include "tls_dll.h"
#include <stdio.h>
#include <crtdbg.h>

#if TEST_DLL == TEST_STATIC_IMPORT_DLL
    #include ".\..\tls_static_dll\lib_tls_static.h"

    #ifdef _DEBUG
        #pragma comment(lib, "..\\bin\\tls_static_dlld.lib")
    #else 
        #pragma comment(lib, "..\\bin\\tls_static_dll.lib")
    #endif 
#elif TEST_DLL == TEST_DYNAMIC_IMPORT_DLL 
    #include <Windows.h>

    typedef int (_cdecl *Ftls_int)();

    typedef char* (_cdecl *Ftls_ptr)();

#else 

    /* nothing to test */

#endif 


void test_dll_tls_api()
{
#if TEST_DLL == TEST_STATIC_IMPORT_DLL
    tls_info_t *pinfo = get_tls_info();
    _ASSERTE(pinfo);

    if (pinfo->binit) {
        printf("thread[%d] get dll tls info (name:%s tid:%d pid:%d)\n",
            pinfo->tid, pinfo->thread_name,pinfo->tid, pinfo->pid);
    }
    else {
        _ASSERT_EXPR(0, "get dll tls info not init!");
    }
    
    int ret = get_tls_int();
    printf("thread[%d] get dll tls int data: %d\n", pinfo->tid, ret);

    char *ptr = get_tls_ptr();
    if (nullptr == ptr) {
        printf("thread[%d] get dll tls ptr init nullptr!\n", pinfo->tid);
    }
    else {
        _ASSERT_EXPR(0, "get dll tls ptr not init!");
    }

#elif TEST_DLL == TEST_DYNAMIC_IMPORT_DLL 

#ifdef _DEBUG
    HMODULE hmod = ::LoadLibrary("tls_static_dlld.dll");
#else 
    HMODULE hmod = ::LoadLibrary("tls_static_dll.dll");
#endif 
    _ASSERT_EXPR(hmod, "load library tls_static_dll.dll failed!");

    Ftls_int ftls_int = (Ftls_int)GetProcAddress(hmod, "get_tls_int");
    _ASSERTE(ftls_int);
    int ret = (*ftls_int)();
    printf("dll tls int data:%d with loadlibrary!\n", ret);

    Ftls_ptr ftls_ptr = (Ftls_ptr)GetProcAddress(hmod, "get_tls_ptr");
    _ASSERTE(ftls_ptr);
    char *ptr = (*ftls_ptr)();
    if (nullptr == ptr) {
        printf("get dll tls ptr init nullptr with loadlibrary!\n");
    }
    else {
        _ASSERT_EXPR(0, "get dll tls ptr not init with loadlibrary!");
    }

    _ASSERT_EXPR(::FreeLibrary(hmod), "FreeLibrary call failed!");

#else 

    /* nothing to do */

#endif 
}