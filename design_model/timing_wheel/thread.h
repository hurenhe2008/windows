#ifndef _THREAD_H_
#define _THREAD_H_

#include <Windows.h>

class Thread
{
public:
    Thread();
    virtual ~Thread();

    bool start();
    bool isrun() const { return m_brun; }
    bool stop(unsigned timeout = INFINITE);

    virtual unsigned run() = 0;

private:
    static unsigned __stdcall thread_proc(void *);

    Thread(const Thread&) = delete;
    Thread& operator=(const Thread&) = delete;

protected:
    volatile bool m_brun;

private:
    HANDLE        m_hthread;
};


#endif //_THREAD_H_
