#ifndef _SINGLETON_H_
#define _SINGLETON_H_

/*
@func:
    app_exist()    //inner call app_exist_ex
@param:
    no parameter
@return:
    if the application has run, return true;
    otherwise return false.
*/
bool app_exist();

/*
@func:
    app_exist()    
@param:
    appname - the application name which identify the app exe.
@return:
    if the application has run, return true;
    otherwise return false.
*/
bool app_exist_ex(const char *appname);

#endif //_SINGLETON_H_