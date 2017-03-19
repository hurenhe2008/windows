#ifndef _THREAD_TLS_H_
#define _THREAD_TLS_H_

#ifdef __cplusplus
extern "C"
{
#endif 

typedef struct thread_data_s {
    char thread_name[101];
    char thread_info[101];
    unsigned thread_seq;
} thread_data_t;


unsigned __stdcall thread_proc(void *data);


#ifdef __cplusplus
}
#endif

#endif //_THREAD_TLS_H_
