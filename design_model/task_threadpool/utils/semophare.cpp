#include "semophare.h"

semophare::semophare()
    : count(1)
    , wakeup(0)
{

}

semophare::~semophare()
{
    count = 0;
    wakeup = 0;
}

void semophare::wait()
{
    std::unique_lock<std::mutex> locker(mutex);
    while (--count < 0)
    {
        cond.wait(locker, [&]()->bool { return wakeup > 0; });
        --wakeup;
    }
}

void semophare::signal()
{
    std::unique_lock<std::mutex> locker(mutex);
    if (++count <= 0)
    {
        ++wakeup;
        cond.notify_one();
    }
}

void semophare::wait1()
{
    std::unique_lock<std::mutex> locker(mutex);
    while (--count < 0)
    {
        cond.wait(locker);
    }
}

void semophare::signal1()
{
    std::unique_lock<std::mutex> locker(mutex);
    if (++count <= 0)
    {
        cond.notify_one();
    }
}