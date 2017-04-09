#ifndef _MUTEX_2_H_
#define _MUTEX_2_H_

#include <Windows.h>
#include "mutex.h"

class Mutex2 : public Mutex
{
public:
    Mutex2();
    ~Mutex2();

    void lock();

    bool trylock();

    void unlock();

private:
    bool init();

private:
    Mutex2(const Mutex2 &) = delete;
    Mutex2& operator=(const Mutex2 &) = delete;

    CRITICAL_SECTION m_cs;
};

#endif //_MUTEX_2_H_