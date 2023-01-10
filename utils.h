#pragma once

#ifdef _MSC_VER
#ifndef WIN32
#define WIN32
#endif

#define _WINSOCKAPI_    // stops windows.h including winsock.h
#include <windows.h>
#endif

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
#include "./easy-encryption/encrypt.h"
#include "./base64/base64.hpp"

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
#pragma comment(lib, "comsuppw.lib")
#pragma comment(lib, "shlwapi.lib")
#else
#define _T(x) x
#define TCHAR char
#define _countof(x) (sizeof(x) / sizeof(x[0]))
#endif

#ifdef UNICODE
#define UNC LR"(\\)"
#define string_t std::wstring
#define stringstream_t std::wstringstream
#else
#define UNC R"(\\)"
#define string_t std::string
#define stringstream_t std::stringstream
#endif

#ifdef WIN32
#define PATH_SEPARATOR _T('\\')
#define PATH_SEPARATOR_STRING _T("\\")
#else
#define PATH_SEPARATOR _T('/')
#define PATH_SEPARATOR_STRING _T("\\")
#endif

namespace utils {
    namespace io {
        namespace directory {
            inline string_t mkdir(const string_t);
            inline bool exists(string_t);
            inline uintmax_t remove(string_t path, bool recursive = true);
        }

        namespace file {
            inline bool exists(string_t);
            inline bool remove(string_t);
        }

        inline string_t directoryPath(string_t);

        inline string_t fileName(string_t);
    }
    namespace log {
#ifdef WIN32
        inline void write(string_t);
#endif
    }
}
