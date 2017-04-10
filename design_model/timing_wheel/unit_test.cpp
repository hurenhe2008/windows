#include <stdio.h>
#include <stdlib.h>
#include "timing_task_wheel.h"

static int s_data = 0;
static int s_cnt = 0;
static const int s_sleep_time = 10000;   //10s

static void insert_task();
static bool task_touch(void *data);
static void task_cancel(void *data);
static void task_error(void *data);

int main(int argc, const char *argv[])
{
    if (!TimingTaskWheel::instance().start()) {
        abort();
    }

    while (s_cnt++ < 10) {
        insert_task();

        Sleep(s_sleep_time);
    }

    Sleep(1000 * 60 * 10);
    
    TimingTaskWheel::instance().destroy();
    
    return 0;
}

void insert_task()
{
    timing_task_t task;
    task.touch_func = task_touch;
    task.error_func = task_error;
    task.cancel_func = task_cancel;
    task.touch_after_seconds = 60;

    for (int i = 0; i < 10; ++i) {
        task.data = (void *)s_data;
        task.touch_after_seconds += s_data;
        TimingTaskWheel::instance().insertTask(&task);
        ++s_data;
    }
}

bool task_touch(void *data)
{
    int taskid = (int)data;

    if (0 == (taskid & 0x01)) {
        printf("task id: %d excute success!\n", taskid);
        return true;
    }
    else {
        printf("task id: %d excute failed!\n", taskid);
        return false;
    }
    
}

void task_cancel(void *data)
{
    int taskid = (int)data;

    printf("task id: %d is canceled!\n", taskid);
}

void task_error(void *data)
{
    int taskid = (int)data;

    printf("Error: task id(%d) failed!\n", taskid);
}
