#define WIN32_LEAN_AND_MEAN
#include "timing_task_wheel.h"
#include "mutex_locker.h"
#include <Windows.h>
#include <winsock2.h>
#include <new>

#pragma comment(lib, "ws2_32.lib")

TimingTaskWheel* TimingTaskWheel::mp_self = nullptr;
Mutex2 TimingTaskWheel::m_inst_mutex;

TimingTaskWheel& TimingTaskWheel::instance()
{
    if (!mp_self) {
        /* lock */
        m_inst_mutex.lock();
        /* double check */
        if (!mp_self) {
            /* avoid cpu change constructor and assign sequence */
            TimingTaskWheel *tmp = new TimingTaskWheel();
            if (!tmp || !tmp->init()) {
                delete tmp;
                tmp = nullptr;
            }      
            mp_self = tmp;
        }
        /* unlock */
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
    WSADATA wsdata;
    if (0 != WSAStartup(MAKEWORD(2, 2), &wsdata)) {
        return false;
    }

    MutexLocker lock(&m_task_mutex);

    for (int i = 0; i < TASK_QUEUE_SIZE; ++i) {
        m_task_queue[i].clear();
    }

    return true;
}

bool TimingTaskWheel::uninit()
{
    /* stop thread if run */
    if (!this->stop())  return false;

    if (0 != WSACleanup()) return false;

    MutexLocker lock(&m_task_mutex);

    for (int i = 0; i < TASK_QUEUE_SIZE; ++i) {

        task_queue_t::const_iterator cit = m_task_queue[i].begin();

        for (; cit != m_task_queue[i].end(); ++cit) {
            /*task not finish, callback notify*/
            if (cit->touch_after_seconds > 0) {
                cit->cancel_func(cit->data);
            }
        }

        m_task_queue[i].clear();
    }

    return true;
}


unsigned TimingTaskWheel::run()
{ 
    struct timeval time_val = { 0 };
    unsigned last_time = 0;
    int interval = 0;
    bool bfirst = true;
    int retval = 1;

    while (m_brun) {
        
        if (!bfirst) {
            interval = last_time + PERIOD_TIME - GetTickCount();
            if (interval < 0) {
                interval = 0;
            }
        }
        else {
            last_time = GetTickCount();
            interval = PERIOD_TIME;
            bfirst = false;
        }

        time_val.tv_sec = (interval < 1000) ? 0 : (interval / 1000);
        time_val.tv_usec = (0 == interval) ? 0 : (interval % 1000);

        /* time interval */
        int ret = select(0, nullptr, nullptr, nullptr, &time_val);
        if (ret < 0) {
            if (WSAEINTR == WSAGetLastError()) {
                continue;
            }
            retval = 0;
            break;
        }

        handleTaskQueue();
    }

    return retval;
}

void TimingTaskWheel::handleTaskQueue()
{
    MutexLocker lock(&m_task_mutex);

    if (m_curr_pos >= TASK_QUEUE_SIZE) {
        /* error happen */
        abort(); 
    }
 
    task_queue_t &queue = m_task_queue[m_curr_pos];
    task_queue_t::iterator it = queue.begin();

    for (; it != queue.end(); ) {
        if (it->touch_after_cycles <= 0) {
            if (!it->touch_func(it->data)) {
                it->error_func(it->data);
            }
            it = queue.erase(it);
        }
        else {
            it->touch_after_cycles -= 1;
            it++;
        }
    }

    if (m_curr_pos + 1 < TASK_QUEUE_SIZE)
        ++m_curr_pos;
    else
        m_curr_pos = 0;

}

bool TimingTaskWheel::insertTask(timing_task_t *task)
{
    MutexLocker lock(&m_task_mutex);

    if (m_curr_pos >= TASK_QUEUE_SIZE) {
        abort();
    }

    /* cycles to recycle */
    unsigned cycles = task->touch_after_seconds / TASK_QUEUE_SIZE;
    /* insert position */
    unsigned insert_pos = m_curr_pos + 
        task->touch_after_seconds % TASK_QUEUE_SIZE;
    
    task->touch_after_cycles = cycles;

    if (insert_pos >= TASK_QUEUE_SIZE) {
        insert_pos = insert_pos % TASK_QUEUE_SIZE;
    }
  
    m_task_queue[insert_pos].push_back(*task);

    return true;
}
