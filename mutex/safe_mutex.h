#ifndef _SAFE_MUTEX_H_
#define _SAFE_MUTEX_H_

#include <Windows.h>

class SafeMutex
{
public:
    static SafeMutex* create();
    ~SafeMutex();

    void lock();

    bool trylock();

    void unlock();

private:
    SafeMutex();

    bool init();

private:
    SafeMutex(const SafeMutex &) = delete;
    SafeMutex& operator=(const SafeMutex &) = delete;

    CRITICAL_SECTION m_cs;
};

#endif //_SAFE_MUTEX_H_