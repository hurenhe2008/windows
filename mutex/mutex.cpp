#include "mutex.h"

Mutex::Mutex()
{
    /* not catch STATUS_NO_MEMORY exception for create something*/
    InitializeCriticalSection(&m_cs);
}

Mutex::~Mutex()
{
    DeleteCriticalSection(&m_cs);
}

void Mutex::lock()
{
    /*not catch EXCEPTION_INVALID_HANDLE exception for create semaphore*/
    EnterCriticalSection(&m_cs);
}

bool Mutex::trylock()
{
    return TryEnterCriticalSection(&m_cs) ? true : false;
}

void Mutex::unlock()
{
    LeaveCriticalSection(&m_cs);
}