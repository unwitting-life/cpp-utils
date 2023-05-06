#pragma once
#pragma message ("* ********************************************************************************* *")
#pragma message ("* URL    : https://github.com/unwitting-life/cpp-utils                              *")
#pragma message ("* License: https://creativecommons.org/licenses/by-nc/4.0/                          *")
#pragma message ("* Author : Yuanlei Huang                                                            *")
#pragma message ("* Note   : This program only support with ISO C++20                                 *")
#pragma message ("* Lastest: 2023/05/05                                                               *")
#pragma message ("* ********************************************************************************* *")

#ifdef _MSC_VER
#ifndef WIN32
#define WIN32
#endif
#endif

#ifdef UNICODE
#define string_t _wstring
#define stringstream_t std::wstringstream
#define ofstream_t std::wofstream
#else
#define _w _string
#define stringstream_t std::stringstream
#define ofstream_t std::ofstream
#endif

#ifdef WIN32
#define UNC_W LR"(\\)"
#define UNC_A R"(\\)"
#define PATH_SEPARATOR_W L'\\'
#define PATH_SEPARATOR_STRING_W L"\\"
#define PATH_SEPARATOR_A '\\'
#define PATH_SEPARATOR_STRING_A "\\"

#ifdef UNICODE
#define UNC UNC_W
#define PATH_SEPARATOR PATH_SEPARATOR_W
#define PATH_SEPARATOR_STRING PATH_SEPARATOR_STRING_W
#else
#define UNC UNC_A
#define PATH_SEPARATOR PATH_SEPARATOR_A
#define PATH_SEPARATOR_STRING PATH_SEPARATOR_STRING_A
#endif
#else
#define PATH_SEPARATOR _T('/')
#define PATH_SEPARATOR_STRING _T("\\")
#endif

#ifdef WIN32
#include <WinSock2.h>
#define _WINSOCKAPI_    // stops windows.h including winsock.h
#include <windows.h>
#endif

// https://blog.csdn.net/panjunnn/article/details/115750622
#ifdef _HAS_STD_BYTE
#undef _HAS_STD_BYTE
#endif
#define _HAS_STD_BYTE 0

#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <sys/stat.h>
#include <sstream>
#include <codecvt>
#include <unordered_map>
#include <thread>
#include <mutex>
#include <regex>
#include <functional>
#include "easy-encryption/encrypt.h"
#include "base64/base64.hpp"
#include "curl/include/curl/curl.h"

#ifdef WIN32
#include <iostream>
#include <stdio.h>
#include <conio.h>
#include <iostream>
#include <direct.h>
#include <io.h>
#include <shlobj_core.h>
#include <Shlwapi.h>
#include <comutil.h>  
#include <tchar.h>
#include <wincrypt.h>
#include <Guiddef.h>
#pragma comment(lib, "comsuppw.lib")
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, __FILE__ "/../curl/builds/libcurl-vc-x64-release-dll-ipv6-sspi-schannel/lib/libcurl.lib")
#else
#define L
#define _T(x) x
#define TCHAR char
#define _countof(x) (sizeof(x) / sizeof(x[0]))
#endif

#if 0
namespace utils {
    namespace strings {
        constexpr auto CR = _T("\r");
        constexpr auto LF = _T("\n");
        inline std::wstring s2w(std::string);
    }

    namespace io {
        namespace path {
#ifdef WIN32
            inline string_t GetFileName(string_t);
#endif
        }

        namespace directory {
            inline string_t mkdir(const string_t);
            inline bool exists(string_t);
            inline uintmax_t remove(string_t path, bool recursive = true);
        }

        namespace file {
            inline bool existsW(std::wstring);
            inline bool existsA(std::string);
            inline bool exists(string_t);
            inline bool exists(string_t);
            inline bool remove(string_t);
        }
    }

    namespace regex {
        inline std::wstring GetPathPatternW(std::wstring);
        inline std::string GetPathPatternA(std::string);
        inline string_t GetPathPattern(string_t);
    }

    namespace log {
#ifdef WIN32
        inline void write(string_t);
#endif
    }
}
#endif