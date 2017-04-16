#include "conditon.h"
#include "mutex.h"
#include <assert.h>

#ifdef __linux__
#include <unistd.h>
#endif 


Condition::Condition()
{
    assert(Init());
}

Condition::~Condition()
{
    assert(UnInit());
}

bool Condition::Wait(Mutex &mutex)
{
#if defined(_WIN32)
    return (SleepConditionVariableCS(&m_cond, &mutex.m_mutex,
        INFINITE)) ? true : false;
#elif defined(__linux__)
    return 0 == pthread_cond_wait(&m_cond, &mutex.m_mutex);
#else 
    #error("this system not support!");
#endif 
    return true;
}

bool Condition::WaitTime(Mutex &mutex, unsigned milliseconds)
{
#if defined(_WIN32)
    return (SleepConditionVariableCS(&m_cond, &mutex.m_mutex, 
        milliseconds)) ? true : false;
#elif defined(__linux__)
    struct timespec timeout;
    timeout.tv_sec = milliseconds / 1000;
    timeout.tv_nsec = (milliseconds % 1000) * 1000 * 1000; /*nanosecond*/
    return 0 == pthread_cond_timedwait(&m_cond, &mutex.m_mutex, &timeout);
#else 
    #error("this system not support!");
#endif 
    return true;
}

bool Condition::Signal()
{
#if defined(_WIN32)
    WakeConditionVariable(&m_cond);
#elif defined(__linux__)
    return 0 == pthread_cond_signal(&m_cond);
#else 
    #error("this system not support!");
#endif 
    return true;
}

bool Condition::Broadcast()
{
#if defined(_WIN32)
    WakeAllConditionVariable(&m_cond);
#elif defined(__linux__)
    return 0 == pthread_cond_broadcast(&m_cond);
#else 
    #error("this system not support!");
#endif 
    return true;
}

bool Condition::Init()
{
#if defined(_WIN32)
    InitializeConditionVariable(&m_cond);
#elif defined(__linux__)
    return 0 == pthread_cond_init(&m_cond, nullptr);
#else 
    #error("this system not support!");
#endif 
    return true;
}

bool Condition::UnInit()
{
#if defined(_WIN32)
    /* not release */
#elif defined(__linux__)
    return 0 == pthread_cond_destroy(&m_cond);
#else 
    #error("this system not support!");
#endif 
    return true;
}
