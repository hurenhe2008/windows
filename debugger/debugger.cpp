#include <stdio.h>

#include <Windows.h>
#include <DbgHelp.h>   /*debug help api*/
#include <TlHelp32.h>  /*tool help api*/
#include <Psapi.h>     /*process api*/
//#include <debugapi.h>  /*debugger api*/

void api_usage()
{
    //WaitForDebugEvent();       //wait for debugged process debug event
    //ContinueDebugEvent();      //continue next deug event

    //CreateProcess(DEBUG_PROCESS | DEBUG_ONLY_THIS_PROCESS);   //start a debugged process
    //DebugActiveProcess();      //attach to a running process to debug
    //DebugActiveProcessStop();  //stop debug running process

    //DebugBreak();              // INT 3; ret;
    //DebugBreakProcess();       //debug break process

    //IsDebuggerPresent();       //check whether the process debugged
    //CheckRemoteDebuggerPresent();   //check whether the process debugger with remote debugger

    //OutputDebugString();       //output string to debugger

    //FatalExit();               //force to exit the debugged process
    //FatalAppExit();           

    //FlushInstructionCache();   //flush instruction cache

    //GetThreadContext();        //read thread context
    //GetThreadSelectorEntry();  //read thread selector entry
    //SetThreadContext();        //set thread context
    //ReadProcessMemory();       //read process memory address
    //WriteProcessMemory();      //write to process memory address
}

typedef struct debug_process_info_s {
    HANDLE hproc;
    unsigned pid;
} debug_process_info_t;

static debug_process_info_t debug_process = { 0 };

static bool attach_to_debug_process(const char *name);
static bool stop_attach_to_debug_process();
static unsigned get_debug_process_id(const char *name);
static bool create_debug_process(const char *name);
static int handle_debug_event(const DEBUG_EVENT *de);


int main(int argc, char *argv[])
{
    const char *processname = "test_win_process.exe";
    DEBUG_EVENT de = { 0 };

    printf("attach to debug process: %s\n", processname);
    if (!attach_to_debug_process(processname)) {
        printf("attach to debug process failed!");
        getchar();
        exit(-1);
    }

    while (true) {
        printf("waitfor debug event!\n");
        if (!WaitForDebugEvent(&de, INFINITE)) {
            printf("WaitForDebugEvent error:%d!\n", GetLastError());
            break;
        }

        if (0 == handle_debug_event(&de)) {
            break; /*debug process exit!*/
        }

        printf("continue debug event!\n");
        if (!ContinueDebugEvent(de.dwProcessId, de.dwThreadId, 
            DBG_CONTINUE)) {
            printf("ContinueDebugEvent error:%d\n", GetLastError());
            break;
        }
    }

    printf("stop attach to debug process!\n");
    if (!stop_attach_to_debug_process()) {
        printf("DebugActiveProcessStop error:%d\n", GetLastError());
    }

    printf("press any key to exit debugger!");
    getchar();

    return 0;
}

bool attach_to_debug_process(const char *name)
{
    debug_process.pid = get_debug_process_id(name);

    if (!debug_process.pid) {
        return create_debug_process(name);
    }

    debug_process.hproc = 
        OpenProcess(PROCESS_ALL_ACCESS, false, debug_process.pid);

    if (!debug_process.hproc ||
        !DebugActiveProcess(debug_process.pid)) {
        return false;
    }
    
    return true;
}

bool stop_attach_to_debug_process()
{
    if (!debug_process.pid) {
        return true;
    }

    return DebugActiveProcessStop(debug_process.pid) ? true : false;
}

unsigned get_debug_process_id(const char *name)
{
    unsigned pid = 0;
    BOOL bret = FALSE;
    PROCESSENTRY32 ps;

    HANDLE hsnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (INVALID_HANDLE_VALUE == hsnap) {
        return pid;
    }

    bret = Process32First(hsnap, &ps);
    while (bret) {

        if (0 == strcmp(ps.szExeFile, name)) {
            pid = ps.th32ProcessID;
            break;
        }

        printf("process name:%s\n", ps.szExeFile);
        bret = Process32Next(hsnap, &ps);
    }

    CloseHandle(hsnap);

    return pid;
}

bool create_debug_process(const char *name)
{
    STARTUPINFO si = { 0 };
    si.cb = sizeof si;
    PROCESS_INFORMATION pi = { 0 };

    if (!CreateProcess(name, nullptr, nullptr, nullptr, false,
        CREATE_DEFAULT_ERROR_MODE | DEBUG_ONLY_THIS_PROCESS | CREATE_NEW_CONSOLE,
        nullptr, nullptr, &si, &pi)) {
        return false;
    }

    debug_process.hproc = pi.hProcess;
    debug_process.pid = pi.dwProcessId;

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return true;
}

int handle_debug_event(const DEBUG_EVENT *de)
{
    int ret = 1;  /*debug process not exit!*/

    switch (de->dwDebugEventCode) {
    case CREATE_PROCESS_DEBUG_EVENT:
        printf("create process debug event\n");
        break;

    case CREATE_THREAD_DEBUG_EVENT:
        printf("create thread debug event\n");
        break;

    case EXCEPTION_DEBUG_EVENT:
        printf("exception debug event: %lu\n", 
            de->u.Exception.ExceptionRecord.ExceptionCode);
        break;

    case EXIT_PROCESS_DEBUG_EVENT:
        ret = 0;   /*debug process exit!*/
        printf("exit process debug event\n");
        break;

    case EXIT_THREAD_DEBUG_EVENT:
        printf("exit thread debug event\n");
        break;

    case LOAD_DLL_DEBUG_EVENT:
        printf("load dll debug event\n");
        break;

    case OUTPUT_DEBUG_STRING_EVENT:
        {
            char tmp[1024] = { 0 };
            ReadProcessMemory(debug_process.hproc, 
                de->u.DebugString.lpDebugStringData,
                tmp, sizeof tmp - 1, nullptr);
            printf("output debug stirng event: %d - %s\n",
                de->u.DebugString.nDebugStringLength, tmp);
        }
        break;

    case RIP_EVENT:
        printf("rip debug event\n");
        break;

    case UNLOAD_DLL_DEBUG_EVENT:
        printf("unload dll debug event\n");
        break;
    }

    return ret;
}



