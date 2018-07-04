#ifndef _THREAD_H_
#define _THREAD_H_

#include <Windows.h>

class Thread
{
public:
    Thread();
    virtual ~Thread();

    bool start();
    bool stop();

    virtual unsigned run() = 0;

protected:
	bool is_to_run() { return m_brun; }

private:
    static unsigned __stdcall thread_proc(void *);

private:
	volatile bool m_brun;
    HANDLE m_hthread;

	//disable assign operator and copy.
	Thread(const Thread&) = delete;
	Thread& operator=(const Thread&) = delete;
};

#endif //_THREAD_H_
