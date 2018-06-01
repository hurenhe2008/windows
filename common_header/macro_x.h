#ifndef _MACRO_X_H_
#define _MACRO_X_H_

#include <stdio.h>

void test_func()
{
    printf("test_func: %p \n", &test_func);
}

void test_func1()
{
    printf("test_func1: %p \n", &test_func1);
}


#pragma push_macro("test_func")  //push macro

#undef test_func
#define test_func test_func1
void test_print()
{
    test_func();
}

#pragma pop_macro("test_func")    //pop macro




int main(void)
{
    test_print();  //will call test_func1

    return 0;
}


#endif //_MACRO_X_H_
