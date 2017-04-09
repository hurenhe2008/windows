#include "mutex_locker.h"
#include "mutex.h"

MutexLocker::MutexLocker(Mutex *mutex)
    : mp_mutex(mutex)
{
    if (mp_mutex) {
        mp_mutex->lock();
    }
}

MutexLocker::~MutexLocker()
{
    if (mp_mutex) {
        mp_mutex->unlock();
    }   
}