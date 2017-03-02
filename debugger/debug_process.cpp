#include <stdio.h>
#include <Windows.h>
//#include <debugapi.h>  

void api_usage()
{
    //DebugBreak();           //INT 3;  ret;
    //OutputDebugString();    //output string to debugger
}

int main(int argc, const char *argv[])
{
    int cnt = 0;

    printf("debugged process start!\n");

    while (cnt < 10) {

        printf("call OutputDebugString\n");
        OutputDebugString("call OutputDebugString!");
        Sleep(1000);

        //printf("call DebugBreak\n");
        //DebugBreak();   //INT 3; ret;
        //Sleep(1000);

        ++cnt;
    }

    printf("debugged process exit!\n");

    return 0;
}