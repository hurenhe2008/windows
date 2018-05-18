#ifndef _SINGLETONX_H_
#define _SINGLETONX_H_

#include <mutex>
#include <new>

#define DISCOPY_AND_ASSIGN(classname)          \
    classname(classname &) = delete;           \
    void operator=(classname &) = delete;      \


/***
*
* regist event callback to exit manager.
*
***/
typedef void(*Func)(void *);
void regist_exit_event(Func pFunc, void *data)
{
    //regist event to exit manager.
}

  
/***
*
* re-define allocate and release 
*
***/
template<typename T>
struct Allocator
{
    static T* alloc()
    {
        return new T;
    }

    static void free(T *p)
    {
        delete p;
    }
};

/***
*
* SingletonX is safe-thread to access. 
* if SingletonX is used in multi-thread, must keep T's interface is safe-thread to access.
*
* used example: 
* SingletonX<classname> x;   
* >. x->function();    
* >. (*x).function();
* >. SingletonX<classname>::instance()->function();
*
***/
template<typename T, typename Traits=Allocator<T> >
class SingletonX
{
public:
    static T* instance()
    {   
        if (!m_pInstance) //double-check mutex
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            if (!m_pInstance)
            {
                m_pInstance = Traits::alloc();

                regist_exit_event(destroy, nullptr);
            }
        }

        return m_pInstance;
    }

    static void destroy(void * data)
    {
        Traits::free(m_pInstance);
        m_pInstance = nullptr;
    }

    T* operator->()
    {
        return instance();
    }

    T& operator*()
    {
        return *instance();
    }

public:
    SingletonX() {};  

private:
    static T* volatile m_pInstance;
    static std::mutex m_mutex;

    DISCOPY_AND_ASSIGN(SingletonX);
};

template<typename T, typename Traits>
T* volatile SingletonX<T, Traits>::m_pInstance = nullptr;

template<typename T, typename Traits>
std::mutex SingletonX<T, Traits>::m_mutex;


#endif //_SINGLETONX_H_
