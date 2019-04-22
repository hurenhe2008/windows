// c:\Program Files\Microsoft SDKs\Windows\v7.0A\Include\WinNls.h
#define CP_ACP                    0           // default to ANSI code page
#define CP_OEMCP                  1           // default to OEM  code page
#define CP_MACCP                  2           // default to MAC  code page
#define CP_THREAD_ACP             3           // current thread's ANSI code page
#define CP_SYMBOL                 42          // SYMBOL translations

#define CP_UTF7                   65000       // UTF-7 translation
#define CP_UTF8                   65001       // UTF-8 translation

// -- c:\Program Files\Microsoft SDKs\Windows\v7.0A\Include\WinNT.h
//
// UNICODE (Wide Character) types
//
#ifndef _MAC
typedef wchar_t WCHAR;    // wc,   16-bit UNICODE character
#else
// some Macintosh compilers don't define wchar_t in a convenient location, or define it as a char
typedef unsigned short WCHAR;    // wc,   16-bit UNICODE character
#endif

typedef WCHAR *PWCHAR, *LPWCH, *PWCH;
typedef CONST WCHAR *LPCWCH, *PCWCH;

typedef __nullterminated WCHAR *NWPSTR, *LPWSTR, *PWSTR;
......
typedef __nullterminated CONST WCHAR *LPCWSTR, *PCWSTR;
......

//
// ANSI (Multi-byte Character) types 
//
typedef CHAR *PCHAR, *LPCH, *PCH;
typedef CONST CHAR *LPCCH, *PCCH;

typedef __nullterminated CHAR *NPSTR, *LPSTR, *PSTR;
......
typedef __nullterminated CONST CHAR *LPCSTR, *PCSTR;
......


//
// 多字节(一般指GBK) utf8 Unicode 编码互转
/*
MultiByteToWideChar: 将MultiByte(多字节编码(CP_ACP)、GBK(CP_ACP)、UTF8(CP_UTF8))转换为WideChar(Unicode 编码)。
    如 MultiByteToWideChar(CP_ACP, 0, old_str, old_str_len, new_str, new_Len);  
    表示将CP_ACP类型的old_str转换为WideChar类型；CP_ACP标识 从哪种类型的MultiByte转换为WideChar。
WideCharToMultiByte: 将WideChar(Unicode 编码)转换为MultiByte(多字节编码(CP_ACP)、GBK(CP_ACP)、UTF8(CP_UTF8))。
    如 WideCharToMultiByte(CP_UTF8, 0, old_str, old_str_len, new_str, new_Len, NULL, NULL);  
    表示将WideChar类型的old_str转换为CP_UTF8类型的MultiByte。CP_ACP标识 WideChar要转换为哪种类型的MultiByte。
MultiByte1 转换为 MultiByte2 ，先将MultiByte1转为将WideChar，再从将WideChar转为MultiByte2。
*/

std::string Utf8ToGBK(const char* strUtf8)
{
    //不是utf8编码，返回原串
    unsigned char code_mark = strUtf8[0];
    if(code_mark <= 0xE0)
    {
        return strUtf8;
    }

    int len=MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)strUtf8, -1, NULL,0); 
    unsigned short * wszGBK = new unsigned short[len+1];       
    memset(wszGBK, 0, len * 2 + 2); 
    MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)strUtf8, -1, (LPWSTR)wszGBK, len);
    len = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)wszGBK, -1, NULL, 0, NULL, NULL);
    char *szGBK=new char[len + 1]; 
    memset(szGBK, 0, len + 1); 
    WideCharToMultiByte (CP_ACP, 0, (LPCWSTR)wszGBK, -1, (LPSTR)szGBK, len, NULL,NULL);
    std::string gbkString = szGBK;
    delete[] wszGBK;
    delete[] szGBK;
    return gbkString;
}

// 多字节编码转为UTF8编码  
bool MBToUTF8(vector<char>& pu8, const char* pmb, int32 mLen)  
{  
	// convert an MBCS string to widechar   
	int32 nLen = MultiByteToWideChar(CP_ACP, 0, pmb, mLen, NULL, 0);  

	WCHAR* lpszW = NULL;  
	try  
	{  
	lpszW = new WCHAR[nLen];  
	}  
	catch(bad_alloc &memExp)  
	{  
	return false;  
	}  

	int32 nRtn = MultiByteToWideChar(CP_ACP, 0, pmb, mLen, lpszW, nLen);  

	if(nRtn != nLen)  
	{  
	delete[] lpszW;  
	return false;  
	}  
	// convert an widechar string to utf8  
	int32 utf8Len = WideCharToMultiByte(CP_UTF8, 0, lpszW, nLen, NULL, 0, NULL, NULL);  
	if (utf8Len <= 0)  
	{  
	 return false;  
	}  
	pu8.resize(utf8Len);  
	nRtn = WideCharToMultiByte(CP_UTF8, 0, lpszW, nLen, &*pu8.begin(), utf8Len, NULL, NULL);  
	delete[] lpszW;  

	if (nRtn != utf8Len)  
	{  
	 pu8.clear();  
	 return false;  
	}  
	return true;  
}  
  


// UTF8编码转为多字节编码  
bool UTF8ToMB(vector<char>& pmb, const char* pu8, int32 utf8Len)  
{  
    // convert an UTF8 string to widechar   
    int32 nLen = MultiByteToWideChar(CP_UTF8, 0, pu8, utf8Len, NULL, 0);  
  
    WCHAR* lpszW = NULL;  
    try  
    {  
        lpszW = new WCHAR[nLen];  
    }  
    catch(bad_alloc &memExp)  
    {  
        return false;  
    }  
  
    int32 nRtn = MultiByteToWideChar(CP_UTF8, 0, pu8, utf8Len, lpszW, nLen);  
  
    if(nRtn != nLen)  
    {  
        delete[] lpszW;  
        return false;  
    }  
  
    // convert an widechar string to Multibyte   
    int32 MBLen = WideCharToMultiByte(CP_ACP, 0, lpszW, nLen, NULL, 0, NULL, NULL);  
    if (MBLen <=0)  
    {  
        return false;  
    }  
    pmb.resize(MBLen);  
    nRtn = WideCharToMultiByte(CP_ACP, 0, lpszW, nLen, &*pmb.begin(), MBLen, NULL, NULL);  
    delete[] lpszW;  
  
    if(nRtn != MBLen)  
    {  
        pmb.clear();  
        return false;  
    }  
    return true;  
}  
  
// 多字节编码转为Unicode编码  
bool MBToUnicode(vector<wchar_t>& pun, const char* pmb, int32 mLen)  
{  
    // convert an MBCS string to widechar   
    int32 uLen = MultiByteToWideChar(CP_ACP, 0, pmb, mLen, NULL, 0);  
  
    if (uLen<=0)  
    {  
        return false;  
    }  
    pun.resize(uLen);  
  
    int32 nRtn = MultiByteToWideChar(CP_ACP, 0, pmb, mLen, &*pun.begin(), uLen);  
  
    if (nRtn != uLen)  
    {  
        pun.clear();  
        return false;  
    }  
    return true;  
}  
  
//Unicode编码转为多字节编码  
bool UnicodeToMB(vector<char>& pmb, const wchar_t* pun, int32 uLen)  
{  
    // convert an widechar string to Multibyte   
    int32 MBLen = WideCharToMultiByte(CP_ACP, 0, pun, uLen, NULL, 0, NULL, NULL);  
    if (MBLen <=0)  
    {  
        return false;  
    }  
    pmb.resize(MBLen);  
    int nRtn = WideCharToMultiByte(CP_ACP, 0, pun, uLen, &*pmb.begin(), MBLen, NULL, NULL);  
  
    if(nRtn != MBLen)  
    {  
        pmb.clear();  
        return false;  
    }  
    return true;  
}  
  
// UTF8编码转为Unicode  
bool UTF8ToUnicode(vector<wchar_t>& pun, const char* pu8, int32 utf8Len)  
{  
    // convert an UTF8 string to widechar   
    int32 nLen = MultiByteToWideChar(CP_UTF8, 0, pu8, utf8Len, NULL, 0);  
    if (nLen <=0)  
    {  
        return false;  
    }  
    pun.resize(nLen);  
    int32 nRtn = MultiByteToWideChar(CP_UTF8, 0, pu8, utf8Len, &*pun.begin(), nLen);  
  
    if(nRtn != nLen)  
    {  
        pun.clear();  
        return false;  
    }  
  
    return true;  
}  
  
// Unicode编码转为UTF8  
bool UnicodeToUTF8(vector<char>& pu8, const wchar_t* pun, int32 uLen)  
{  
    // convert an widechar string to utf8  
    int32 utf8Len = WideCharToMultiByte(CP_UTF8, 0, pun, uLen, NULL, 0, NULL, NULL);  
    if (utf8Len<=0)  
    {  
        return false;  
    }  
    pu8.resize(utf8Len);  
    int32 nRtn = WideCharToMultiByte(CP_UTF8, 0, pun, uLen, &*pu8.begin(), utf8Len, NULL, NULL);  
  
    if (nRtn != utf8Len)  
    {  
        pu8.clear();  
        return false;  
    }  
    return true;  
}