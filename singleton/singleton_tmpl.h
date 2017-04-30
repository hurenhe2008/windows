#ifndef _SINGLETON_TMP_H_
#define _SINGLETON_TMP_H_

#include <new>
#include <mutex>

/********************************************************************
@singleton template:
    instance(): get signleton instance.
    destroy(): destroy singleton instance when exit.
    
@use:
    //inherit SingletonImpl
    class XXX : public SingletonImpl<XXX>
    {
        ......   
    };
********************************************************************/

template <typename T>
class SingletonTmpl
{
public:
    static T& instance();

    /* not safe-thread, advise not call it if you can't control */
    static void destroy();

private:
    SingletonTmpl(const SingletonTmpl &) = delete;
    SingletonTmpl& operator=(const SingletonTmpl &) = delete;
    SingletonTmpl* operator&() = delete;

private:
    static T* mp_self;
    static std::mutex m_mutex;
};


template <typename T>
T* SingletonTmpl<T>::mp_self = nullptr;

template<typename T>
std::mutex SingletonTmpl<T>::m_mutex;

template <typename T>
T& SingletonTmpl<T>::instance()
{
    if (!mp_self) {
        /* lock */
        m_mutex.lock();
        if (!mp_self) {   /* double check */
            /* avoid cpu switch assigne and construct operation */
            T *tmp = new(std::nothrow) T;
            mp_self = tmp;
        }
        /* unlock */
        m_mutex.unlock();
    }

    return *mp_self;
}

template <typename T>
void SingletonTmpl<T>::destroy()
{
    /* not safe-thread, advise not call if you can't control */
    /* advise call in mainthread after other threads all exit */
    if (mp_self) {
        delete mp_self;
        mp_self = nullptr;
    }
}


#endif //_SINGLETON_TMP_H_
