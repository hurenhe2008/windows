#ifndef _MUTEX_LOCKER_H_
#define _MUTEX_LOCKER_H_

class Mutex;

class MutexLocker
{
public:
    MutexLocker(Mutex *mutex);

    ~MutexLocker();

private:
    MutexLocker(const MutexLocker &) = delete;
    MutexLocker& operator=(const MutexLocker &) = delete;

    Mutex* mp_mutex;
};

//avoid code like:  "MutexLocker(&mutex);"  
//correct code is:  "MutexLocker locker(&mutex);"
#define MutexLocker(x)  static_assert(false, "missing MutexLocker variable name")

#endif //_MUTEX_LOCKER_H_

