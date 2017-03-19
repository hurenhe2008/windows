#ifndef _INIFILE_H_
#define _INIFILE_H_


#ifdef MAX_PATH
#define INI_PATH_LEN     MAX_PATH
#else 
#define INI_PATH_LEN     260
#endif

#define INI_SECTION_LEN  128


class IniFile {
public:
    IniFile();
    ~IniFile();

    bool setIniPath(const char *inipath);

    bool setIniPath(const char *inifile, bool bappdata);  /*APPDATA path*/

    bool setSection(const char *section);

    bool getValue(const char *key, int *intvalue);

    bool getValue(const char *key, char *buf, int bufsize);

    bool setValue(const char *key, int intvalue);

    bool setValue(const char *key, const char *buf);

private:
    IniFile(const IniFile &);
    IniFile & operator = (const IniFile &);

    bool getIniDir(bool bappdata);

private:
    char m_inipath[INI_PATH_LEN];
    char m_section[INI_SECTION_LEN];
};



#endif //_INIFILE_H_