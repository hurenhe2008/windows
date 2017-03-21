#ifndef _LOG_H_
#define _LOG_H_


#ifdef DLLEXPORT
    #define DLLAPI __declspec(dllexport)
#else 
    #define DLLAPI __declspec(dllimport)
#endif 

typedef enum log_file_e {
    Log_Main = 0,
    Log_Network,
    Log_ThirdPart
} log_file_t;

typedef enum log_level_e {
    Info = 0,
    Debug,
    Warn,
    Error,
    Fatal
} log_level_t;

/*
@func: 
    init_log()
@param:
    appname - the log output directory 
    bappdata - true means create log dir under APPDATA
               false means create log dir undex exe's dir.
@return:
    return true if init success.
    otherwise, return false.
*/
DLLAPI bool init_log(const char *appname, bool bappdata);

/*
@func:
    log()
@param:
    type - which file the log will write into by type.
    level - the log level to output.
    msg - the log message with format to write.
    ... - variable parameters to format msg
@return:
    return true if write log success.
    otherwise, return false.
*/
DLLAPI bool log(log_file_t type, log_level_t level, const char *msg, ...);


/* macro for quick log output */
#define log_main_debug(fmt, ...)   log(Log_Main, Debug, fmt, __VA_ARGS__)
#define log_main_info(fmt, ...)    log(Log_Main, Info,  fmt, __VA_ARGS__)
#define log_main_warn(fmt, ...)    log(Log_Main, Warn,  fmt, __VA_ARGS__)
#define log_main_error(fmt, ...)   log(Log_Main, Error, fmt, __VA_ARGS__)
#define log_main_fatal(fmt, ...)   log(Log_Main, Fatal, fmt, __VA_ARGS__)

#define log_net_debug(fmt, ...)   log(Log_Network, Debug, fmt, __VA_ARGS__)
#define log_net_info(fmt, ...)    log(Log_Network, Info,  fmt, __VA_ARGS__)
#define log_net_warn(fmt, ...)    log(Log_Network, Warn,  fmt, __VA_ARGS__)
#define log_net_error(fmt, ...)   log(Log_Network, Error, fmt, __VA_ARGS__)
#define log_net_fatal(fmt, ...)   log(Log_Network, Fatal, fmt, __VA_ARGS__)

#define log_lib_debug(fmt, ...)   log(Log_ThirdPart, Debug, fmt, __VA_ARGS__)
#define log_lib_info(fmt, ...)    log(Log_ThirdPart, Info,  fmt, __VA_ARGS__)
#define log_lib_warn(fmt, ...)    log(Log_ThirdPart, Warn,  fmt, __VA_ARGS__)
#define log_lib_error(fmt, ...)   log(Log_ThirdPart, Error, fmt, __VA_ARGS__)
#define log_lib_fatal(fmt, ...)   log(Log_ThirdPart, Fatal, fmt, __VA_ARGS__)


#endif //_LOG_H_