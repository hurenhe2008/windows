#ifndef __THREAD_H__
#define __THREAD_H__

#if defined(_WIN32) 
    #include <Windows.h>
    #include <process.h>
    typedef HANDLE Thread_t;
    #define Thread_NULL  NULL
    typedef unsigned(__stdcall *Thread_Func)(void *);
#elif defined(__linux__)
    #include <pthread.h>
    typedef pthread_t Thread_t;
    #define Thread_NULL  0
    typedef void *(*Thread_Func)(void *);
#else 
    #error "this system is not supported!"
#endif 


class Thread
{
public:
    Thread(Thread_Func func, void *data);
    virtual ~Thread();

    bool Start();

    bool Stop();

private:
    Thread(const Thread &) = delete;
    Thread& operator=(const Thread &) = delete;

private:
    Thread_t    m_thread_handle;
    Thread_Func m_thread_func;
    void *      m_thread_data;
};

#endif //__THREAD_H__
