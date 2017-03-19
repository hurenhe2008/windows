#include "thread_tls.h"
#include <Windows.h>
#include <stdio.h>   

#ifndef NULL
#define NULL (void *)0
#endif 

#ifndef nullptr
#define nullptr NULL
#endif

/*
s_thread_tls_index:
    all thread will use the same tls index to get value for their tls array.
    the initialized value will be set to TLS_OUT_OF_INDEXES(0xFFFFFFFF),
    it means the invalid tls index.
*/
static unsigned s_thread_tls_index = TLS_OUT_OF_INDEXES;

/*
create_tls(): 
    will create the valid tls index for all threads in their tls array.
    TlsAlloc not only create the valid tls index for all threads, set 
    the valid tls index to INUSE state and clean the valid index's value
    to 0 for all threads in their tls array, avoid the old value in tls 
    index to access exception data value.
*/
static BOOL create_tls();

/*
release_tls():
    set the valid tls index to FREE state for all threads in their tls array.
    and reset the tls index to 0.
*/
static BOOL release_tls();

/*
thread_handle_func():
    thread proc handle function, use TlsGetValue to do something.
*/
static void thread_handle_func();


unsigned __stdcall thread_proc(void *data)
{
    BOOL bret = FALSE;
    thread_data_t *threaddata = (thread_data_t *)data;
    
    if (!create_tls()) {
        printf("thread seq:%d create tls failed!\n", threaddata->thread_seq);
        return -1;
    }

    if (!TlsGetValue(s_thread_tls_index)) {/*must return null, just for check*/
        if (!TlsSetValue(s_thread_tls_index, (void *)threaddata)) {
            printf("thread (seq:%d) set tls failed!\n", threaddata->thread_seq);
            return -1;
        }
    }
    else {
        /*
        if the application come in here, it means TlsAlloc doesn't clean 
        the old value of tls index, it's not result we want to see!!!
        */
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
    thread_data_t *threaddata = (thread_data_t *)TlsGetValue(s_thread_tls_index);
    
    if (!threaddata) {
        printf("can't get tls thread data!\n");
        abort();
    }

#define TEST_TLSFREE_CLEAN_ALL_THREADS_THE_TLS_INDEX
    /*
    free tls index in advance for test, to verify whether TlsFree will reset 
    all threads' the tls index state and reset the tls index value to 0.
    the result is that TlsFree will reset all threads' the tls index state,
    and reset the tls index value to 0.
    */
#ifdef TEST_TLSFREE_CLEAN_ALL_THREADS_THE_TLS_INDEX 
    if (3 == threaddata->thread_seq) {
        if (!release_tls()) {
            printf("thead seq:%d release tls failed!\n", threaddata->thread_seq);
            abort();
        }
        else {
            printf("free the tls index in advance, verify TlsFree will reset all"
                "threads's the tls index state and reset value to 0.\n");
        }
    }
#endif 

    while (cnt++ < 20) {

#ifdef TEST_TLSFREE_CLEAN_ALL_THREADS_THE_TLS_INDEX
        thread_data_t *threaddata = TlsGetValue(s_thread_tls_index);
        if (!threaddata) {  
            /*verify TlsFree will clean all threads' the tls index value and state*/
            printf("can't get tls thread data after call TlsFree!\n");
            abort();
        }
#endif 

        printf("output thread [seq:%d name:%s info:%s]\n",threaddata->thread_seq,
            threaddata->thread_name, threaddata->thread_info);

        Sleep(100);
    }

    if (!release_tls()) {
        printf("thead seq:%d release tls failed!\n", threaddata->thread_seq);
        abort();
    }

    /*free memory created in main thread.*/
    free(threaddata);
}
