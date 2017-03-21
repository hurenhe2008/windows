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


#endif //_SAFE_MUTEX_LOCKER_H_
