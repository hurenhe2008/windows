#include "thread_tls.h"
#include <Windows.h>
#include <stdio.h>

#ifndef NULL
#define NULL (void *)0
#endif 

#ifndef nullptr
#define nullptr NULL
#endif

static unsigned s_thread_tls_index = TLS_OUT_OF_INDEXES;

static BOOL create_tls();
static BOOL release_tls();
static void thread_handle_func();

unsigned __stdcall thread_proc(void *data)
{
    BOOL bret = FALSE;
    thread_data_t *threaddata = (thread_data_t *)data;
    
    if (!create_tls()) {
        printf("thread seq:%d create tls failed!\n", threaddata->thread_seq);
        return -1;
    }

    if (!TlsGetValue(s_thread_tls_index)) {
        if (!TlsSetValue(s_thread_tls_index, (void *)threaddata)) {
            printf("thread (seq:%d) set tls failed!\n", threaddata->thread_seq);
            return -1;
        }
    }

    thread_handle_func();

    return 0;
}

BOOL create_tls()
{
    if (TLS_OUT_OF_INDEXES != s_thread_tls_index) {
        return;
    }

    s_thread_tls_index = TlsAlloc();

    return (TLS_OUT_OF_INDEXES != s_thread_tls_index) ? TRUE : FALSE;
}

BOOL release_tls()
{
    if (TLS_OUT_OF_INDEXES == s_thread_tls_index) {
        return;
    }

    return TlsFree(s_thread_tls_index);
}

void thread_handle_func()
{
    unsigned cnt = 0;
    thread_data_t *threaddata = 
        (thread_data_t *)TlsGetValue(s_thread_tls_index);
    
    if (!threaddata) {
        printf("can't get tls thread data!\n");
        abort();
    }

    while (cnt++ < 20) {
        printf("output thread [seq:%d name:%s info:%s]\n",
            threaddata->thread_seq,
            threaddata->thread_name, threaddata->thread_info);
        Sleep(100);
    }

    if (!TlsSetValue(s_thread_tls_index, 0)) {
        printf("can't set tls thread(seq:%d) 0!\n", threaddata->thread_seq);
    }

    if (!release_tls()) {
        printf("thead seq:%d release tls failed!\n", threaddata->thread_seq);
    }

    /*free memory created in main thread.*/
    free(threaddata);
}