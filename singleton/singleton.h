#ifndef _SINGLETON_H_
#define _SINGLETON_H_

/*
@func:
    app_exist()    
@param:
    no parameter
@return:
    if the application has run, return true;
    otherwise return false.
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
    if the application has run, return true;
    otherwise return false.
*/
bool app_exist_ex(const char *appname);

#endif //_SINGLETON_H_
