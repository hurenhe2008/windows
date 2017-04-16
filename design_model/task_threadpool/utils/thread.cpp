#include "thread.h"
#include <assert.h>


Thread::Thread(Thread_Func func, void *data)
    : m_thread_handle(Thread_NULL)
    , m_thread_func(func)
    , m_thread_data(data)
{
    assert(m_thread_func);
}

Thread::~Thread()
{
    assert(Stop());

#if defined(_WIN32) 
    CloseHandle(m_thread_handle);
#endif 

    m_thread_handle = Thread_NULL;
}

bool Thread::Start()
{
    if (Thread_NULL != m_thread_handle) {
        return true;
    }

#if defined(_WIN32) 
    m_thread_handle = (HANDLE)_beginthreadex(nullptr, 0, 
        m_thread_func, m_thread_data, 0, nullptr);
#elif defined(__linux__)
    if (0 != pthread_create(&m_thread_handle, nullptr,
        m_thread_func, m_thread_data)) {
        m_thread_handle = Thread_NULL;
    }
#else 
    #error "this system not support!"
#endif

    return (Thread_NULL != m_thread_handle);
}

bool Thread::Stop()
{
    if (Thread_NULL != m_thread_handle) {
#if defined(_WIN32)     
        return WAIT_OBJECT_0 == 
            WaitForSingleObject(m_thread_handle, INFINITE);
#elif defined(__linux__)
        return 0 == pthread_join(m_thread_handle, nullptr);
#else
        #error "this system not support!"
#endif 
    }

    return true;
}
