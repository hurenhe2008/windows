#ifndef _SEMOPHARE_H_
#define _SEMOPHARE_H_

#include <mutex>
#include <condition_variable>

class semophare
{
public:
    semophare();
    ~semophare();

public:
    void wait();
    void signal();

    void wait1();
    void signal1();

private:
    int count;
    int wakeup;
    std::mutex mutex;
    std::condition_variable cond;
};


#endif //_SEMOPHARE_H_