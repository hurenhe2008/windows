#include "mutex_locker.h"
#include "mutex.h"

MutexLocker::MutexLocker(Mutex &mutex)
    : m_mutex(mutex)
{
    m_mutex.lock();
}

MutexLocker::~MutexLocker()
{
    m_mutex.unlock();
}