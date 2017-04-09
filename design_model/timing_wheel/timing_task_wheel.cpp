#include "timing_task_wheel.h"
#include "mutex_locker.h"
#include <Windows.h>
#include <new>

TimingTaskWheel* TimingTaskWheel::mp_self = nullptr;

TimingTaskWheel& TimingTaskWheel::instance()
{
    if (!mp_self) {
        m_inst_mutex.lock();
        if (!mp_self) {
            TimingTaskWheel *tmp = new TimingTaskWheel();
            if (!tmp || !tmp->init()) {
                delete tmp;
                tmp = nullptr;
            }
            mp_self = tmp;
        }
        m_inst_mutex.unlock();
    }

    return *mp_self;
}

void TimingTaskWheel::destroy()
{
    if (mp_self) {
        mp_self->uninit();

        delete mp_self;
        mp_self = nullptr;
    }
}

TimingTaskWheel::TimingTaskWheel()
    : m_curr_pos(0)
{

}

TimingTaskWheel::~TimingTaskWheel()
{
    destroy();
}

bool TimingTaskWheel::init()
{
    MutexLocker lock(&m_task_mutex);

    for (int i = 0; i < TASK_QUEUE_SIZE; ++i) {
        m_task_queue[i].clear();
    }

    return true;
}

bool TimingTaskWheel::uninit()
{
    MutexLocker lock(&m_task_mutex);

    for (int i = 0; i < TASK_QUEUE_SIZE; ++i) {
        task_queue_citr cit = m_task_queue[i].begin();
        for (; cit != m_task_queue[i].end(); ++cit) {
            /*task not finish, callback notify*/
            if (cit->touch_after_seconds > 0) {
                cit->nofinish_func(cit->data);
            }
        }
        m_task_queue[i].clear();
    }

    return true;
}


unsigned TimingTaskWheel::run()
{
    const struct timeval time_val = { 1, 0 };
   
    /* time interval */
    int ret = select(0, nullptr, nullptr, nullptr, &time_val);

    /* error */
    if (ret < 0) {  
        return 0;
    }

    /* handle task in queue */
    handleTaskQueue(); 

    return 1;
}

void TimingTaskWheel::handleTaskQueue()
{

}

bool TimingTaskWheel::insertTask(timing_task_t *task)
{
    MutexLocker lock(&m_task_mutex);

    if (m_curr_pos >= TASK_QUEUE_SIZE) {
        m_curr_pos = 0;
    }

    m_task_queue[m_curr_pos].insert(*task);
    return true;
}



