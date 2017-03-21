#ifndef _MUTEX_H_
#define _MUTEX_H_

#include <Windows.h>

class Mutex
{
public:
    Mutex();

    ~Mutex();

    void lock();

    bool trylock();

    void unlock();

private:
    Mutex(const Mutex &) = delete;
    Mutex& operator=(const Mutex &) = delete;

    CRITICAL_SECTION m_cs;
};

#endif _MUTEX_H_