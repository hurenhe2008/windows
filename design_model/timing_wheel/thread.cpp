#include "thread.h"
#include <process.h>

Thread::Thread()
	: m_brun(false)
	, m_hthread(NULL)
{

}

Thread::~Thread()
{
	m_brun = false;

	if (m_hthread) 
	{
		CloseHandle(m_hthread);
		m_hthread = NULL;
	}
}

bool Thread::start()
{
	if (m_hthread && m_brun)
		return true;

	m_brun = true;

	m_hthread = (HANDLE)_beginthreadex(nullptr, 0, thread_proc,
		(void *)this, 0, nullptr);

	return m_hthread ? true : false;
}

unsigned __stdcall Thread::thread_proc(void *data)
{
	Thread *pthread = (Thread *)(data);

	if (pthread)
		return pthread->run();

	return 0;
}

bool Thread::stop()
{
	if (!m_brun && !m_hthread)  return true;

	m_brun = false;

	if (m_hthread)
	{
		DWORD ret = WaitForSingleObject(m_hthread, INFINITE);
		if (WAIT_OBJECT_0 == ret)
		{
			CloseHandle(m_hthread);
			m_hthread = NULL;
		}
		else
		{	
			abort();  //error happen.
		}
	}

	return !m_brun && !m_hthread;
}
