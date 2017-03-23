#include "singleton_app.h"
#include <windows.h>
#include <assert.h>

/* default application name */
#define DEF_APP_NAME "default_application_name"

/* method check singleton */
typedef enum singleton_method_e {
    singleton_method_mutex = 0,
    singleton_method_event,
    singleton_method_dataseg,
    singleton_method_totals
} singleton_method_t;

static singleton_method_t s_singleton_method = singleton_method_mutex;

static bool singleton_mutex_check(const char *name);

static bool singleton_event_check(const char *name);

static bool singleton_dataseg_check(/* use default name */);


bool app_exist()
{
    return app_exist_ex(DEF_APP_NAME);
}

bool app_exist_ex(const char *appname)
{
    switch (s_singleton_method) {
    case singleton_method_mutex:
        return singleton_mutex_check(appname);

    case singleton_method_event:
        return singleton_event_check(appname);

    case singleton_method_dataseg:
        return singleton_dataseg_check();

    case singleton_method_totals:
        abort();  return false;

    default:
        return singleton_event_check(appname);
    }
}

bool singleton_mutex_check(const char *name)
{
    bool bexist = false;

    CreateMutexA(NULL, FALSE, name);

    if (ERROR_ALREADY_EXISTS == GetLastError()) {
        bexist = true;
    }

    return bexist;
}

bool singleton_event_check(const char *name)
{
    bool bexist = false;

    CreateEventA(NULL, TRUE, FALSE, DEF_APP_NAME);

    if (ERROR_ALREADY_EXISTS == GetLastError()) {
        bexist = true;
    }

    return bexist;
}


#pragma data_seg(DEF_APP_NAME)
int g_app_run = 0;
#pragma data_seg()
#pragma comment(linker, "/section:"DEF_APP_NAME",RWS")

bool singleton_dataseg_check()
{
    g_app_run += 1;

    return (g_app_run > 1) ? true : false;
}
