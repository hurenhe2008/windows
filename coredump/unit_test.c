#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "core_dump.h"

int main(int argc, char *argv[]) 
{
#if 1
    coredump_catch_ex("test_coredump");

    //DebugBreak();

    /*__asm {
        INT 3;
    }*/

    *(int *)0 = 0;
#else 

    __try {
        DebugBreak();
    }
    __except (except_filter(GetExceptionInformation())) {
        printf("exception handle code...\n");
    }
	
#endif 

	return 0;
}