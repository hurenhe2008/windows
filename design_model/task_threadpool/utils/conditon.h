#ifndef __CONDITION_H__
#define __CONDITION_H__

#if defined(_WIN32)
    #include <Windows.h>
    typedef CONDITION_VARIABLE Condition_t;
#elif defined(__linux__)
    #include <pthread.h>
    typedef pthread_cond_t Condition_t;
#else 
    #error "this system not support"
#endif 

class Mutex;
class Condition
{
public:
    Condition();
    virtual ~Condition();

    bool Wait(Mutex &mutex);

    bool WaitTime(Mutex &mutex, unsigned milliseconds);

    bool Signal();

    bool Broadcast();

private:
    bool Init();

    bool UnInit();

    Condition(const Condition &) = delete;
    Condition& operator=(const Condition &) = delete;

private:
    Condition_t  m_cond;
};


#endif //__CONDITION_H__