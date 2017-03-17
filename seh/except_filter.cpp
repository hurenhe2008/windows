#include "except_filter.h"
#include <excpt.h>
#include <Windows.h>
#include <stdio.h>

long except_filter(unsigned long errcode, _EXCEPTION_POINTERS *except)
{
    printf("exception code: %ld\n", errcode);

    if (EXCEPTION_NONCONTINUABLE ==
        except->ExceptionRecord->ExceptionFlags) {
        return EXCEPTION_EXECUTE_HANDLER;
    }
    else {
        return EXCEPTION_CONTINUE_EXECUTION;
    }
}