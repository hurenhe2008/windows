#include <stdio.h>
#include <Windows.h>
#include <tchar.h>

void inject_with_thread();
void inject_with_apc();
void inject_with_winhook();
void inject_with_eventhook();

int main(void)
{
    inject_with_thread();

    //inject_with_apc();

    //inject_with_winhook();

    //inject_with_eventhook();

    return 0;
}

void inject_with_thread()
{
    STARTUPINFO si = { 0 };
    si.cb = sizeof si;
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_SHOW;
    PROCESS_INFORMATION pi;

    BOOL bRet = ::CreateProcess(
        _T("test_virtual_inherit_class.exe"),
        NULL,
        NULL,
        NULL,
        FALSE,
        CREATE_SUSPENDED, //需要注意的参数 
        NULL,
        NULL,
        &si,
        &pi);

    char szDllPath[MAX_PATH];
    strcpy_s(szDllPath, MAX_PATH, "inject_dll.dll");

    LPVOID pszDllName =
        ::VirtualAllocEx(
            pi.hProcess, //远程进程句柄 
            NULL,                                  //建议开始地址 
            4096,                                  //分配空间大小 
            MEM_COMMIT,                            //空间初始化全0 
            PAGE_EXECUTE_READWRITE);               //空间权限 

    bRet = ::WriteProcessMemory(pi.hProcess, pszDllName,
        szDllPath, MAX_PATH, NULL);

    HANDLE m_hInjecthread = ::CreateRemoteThread(pi.hProcess,      //远程进程句柄
        NULL,                                                     //安全属性
        0,                                                        //栈大小
        (LPTHREAD_START_ROUTINE)LoadLibraryA,                     //进程处理函数   
        pszDllName,                                               //传入参数
        CREATE_SUSPENDED,                                         //默认创建后的状态
        NULL);                                                    //线程ID

    DWORD dwRet = ResumeThread(pi.hThread);
    dwRet = ResumeThread(m_hInjecthread);

    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    CloseHandle(m_hInjecthread);

    MessageBoxA(0, "程序启动完毕！", "提示", MB_ICONINFORMATION | MB_OK);
}

void inject_with_apc()
{
    STARTUPINFO si = { 0 };
    si.cb = sizeof si;
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_SHOW;
    PROCESS_INFORMATION pi;

    BOOL bRet = ::CreateProcess(
        _T("test_virtual_inherit_class.exe"),
        NULL,
        NULL,
        NULL,
        FALSE,
        CREATE_SUSPENDED, //需要注意的参数 
        NULL,
        NULL,
        &si,
        &pi);

    char szDllPath[MAX_PATH];
    strcpy_s(szDllPath, MAX_PATH, "inject_dll.dll");

    LPVOID pszDllName =
        ::VirtualAllocEx(
            pi.hProcess, //远程进程句柄 
            NULL,                                  //建议开始地址 
            4096,                                  //分配空间大小 (PAGE)
            MEM_COMMIT,                            //空间初始化全0 
            PAGE_EXECUTE_READWRITE);               //空间权限 

    bRet = ::WriteProcessMemory(pi.hProcess, pszDllName,
        szDllPath, MAX_PATH, NULL);

    DWORD dwRet =
        ::QueueUserAPC((PAPCFUNC)LoadLibraryA, pi.hThread, (ULONG_PTR)pszDllName);

    dwRet = ResumeThread(pi.hThread);

    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);

    system("pause");
    MessageBoxA(0, "程序启动完毕！", "提示", MB_ICONINFORMATION | MB_OK);
}

void inject_with_winhook()
{
    //::SetWindowsHookEx();
}

void inject_with_eventhook()
{
    //::SetWinEventHook();
}
