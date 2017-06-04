#ifndef _SAFE_MUTEX_LOCKER_H_
#define _SAFE_MUTEX_LOCKER_H_

class SafeMutex;

class SafeMutexLocker
{
public:
    SafeMutexLocker(SafeMutex *);
    ~SafeMutexLocker();

private:
    SafeMutexLocker(const SafeMutexLocker &) = delete;
    SafeMutexLocker& operator=(const SafeMutexLocker &) = delete;

    SafeMutex*  mp_mutex;
};


//avoid code like:  "SafeMutexLocker(&mutex);"  
//correct code is:  "SafeMutexLocker locker(&mutex);"
#define SafeMutexLocker(x)  static_assert(false, "missing MutexLocker variable name")

#endif //_SAFE_MUTEX_LOCKER_H_
