#include <stdio.h>
#include <stdlib.h>
#include <crtdbg.h>

#include "tls_thread.h"
#include "tls_dll.h"

#define THREAD_CNT 4

int main(int argc, char *argv[])
{
    HANDLE hthreads[THREAD_CNT] = { 0 };

    for (unsigned i = 0; i < THREAD_CNT; ++i) {

        hthreads[i] = tls_thread_start();
        
        _ASSERT_EXPR(hthreads[i], "create thread error!");
    }

    test_dll_tls_api();

    if (!tls_threads_stop(hthreads, THREAD_CNT)) {
        _ASSERT_EXPR(0, "stop threads error!");
    }

    for (unsigned i = 0; i < THREAD_CNT; ++i) {
        _ASSERTE(CloseHandle(hthreads[i]));
        hthreads[i] = NULL;
    }
    
    return 0;
}