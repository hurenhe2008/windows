#include "thread.h"
#include <Windows.h>
#include <process.h>
#include <stdio.h>

#ifndef NULL
#define NULL (void *)0
#endif

#ifndef nullptr
#define nullptr NULL
#endif

static HANDLE s_thread_handle = NULL;

unsigned __stdcall thread_proc(void *);

bool thread_start()
{
    if (NULL == s_thread_handle) {
        printf("_beginthreadex\n");
        s_thread_handle = (HANDLE)_beginthreadex(nullptr, 0,
            thread_proc, nullptr, 0, nullptr);
        printf("finished _beginthreadex\n");
    }

    return (NULL == s_thread_handle) ? false : true;
}

bool thread_stop()
{
    if (NULL == s_thread_handle) {
        return true;
    }

    printf("waitforsingleobject\n");
    DWORD ret = WaitForSingleObject(s_thread_handle, INFINITE);
    printf("finished waitforsingleobject\n");

    if (WAIT_OBJECT_0 == ret) {
        DWORD exitcode = 0;
        if (GetExitCodeThread(s_thread_handle, &exitcode)) {
            printf("thread exit code: %d\n", exitcode);
        }

        if (CloseHandle(s_thread_handle)) {
            s_thread_handle = NULL;
        }
    }

    return (WAIT_OBJECT_0 == ret && NULL == s_thread_handle) ? true : false;
}

unsigned __stdcall thread_proc(void *data)
{
    printf("thread proc function run...\n");

    /*unsigned cnt = 0;
    while (cnt++ < 5) {
    ::Sleep(1000);
    printf("thread cnt:%d\n", cnt);
    }*/

    printf("test for breakpoint (INT 3) \n");
    //DebugBreak();

    printf("call exitthread(1)\n");
    ExitThread(1);   /*not release thread resource, just reference of threadhandle - 1*/
	/*below code never execute*/
    printf("finish call exitthread(1)\n");

    printf("finish thread proc function\n");
    return 0;
}