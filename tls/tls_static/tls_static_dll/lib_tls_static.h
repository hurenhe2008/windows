#ifndef _LIB_TLS_STATIC_H_
#define _LIB_TLS_STATIC_H_


#ifdef DLLEXPORT
    #define DLLAPI __declspec(dllexport)
#else 
    #define DLLAPI __declspec(dllimport)
#endif


#ifdef __cplusplus
extern "C"
{
#endif 

#ifndef bool
#include <stdbool.h>
#endif


typedef struct tls_info_t {
    bool binit;
    char thread_name[20];
    unsigned tid;
    unsigned pid;
} tls_info_t;


DLLAPI tls_info_t* _cdecl get_tls_info();

DLLAPI int _cdecl get_tls_int();

DLLAPI char* _cdecl get_tls_ptr();


#ifdef __cplusplus
}
#endif 

#endif //_LIB_TLS_STATIC_H_
