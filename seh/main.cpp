#include <stdio.h>
#include <stdlib.h>
#include <excpt.h>
//#include <crtdbg.h>
#include <exception>

#include "thread.h"
#include "except_filter.h"
#include "error.h"

/*
   if __try{} block code throw exception, the debugger will 
   catch the exception event (WaitForDebugEvent) before 
   __except filter function handle, until debugger call 
   ContinueDebugEvent __except filter function then handle.
*/

int main(int argc, char *argv[])
{
    int ret = 0;

    __try {     

        //throw new std::bad_alloc();
		
        if (!thread_start()) {
            output_win_error();
            ret = EXIT_FAILURE;
            __leave;
        }

        if (!thread_stop()) {
            output_win_error();
            ret = EXIT_FAILURE;
            __leave;
        }      
    }
    __except(except_filter(GetExceptionCode(), GetExceptionInformation())) { /*exception filter code*/
        /*exception handle code*/
        printf("exception handle code\n");
    }

    return ret;
}


