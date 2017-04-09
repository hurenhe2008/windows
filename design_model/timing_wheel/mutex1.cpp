#include "mutex1.h"
#include <new>

/* ms recommended value (process defalut heap critical_section spincout) */
/* try cycle before thread goto sleep */
#define CRITICAL_SECTION_SPINCOUT_VALUE    4000         
/* create semaphore when create critical_section */
#define CRITICAL_SECTION_SPINCOUT_FLAG     0x80000000    

Mutex1* Mutex1::create()
{
    Mutex1 *tmp = new(std::nothrow) Mutex1;

    if (tmp && tmp->init()) {
        return tmp;
    }

    delete tmp;
    return tmp = nullptr;
}

Mutex1::Mutex1()
{

}

Mutex1::~Mutex1()
{
    DeleteCriticalSection(&m_cs);
}

bool Mutex1::init()
{
    /* spincount in [0 - 0x00FFFFFF] */
    DWORD spincount = CRITICAL_SECTION_SPINCOUT_VALUE;

    /* initialize critical_section and create semaphore */
    spincount |= CRITICAL_SECTION_SPINCOUT_FLAG;

    BOOL bret = InitializeCriticalSectionAndSpinCount(&m_cs, spincount);

    return bret ? true : false;
}

void Mutex1::lock()
{
    EnterCriticalSection(&m_cs);
}

bool Mutex1::trylock()
{
    return TryEnterCriticalSection(&m_cs) ? true : false;
}

void Mutex1::unlock()
{
    LeaveCriticalSection(&m_cs);
}