#include <stdio.h>
#include <stdlib.h>
#include <new>
#include "task_thread_pool.h"

#if defined(_WIN32)
    #include <Windows.h>
    #define sleepx(t) Sleep(t * 1000)
#elif defined(__linux__)
    #include <unistd.h>
    #include <pthread.h>
    #define sleepx(t) sleep(t)
#else
    #error "this system not support"
#endif 

static TaskThreadMgr* enable_task_mgr();
static void input_tasks(TaskThreadMgr *mgr);
static void stop_task_mgr(TaskThreadMgr *mgr);
static void error_exit(const char *desc);
static unsigned get_tid();

static bool task_handle(void *data);
static void task_error(void *data);
static void task_cancel(void *data);



int main(void)
{
    printf("enable task_thread_mgr...\n");
    TaskThreadMgr *mgr = enable_task_mgr();

    printf("input tasks into task_thread_mgr...\n");
    input_tasks(mgr);

    printf("stop task_thread_mgr...\n");
    stop_task_mgr(mgr);

    printf("client will exit...\n"); getchar();

    return 0;
}

TaskThreadMgr* enable_task_mgr()
{
    TaskThreadMgr *mgr = new(std::nothrow) TaskThreadMgr();
    if (!mgr) {
        error_exit("create TaskThreadMgr failed!");
    }

    if (!mgr->Start()) {
        error_exit("start TaskThreadMgr failed!");
    }

    return mgr;
}

void input_tasks(TaskThreadMgr *mgr)
{
    const unsigned cycles = 10;
    const unsigned ones = 5;
    unsigned start = 0;

    task_info_t task;
    task.handle_fun = task_handle;
    task.error_fun = task_error;
    task.cancel_fun = task_cancel;

    while (start < cycles) {       
        for (unsigned i = 0; i < ones; ++i) {
            task.data = (void *)(start * ones + i);
            mgr->AddTask(&task);
        }

        ++start;
        sleepx(2);
    }
}

void stop_task_mgr(TaskThreadMgr *mgr)
{
    if (!mgr->Stop()) {
        error_exit("stop TaskThreadMgr failed!");
    }

    delete mgr;
}

void error_exit(const char *desc)
{
    printf("Error: %s\n", desc);

    getchar();   exit(-1);
}

unsigned get_tid()
{
    unsigned tid = 0;
#if defined(_WIN32) 
    tid = GetCurrentThreadId();
#elif defined(__linux__)
    tid = pthread_self();
#else 
    #error "this system not support!"
#endif 
    return tid;
}

bool task_handle(void *data)
{
    unsigned taskid = (unsigned)(data);

    if (!(taskid & 0x01)) {
        return false;
    }

    printf("[TASK tid:%u]: %u is handled!\n", get_tid(), taskid);
    return true;
}

void task_error(void *data)
{
    unsigned taskid = (unsigned)(data);

    printf("[TASK tid:%u]: taskid %u handle error!\n", get_tid(), taskid);
}

void task_cancel(void *data)
{
    unsigned taskid = (unsigned)(data);

    printf("[TASK tid:%u]: %u is canceled to handle!\n", get_tid(), taskid);
}
