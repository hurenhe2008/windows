#ifndef _MUTEX_1_H_
#define _MUTEX_1_H_

#include <Windows.h>
#include "mutex.h"

class Mutex1 : public Mutex
{
public:
    static Mutex1* create();
    ~Mutex1();

    void lock();

    bool trylock();

    void unlock();

private:
    Mutex1();

    bool init();

private:
    Mutex1(const Mutex1 &) = delete;
    Mutex1& operator=(const Mutex1 &) = delete;

    CRITICAL_SECTION m_cs;
};

#endif //_MUTEX_1_H_
