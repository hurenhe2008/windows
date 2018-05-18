#include <stdio.h>  
#include <locale.h>  
#include <Shlobj.h> 

typedef struct csidl_s
{
    int nkey;
    const char *pdesc;
} csidl_t;

static const csidl_t s_csidl[] =
{
    { CSIDL_ADMINTOOLS, "CSIDL_ADMINTOOLS" },
    { CSIDL_APPDATA, "CSIDL_APPDATA" },
    { CSIDL_CDBURN_AREA, "CSIDL_CDBURN_AREA" },
    { CSIDL_COMMON_ADMINTOOLS, "CSIDL_COMMON_ADMINTOOLS" },
    { CSIDL_COMMON_APPDATA, "CSIDL_COMMON_APPDATA" },
    { CSIDL_COMMON_DESKTOPDIRECTORY, "CSIDL_COMMON_DESKTOPDIRECTORY" },
    { CSIDL_COMMON_DOCUMENTS, "CSIDL_COMMON_DOCUMENTS"},
    { CSIDL_COMMON_FAVORITES, "CSIDL_COMMON_FAVORITES" },
    { CSIDL_COMMON_MUSIC, "CSIDL_COMMON_MUSIC" },
    { CSIDL_COMMON_PICTURES, "CSIDL_COMMON_PICTURES" },
    { CSIDL_COMMON_PROGRAMS, "CSIDL_COMMON_PROGRAMS" },
    { CSIDL_COMMON_STARTMENU, "CSIDL_COMMON_STARTMENU" },
    { CSIDL_COMMON_STARTUP, "CSIDL_COMMON_STARTUP" },
    { CSIDL_COMMON_TEMPLATES, "CSIDL_COMMON_TEMPLATES" },
    { CSIDL_COMMON_VIDEO, "CSIDL_COMMON_VIDEO" },
    { CSIDL_COOKIES, "CSIDL_COOKIES" },
    { CSIDL_FAVORITES, "CSIDL_FAVORITES" },
    { CSIDL_HISTORY, "CSIDL_HISTORY" },
    { CSIDL_INTERNET_CACHE, "CSIDL_INTERNET_CACHE" },
    { CSIDL_LOCAL_APPDATA, "CSIDL_LOCAL_APPDATA" },
    { CSIDL_MYDOCUMENTS, "CSIDL_MYDOCUMENTS" },
    { CSIDL_MYMUSIC, "CSIDL_MYMUSIC" },
    { CSIDL_MYPICTURES, "CSIDL_MYPICTURES" },
    { CSIDL_MYVIDEO, "CSIDL_MYVIDEO" },
    { CSIDL_PROFILE, "CSIDL_PROFILE" },
    { CSIDL_PROGRAM_FILES, "CSIDL_PROGRAM_FILES" },
    { CSIDL_PROGRAM_FILES_COMMON, "CSIDL_PROGRAM_FILES_COMMON" },
    { CSIDL_PROGRAMS, "CSIDL_PROGRAMS" },
    { CSIDL_RECENT, "CSIDL_RECENT" },
    { CSIDL_SENDTO, "CSIDL_SENDTO" },
    { CSIDL_STARTMENU, "CSIDL_STARTMENU" },
    { CSIDL_STARTUP, "CSIDL_STARTUP" },
    { CSIDL_SYSTEM, "CSIDL_SYSTEM" },
    { CSIDL_TEMPLATES, "CSIDL_TEMPLATES" },
    { CSIDL_WINDOWS, "CSIDL_WINDOWS" },
    { 0, nullptr }
};

int main()
{
    setlocale(LC_ALL, "chs");

    char szPath[MAX_PATH + 1] = { 0 };

    for (unsigned i = 0; s_csidl[i].nkey; ++i)
    {
        if (SUCCEEDED(SHGetFolderPathA(NULL, s_csidl[i].nkey, NULL, SHGFP_TYPE_CURRENT, szPath)))
        {
            printf("%s:   %s\n", s_csidl[i].pdesc, szPath);
        }
    }

    system("pause");
    return 0;
}