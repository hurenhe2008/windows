#ifndef _MUTEX_H_
#define _MUTEX_H_

class Mutex
{
public:
    virtual void lock() = 0;

    virtual bool trylock() = 0;

    virtual void unlock() = 0;
};


#endif //_MUTEX_H_