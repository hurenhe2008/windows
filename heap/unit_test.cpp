#include "heap_alloc.h"
#include <stdio.h>
#include <stdlib.h>

class HeapTest 
{
public:
    HeapTest() :m(1), n(1) {}

    HeapTest(int _m, int _n) : m(_m), n(_n) {}

    virtual ~HeapTest()
    {
        m = 0;
        n = 0;
    }

    virtual void show() 
    {
        printf("m:%d, n:%d\n", m, n);
    }

public:
    static void* operator new(size_t size);
    static void* operator new[](size_t size);
    static void operator delete(void *pdata);
    static void operator delete[](void *pdata);

private:
    int m;
    int n;
};

void* HeapTest::operator new(size_t size)
{
    return CHeapAlloc::instance().allocate(size);
}

void* HeapTest::operator new[](size_t size)
{
    return CHeapAlloc::instance().allocate(size);
}

void HeapTest::operator delete(void *pdata)
{
    CHeapAlloc::instance().deallocate(pdata);
}

void HeapTest::operator delete[](void *pdata)
{
    CHeapAlloc::instance().deallocate(pdata);
}

int main(int argc, const char *argv[])
{
    HeapTest *p = new HeapTest(10, 5);
    p->show();
    delete p;

    HeapTest *parr = new HeapTest[10]();
    for (unsigned i = 0; i < 10; ++i) {
        (parr + i)->show();
    }
    delete[] parr;


#define TEST_STRING "hello, world!"
    char *str = (char *)CHeapAlloc::instance().allocate(100);
    unsigned len = strlen(TEST_STRING);
    memcpy_s(str, 99, TEST_STRING, len);
    str[len] = '\0';

    printf("string: %s\n", str);
    CHeapAlloc::instance().deallocate((void *)str);

    CHeapAlloc::instance().destroy();

    getchar();
    return 0;
}