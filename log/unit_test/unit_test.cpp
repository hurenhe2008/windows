#include <stdio.h>
#include <stdlib.h>
#include "log.h"

#define APP_NAME  "test_log_lib"

int main(int argc, char *argv[])
{
    system("color f5");

    if (!init_log(APP_NAME, false)) {
        printf("init log failed!");
        abort();
    }

    unsigned i = 0;
    while (i++ < 5) {
        log_main_info("main info: %d", i);

        log_main_debug("main debug: %d", i);

        log_main_warn("main warn: %d", i);


        log_main_error("main error: %d", i);

        log_main_fatal("main fatal: %d", i);
    }

    i = 0;
    while (i++ < 5) {
        log_net_info("net info: %d", i);

        log_net_debug("net debug: %d", i);

        log_net_warn("net warn: %d", i);

        log_net_error("net error: %d", i);

        log_net_fatal("net fatal: %d", i);
    }

    i = 0;
    while (i++ < 5) {
        log_lib_info("lib info: %d", i);

        log_lib_debug("lib debug: %d", i);

        log_lib_warn("lib warn: %d", i);

        log_lib_error("lib error: %d", i);

        log_lib_fatal("lib fatal: %d", i);
    }
   
    system("pause");

    return 0;
}