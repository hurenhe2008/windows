#ifndef _COMM_MACRO_H_
#define _COMM_MACRO_H_

/*just display some useful macro to fast develop.*/

#ifdef _DEBUG
    #include <crtdbg.h>
    #define ASSERTX(exp)            _ASSERTE(exp)
    #define ASSERTX_STR(exp, msg)   _ASSERT_EXPR(exp, msg)
#else 
    #define ASSERTX(x)              if (!(x)) { *(int *)0 = 0;}
    #define ASSERTX_STR(x, msg)     if (!(x)) { *(int *)0 = 0;}
#endif


#define SAFE_DELETE(p)    do { if (p) { delete p; p = NULL; } } while(0)
#define SAFE_RELEASE(p)   do { if (p) { p->release(); } } while(0)


#define MIN(x, y)         ((x) < (y) ? (x) : (y))
#define MAX(x, y)         ((x) > (y) ? (x) : (y))


#define MIN_3(x, y, z)    MIN(MIN(x, y), MIN(y, z))
#define MAX_3(x, y, z)    MAX(MAX(x, y), MAX(y, z))
#define MID_3(x, y, z)    (MAX(x, y) != MAX(y, z)) ? MIN(MAX(x, y), MAX(y, z)) : MAX(x, z)
#define MID_3x(x, y, z)   ((x) > (y) ? ((x) > (z) ? ((y) > (z) ? (y) : (z)) : (x)) : ((x) > (z) ? (x) : ((y) > (z) ? (z) : (y))))


#define TOSTR2(n)         #n
#define TOSTR(n)          TOSTR2(n)    /* second expansion: get n's value and turn to string */

#define MSGX(str)         message(str " has happend in (line:" TOSTR2(__LINE__) " func:" __FUNCTION__ " file:" __FILE__ ")")
#pragma MSGX("test MSGX!")  


/*
procompile macro :
    #ifndef/#ifdef ... #elif ... #else ... #endif     //condition judge sentence
    #error str                                        //output the error str when compiling and compile will interrupt.
    #pragma message(str)                              //output the message str when compiling.
    #pragma comment(lib, "")                          //linker the import library.
*/

#ifndef xxx
    //do something
    #pragma message("correct")
#else
    //do other something
    #error "something wrong"
#endif

#pragma message("test pragma message")
#pragma MSGX("test MSGX macro instead message")


#endif //_COMM_MACRO_H_
