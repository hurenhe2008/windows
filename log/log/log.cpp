#include "log.h"
#include <io.h>
#include <stdio.h>
#include <stdarg.h>
#include <windows.h>
#include <ShlObj.h>
#include <log4cpp/PropertyConfigurator.hh>
#include <log4cpp/Category.hh>


#define LOG_MSG_MAX_LEN        2048 
#define LOG_TEMPL_CONF_FILE    "log.property"
#define LOG_TEMPL_MOD_FLAG     ".fileName="
#define log_debug              OutputDebugStringA

static char s_log_msg[LOG_MSG_MAX_LEN + 1] = { 0 }; /*log msg buff*/
static char s_log_dir[MAX_PATH] = { 0 };            /*log dir*/
static char s_log_conf_file[MAX_PATH] = { 0 };      /*log configure file path*/

static bool gen_log_dir(const char *appname, bool bappdata);
static bool get_conf_file(const char *appname);
static bool set_log4_conf(const char *confile);
static bool get_appdata_path(char *path, unsigned size);
static bool get_exedir_path(char *path, unsigned size);
static bool gen_log_dir(char *parentpath, unsigned size, const char *dir);
static bool gen_conf_file(const char *filepath);


bool init_log(const char *appname, bool bappdata)
{
    if (!appname) return false;

    if (!gen_log_dir(appname, bappdata)) {
        return false;
    }

    if (!get_conf_file(appname)) {
        return false;
    }

    return set_log4_conf(s_log_conf_file);
}

bool log(log_file_t type, log_level_t level, const char *msg, ...)
{
    int ret = 0;
    va_list pl = nullptr;
    const char *pfile = nullptr;

    va_start(pl, msg);
    ret = vsnprintf_s(s_log_msg, LOG_MSG_MAX_LEN, _TRUNCATE, msg, pl);
    va_end(pl);

    if (ret < 0) {
        /* s_log_msg buffer too small, truncated. */
    }

    switch (type) {
    case Log_Main:
        pfile = "Log_Main";
        break;

    case Log_Network:
        pfile = "Log_Net";
        break;

    case Log_ThirdPart:
        pfile = "Log_ThirdPart";
        break;

    default:
        pfile = "Log_Main";
    }

    switch (level)
    {
    case Info:
        log4cpp::Category::getInstance(pfile).info(s_log_msg);
        break;

    case Debug:
        log4cpp::Category::getInstance(pfile).debug(s_log_msg);
        break;

    case Warn:
        log4cpp::Category::getInstance(pfile).warn(s_log_msg);
        break;

    case Error:
        log4cpp::Category::getInstance(pfile).error(s_log_msg);
        break;

    case Fatal:
        log4cpp::Category::getInstance(pfile).fatal(s_log_msg);
        break;

    default:
        log4cpp::Category::getInstance(pfile).debug(s_log_msg);
        break;
    }

    return true;
}

bool gen_log_dir(const char *appname, bool bappdata)
{
    if (!appname) return false;

    if (bappdata) {  /* get log dir from APPDATA */
        if (!get_appdata_path(s_log_dir, MAX_PATH)) {
            log_debug("can't get APPDATA dir!");
            return false;
        }
    }
    else { /* get log dir from exe's parent dir */
        if (!get_exedir_path(s_log_dir, MAX_PATH)) {
            log_debug("can't get exe dir!");
            return false;
        }  
    }

    /* _access mode [0: exist   2: write  4: read  6:rdwr] */
    if (!bappdata && -1 == _access(s_log_dir, 2)) {
        log_debug("log dir doesn't have write access:");
        log_debug(s_log_dir);

        log_debug("try to get APPDATA dir for instead!");
        if (!get_appdata_path(s_log_dir, MAX_PATH)) {
            log_debug("can't get APPDATA dir!");
            return false;
        }
    }

    if (!gen_log_dir(s_log_dir, MAX_PATH, "log") ||
        !gen_log_dir(s_log_dir, MAX_PATH, appname)) {
        return false;
    }

    return true;
}

bool get_conf_file(const char *appname)
{
    bool bret = false;

    if (!appname)  return false;

    memset(s_log_conf_file, 0, MAX_PATH);

    strcpy_s(s_log_conf_file, MAX_PATH, s_log_dir);
    strcat_s(s_log_conf_file, MAX_PATH, "\\");
    strcat_s(s_log_conf_file, MAX_PATH, appname);
    strcat_s(s_log_conf_file, MAX_PATH, ".property");

    if (-1 != _access(s_log_conf_file, 0)) { /* exist */
        log_debug("log config file already exists:");     
        log_debug(s_log_conf_file); 
        return true;
    }

    if (!gen_conf_file(s_log_conf_file)) {
        log_debug("generate log config file fail:");
        log_debug(s_log_conf_file);
        return false;
    }

    return true;
}

bool set_log4_conf(const char *confile)
{
    bool bret = true;

    try {
        log4cpp::PropertyConfigurator::configure(confile);
    }
    catch (log4cpp::ConfigureFailure &e) {
        log_debug("load log confile file failed. [reason]:");
        log_debug(e.what());
        bret = false;
    }

    return bret;
}

bool get_appdata_path(char *path, unsigned size)
{
    if (!path) return false;

    memset(path, 0, size);
 
    /* method one */
    unsigned need_size = 0;
    if (0 == getenv_s(&need_size, path, size, "APPDATA") &&
        need_size < size) {
        return true;        
    }

    /* method two */
    HRESULT hr = SHGetFolderPathA(NULL, CSIDL_COMMON_APPDATA, NULL, 0, path);

    return SUCCEEDED(hr) ? true : false;
}

bool get_exedir_path(char *path, unsigned size)
{
    if (!GetModuleFileNameA(NULL, path, size)) {
        log_debug("can't get exe parent dir!");
        return false;
    }

    char *pos = strrchr(path, '\\');
    if (!pos) {
        log_debug("can't get exe dir last flag \\!");
        return false;
    }

    *pos = '\0';   /* remove last string from last '\\' */

    return true;
}

bool gen_log_dir(char *parentpath, unsigned size, const char *dir)
{
    if (0 != strcat_s(parentpath, size, "\\") ||
        0 != strcat_s(parentpath, size, dir)) {
        log_debug("can't append log dir:");
        log_debug(parentpath);
        return false;
    }

    if (-1 == _access(parentpath, 0) &&
        !CreateDirectoryA(parentpath, NULL)) {
        log_debug("create log dir fail:");
        log_debug(parentpath);
        return false;
    }

    return true;
}

static bool gen_conf_file(const char *filepath)
{
    if (!filepath)  return false;
    
    bool bret = false;

    char log_templ_conf_file[MAX_PATH] = { 0 };
    if (!get_exedir_path(log_templ_conf_file, MAX_PATH)) {
        log_debug("can't get exe dir for template log configure file!");
        return bret;
    }

    if (0 != strcat_s(log_templ_conf_file, MAX_PATH, "\\") ||
        0 != strcat_s(log_templ_conf_file, MAX_PATH, LOG_TEMPL_CONF_FILE)) {
        log_debug("can't append log template config file path!");
        log_debug(log_templ_conf_file);
        return bret;
    }

    FILE *fp = nullptr, *fp_new = nullptr;
    if (!(fp = fopen(log_templ_conf_file, "r+")) ||
        !(fp_new = fopen(filepath, "w"))) {

        log_debug("can't open log template conf file:");
        log_debug(log_templ_conf_file);
        log_debug("or can't create app config file!");
        log_debug(filepath);

        goto _err;
    }

#define CONF_LINE_MAX_LEN   360   /* must > line max data len */
    char line_data[CONF_LINE_MAX_LEN] = { 0 };
    char path_data[MAX_PATH] = { 0 };

    do {
        if (!fgets(line_data, CONF_LINE_MAX_LEN, fp)) {
            if (feof(fp)) {   
                bret = true;  /* eof */
            }
            else {
                bret = false;  /* error: ferror(fp) */
            }
            break;   
        }

        char *flag_pos = strstr(line_data, LOG_TEMPL_MOD_FLAG);
        if (!flag_pos) {
            fputs(line_data, fp_new);
            continue;
        }

        int flag_len = strlen(LOG_TEMPL_MOD_FLAG);
        char *mod_pos = flag_pos + flag_len;
        if (0 != strcpy_s(path_data, MAX_PATH, s_log_dir) ||
            0 != strcat_s(path_data, MAX_PATH, "\\") ||
            0 != strcat_s(path_data, MAX_PATH, mod_pos)) {
            log_debug("modify log tmpl config file path failed!");
            bret = false;
            break;
        }
   
        if (0 != strcpy_s(mod_pos,
            CONF_LINE_MAX_LEN - (mod_pos - line_data + 1),
            path_data)) {
            log_debug("can't append log config file:");
            log_debug(line_data);
            log_debug(path_data);
            bret = false;
            break;
        }

        *(mod_pos + strlen(path_data)) = '\0';

        fputs(line_data, fp_new);

    } while (true);

_err:
    if (fp) fclose(fp);
    if (fp_new) fclose(fp_new);

    return bret;
}