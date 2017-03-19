#include "inifile.h"

#include <windows.h>
#include <io.h>
#include <strsafe.h>
#include <stdlib.h>


IniFile::IniFile()
{

}

IniFile::~IniFile()
{

}

bool IniFile::setIniPath(const char *inipath)
{
    int len = strlen(inipath);
    if (!inipath || len > (sizeof(m_inipath)-1)) {
        return false;
    }

    memset(m_inipath, 0, sizeof m_inipath);
    strcpy_s(m_inipath, len, inipath);
    /*0: file exit  2:write access  4:read access  6:execute acess*/
    return (-1 == _access(m_inipath, 0)) ? false : true;
}

bool IniFile::setIniPath(const char *inifile, bool bappdata)
{
    if (!inifile || strlen(inifile) > (sizeof(m_inipath)-1)) {
        return false;
    }

    if (!getIniDir(bappdata)) {
        return false;
    }

    if (bappdata) {
        strcat_s(m_inipath, sizeof m_inipath, "\\MPRInStudy");
    }

    strcat_s(m_inipath, sizeof m_inipath, "\\");
    strcat_s(m_inipath, sizeof m_inipath, inifile);
    /*0: file exit  2:write access  4:read access  6:execute acess*/
    return (-1 == _access(m_inipath, 0)) ? false : true;
}

bool IniFile::setSection(const char *section)
{
    int len = strlen(section);

    if (!section || len >= sizeof(m_section)) {
        return false;
    }

    strcpy_s(m_section, sizeof m_section, section);
    m_section[len] = '\0';
    return true;
}

bool IniFile::getIniDir(bool bappdata)
{
    memset(m_inipath, 0, sizeof m_inipath);

    if (bappdata) {
        unsigned need_size = 0;
        if (0 != getenv_s(&need_size, m_inipath, sizeof m_inipath, "APPDATA")) {
            return false;
        }
    }
    else {
        ::GetModuleFileNameA(NULL, m_inipath, sizeof m_inipath);
        char *ppos = strrchr(m_inipath, '\\');
        *ppos = '\0';
    }
    return true;
}

bool IniFile::getValue(const char *key, int *intvalue)
{
    if (!key || !intvalue) return false;

    *intvalue = 0;

    *intvalue = ::GetPrivateProfileIntA(m_section, key, 0, m_inipath);

    return true;
}

bool IniFile::getValue(const char *key, char *buf, int bufsize)
{
    if (!key || !buf) return false;

    memset(buf, 0, bufsize);

    int ret = ::GetPrivateProfileStringA(m_section, key, "", buf, bufsize,
        m_inipath);

    return (ret <= 0 || strlen(buf) == 0) ? false : true;
}

bool IniFile::setValue(const char *key, int intvalue)
{
    if (!key) return false;

    char tmpbuf[16];
    StringCbPrintfA(tmpbuf, sizeof tmpbuf, "%d", intvalue);

    BOOL bret = WritePrivateProfileStringA(m_section, key, tmpbuf, m_inipath);

    return bret ? true : false;
}

bool IniFile::setValue(const char *key, const char *buf)
{
    if (!key || !buf) return false;

    BOOL bret = WritePrivateProfileStringA(m_section, key, buf, m_inipath);

    return bret ? true : false;
}