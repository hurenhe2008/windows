#include "task_thread_pool.h"
#include ".\utils\thread.h"
#include <new>

#if defined(_WIN32) 
    #include <Windows.h>
    unsigned __stdcall thread_pool_proc(void *data);
#elif defined(__linux__)
    #include <sys/sysinfo.h>
    void* thread_pool_proc(void *data);
#else 
    #error "this system not support!"
#endif 

TaskThreadMgr::TaskThreadMgr(unsigned queuesize, unsigned threadcnt)
    : m_task_queue(queuesize)
    , m_bstop(true)
    , m_thread_cnt(threadcnt)
    , m_thread_list(nullptr)
{
    if (m_thread_cnt <= 0) {
        m_thread_cnt = CalcIdealThreadCnt();
    }
}

TaskThreadMgr::~TaskThreadMgr()
{
    UnInit();
}

bool TaskThreadMgr::AddTask(const task_info_t *task)
{
    bool bret = false;

    m_task_mutex.Lock();

    while (m_task_queue.Full() && !m_bstop) {
        m_cond_non_full.Wait(m_task_mutex);
    }

    if (m_bstop) {
        m_task_mutex.UnLock();
        return false;   
    }

    bret = m_task_queue.Enqueue(task);

    m_task_mutex.UnLock();
    m_cond_non_empty.Signal();

    return bret;
}

bool TaskThreadMgr::Start()
{
    m_bstop = false;

    return Init();
}

unsigned __stdcall thread_pool_proc(void *data)
{
    TaskThreadMgr *pmgr = reinterpret_cast<TaskThreadMgr *>(data);

    if (!pmgr) {
        return 1;
    }

    return pmgr->run() ? 0 : 1;
}

bool TaskThreadMgr::run()
{
    while (!m_bstop) {
        m_task_mutex.Lock();

        //must judge m_bstop here.
        //when stop, here maybe dead cycle if queue emtpy 
        while (m_task_queue.Empty() && !m_bstop) {  
            m_cond_non_empty.Wait(m_task_mutex);
        }

        //want task all finish
        /*if (m_bstop && m_task_queue.Empty()) {
            m_task_mutex.UnLock();
            break;
        }*/

        //want exit rightnow, maybe some task not handle
        if (m_bstop) {   
            m_task_mutex.UnLock();
            break;  
        }

        task_info_t &task = m_task_queue.Dequeue(); 
        m_task_mutex.UnLock();

        m_cond_non_full.Signal();
        
        if (!task.handle_fun(task.data)) {
            task.error_fun(task.data);
        }
    }

    return true;
}

bool TaskThreadMgr::Stop()
{
    bool bret = true; 

    m_bstop = true;
    m_cond_non_empty.Broadcast();
    m_cond_non_full.Broadcast();

    for (unsigned i = 0; i < m_thread_cnt; ++i) {
        bret = m_thread_list[i]->Stop();
    }

    return bret;
}

bool TaskThreadMgr::Init()
{
    bool binit = true;

    if (!m_task_queue.Init()) {
        return false;
    }

    if (!m_thread_list) {
        if (m_thread_cnt <= 0) {
            m_thread_cnt = CalcIdealThreadCnt();
        }
        
        m_thread_list = new(std::nothrow) Thread *[m_thread_cnt];
        if (!m_thread_list) {
            return false;
        }
        memset(m_thread_list, 0, sizeof(Thread*) * m_thread_cnt);

        for (unsigned i = 0; i < m_thread_cnt; ++i) {
            m_thread_list[i] = new(std::nothrow) 
                Thread(&thread_pool_proc, this);

            if (!m_thread_list[i] || !m_thread_list[i]->Start()) { 
                binit = false;
            }
        }
    }

    return binit;
}

bool TaskThreadMgr::UnInit()
{
    if (!Stop()) {   //stop first, maybe fail
        return false;
    }

    if (m_thread_list) {
        for (unsigned i = 0; i < m_thread_cnt; ++i) {
            if (m_thread_list[i]) {
                delete m_thread_list[i];
                m_thread_list[i] = nullptr;
            }
        }

        delete[] m_thread_list;
        m_thread_list = nullptr;
    }
    m_thread_cnt = 0;

    //if task not be handled, a chance to notify
    if (!m_task_queue.Empty()) {
        task_info_t &task = m_task_queue.Dequeue();
        task.cancel_fun(task.data);
    }

    return true;
}

unsigned TaskThreadMgr::CalcIdealThreadCnt()
{
    // ideals = 2 * CPUs + 2

    unsigned ideals = 1;

#if defined(_WIN32) 
    SYSTEM_INFO syinfo;
    GetSystemInfo(&syinfo);
    ideals = syinfo.dwNumberOfProcessors * 2 + 2;
#elif defined(__linux__)
    ideals = get_nprocs();
#else 
    #error "this system not support!"
#endif 

    return ideals;
}