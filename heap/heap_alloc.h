#ifndef __HEAP_ALLOC_H__
#define __HEAP_ALLOC_H__

#include <Windows.h>
#include <new>
#include <mutex>

template<typename T>
class Singleton
{
public:
    static T& instance()
    {
        if (ms_ptr) {
            return *ms_ptr;
        }
        ms_mutex.lock();
        if (ms_ptr) {
            ms_mutex.unlock();
            return *ms_ptr;
        }
        T* tmp = new(std::nothrow) T();
        if (!tmp) {
            ms_mutex.unlock();
            throw std::bad_alloc();
        }
        ms_ptr = tmp;
        ms_mutex.unlock();
        return *ms_ptr;
    }

    static void destroy()
    {
        if (ms_ptr) {
            delete ms_ptr;
            ms_ptr = nullptr;
        }
    }

protected:
    Singleton() {}
    virtual ~Singleton() {}

private:
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

private:
    static T* ms_ptr;
    static std::mutex ms_mutex;
};

template<typename T>
T* Singleton<T>::ms_ptr = nullptr;

template<typename T>
std::mutex Singleton<T>::ms_mutex;



class heap_error : public std::exception
{
public:
    heap_error() throw() : 
        exception("heap call error", 1)
    {
    }
};


class CHeapAlloc : public Singleton<CHeapAlloc>
{
public:   
    friend Singleton<CHeapAlloc>;

    void* allocate(size_t size);

    void deallocate(void *pdata, size_t size = 0);

private:
    void init();

    void uninit();

protected:
    CHeapAlloc();
    virtual ~CHeapAlloc();

private:
    static unsigned ms_alloc_count;
    static HANDLE ms_heap_handle;
};


#endif //__HEAP_ALLOC_H__
