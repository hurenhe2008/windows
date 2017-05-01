#include "heap_alloc.h"

unsigned CHeapAlloc::ms_alloc_count = 0;
HANDLE CHeapAlloc::ms_heap_handle = NULL;

CHeapAlloc::CHeapAlloc()
{
    
}

CHeapAlloc::~CHeapAlloc()
{
    uninit();
}

void CHeapAlloc::init()
{
    if (!ms_heap_handle) {
        //HEAP_GENERATE_EXCEPTIONS
        ms_heap_handle = ::HeapCreate(0, 1024 * 1024, 0);
    }

    if (!ms_heap_handle) {
        throw heap_error();
    }
}

void CHeapAlloc::uninit()
{
    if (!ms_heap_handle) {
        return;
    }

    if (ms_alloc_count > 0) {
        //some heap not free.
    }

    if (!::HeapDestroy(ms_heap_handle)) {
        throw heap_error();
    }

    ms_heap_handle = NULL;
}

void* CHeapAlloc::allocate(size_t size)
{
    if (!ms_heap_handle) {
        init(); // throw exception if fail
    }

    //HeapLock in HeapAlloc.
    void *tmp = ::HeapAlloc(ms_heap_handle, HEAP_ZERO_MEMORY, size);

    if (nullptr == tmp) {
        throw heap_error();
    }

    ++ms_alloc_count;
    return tmp;
}

void CHeapAlloc::deallocate(void *pdata, size_t size)
{
    if (!ms_heap_handle || !pdata) {
        return;
    }

    //HeapUnlock in HeapFree.
    if (!::HeapFree(ms_heap_handle, 0, pdata)) {
        throw heap_error();
    }

    --ms_alloc_count;

    if (ms_alloc_count <= 0) {
        //destroy heap ?
    }
}

