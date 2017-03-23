#ifndef _TLS_THREAD_H_
#define _TLS_THREAD_H_

#include <windows.h>

HANDLE tls_thread_start();

bool tls_thread_stop(HANDLE hthread);

bool tls_threads_stop(HANDLE hthread[], unsigned cnt);


#endif //_TLS_THREAD_H_
