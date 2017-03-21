#include "safe_mutex_locker.h"
#include "safe_mutex.h"

SafeMutexLocker::SafeMutexLocker(SafeMutex *mutex)
    : mp_mutex(mutex)
{
    if (mp_mutex) {
        mp_mutex->lock();
    }
}

SafeMutexLocker::~SafeMutexLocker()
{
    if (mp_mutex) {
        mp_mutex->unlock();
    }

}