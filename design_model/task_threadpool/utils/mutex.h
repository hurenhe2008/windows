#ifndef __MUTEX_H__
#define __MUTEX_H__

#if defined(_WIN32) 
    #include <Windows.h>
    typedef CRITICAL_SECTION Mutex_t;
    #define Mutex_NULL 
#elif defined(__linux__)
    #include <pthread.h>
    typedef pthread_mutex_t Mutex_t;
    #define Mutex_NULL PTHREAD_MUTEX_INITIALIZER
#else 
    #error "this system not support!"
#endif 

class Condition;

class Mutex
{
public:
    friend class Condition;

    Mutex();
    virtual ~Mutex();

    bool Lock();

    bool TryLock();

    bool UnLock();

private:
    bool Init();

    bool UnInit();

    Mutex(const Mutex &) = delete;
    Mutex& operator=(const Mutex &) = delete;

private:
    Mutex_t  m_mutex;
};


#endif //__MUTEX_H__