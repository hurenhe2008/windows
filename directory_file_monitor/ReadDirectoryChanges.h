/*
function declare:
https://msdn.microsoft.com/en-us/library/aa365465(v=vs.85).aspx


BOOL WINAPI ReadDirectoryChangesW(
  _In_        HANDLE                          hDirectory,                 //directory handle.
  _Out_       LPVOID                          lpBuffer,                   //
  _In_        DWORD                           nBufferLength,
  _In_        BOOL                            bWatchSubtree,              //monitor sub dir tree.
  _In_        DWORD                           dwNotifyFilter,             //The filter conditions that satisfy a change notification wait.  
  _Out_opt_   LPDWORD                         lpBytesReturned,            //synchronous calls return size,  asynchronous call undefined.
  _Inout_opt_ LPOVERLAPPED                    lpOverlapped,               //asynchronous operation  OVERLAPPED, synchronous calls NULL
  _In_opt_    LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine         //callback function.   //https://msdn.microsoft.com/en-us/library/aa364052(v=vs.85).aspx
);


VOID CALLBACK FileIOCompletionRoutine(
  _In_    DWORD        dwErrorCode,
  _In_    DWORD        dwNumberOfBytesTransfered,
  _Inout_ LPOVERLAPPED lpOverlapped
);

typedef VOID (WINAPI *LPOVERLAPPED_COMPLETION_ROUTINE)(
    _In_    DWORD        dwErrorCode,
    _In_    DWORD        dwNumberOfBytesTransfered,
    _Inout_ LPOVERLAPPED lpOverlapped
);

*/