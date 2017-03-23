#include "singleton.h"

#include <new>
#include <mutex>

static std::mutex s_mutex;
Singleton* Singleton::mp_self = nullptr;

Singleton& Singleton::instance()
{
    if (!mp_self) {
        /* lock */
        s_mutex.lock();
        if (!mp_self) {   /* double check */
            /* avoid cpu switch assigne and construct operation */
            Singleton *tmp = new(std::nothrow) Singleton;
            /* return nullptr if init failed */
            if (tmp && !tmp->init()) {
                delete tmp;
                tmp = nullptr;
            }
            mp_self = tmp;
        }
        /* unlock */
        s_mutex.unlock();      
    }

    return *mp_self;
}

void Singleton::destroy()
{
    /* not safe-thread, advise not call if you can't control */
    /* advise call in mainthread after other threads all exit */
    if (mp_self) {
        delete mp_self;
        mp_self = nullptr;
    }
}

Singleton::Singleton()
{
    /* members init */
}

Singleton::~Singleton()
{
    /* members unit */
}

bool Singleton::init()
{
    /* singleton init */
    return true;
}


//============================================================================
SingletonF& SingletonF::instance()
{
    static SingletonF self;
    
    return self;
}

SingletonF::SingletonF()
{
    /* members init */
}

SingletonF::~SingletonF()
{
    /* members unit */
}

bool SingletonF::init()
{
    /* singleton init */
    return true;
}

