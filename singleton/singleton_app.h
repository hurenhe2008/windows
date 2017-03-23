#ifndef _SINGLETON_APP_H_
#define _SINGLETON_APP_H_


#ifdef __cplusplus
extern "C" {
#endif 

#ifndef bool
#include <stdbool.h>
#endif 

    /*
    @func:
    app_exist()
    @param:
    no parameter
    @return:
    if the application has run, return 1;
    otherwise return 0.
    @note:
    app_exist_ex(DEF_APP_NAME) will called inner app_exist().
    */
    bool app_exist();

    /*
    @func:
    app_exist()
    @param:
    appname - the application name which identify the app exe.
    @return:
    if the application has run, return 1;
    otherwise return 0.
    */
    bool app_exist_ex(const char *appname);

#ifdef __cplusplus
}
#endif 

#endif //_SINGLETON_APP_H_

