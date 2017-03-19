#ifndef _CORE_DUMP_H_
#define _CORE_DUMP_H_

#ifdef __cplusplus
extern "C"
{
#endif 


struct _EXCEPTION_POINTERS;

/*
use example:
__try{
    _normal_code_
}
__except(except_filter(GetExceptionInformation())) {
    _except_handle_code_
}
*/
long __stdcall except_filter(struct _EXCEPTION_POINTERS *);

/*
use example: 
__try{
    _normal_code_
}
__except(except_filter(GetExceptionCode(), GetExceptionInformation())) {
    _except_handle_code_
}
*/
long __stdcall except_filter_ex(unsigned, struct _EXCEPTION_POINTERS *);


/*
use example:
int main(int argc, char *argv[]) 
{
    coredump_catch();  //must before normal code.

    ... normal_code ...
}
*/
void coredump_catch();

/*
use example:
int main(int argc, char *argv[])
{
    coredump_catch_ex("appname");  //must before normal code.

    ... normal_code ...
}
*/
void coredump_catch_ex(const char *appname);


#ifdef __cplusplus
}
#endif


#endif //_CORE_DUMP_H_
