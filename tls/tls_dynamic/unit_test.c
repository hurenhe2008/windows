#include <stdio.h>
#include <stdlib.h>

#include <Windows.h>
#include <process.h>
#include "thread_tls.h"

#define THREAD_CNT   4
const char *s_thread_info = "thread hello world";
const char *s_thread_name = "thread name with ";


int main(int argc, char *argv[])
{
	unsigned i = 0;
	thread_data_t *pdata = NULL;
	unsigned threadinfolen = strlen(s_thread_info);
    unsigned threadnamelen = strlen(s_thread_name);
	HANDLE hthreadarray[THREAD_CNT] = { 0 };
    
    for (i = 0; i < THREAD_CNT; ++i) {
	
        /*memory free with thread for now test.*/
        pdata = (thread_data_t *)malloc(sizeof(thread_data_t));
        if (!pdata) abort();

        memcpy(pdata->thread_info, s_thread_info, threadinfolen);
        pdata->thread_info[threadinfolen] = '\0';
        pdata->thread_seq = i;
        memcpy(pdata->thread_name, s_thread_name, threadnamelen);
        pdata->thread_name[threadnamelen] = '0' + i;
        pdata->thread_name[threadnamelen + 1] = '\0';

        hthreadarray[i] = _beginthreadex(NULL, 0, thread_proc,
            (void *)pdata, 0, NULL);
    }
    
	/* wait thread for exit */
    DWORD ret = WaitForMultipleObjects(THREAD_CNT, hthreadarray, true, INFINITE);
    if ((WAIT_OBJECT_0 + THREAD_CNT - 1) != ret) {
        printf("wait for %d thread failed!\n", THREAD_CNT);
    }
	
    return 0;
}
