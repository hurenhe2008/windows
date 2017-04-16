#ifndef __TASK_THREAD_POOL_H__
#define __TASK_THREAD_POOL_H__

#include "task_queue.h"
#include "mutex.h"
#include "conditon.h"

class Thread;
class TaskThreadMgr
{
public:
    TaskThreadMgr(unsigned queuesize = 256, unsigned threadcnt = 0);
    virtual ~TaskThreadMgr();

    bool AddTask(const task_info_t *task);

    bool Start();

    bool Stop();

    bool run();

private:
    bool Init();

    bool UnInit();
    
    unsigned CalcIdealThreadCnt();

    TaskThreadMgr(const TaskThreadMgr &) = delete;
    TaskThreadMgr& operator=(const TaskThreadMgr &) = delete;

private:
    volatile bool  m_bstop;
    unsigned       m_thread_cnt;
    Thread       **m_thread_list;
    TaskQueue      m_task_queue; 
    Mutex          m_task_mutex;
    Condition      m_cond_non_empty;
    Condition      m_cond_non_full;
};

#endif //__TASK_THREAD_POOL_H__
