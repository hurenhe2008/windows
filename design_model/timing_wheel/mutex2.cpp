
#include "mutex2.h"
#include <new>


#ifdef _DEBUG
    #include <crtdbg.h>
    #define assert_x(exp, str)      _ASSERT_EXPR(exp, str)
#else 
    #include <exception>
    #define assert_x(exp, str)                               \
        if (!(exp)) {                                        \
            std::exception * expt = new std::exception(str); \
            throw expt;                                      \
        }
#endif 


/* ms recommended value */
/* try cycle before thread goto sleep */
#define CRITICAL_SECTION_SPINCOUT_VALUE    4000         
/* create semaphore when create critical_section */
#define CRITICAL_SECTION_SPINCOUT_FLAG     0x80000000    


Mutex2::Mutex2()
{
    assert_x(init(), "mutex create failed!");
}

Mutex2::~Mutex2()
{
    DeleteCriticalSection(&m_cs);
}

bool Mutex2::init()
{
    /* spincount in [0 - 0x00FFFFFF] */
    DWORD spincount = CRITICAL_SECTION_SPINCOUT_VALUE;

    /* initialize critical_section and create semaphore */
    spincount |= CRITICAL_SECTION_SPINCOUT_FLAG;

    BOOL bret = InitializeCriticalSectionAndSpinCount(&m_cs, spincount);

    return bret ? true : false;
}

void Mutex2::lock()
{
    EnterCriticalSection(&m_cs);
}

bool Mutex2::trylock()
{
    return TryEnterCriticalSection(&m_cs) ? true : false;
}

void Mutex2::unlock()
{
    LeaveCriticalSection(&m_cs);
}