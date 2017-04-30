#include <stdio.h>
#include <stdlib.h>

#include "singleton.h"
#include "singleton_tmpl.h"
#include "singleton_app.h"

#define APP_NAME "test_singleton"


class TestClass1 : public SingletonTmpl<TestClass1>
{
    /* nothing, just test */
};

class TestClass2 : public SingletonTmpl<TestClass2>
{
    /* nothing, just test */
};

int main(int argc, char *argv[])
{
    /* whether app already run */
    if (app_exist_ex(APP_NAME)) {
        printf("%s application has already run, exit!\n", APP_NAME);
        exit(0);
    }

    /* common singleton use */
    Singleton &test = Singleton::instance();

    /* some front endpoint use */
    SingletonF &ftest = SingletonF::instance();
    if (!ftest.init()) {
        printf("something error when init SingletonF!\n");
    }
    
    /* use template to accomplis singleton effect */
    TestClass1 &tmpltest1 = TestClass1::instance();
    TestClass2 &tmpltest2 = TestClass2::instance();

    return 0;
}
