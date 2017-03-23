#include "tls_thread.h"
#include "tls_dll.h"

#include <process.h>
#include <crtdbg.h>
#include <stdio.h>

__declspec(thread) unsigned thread_id = 0;

static unsigned __stdcall thread_proc(void *data);

HANDLE tls_thread_start()
{
    HANDLE hthread = (HANDLE)_beginthreadex(nullptr, 0, 
        &thread_proc, 0, 0, nullptr);

    _ASSERT_EXPR(hthread, "create thread failed!");

    return hthread;
}

unsigned __stdcall thread_proc(void *data)
{
    thread_id = ::GetCurrentThreadId();

    test_dll_tls_api();

    unsigned i = 0;
    while (i++ < 20) {
        printf("test local thread[%d] %d\n", thread_id, i);
    }

    return 0;
}

bool tls_thread_stop(HANDLE hthread)
{
    if (!hthread) {
        return true;
    }

    DWORD ret = ::WaitForSingleObject(hthread, INFINITE);
    
    return (WAIT_OBJECT_0 == ret) ? true : false;
}

bool tls_threads_stop(HANDLE hthreads[], unsigned cnt)
{
    if (!hthreads || !cnt) {
        return true;
    }

    DWORD ret = ::WaitForMultipleObjects(cnt, hthreads, TRUE, INFINITE);
    /* will return min value if all events signal */
  
    return (WAIT_OBJECT_0 == ret) ? true : false;
}


