#ifndef _TIMING_TASK_WHEEL_H_
#define _TIMING_TASK_WHEEL_H_

#include <set>
#include "mutex2.h"
#include "timing_task.h"
#include "thread.h"

class TimingTaskWheel : public Thread
{
public:
    static TimingTaskWheel& instance();
    static void destroy();

    bool insertTask(timing_task_t *);

private:
    TimingTaskWheel();
    ~TimingTaskWheel();

    bool init(); 
    bool uninit();
    unsigned run();
    void handleTaskQueue();

private:
    TimingTaskWheel(const TimingTaskWheel &) = delete;
    TimingTaskWheel& operator=(const TimingTaskWheel&) = delete;
    TimingTaskWheel* operator&() = delete;

    static Mutex2            m_inst_mutex;
    static TimingTaskWheel  *mp_self;

    unsigned                 m_curr_pos;
    Mutex2                   m_task_mutex;

    typedef std::set<timing_task_t> task_queue_t;
    typedef task_queue_t::iterator  task_queue_itr;
    typedef task_queue_t::const_iterator task_queue_citr;
#define TASK_QUEUE_SIZE  3600
    task_queue_t             m_task_queue[TASK_QUEUE_SIZE];
};




#endif //_TIMING_TASK_WHEEL_H_