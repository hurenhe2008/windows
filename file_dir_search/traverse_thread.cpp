#include "traverse_thread.h"
#include <process.h>
#include <Windows.h>
#include <stdio.h>

static uintptr_t s_thread_handle = 0;
static bool volatile s_brun = false;

static unsigned __stdcall thread_proc(void  *data);
static unsigned long long traverse_folder(const char *folder);
static unsigned long long traverse_folder_norecursive(const char *folder);

bool traverse_start(traverse_data_t *data)
{
    s_brun = true;

    s_thread_handle = _beginthreadex(nullptr, 0, &thread_proc, 
        (void *)data, 0, nullptr);
    
    if (0 == s_thread_handle) {
        s_brun = false;
    }

    return s_brun;
}

bool traverse_stop()
{
    s_brun = false;

    DWORD timeout = 200;   /*ms*/

    if (s_thread_handle) {
        DWORD ret = WaitForSingleObject((HANDLE)s_thread_handle, timeout);

        if (WAIT_OBJECT_0 != ret) {
            s_brun = true;
        }
        else {
            CloseHandle((HANDLE)s_thread_handle);
        }
    }
    return !s_brun;
}

unsigned __stdcall thread_proc(void  *data)
{
    traverse_data_t *pinfo = (traverse_data_t *)data;

    unsigned long long size = traverse_folder_norecursive(pinfo->data);

    printf("query root directory: %s     %lldbyte\n", pinfo->data, size);

    return 0;
}

unsigned long long traverse_folder(const char *folder)
{
    char searchpath[MAX_PATH + 1] = { 0 };
    WIN32_FIND_DATAA fdata;
    unsigned long long sizeoffiles = 0;

    strcat_s(searchpath, MAX_PATH, folder);
    strcat_s(searchpath, MAX_PATH, "\\*.*");

    HANDLE hfind = ::FindFirstFileA(searchpath, &fdata);
    if (INVALID_HANDLE_VALUE == hfind) {
        return sizeoffiles;
    }

    while(s_brun) {
        if (fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            if (0 != strcmp(fdata.cFileName, ".") &&
                0 != strcmp(fdata.cFileName, "..")) {

                char *newfolder = searchpath;
                memset(newfolder, 0, MAX_PATH);

                strcat_s(newfolder, MAX_PATH, folder);
                strcat_s(newfolder, MAX_PATH, "\\");
                strcat_s(newfolder, MAX_PATH, fdata.cFileName);

                unsigned long long size = traverse_folder(newfolder);
                printf("directory: %s     %lldbyte\n", newfolder, size);
                sizeoffiles = sizeoffiles + size;
            }
            else {
                /*not hanlde '.' and '..' directory*/
            }
        }
        else {
            sizeoffiles = sizeoffiles +
                ((unsigned long long)fdata.nFileSizeHigh << 32) + fdata.nFileSizeLow;
            printf("file: %s\\%s\n", folder, fdata.cFileName);
        }

        if (!::FindNextFileA(hfind, &fdata)) {
            break;
        }
    }

    ::FindClose(hfind);
    return sizeoffiles;
}

/*simulate the stack FILO effective function*/

typedef struct stack_data_s {
    unsigned long long cursize;
    char curfolder[MAX_PATH + 1];
    HANDLE hfind;  
} stack_data_t;

#include <stack>

unsigned long long traverse_folder_norecursive(const char *folder)
{
    char searchpath[MAX_PATH + 1] = { 0 };
    unsigned long long size = 0;
    int state = 0;
    WIN32_FIND_DATAA fdata;
    std::stack<stack_data_t> stack;

    stack_data_t xdata;
    xdata.hfind = INVALID_HANDLE_VALUE;
    xdata.cursize = 0;
    memcpy_s(xdata.curfolder, MAX_PATH, folder, MAX_PATH);
    
    stack.push(xdata);

    while (s_brun && !stack.empty()) {

        if (0 == state) {   /*start*/
            stack_data_t &data = stack.top();

            memset(searchpath, 0, MAX_PATH);
            strcat_s(searchpath, MAX_PATH, data.curfolder);
            strcat_s(searchpath, MAX_PATH, "\\*.*");

            fdata = { 0 };
            data.hfind = ::FindFirstFileA(searchpath, &fdata);
            if (INVALID_HANDLE_VALUE == data.hfind) {
                stack.pop();
                state = 4;
            }
            else {
                state = 1;
            }
        }

        if (1 == state) {
            if (fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                if (0 != strcmp(fdata.cFileName, ".") &&
                    0 != strcmp(fdata.cFileName, "..")) {
                    state = 2;
                }
                else {
                    state = 4;
                }
            }
            else {
                state = 3;
            }
        }

        if (2 == state) {   /*directory*/
            stack_data_t data = { 0 };
            data.hfind = INVALID_HANDLE_VALUE;

            strcat_s(data.curfolder, MAX_PATH, stack.top().curfolder);
            strcat_s(data.curfolder, MAX_PATH, "\\");
            strcat_s(data.curfolder, MAX_PATH, fdata.cFileName);

            stack.push(data);

            state = 0;
            continue;
        }

        if (3 == state) {   /*file*/

            stack.top().cursize = stack.top().cursize
                + ((unsigned long long)fdata.nFileSizeHigh << 32)
                + fdata.nFileSizeLow;
            printf("file: %s\\%s\n", stack.top().curfolder, fdata.cFileName);

            state = 4;
        }

        if (4 == state) {   /*next read*/
            if (!stack.empty()) {
                stack_data_t &data = stack.top();
                fdata = { 0 }; 
                if (!::FindNextFileA(data.hfind, &fdata)) {

                    size = data.cursize;
                    ::FindClose(data.hfind);
                    printf("directory: %s     %lldbyte\n", data.curfolder, data.cursize);
                    stack.pop();

                    if (!stack.empty()) {
                        /*size is files'size under directory, must add to parent directory*/
                        stack.top().cursize += size;
                    }
                    state = 4;
                }
                else {
                    state = 1; /*handle file or directory*/
                }
            }   
        }
    }

    return size;
}