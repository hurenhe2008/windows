#include <stdio.h>

#include "singleton.h"

#define APP_NAME "test_singleton"

int main(int argc, char *argv[])
{
    /*at first, check the app whether already run*/
    if (app_exist_ex(APP_NAME)) {
        printf("%s application has already run, exit!\n", APP_NAME);
        exit(0);
    }

    /*
    ... application real code ...
    */

    return 0;
}