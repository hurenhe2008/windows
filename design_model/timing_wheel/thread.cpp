#include "thread.h"
#include <process.h>

Thread::Thread()
    : m_brun(false)
    , m_hthread(NULL)
{

}

Thread::~Thread()
{

}

bool Thread::start()
{
    if (m_hthread && m_brun) {
        return true;
    }

    m_hthread = (HANDLE)_beginthreadex(nullptr, 0, thread_proc, 
        (void *)this, CREATE_SUSPENDED, nullptr);

    if (NULL == m_hthread) {
        m_brun = false;
        return m_brun;
    }

    m_brun = true;
    if (-1 == ResumeThread(m_hthread)) {
        m_brun = false;
        CloseHandle(m_hthread);
        m_hthread = NULL;
    }

    return m_brun;
}

unsigned __stdcall Thread::thread_proc(void *data)
{
    Thread *pthread = (Thread *)(data);
    
    if (pthread) {
        return pthread->run();
    }

    return 0;
}

bool Thread::stop(unsigned timeout)
{
    m_brun = false;

    DWORD ret = WaitForSingleObject(m_hthread, timeout);

    if (ret != WAIT_OBJECT_0) {
        m_brun = true;
    }
    else {
        CloseHandle(m_hthread);
        m_hthread = NULL;
    }

    return !m_brun;
}