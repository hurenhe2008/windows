#ifndef __TASK_QUEUE_H__
#define __TASK_QUEUE_H__

#include "task.h"

class TaskQueue
{
public:
    TaskQueue(unsigned size);
    virtual ~TaskQueue();

    bool Init();

    bool UnInit();

    bool Enqueue(const task_info_t *);

    task_info_t Dequeue();

    bool Full();

    bool Empty();

private:
    TaskQueue(const TaskQueue &) = delete;
    TaskQueue& operator=(const TaskQueue &) = delete;

private:
    unsigned      m_read_pos;
    unsigned      m_write_pos;
    unsigned      m_queue_size;
    task_info_t  *m_task_queue;
};







#endif //__TASK_QUEUE_H__