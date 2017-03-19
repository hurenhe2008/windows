#include <stdio.h>
#include <Windows.h>
#include <TlHelp32.h>    

typedef struct thread_data_s {
    char info[MAX_PATH];
} thread_data_t;

typedef int(__stdcall *FMsgBox)(HWND, LPCSTR, LPCSTR, UINT);

static unsigned get_remote_process_id(const char *name);
static HANDLE get_remote_process_handle(const char *name);
static unsigned __stdcall thread_proc(void *data);

static const int s_max_virtual_size = 1024 * 2;


int main(int argc, const char *argv[])
{
    HANDLE hproc = NULL, hremthread = NULL;
    thread_data_t tdata = { 0 };
    LPVOID pFunAddr = nullptr, pData = nullptr;

    printf("remote_thread_inject app start!\n");
    if (argc < 2) {
        printf("please input remote exist running process name!\n");
        system("pause");
        exit(-1);
    }

    printf("start inject remote thread to process(%s)!\n", argv[1]);
    strcpy_s(tdata.info, sizeof(tdata.info) - 1, argv[1]);

    hproc = get_remote_process_handle(argv[1]);
    if (NULL == hproc) {
        printf("get remote process(%s) handle fail!\n", argv[1]);
        exit(-1);
    }

    pFunAddr = VirtualAllocEx(hproc, NULL, s_max_virtual_size,
        MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    pData = VirtualAllocEx(hproc, NULL, s_max_virtual_size,
        MEM_COMMIT, PAGE_READWRITE);
    if (!pFunAddr || !pData) {
        printf("virtual allocate remote process memory failed!\n");
        goto _clean;
    }

    if (!WriteProcessMemory(hproc, pFunAddr, &thread_proc, 
        s_max_virtual_size, nullptr) ||
        !WriteProcessMemory(hproc, pData, &tdata, sizeof tdata, nullptr)) {
        printf("write remote process memory failed!\n");
        goto _clean;
    }

    hremthread = ::CreateRemoteThread(hproc, nullptr, 0, 
        (LPTHREAD_START_ROUTINE)pFunAddr, pData, 0, 0);
    if (NULL == hremthread) {
        printf("create remote thread error(%d)\n", GetLastError());
    }

_clean:
    if (hremthread) {
        CloseHandle(hremthread);
        hremthread = NULL;
    }

    if (pFunAddr) {
        if (!VirtualFreeEx(hproc, pFunAddr, s_max_virtual_size, MEM_RELEASE)) {
            printf("virtual free function failed!\n");
        }
    }

    if (pData) {
        if (!VirtualFreeEx(hproc, pData, s_max_virtual_size, MEM_RELEASE)) {
            printf("virtual free data failed!\n");
        }
    }

    printf("remote_thread_inject app exit!\n");
    getchar();

    return 0;
}

unsigned get_remote_process_id(const char *name)
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

HANDLE get_remote_process_handle(const char *name)
{
    unsigned pid = get_remote_process_id(name);

    if (!pid) {
        return NULL;
    }

    return OpenProcess(PROCESS_ALL_ACCESS, false, pid);
}

unsigned __stdcall thread_proc(void *data)
{
    thread_data_t *pdata = (thread_data_t *)(data);

    HINSTANCE hlns = ::LoadLibrary(TEXT("user32.dll"));

    FMsgBox pmsgbox = (FMsgBox)::GetProcAddress(hlns, "MessageBoxA");

    (*pmsgbox)(NULL, pdata->info, "remote thread inject title", MB_OK);

    ::FreeLibrary(hlns);

    printf("remote thread call: %s\n", pdata->info);

    return 0;
}
