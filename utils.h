#pragma once
#include <string>

#ifdef WIN32
#define _WINSOCKAPI_    // stops windows.h including winsock.h
#include <windows.h>
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
#endif

#define UNC R"(\\)"

#ifdef UNICODE
#define string_t std::wstring
#else
#define string_t std::string
#endif

#ifdef WIN32
#define PATH_SEPARATOR '\\'
#else
#define PATH_SEPARATOR '/'
#endif

namespace utils {
    template<class T, class U> inline string_t replace(
        string_t s, const T& target, const U& replacement, bool replace_first = 0, bool replace_empty = 0) {
        using S = string_t;
        using C = string_t::value_type;
        using N = string_t::size_type;
        struct {
            auto len(const S& s) { return s.size(); }
            auto len(const C* p) { return std::char_traits<C>::length(p); }
            auto len(const C  c) { return 1; }
            auto sub(S* s, const S& t, N pos, N len) { s->replace(pos, len, t); }
            auto sub(S* s, const C* t, N pos, N len) { s->replace(pos, len, t); }
            auto sub(S* s, const C  t, N pos, N len) { s->replace(pos, len, 1, t); }
            auto ins(S* s, const S& t, N pos) { s->insert(pos, t); }
            auto ins(S* s, const C* t, N pos) { s->insert(pos, t); }
            auto ins(S* s, const C  t, N pos) { s->insert(pos, 1, t); }
        } util;

        N target_length = util.len(target);
        N replacement_length = util.len(replacement);
        if (target_length == 0) {
            if (!replace_empty || replacement_length == 0) return s;
            N n = s.size() + replacement_length * (1 + s.size());
            s.reserve(!replace_first ? n : s.size() + replacement_length);
            for (N i = 0; i < n; i += 1 + replacement_length) {
                util.ins(&s, replacement, i);
                if (replace_first) break;
            }
            return s;
        }

        N pos = 0;
        while ((pos = s.find(target, pos)) != string_t::npos) {
            util.sub(&s, replacement, pos, target_length);
            if (replace_first) return s;
            pos += replacement_length;
        }
        return s;
    }

    string_t sprintf(string_t format, ...) {
        string_t s;
        char buffer[8192] = { 0 };

        // https://stackoverflow.com/questions/1056411/how-to-pass-variable-number-of-arguments-to-printf-sprintf
        va_list args;
        va_start(args, format);
        vsnprintf(buffer, _countof(buffer) - 1, format.c_str(), args);
        va_end(args);
        s = buffer;
        return s;
    }

#ifdef WIN32
    void writeLog(string_t);

    std::string w2s(const std::wstring& ws) {
        _bstr_t t = ws.c_str();
        char* pchar = (char*)t;
        std::string result = pchar;
        return result;
    }

    std::wstring s2w(const std::string& s) {
        _bstr_t t = s.c_str();
        wchar_t* pwchar = (wchar_t*)t;
        std::wstring result = pwchar;
        return result;
    }

    inline BOOL hash(BYTE* pData, DWORD dwDataLength, ALG_ID algHashType, BYTE** ppHashData, DWORD* pdwHashDataLength) {
        HCRYPTPROV hCryptProv = NULL;
        HCRYPTHASH hCryptHash = NULL;
        BYTE* pHashData = NULL;
        DWORD dwHashDataLength = 0;
        DWORD dwTemp = 0;
        BOOL bRet = FALSE;

        do {
            bRet = ::CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT);
            if (FALSE == bRet) {
                break;
            }

            bRet = ::CryptCreateHash(hCryptProv, algHashType, NULL, NULL, &hCryptHash);
            if (FALSE == bRet) {
                break;
            }

            bRet = ::CryptHashData(hCryptHash, pData, dwDataLength, 0);
            if (FALSE == bRet) {
                break;
            }

            dwTemp = sizeof(dwHashDataLength);
            bRet = ::CryptGetHashParam(hCryptHash, HP_HASHSIZE, (BYTE*)(&dwHashDataLength), &dwTemp, 0);
            if (FALSE == bRet) {
                break;
            }

            pHashData = new BYTE[dwHashDataLength];
            if (NULL == pHashData) {
                bRet = FALSE;
                break;
            }
            ::RtlZeroMemory(pHashData, dwHashDataLength);

            bRet = ::CryptGetHashParam(hCryptHash, HP_HASHVAL, pHashData, &dwHashDataLength, 0);
            if (FALSE == bRet) {
                break;
            }

            *ppHashData = pHashData;
            *pdwHashDataLength = dwHashDataLength;

        } while (FALSE);

        if (FALSE == bRet) {
            if (pHashData) {
                delete[]pHashData;
                pHashData = NULL;
            }
        }
        if (hCryptHash) {
            ::CryptDestroyHash(hCryptHash);
        }
        if (hCryptProv) {
            ::CryptReleaseContext(hCryptProv, 0);
        }

        return bRet;
    }

    inline string_t GetFileName(string_t path) {
        string_t fileName;
        auto pos = path.find("\\");
        if (pos != string_t::npos) {
            fileName = path.substr(pos + 1);
        }
        return fileName;
    }

    inline string_t GetDirectoryPath(string_t path) {
        string_t fileName;
        auto pos = path.find_last_of("\\");
        if (pos != string_t::npos) {
            fileName = path.substr(0, pos);
        }
        return fileName;
    }

    inline void CreateDirectory(string_t path) {
        string_t::size_type tmp_pos_begin = 0;
        string_t::size_type tmp_pos = 0;

        // UNC
        if (path.find(UNC) == 0) {
            tmp_pos = path.find('\\', strlen(UNC));
        } else {
            tmp_pos = path.find('\\', tmp_pos_begin);
        }
        while (tmp_pos != path.npos) {
            string_t tmpdir = path.substr(0, tmp_pos);
            if (tmpdir.empty()) {
                return;
            }
            if (!PathFileExists(tmpdir.c_str())) {
                ::CreateDirectory(tmpdir.c_str(), nullptr);
            }
            tmp_pos_begin = tmp_pos + 1;
            tmp_pos = path.find('\\', tmp_pos_begin);
        }
        if (!PathFileExists(path.c_str())) {
            ::CreateDirectory(path.c_str(), nullptr);
        }
    }

    inline string_t GetDateTime() {
        SYSTEMTIME time = { 0 };
        GetLocalTime(&time);

        char buffer[MAX_PATH] = { 0 };
        _snprintf_s(buffer, _countof(buffer) - 1, _T("%04d/%02d/%02d %02d:%02d:%02d.%03d"),
            time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);
        return buffer;
    }

    inline string_t GetRepository() {
        char buffer[MAX_PATH] = { 0 };
        SHGetSpecialFolderPath(0, buffer, CSIDL_PERSONAL, false);
        string_t m2 = buffer;
        m2 += "\\..\\.m2\\Repository";
        return m2;
    }

    inline string_t GetWorkingDirectory() {
        char buffer[MAX_PATH] = { 0 };
        GetModuleFileName(nullptr, buffer, _countof(buffer) - 1);
        string_t directory = buffer;
        auto pos = directory.find_last_of("\\");
        if (pos != string_t::npos) {
            directory = directory.substr(0, pos);
        }
        return directory;
    }

    inline string_t GetExeFileName() {
        TCHAR buffer[MAX_PATH] = { 0 };
        GetModuleFileName(nullptr, buffer, _countof(buffer) - 1);
        auto exe = string_t(buffer);
        auto pos = exe.find_last_of(PATH_SEPARATOR);
        if (pos != string_t::npos) {
            exe = exe.substr(pos + 1);
        }
        return exe;
    }

    inline string_t GetExeFileNameWithoutExt() {
        auto exe = GetExeFileName();
        auto pos = exe.find_last_of(".");
        if (pos != string_t::npos) {
            exe = exe.substr(0, pos);
        }
        return exe;
    }

    struct CONSOLE_TEXT {
        string_t text;
        WORD color;
        CONSOLE_TEXT() {
            this->color = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
        }

        CONSOLE_TEXT(string_t text) {
            this->text = text;
            this->color = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
        }

        CONSOLE_TEXT(string_t text, WORD color) {
            this->text = text;
            this->color = color;
        }
    };

    static LPCRITICAL_SECTION pCriticalSection = nullptr;
    LPCRITICAL_SECTION GetCriticalSection() {
        if (!pCriticalSection) {
            pCriticalSection = new CRITICAL_SECTION();
            InitializeCriticalSection(pCriticalSection);
        }
        return pCriticalSection;
    }

    // https://blog.csdn.net/AWMsilverL/article/details/125714259
    inline void println(std::vector<CONSOLE_TEXT> words, bool crlf = true) {
        static HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hConsole != INVALID_HANDLE_VALUE) {
            EnterCriticalSection(GetCriticalSection());
            SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);

            string_t line;
            auto datetime = sprintf("[%s] ", GetDateTime().c_str());
            _tprintf(_T("%s"), datetime.c_str());
            line += datetime;

            for (auto& w : words) {
                SetConsoleTextAttribute(hConsole, w.color);
                _tprintf(_T("%s"), w.text.c_str());
                line += w.text;
            }
            if (crlf) {
                _tprintf(_T("\n"));
                line += _T("\n");
            }
            writeLog(line);
            LeaveCriticalSection(GetCriticalSection());
        }
    }

    inline void println(string_t text) {
        std::vector<CONSOLE_TEXT> v;
        v.push_back(CONSOLE_TEXT(text));
        println(v);
    }

    inline void println(string_t text, WORD color) {
        std::vector<CONSOLE_TEXT> v;
        v.push_back(CONSOLE_TEXT(text, color));
        println(v);
    }

    inline void print(string_t text, WORD color) {
        std::vector<CONSOLE_TEXT> v;
        v.push_back(CONSOLE_TEXT(text, color));
        println(v, false);
    }

    inline void print_red(string_t text) {
        print(text, FOREGROUND_RED | FOREGROUND_INTENSITY);
    }

    inline void print_green(string_t text) {
        print(text, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    }

    inline void print_blue(string_t text) {
        print(text, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    }

    void writeLog(string_t log) {
        TCHAR buffer[MAX_PATH] = { 0 };
        GetModuleFileName(nullptr, buffer, _countof(buffer) - 1);
        auto logFilePath = string_t(buffer);
        auto pos = logFilePath.find_last_of('.');
        if (pos != string_t::npos) {
            logFilePath = logFilePath.substr(0, pos);
        }
        logFilePath += ".log";
        FILE* file = nullptr;
        fopen_s(&file, logFilePath.c_str(), "ab+");
        if (file) {
            fseek(file, 0, SEEK_END);
            std::string text;
#ifdef UNICODE
            text = w2s(log);
#else
            text = log;
#endif
            fwrite(log.c_str(), sizeof(log.c_str()[0]), log.size(), file);
            fclose(file);
            file = nullptr;
        }
    }
#endif
}

