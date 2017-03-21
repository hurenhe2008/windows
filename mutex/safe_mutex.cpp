#include "safe_mutex.h"
#include <new>

/* ms recommended value (process defalut heap critical_section spincout) */
/* try cycle before thread goto sleep */
#define CRITICAL_SECTION_SPINCOUT_VALUE    4000         
/* create semaphore when create critical_section */
#define CRITICAL_SECTION_SPINCOUT_FLAG     0x80000000    

SafeMutex* SafeMutex::create()
{
    SafeMutex *tmp = new(std::nothrow) SafeMutex;

    if (tmp && tmp->init()) {
        return tmp;
    }

    delete tmp;
    return tmp = nullptr;
}

SafeMutex::SafeMutex()
{

}

SafeMutex::~SafeMutex()
{
    DeleteCriticalSection(&m_cs);
}

bool SafeMutex::init()
{
    /* spincount in [0 - 0x00FFFFFF] */
    DWORD spincount = CRITICAL_SECTION_SPINCOUT_VALUE;
    
    /* initialize critical_section and create semaphore */
    spincount |= CRITICAL_SECTION_SPINCOUT_FLAG;
    
    BOOL bret = InitializeCriticalSectionAndSpinCount(&m_cs, spincount);
    
    return bret ? true : false;
}

void SafeMutex::lock()
{
    EnterCriticalSection(&m_cs);
}

bool SafeMutex::trylock()
{
    return TryEnterCriticalSection(&m_cs) ? true : false;
}

void SafeMutex::unlock()
{
    LeaveCriticalSection(&m_cs);
}