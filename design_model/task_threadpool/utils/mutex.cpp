#include "mutex.h"
#include <assert.h>

#ifdef _WIN32
    #define MUTEX_SPINCOUT          4000
    #define MUTEX_SPINCOUT_FLAG     0x80000000
#endif 

Mutex::Mutex()
    :m_mutex(Mutex_NULL)
{
    assert(Init());
}

Mutex::~Mutex()
{
    assert(UnInit());
}

bool Mutex::Lock()
{
#if defined(_WIN32)
    EnterCriticalSection(&m_mutex);
#elif defined(__linux__)
    return 0 == pthread_mutex_lock(&m_mutex);
#else 
    #error "this system not support!"
#endif 

    return true;
}

bool Mutex::TryLock()
{
#if defined(_WIN32)
    return TryEnterCriticalSection(&m_mutex) ? true : false;
#elif defined(__linux__)
    return 0 == pthread_mutex_trylock(&m_mutex);
#else 
    #error "this system not support!"
#endif 
    return true;
}

bool Mutex::UnLock()
{
#if defined(_WIN32)
    LeaveCriticalSection(&m_mutex);
#elif defined(__linux__)
    return 0 == pthread_mutex_unlock(&m_mutex);
#else 
    #error "this system not support!"
#endif 
    return true;
}

bool Mutex::Init()
{
#if defined(_WIN32)
    DWORD spincout = MUTEX_SPINCOUT;
    spincout |= MUTEX_SPINCOUT_FLAG;
    BOOL bret = InitializeCriticalSectionAndSpinCount(&m_mutex, spincout);
    return bret ? true : false;   
#elif defined(__linux__)
    return 0 == pthread_mutex_init(&m_mutex, nullptr);
#else 
    #error "this system not support!"
#endif 
    return true;
}

bool Mutex::UnInit()
{
#if defined(_WIN32)
    DeleteCriticalSection(&m_mutex);
#elif defined(__linux__)
    return 0 == pthread_mutex_destroy(&m_mutex);
#else 
    #error "this system not support!"
#endif 
    return true;
}