#ifndef UTILS_H
#define UTILS_H
#pragma message ("* ********************************************************************************* *")
#pragma message ("* URL    : https://github.com/unwitting-life/cpp-_utils                             *")
#pragma message ("* License: https://creativecommons.org/licenses/by-nc/4.0/                          *")
#pragma message ("* Author : Yuanlei Huang                                                            *")
#pragma message ("* Note   : This program only support with ISO C++20                                 *")
#pragma message ("* Latest : 2023/05/09                                                               *")
#pragma message ("* ********************************************************************************* *")

#ifdef _MSC_VER
#ifndef WIN32
#define WIN32
#endif

#include <atlimage.h>
#include <WinSock2.h>

#define _WINSOCKAPI_    // stops windows.h including winsock.h

#include <windows.h>

#endif

#ifdef UNICODE
#define string_t _utils::_wstring
#define _tstring std::wstring
#define stringstream_t std::wstringstream
#define ofstream_t std::wofstream
#else
#define string_t _utils::_string
#define _tstring std::string
#define stringstream_t std::stringstream
#define ofstream_t std::ofstream
#endif
#define utils string_t

#ifdef WIN32
#define UNCw LR"(\\)"
#define UNC_A R"(\\)"
#define PATH_SEPARATORW L'\\'
#define PATH_SEPARATOR_STRINGW L"\\"
#define PATH_SEPARATORA '\\'
#define PATH_SEPARATOR_STRING_A "\\"

#ifdef UNICODE
#define UNC UNCw
#define PATH_SEPARATOR PATH_SEPARATORW
#define PATH_SEPARATOR_STRING PATH_SEPARATOR_STRINGW
#else
#define UNC UNC_A
#define PATH_SEPARATOR PATH_SEPARATORA
#define PATH_SEPARATOR_STRING PATH_SEPARATOR_STRING_A
#endif
#else
#define PATH_SEPARATOR _T('/')
#define PATH_SEPARATOR_STRING _T("\\")
#endif

// https://blog.csdn.net/panjunnn/article/details/115750622
#ifdef _HAS_STD_BYTE
#undef _HAS_STD_BYTE
#endif
#define _HAS_STD_BYTE 0

#include <string>
#include <vector>
#include <fstream>
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
#include <tlhelp32.h>
#include <processthreadsapi.h>
#include <shellapi.h>

#pragma comment(lib, "comsuppw.lib")
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "Normaliz.lib")
#pragma comment(lib, "wldap32.lib" )
#pragma comment(lib, "crypt32.lib" )
#pragma comment(lib, __FILE__ "/../curl/builds/libcurl-vc-x64-debug-static-ipv6-sspi-schannel/lib/libcurl_a_debug.lib")
#else
#define L
#define _T(x) x
#define TCHAR char
#define _countof(x) (sizeof(x) / sizeof(x[0]))
#endif

#include "base64/base64.hpp"

#define CONTENT_DISPOSITION "Content-Disposition"
#define AUTHORIZATION "Authorization"
#define CONTENT_TYPE "Content-Type"
#define CURL_STATICLIB

#include "curl/include/curl/curl.h"

#define HOST_ANY (_T("0.0.0.0"))
#define SSL_PORT (443)
#define LOCALHOST_CRT_FILE_NAME (_T("localhost.crt"))
#define LOCALHOST_KEY_FILE_NAME (_T("localhost.key"))
#define CERTIFICATE "-----BEGIN CERTIFICATE-----\n"                                        \
                    "MIIDDTCCAfWgAwIBAgIJAKG8cwnciIStMA0GCSqGSIb3DQEBCwUAMBQxEjAQBgNV\n"   \
                    "BAMTCWxvY2FsaG9zdDAeFw0yMjEwMTQwNTE0MjhaFw0yMzEwMTQwNTE0MjhaMBQx\n"   \
                    "EjAQBgNVBAMTCWxvY2FsaG9zdDCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoC\n"   \
                    "ggEBAOSC/CsB7nBNBuPmHlV9pBUKBQIeT+s38d5aHNr+xzOV2DsienLEGbdIT11o\n"   \
                    "ztqQ73wqKcWqXWoYJM6oj7P2XIed33RkWF4PrC0uWy1RC9YEBtxitfxUr+KKFq2O\n"   \
                    "r7FBLkqH5EImzGsZfEtajhh4kTElhxDW7G6xR7uMaZbdZ9sCP79aESKr/MRkdbyf\n"   \
                    "rE/tRzka9VSbFsRUjJEJZVqdLyHauTV3t69O/Z1kan+Wsd4fclgYidVn9WLJ/4EB\n"   \
                    "jE7bPJnibMYaY6haD0AMUkYeXc3C+HTmIFRAjr6NTgrVa9NJUQE88vVVSG8NZfJu\n"   \
                    "U4Hzu0zIusddQcNgPWLDJozRTWECAwEAAaNiMGAwDAYDVR0TAQH/BAIwADAOBgNV\n"   \
                    "HQ8BAf8EBAMCBaAwFgYDVR0lAQH/BAwwCgYIKwYBBQUHAwEwFwYDVR0RAQH/BA0w\n"   \
                    "C4IJbG9jYWxob3N0MA8GCisGAQQBgjdUAQEEAQIwDQYJKoZIhvcNAQELBQADggEB\n"   \
                    "AIseuRktMe0gdqnXhNwSCausn97UPvEcV5b4NOG14jMomn8oYGRXQPRkypBFoug6\n"   \
                    "vnkL+mBi0PBslMhaZGB4RVnpTd4NHuRt52XuOr6t1bPHvZN4+2uBmo6ACnocSrZ1\n"   \
                    "PxVGjMa3kI5GeyOKw+c8la5WRrqyDI27PWjo7fpNzUwfKC6jAEZmoYfyF2ZeSiod\n"   \
                    "+j6SueaQHJF/FrT0qhDWnfC0z7nEGvwMkE9Eqpl5Zu2Q7BRnKoCnjzRv85o+XZsr\n"   \
                    "Y6TMEWiWoViC3wjPKVE1qC62VRP3caIYOrL99YgWSrHeaoGuNDyo4PUNsu3VwE95\n"   \
                    "L6LkjC+maAA4+rUexyFvLfc=\n"                                           \
                    "-----END CERTIFICATE-----\n"

#define PRIVATE_KEY "-----BEGIN PRIVATE KEY-----\n"                                        \
                    "MIIEvgIBADANBgkqhkiG9w0BAQEFAASCBKgwggSkAgEAAoIBAQDkgvwrAe5wTQbj\n"   \
                    "5h5VfaQVCgUCHk/rN/HeWhza/sczldg7InpyxBm3SE9daM7akO98KinFql1qGCTO\n"   \
                    "qI+z9lyHnd90ZFheD6wtLlstUQvWBAbcYrX8VK/iihatjq+xQS5Kh+RCJsxrGXxL\n"   \
                    "Wo4YeJExJYcQ1uxusUe7jGmW3WfbAj+/WhEiq/zEZHW8n6xP7Uc5GvVUmxbEVIyR\n"   \
                    "CWVanS8h2rk1d7evTv2dZGp/lrHeH3JYGInVZ/Viyf+BAYxO2zyZ4mzGGmOoWg9A\n"   \
                    "DFJGHl3Nwvh05iBUQI6+jU4K1WvTSVEBPPL1VUhvDWXyblOB87tMyLrHXUHDYD1i\n"   \
                    "wyaM0U1hAgMBAAECggEAcC7V+Y8zLbLDBTM+BR5vZVzL5KRY+9M1TMHUlLaruOmB\n"   \
                    "hFaC4sDfFxnKzMLdenPY/o38f+wJfEJqH4KlOeZiurmIsC1rLJJpJq0xWegF9YV4\n"   \
                    "kM1tJnm093RvebkpC85CGZjpOIivpoyWdq/4Po9CSGxyCJRa8mTaIHrog4YirGNa\n"   \
                    "hfxG4rY8cU+0+QbP52bDL7DGHVJ7dAR4UuQDOi+M8ymzhNIR6VLX2Ax3HlkFOJkp\n"   \
                    "aW0JAGP2jSQib22MQqDJR+60hYKFqcuCZv+eVWrHX6FgyvpPLpF2ZO78ZiHmbcP1\n"   \
                    "Y7VVRyzM/nF15pgWymhiXA5GX2L6FcAHNMrRXBDquQKBgQDmwWjilIZmk/1ljyLI\n"   \
                    "1jZW8YwV9neGNlHanuoF4yTesn2f5iY3ga/KcaimV1wc8JaRTHEPOkabQVkAl/t3\n"   \
                    "ZLhQlz/gw9vHzDoTo9MhxLAoVxhebXZXxnFCHl5wOEyEOEHegIoJ01YlYXB3j8nT\n"   \
                    "ZOljbKyz0p9h9uA6UUp3xUdzZwKBgQD9grvRKspEGFcZh2qk8yELgP15AvMyjMUX\n"   \
                    "YiKBxkiAR/j/ahqgDsaLa3r5JHzxwRHERPBsQbBb42Op41fAijvHi/v2CdvbPZ4Q\n"   \
                    "T0QEFTXF/IZIpQ0B2fNaAXujbU6LqQOSNHvHUAieuQeAP20YOfIh/SmVAJo3YXsm\n"   \
                    "mLQyqctD9wKBgQCHyJ+TVJqRmdut+0AYthRbrN8lvVYv357i6kgDFtoHQTiy8pbi\n"   \
                    "aqmqkp8O4rENRnUTIswUjdImYZmhw8zxQI9EvPuVoyqcswwpqmj+oeKWmkl4pyjQ\n"   \
                    "FbFnmr8vgbFkzEXWTATuKzGx5VhswmucvPCo1LX7SqEG4GKpblRHjgqxWQKBgQCZ\n"   \
                    "GYoKAViUFUsMzN5X7ckoJGdv3Js5DRC+VXLXgwMoDc9PmSmpPiMj8e4bZrDaY6oy\n"   \
                    "KRw1lNpINCRtZjIJwrHliiHJbhzL5AOAgA5olNKb1izW1G7tjk3MLpcXlJzZ9WGF\n"   \
                    "u5KBRVFfYgkop6Vf+ybC5hxlsy5vgbyZC6HP8wEh8wKBgHA3ZMU7s6ZcBDXneKuk\n"   \
                    "NZ1GGscGV0WRgqsaHPlHKlMSZ3Sy3MtHSoh1aTTKJiF7ESwVR+B350gV8JK77baC\n"   \
                    "yaPYwGgV2rxhvrtfLhG5wQ6EFh9OO63iLCPv8JvPugehQn917WjuiiYKfn9jEi1d\n"   \
                    "qSov5MqodQK1DhdgI10IfiuX\n"                                           \
                    "-----END PRIVATE KEY-----\n"
#define CPPHTTPLIB_OPENSSL_SUPPORT

#include "cpp-httplib/v0.11.4+/httplib.h"

#ifdef WIN32
#ifdef _WIN64
#pragma comment(lib, __FILE__ "/../OpenSSL/3.0.6+/VC-WIN64A/lib/libcrypto.lib")
#pragma comment(lib, __FILE__ "/../OpenSSL/3.0.6+/VC-WIN64A/lib/libssl.lib")
#else
#pragma comment(lib, __FILE__ "/../OpenSSL/3.0.6+/VC-WIN32/lib/libcrypto.lib")
#pragma comment(lib, __FILE__ "/../OpenSSL/3.0.6+/VC-WIN32/lib/libssl.lib")
#endif
#endif

class _utils {
public:
    class _wstring : public std::wstring {
    public:
        static inline std::wstring s2w(std::string s, UINT cp = CP_ACP) {
            std::wstring w;
            if (!s.empty()) {
#ifdef WIN32
                auto size = MultiByteToWideChar(cp, 0, s.c_str(), -1, nullptr, 0);
                if (size) {
                    auto p = std::shared_ptr<wchar_t>(new wchar_t[size + 1]);
                    memset(p.get(), 0, (static_cast<size_t>(size) + 1) * sizeof(wchar_t));
                    MultiByteToWideChar(cp, 0, s.c_str(), -1, p.get(), size);
                    w.append(p.get(), size);
                }
#else
                size_t size;
            pos = s.find(static_cast<char>(0), begin);
            while (pos != std::string::npos) {
                auto segment = std::string(&s[begin], pos - begin);
                auto converted = std::wstring(segment.size(), 0);
                size = mbstowcs(&converted[0], &segment[0], converted.size());
                converted.resize(size);
                w.append(converted);
                w.append({ 0 });
                begin = pos + 1;
                pos = s.find(static_cast<char>(0), begin);
            }
            if (begin < s.length()) {
                auto segment = std::string(&(*this)[begin], s.length() - begin);
                auto converted = std::wstring(segment.size(), 0);
                size = mbstowcs(&converted[0], &segment[0], converted.size());
                converted.resize(size);
                w.append(converted);
            }
#endif
            }
            return w;
        }

        static inline std::string w2s(std::wstring w) {
#ifdef WIN32
            auto s = std::string();
            if (!w.empty()) {
                auto size = WideCharToMultiByte(CP_ACP, 0, w.c_str(), -1, nullptr, 0, nullptr, nullptr);
                auto p = std::shared_ptr<char>(new char[size + 1]);
                memset(p.get(), 0, sizeof(p.get()[0]) * (size + 1));
                size = WideCharToMultiByte(CP_ACP, 0, w.c_str(), -1, p.get(), size + 1, nullptr, nullptr);
                if (size >= 1) {
                    s = std::string(p.get(), strlen(p.get()));
                }
            }
            return s;
#else
            std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> convert;
        return convert.to_bytes(s);
#endif
        }

        static inline std::string t2s(std::wstring s) {
            return w2s(s);
        }

        static inline std::string t2s(std::string s) {
            return s;
        }

        static inline _tstring t2t(std::wstring s) {
#ifdef UNICODE
            return s;
#else
            return s2w(s);
#endif
        }

        static inline _tstring t2t(std::string s) {
#ifdef UNICODE
            return s2w(s);
#else
            return s;
#endif
        }

        static inline std::string replace_t(std::string s, const std::string target, const std::string replacement, bool once) {
            if (s.empty() || target.empty()) {
                return s;
            }
            using S = std::string;
            using C = std::string::value_type;
            using N = std::string::size_type;
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
            N pos = 0;
            while ((pos = s.find(target, pos)) != std::string::npos) {
                util.sub(&s, replacement, pos, target_length);
                if (once) return s;
                pos += replacement_length;
            }
            return s;
        }

        static inline std::string replace_t(std::string s, const std::string target, const std::string replacement) {
            return replace_t(s, target, replacement, false);
        }

        static inline std::string replace_t(std::string s, const char *target, const char replacement) {
            return replace_t(s, target, std::string({replacement}), false);
        }

        static inline std::string replace_t(std::string s, const char target, const char replacement) {
            return replace_t(s, std::string({target}), std::string({replacement}), false);
        }

        static inline std::wstring replace_t(std::wstring s, const std::wstring target, const std::wstring replacement, bool once) {
            if (s.empty() || target.empty()) {
                return s;
            }
            using S = std::wstring;
            using C = std::wstring::value_type;
            using N = std::wstring::size_type;
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
            N pos = 0;
            while ((pos = s.find(target, pos)) != std::wstring::npos) {
                util.sub(&s, replacement, pos, target_length);
                if (once) return s;
                pos += replacement_length;
            }
            return s;
        }

        static inline std::wstring replace_t(std::wstring s, const std::wstring target, const std::wstring replacement) {
            return replace_t(s, target, replacement, false);
        }

        static inline std::wstring replace_t(std::wstring s, const wchar_t *target, const wchar_t replacement) {
            return replace_t(s, target, std::wstring({replacement}), false);
        }

        static inline std::string format(std::string f, ...) {
            //@formatter:off
            std::string s;
            va_list args;
            va_start(args, f);
#ifdef WIN32
            size_t size = (_vscprintf(f.c_str(), args) + 1) * sizeof(char);
#else
            size_t size = (vsnprintf(nullptr, 0, f.c_str(), args) + 1) * sizeof(TCHAR);
#endif
            va_end(args);
            auto p = std::shared_ptr<char>(new char[size]);
            memset(p.get(), 0, size);
            va_start(args, f);
#ifdef WIN32
            _vsnprintf_s(p.get(), size, size - 1, f.c_str(), args);
#else
            vsnprintf(p.get(), size, f.c_str(), args);
#endif
            va_end(args);
            s = p.get();
            //@formatter:on
            return s;
        }

        static inline std::wstring format(std::wstring f, ...) {
            //@formatter:off
            std::wstring s;
            va_list args;
            va_start(args, f);
#ifdef WIN32
            size_t size = (_vscwprintf(f.c_str(), args) + 1) * sizeof(wchar_t);
#else
            size_t size = (vsnprintf(nullptr, 0, f.c_str(), args) + 1) * sizeof(TCHAR);
#endif
            va_end(args);
            auto p = std::shared_ptr<wchar_t>(new wchar_t[size]);
            memset(p.get(), 0, size);
            va_start(args, f);
#ifdef WIN32
            _vsnwprintf_s(p.get(), size, size - 1, f.c_str(), args);
#else
            vsnprintf(p.get(), size, f.c_str(), args);
#endif
            va_end(args);
            s = p.get();
            //@formatter:on
            return s;
        }

        static inline std::wstring right_t(std::wstring s, size_t size) {
            return size >= s.size() ? std::wstring() : s.substr(s.size() - size);
        }

        static inline std::wstring left_t(std::wstring s, size_t size) {
            return size >= s.size() ? std::wstring() : s.substr(0, size);
        }

        static inline std::vector<std::wstring> find_t(std::wstring s, std::wstring prefix, std::wstring suffix) {
            auto v = std::vector<std::wstring>();
            auto prefix_ = s.find(prefix);
            while (prefix_ != std::wstring::npos) {
                auto suffix_ = suffix.empty() ? std::wstring::npos : s.find(suffix, prefix_ + prefix.size());
                if (suffix_ == std::wstring::npos) {
                    v.push_back(s.substr(prefix_));
                    prefix_ = std::wstring::npos;
                } else {
                    v.push_back(s.substr(prefix_, suffix_ - prefix_ + suffix.size()));
                    prefix_ = s.find(prefix, suffix_ + suffix.size());
                }
            }
            return v;
        }

        static inline std::vector<std::wstring> truncate_t(std::wstring s, std::wstring prefix, std::wstring suffix) {
            auto v = std::vector<std::wstring>();
            for (auto &e: find_t(s, prefix, suffix)) {
                auto w = std::wstring(e);
                w = right_t(w, w.size() - prefix.size());
                if (!suffix.empty()) {
                    w = left_t(w, w.size() - suffix.size());
                }
                v.push_back(w);
            }
            return v;
        }

        class _base_string_t {
        public:
            _base_string_t(_tstring s) : m_str(s) {}

            const LPCTSTR c_str() { return this->m_str.c_str(); };

        protected:
            _tstring m_str;
        };

        class system {
        public:
            class process {
            public:
                static inline DWORD get_parent_process_id() {
#ifdef WIN32
                    auto ppid = DWORD(0);
                    auto pid = GetCurrentProcessId();
                    PROCESSENTRY32 pe32 = {0};
                    auto hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
                    if (hSnapshot != INVALID_HANDLE_VALUE) {
                        ZeroMemory(&pe32, sizeof(pe32));
                        pe32.dwSize = sizeof(pe32);
                        if (Process32First(hSnapshot, &pe32)) {
                            do {
                                if (pe32.th32ProcessID == pid) {
                                    ppid = pe32.th32ParentProcessID;
                                    break;
                                }
                            } while (Process32Next(hSnapshot, &pe32));
                        }
                    }
                    if (hSnapshot != INVALID_HANDLE_VALUE) {
                        CloseHandle(hSnapshot);
                    }
                    return ppid;
#endif
                }

                static inline long start(_tstring exe, _tstring params, _tstring workingDirectory, std::function<void(std::string _stdout)> _stdout) {
                    auto exitCode = long(0L);
                    auto pipeReadHandle = HANDLE(nullptr);
                    auto pipeWriteHandle = HANDLE(nullptr);
                    SECURITY_ATTRIBUTES sa = {sizeof(SECURITY_ATTRIBUTES)};
                    sa.bInheritHandle = TRUE;
                    sa.lpSecurityDescriptor = nullptr;
                    if (CreatePipe(&pipeReadHandle, &pipeWriteHandle, &sa, 0) && SetHandleInformation(pipeReadHandle, HANDLE_FLAG_INHERIT, FALSE)) {
                        STARTUPINFOEX si = {0};
                        si.StartupInfo.cb = sizeof(si);
                        si.StartupInfo.hStdOutput = pipeWriteHandle;
                        si.StartupInfo.hStdError = pipeReadHandle;
                        si.StartupInfo.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
                        si.StartupInfo.wShowWindow = SW_SHOW;
                        HANDLE inheritHandles[] = {pipeWriteHandle};
                        auto size = SIZE_T(0);
                        InitializeProcThreadAttributeList(nullptr, _countof(inheritHandles), 0, &size);
                        if (size > 0) {
                            auto lpAttributeList = reinterpret_cast<LPPROC_THREAD_ATTRIBUTE_LIST >(malloc(size));
                            if (lpAttributeList) {
                                if (InitializeProcThreadAttributeList(lpAttributeList, _countof(inheritHandles), 0, &size) &&
                                    UpdateProcThreadAttribute(lpAttributeList, 0, PROC_THREAD_ATTRIBUTE_HANDLE_LIST, inheritHandles, sizeof(inheritHandles), nullptr, nullptr)) {
                                    si.lpAttributeList = lpAttributeList;
                                    PROCESS_INFORMATION pi = {nullptr};
                                    std::thread pipe_thread([&]() {
                                        auto numberOfBytesRead = DWORD(0);
                                        char buffer[8192] = {0};
                                        while (::ReadFile(pipeReadHandle, buffer, sizeof(buffer), &numberOfBytesRead, nullptr)) {
                                            _stdout(std::string(buffer, numberOfBytesRead));
                                        }
                                    });
                                    if (CreateProcess(exe.empty() ? nullptr : exe.c_str(),
                                                      const_cast<LPTSTR>(params.empty() ? nullptr : params.c_str()),
                                                      nullptr,
                                                      nullptr,
                                                      TRUE,
                                                      NORMAL_PRIORITY_CLASS | EXTENDED_STARTUPINFO_PRESENT,
                                                      nullptr,
                                                      workingDirectory.empty() ? nullptr : workingDirectory.c_str(),
                                                      &si.StartupInfo,
                                                      &pi)) {
                                        wait(pi.hProcess, INFINITE);
                                        GetExitCodeProcess(pi.hProcess, reinterpret_cast<LPDWORD>(&exitCode));
                                        CloseHandle(pi.hProcess);
                                        CloseHandle(pi.hThread);
                                    } else {
                                        exitCode = GetLastError();
                                    }
                                    CloseHandle(pipeWriteHandle);
                                    CloseHandle(pipeReadHandle);
                                    pipe_thread.join();
                                }
                                free(lpAttributeList);
                            }
                        }
                    }
                    return exitCode;
                }

                static inline long start(_tstring exe, _tstring params, _tstring workingDirectory) {
                    return start(exe, params, workingDirectory, [](std::string _stdout) {});
                }

                static inline long start(_tstring exe, _tstring params) {
                    return start(exe, params, _tstring(), [](std::string _stdout) {});
                }

                static inline long start(_tstring cmd) {
                    return start(_tstring(), cmd, _tstring(), [](std::string _stdout) {});
                }

                static inline HANDLE shell_execute(_tstring exe, _tstring params, _tstring workingDirectory) {
                    auto handle = HANDLE(nullptr);
                    SHELLEXECUTEINFO se = {0};
                    se.cbSize = sizeof(SHELLEXECUTEINFO);
                    se.fMask = SEE_MASK_NOCLOSEPROCESS;
                    se.hwnd = nullptr;
                    se.lpVerb = nullptr;
                    se.lpFile = exe.c_str();
                    se.lpParameters = params.c_str();
                    se.lpDirectory = workingDirectory.c_str();
                    se.nShow = SW_SHOW;
                    se.hInstApp = nullptr;
                    if (ShellExecuteEx(&se)) {
                        handle = se.hProcess;
                    }
                    return handle;
                }

                static inline HANDLE shell_execute(_tstring exe, _tstring params) {
                    return shell_execute(exe, params, _tstring());
                }

                static inline HANDLE shell_execute(_tstring exe) {
                    return shell_execute(exe, _tstring(), _tstring());
                }

                static inline void wait(HANDLE handle, DWORD timeout = INFINITE) {
                    if (handle) {
                        WaitForSingleObject(handle, timeout);
                    }
                }

                static inline void wait(DWORD pid, DWORD timeout = INFINITE) {
                    auto handle = OpenProcess(SYNCHRONIZE, FALSE, pid);
                    if (handle) {
                        wait(handle, timeout);
                        CloseHandle(handle);
                    }
                }
            };

            class io {
            public:
                class path : public _base_string_t {
                public:
                    static inline bool exists(_tstring path) {
                        auto b = false;
                        if (!path.empty() && path[path.size() - 1] == PATH_SEPARATOR &&
                            (!(path.size() >= 2 && path[path.size() - 2] == PATH_SEPARATOR))) {
                            path = path.substr(0, path.size() - 1);
                        }
                        try {
                            b = std::filesystem::exists(path);
                        } catch (...) {
                        }
                        return b;
                    }

                    static inline bool is_file(_tstring path) {
                        return exists(path) && !std::filesystem::is_directory(path);
                    }

                    static inline bool is_directory(_tstring path) {
                        return exists(path) && std::filesystem::is_directory(path);
                    }

                    static inline _tstring name_t(_tstring path) {
                        auto s = replace_t(path, _T("/"), PATH_SEPARATOR);
                        auto pos = s.find_last_of(PATH_SEPARATOR);
                        return (pos == std::string::npos ? s : s.substr(pos + 1));
                    }

                    static inline _tstring combine_t(_tstring path1, _tstring path2) {
                        _tstring s1 = replace_t(path1, _T("/"), PATH_SEPARATOR);
                        _tstring s2 = replace_t(path2, _T("/"), PATH_SEPARATOR);
                        if (!s1.empty() && s1.at(s1.size() - 1) != PATH_SEPARATOR) {
                            if (s2.empty() || s2.at(0) != PATH_SEPARATOR) {
                                s1 += PATH_SEPARATOR;
                            }
                        }
                        s1 += s2;
                        return s1;
                    }

                    static inline _tstring directory(_tstring path) {
                        _tstring directoryName_ = path;
                        auto pos = path.find_last_of(PATH_SEPARATOR);
                        if (pos == _tstring::npos) {
                            directoryName_ = _T(".");
                        } else {
                            directoryName_ = directoryName_.substr(0, pos);
                        }
                        return directoryName_;
                    }

                    static inline string_t executable_file_directory() {
                        TCHAR buffer[MAX_PATH] = {0};
                        GetModuleFileName(nullptr, buffer, _countof(buffer) - 1);
                        auto exe = string_t(buffer);
                        auto pos = exe.find_last_of(PATH_SEPARATOR);
                        if (pos != string_t::npos) {
                            exe = exe.substr(0, pos);
                        }
                        return exe;
                    }

                    static inline _tstring working_directory() {
#ifdef WIN32
                        auto workingDirectory = new TCHAR[MAX_PATH];
                        GetCurrentDirectory(MAX_PATH, workingDirectory);
#endif
                        return workingDirectory;
                    }

                    static inline _tstring name_without_extension_t(_tstring path) {
                        _tstring s = path;
                        auto pos = path.find_last_of(PATH_SEPARATOR);
                        if (pos != _tstring::npos) {
                            s = s.substr(pos + 1);
                        }
                        pos = s.find_last_of(_T("."));
                        if (pos != _tstring::npos) {
                            s = s.substr(0, pos);
                        }
                        return s;
                    }

                public:
                    path(_tstring path) : _base_string_t(path) { }

                    _tstring combine(_tstring path) {
                        return utils::system::io::path::combine_t(this->m_str, path);
                    }

                    _tstring name() {
                        return utils::system::io::path::name_t(this->m_str);
                    }

                    _tstring name_without_extension() {
                        return utils::system::io::path::name_without_extension_t(this->m_str);
                    }
                };

                class file : public _base_string_t {
                public:
                    file(_tstring path) : _base_string_t(path) { }

                    bool exists() {
                        auto b = false;
                        auto path = _tstring(this->m_str);
                        if (!path.empty() && path[path.size() - 1] == PATH_SEPARATOR &&
                            (!(path.size() >= 2 && path[path.size() - 2] == PATH_SEPARATOR))) {
                            path = path.substr(0, path.size() - 1);
                        }
                        try {
                            b = std::filesystem::exists(path) && !std::filesystem::is_directory(path);
                        } catch (...) {
                        }
                        return b;
                    }

                    bool remove() {
                        auto b = true;
                        if (this->exists()) {
                            try {
                                b = std::filesystem::remove(this->m_str);
                            } catch (...) {

                            }
                        }
                        return b;
                    }

                    bool move(_tstring newPath) {
                        auto b = true;
                        auto _new = _tstring(newPath);
                        try {
                            if (std::filesystem::exists(_new) && std::filesystem::is_directory(_new)) {
                                _new = utils::system::io::path::combine_t(newPath, utils::system::io::path::name_t(this->m_str));
                            }
                            if (std::filesystem::exists(_new)) {
                                if (std::filesystem::is_directory(_new)) {
                                    b = false;
                                } else {
                                    b = std::filesystem::remove(_new);
                                }
                            }
                        } catch (...) {
                            b = false;
                        }
                        if (b) {
                            b = ::rename(t2s(this->m_str).c_str(), t2s(_new).c_str()) == 0;
                            if (b) {
                                this->m_str = _new;
                            }
                        }
                        return b;
                    }

                    bool rename(_tstring newPath) {
                        return this->move(newPath);
                    }

                    inline unsigned long long size() {
                        return this->exists() ? std::filesystem::file_size(this->m_str) : -1;
                    }
                };

                class ofstream : public file {
                public:
                    static inline void writeBytes(_tstring file, const char *data, size_t size, bool createNew = true) {
                        auto f = ofstream(file, createNew);
                        f.write(data, size);
                        f.close();
                    }

                    static inline void appendBytes(_tstring file, const char *data, size_t size) {
                        ofstream::writeBytes(file, data, size, false);
                    }

                    ofstream(_tstring path, bool createNew = false) : file(path), m_closed(false) {
                        if (createNew) {
                            this->remove();
                        }
                        this->m_file.open(t2s(path).c_str(), std::ios::app | std::ios::binary);
                        if (!createNew) {
                            this->m_file.seekp(0, std::ios::end);
                        }
                    }

                    ~ofstream() {
                        this->close();
                    }

                    inline void write(const char *data, size_t size) {
                        this->m_file.write(data, size);
                    }

                    inline void close() {
                        if (!m_closed) {
                            this->m_closed = true;
                            this->m_file.flush();
                            this->m_file.close();
                        }
                    }

                private:
                    std::ofstream m_file;
                    bool m_closed;
                };

                class image : public file {
                public:
#ifdef WIN32
                    bool png(const _tstring dst = _tstring()) {
                        auto path = _tstring(dst);
                        if (path.empty()) {
                            auto png = utils::format(_T("%s.png"), utils::system::io::path::name_without_extension_t(this->m_str));
                            path = utils::system::io::path::combine_t(utils::system::io::path::directory(this->m_str), png.c_str());
                        }
                        CImage image;
                        image.Load(this->m_str.c_str());
                        return image.Save(path.c_str(), Gdiplus::ImageFormatPNG) == S_OK;
                    }

#endif
                };

                class directory : public _base_string_t {
                public :
                    static inline std::vector<_tstring> enumerate_t(_tstring path, bool recursive = true) {
                        std::vector<_tstring> directories;
                        if (path::is_directory(path)) {
                            for (const auto &entry: std::filesystem::directory_iterator(path)) {
#ifdef UNICODE
                                _tstring p = reinterpret_cast<LPCTSTR>(entry.path().u16string().c_str());
#else
                                _tstring p = reinterpret_cast<LPCTSTR>(entry.path().string().c_str());
#endif
                                if (entry.is_directory()) {
                                    if (recursive) {
                                        for (auto &directory: enumerate_t(p, recursive)) {
                                            directories.push_back(directory);
                                        }
                                    }
                                    directories.push_back(p);
                                }
                            }
                        }
                        return directories;
                    }

                    directory(_tstring path) : _base_string_t(path) {}

                    inline _tstring Create() {
                        if (!this->m_str.empty()) {
                            _tstring::size_type tmp_pos_begin = 0;
                            _tstring::size_type tmp_pos;
#ifdef WIN32
                            if (this->m_str.find(UNC) == 0) {
                                tmp_pos = this->m_str.find(PATH_SEPARATOR, _tcslen(UNC));
                            } else {
                                tmp_pos = this->m_str.find(PATH_SEPARATOR, tmp_pos_begin);
                            }
#else
                            tmp_pos = this->m_path.find(PATH_SEPARATOR_W, tmp_pos_begin);
#endif
                            while (tmp_pos != _tstring::npos) {
                                auto tmpdir = this->m_str.substr(0, tmp_pos);
                                if (tmpdir.empty()) {
                                    return this->m_str;
                                }
                                if (!std::filesystem::exists(tmpdir)) {
#ifdef WIN32
                                    ::CreateDirectory(tmpdir.c_str(), nullptr);
#else
                                    ::mkdir(tmpdir.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
#endif
                                }
                                tmp_pos_begin = tmp_pos + 1;
                                tmp_pos = this->m_str.find(PATH_SEPARATOR, tmp_pos_begin);
                            }
                            if (!std::filesystem::exists(this->m_str)) {
#ifdef WIN32
                                ::CreateDirectory(this->m_str.c_str(), nullptr);
#else
                                ::mkdir(this->m_path.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
#endif
                            }
                        }
                        return this->m_str;
                    }

                    bool move(_tstring newPath) {
                        auto b = ::rename(t2s(this->m_str).c_str(), t2s(newPath).c_str()) == 0;
                        this->m_str = newPath;
                        return b;
                    }

                    _tstring name() {
                        _tstring name = utils::replace_t(this->m_str, _T("/"), PATH_SEPARATOR);
                        auto pos = name.find_last_of(PATH_SEPARATOR);
                        if (pos != std::wstring::npos) {
                            name = name.substr(pos + 1);
                        }
                        return name;
                    }

                    bool rename(_tstring newPath) {
                        return this->move(newPath);
                    }

                    std::vector<_tstring> enumerate(bool recursive = true) {
                        return enumerate_t(this->m_str, recursive);
                    }

                    bool exists() {
                        return utils::system::io::path::is_directory(this->m_str);
                    }
                };
            };
        };

        class http {
        public:
            class header {
            public:
                static constexpr auto Basic = "Basic ";
                static constexpr auto Location = "Location";
            };

            class status {
            public:
                static const int Ok = 200;
                static const int PermanentlyMoved = 301;
                static const int TemporarilyMoved = 302;
                static const int Unauthorized = 401;
                static const int NotFound = 404;
                static const int InternalServerError = 500;
            };

            struct CURLDATA {
            public:
                std::string uri;
                _tstring fileName;
                std::vector<std::string> headers;
                std::string body;
                utils::system::io::ofstream *ofstream;
                std::function<bool(CURLDATA *data)> f;

                CURLDATA(std::string uri) {
                    this->uri = uri;
                    this->fileName.clear();
                    this->headers.clear();
                    this->body.clear();
                    this->ofstream = nullptr;
                }

                ~CURLDATA() {
                    if (this->ofstream) {
                        this->ofstream->close();
                        this->ofstream = nullptr;
                    }
                }
            };

            static inline size_t write_header(char *buffer, size_t size, size_t nmemb, CURLDATA *params) {
                auto numberOfBytesWritten = size_t(0);
                if (params) {
                    numberOfBytesWritten = size * nmemb;
                    params->headers.push_back(std::string(buffer, numberOfBytesWritten));
                }
                return numberOfBytesWritten;
            }

            static inline size_t write_string(void *p, size_t size, size_t nmemb, CURLDATA *params) {
                auto numberOfBytesWritten = size_t(0);
                if (params) {
                    numberOfBytesWritten = size * nmemb;
                    params->body.append(reinterpret_cast<char *>(p), numberOfBytesWritten);
                }
                return numberOfBytesWritten;
            }

            static inline size_t write_file(void *p, size_t size, size_t nmemb, CURLDATA *params) {
                auto numberOfBytesWritten = size_t(0);
                if (params) {
                    if (!params->ofstream) {
                        params->fileName.clear();
                        for (auto &e: params->headers) {
                            if (e.starts_with(CONTENT_DISPOSITION)) {
                                auto filename = truncate_t(t2t(e), _T("filename=\""), _T("\""));
                                if (!filename.empty()) {
                                    params->fileName = filename[0];
                                }
                                break;
                            }
                        }
                        if (params->fileName.empty()) {
                            params->fileName = _tstring(t2t(params->uri));
                            auto pos = params->fileName.find_last_of(_T('?'));
                            if (pos != _tstring::npos) {
                                params->fileName = params->fileName.substr(0, pos);
                            }
                            pos = params->fileName.find_last_of(_T('/'));
                            if (pos != _tstring::npos) {
                                params->fileName = params->fileName.substr(pos + 1);
                            }
                        }
                        if (!params->fileName.empty()) {
                            if (params->f(params)) {
                                params->ofstream = new utils::system::io::ofstream(params->fileName, true);
                            }
                        }
                    }
                    if (params->ofstream) {
                        numberOfBytesWritten = size * nmemb;
                        params->ofstream->write(reinterpret_cast<char *>(p), numberOfBytesWritten);
                    } else {
                        numberOfBytesWritten = 0;
                    }
                }
                return numberOfBytesWritten;
            }

            static inline size_t write_null(void *p, size_t size, size_t nmemb, CURLDATA *params) {
                UNREFERENCED_PARAMETER(p);
                UNREFERENCED_PARAMETER(size);
                UNREFERENCED_PARAMETER(nmemb);
                UNREFERENCED_PARAMETER(params);
                return 0;
            }

            class curl {
            public:
                curl(std::string uri, std::string proxyHost = std::string(), std::string proxyUserName = std::string(), std::string proxyPassword = std::string()) {
                    this->m_uri = uri;
                    this->m_proxyHost = proxyHost;
                    this->m_proxyUserName = proxyUserName;
                    this->m_proxyPassword = proxyPassword;
                    this->m_cookie.clear();
                }

                inline void setUri(std::string uri) {
                    this->m_uri = uri;
                }

                inline void setUri(std::wstring uri) {
                    this->m_uri = w2s(uri);
                }

                inline void setCookie(std::string cookie) {
                    this->m_cookie = cookie;
                }

                inline void setProxy(std::string proxyHost, std::string proxyUserName = std::string(), std::string proxyPassword = std::string()) {
                    this->m_proxyHost = proxyHost;
                    this->m_proxyUserName = proxyUserName;
                    this->m_proxyPassword = proxyPassword;
                }

                inline std::string Get() {
                    CURLDATA params(this->m_uri);
                    auto curl = curl_easy_init();
                    if (curl) {
                        curl_easy_setopt(curl, CURLOPT_URL, this->m_uri.c_str());
                        this->setOptions(curl);
                        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
                        curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, http::write_header);
                        curl_easy_setopt(curl, CURLOPT_HEADERDATA, &params);
                        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, http::write_string);
                        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &params);
                        curl_easy_perform(curl);
                        curl_easy_cleanup(curl);
                    }
                    return params.body;
                }

                inline std::vector<std::string> Query() {
                    CURLDATA params(this->m_uri);
                    auto curl = curl_easy_init();
                    if (curl) {
                        this->setOptions(curl);
                        if (CURLE_OK == curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_null) &&
                            CURLE_OK == curl_easy_setopt(curl, CURLOPT_FILE, nullptr) &&
                            CURLE_OK == curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, write_header) &&
                            CURLE_OK == curl_easy_setopt(curl, CURLOPT_HEADERDATA, &params) &&
                            CURLE_OK == curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L) &&
                            CURLE_OK == curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 0L) &&
                            CURLE_OK == curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L) &&
                            CURLE_OK == curl_easy_setopt(curl, CURLOPT_URL, this->m_uri.c_str())) {
                            curl_easy_perform(curl);
                        }
                        curl_easy_cleanup(curl);
                    }
                    return params.headers;
                }

                inline _tstring Download(std::function<bool(CURLDATA *)> f) {
                    CURLDATA params(this->m_uri);
                    params.f = f;
                    auto curl = curl_easy_init();
                    if (curl) {
                        this->setOptions(curl);
                        if (CURLE_OK == curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_file) &&
                            CURLE_OK == curl_easy_setopt(curl, CURLOPT_FILE, &params) &&
                            CURLE_OK == curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, write_header) &&
                            CURLE_OK == curl_easy_setopt(curl, CURLOPT_HEADERDATA, &params) &&
                            CURLE_OK == curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L) &&
                            CURLE_OK == curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 0L) &&
                            CURLE_OK == curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L) &&
                            CURLE_OK == curl_easy_setopt(curl, CURLOPT_URL, this->m_uri.c_str())) {
                            curl_easy_perform(curl);
                        }
                        curl_easy_cleanup(curl);
                    }
                    if (params.ofstream) {
                        params.ofstream->close();
                    }
                    return params.fileName;
                }

                inline _tstring Download() {
                    return this->Download([](CURLDATA *) { return true; });
                }

            private:
                std::string m_uri;
                std::string m_proxyHost;
                std::string m_proxyUserName;
                std::string m_proxyPassword;
                std::string m_cookie;

                inline CURLcode setOptions(CURL *curl) {
                    auto code = CURLcode(CURLE_OK);
                    if (!this->m_proxyHost.empty()) {
                        code = curl_easy_setopt(curl, CURLOPT_PROXY, this->m_proxyHost.c_str());
                        if (code == CURLE_OK && !this->m_proxyUserName.empty()) {
                            code = curl_easy_setopt(curl, CURLOPT_PROXYUSERNAME, this->m_proxyUserName.c_str());
                        }
                        if (code == CURLE_OK && !this->m_proxyPassword.empty()) {
                            code = curl_easy_setopt(curl, CURLOPT_PROXYPASSWORD, this->m_proxyPassword.c_str());
                        }
                    }
                    if (!this->m_cookie.empty()) {
                        code = curl_easy_setopt(curl, CURLOPT_COOKIE, this->m_cookie.c_str());
                    }
                    return code;
                }
            };

            class httplib {
            public:
                struct REQUEST {
                    struct HEADERS {
                        void add(std::string key, std::string value) {
                            this->m_headers.insert(std::make_pair(key, value));
                        }

                        ::httplib::Headers m_headers;
                    } headers;

                    _tstring contentType;
                };

                static inline int timeout() { return m_timeout; }

                static inline void setTimeout(int timeout) { m_timeout = timeout; }

#ifdef WIN32

                static inline void create_new_certificate(bool overwrite = true) {
                    utils::system::io::ofstream::writeBytes(utils::system::io::path::combine_t(utils::system::io::path::executable_file_directory(), LOCALHOST_CRT_FILE_NAME), CERTIFICATE, strlen(CERTIFICATE), overwrite);
                    utils::system::io::ofstream::writeBytes(utils::system::io::path::combine_t(utils::system::io::path::executable_file_directory(), LOCALHOST_KEY_FILE_NAME), PRIVATE_KEY, strlen(PRIVATE_KEY), overwrite);
                }

#endif

                static inline void create_static_file_server(_tstring directory, _tstring cert_path, _tstring private_key_path,
                                                             _tstring host, int port, _tstring basicUserName, _tstring basicPassword,
                                                             std::function<void(const ::httplib::Request &, ::httplib::Response &)> Get,
                                                             std::function<void(const ::httplib::Request &, ::httplib::Response &)> Post) {
                    auto cert = cert_path;
                    auto pem = private_key_path;
                    if (cert.empty() || pem.empty()) {
                        create_new_certificate();
                        cert = utils::system::io::path::combine_t(utils::system::io::path::executable_file_directory(), LOCALHOST_CRT_FILE_NAME);
                        pem = utils::system::io::path::combine_t(utils::system::io::path::executable_file_directory(), LOCALHOST_KEY_FILE_NAME);;
                    }
                    auto sslServer = ::httplib::SSLServer(utils::t2s(cert).c_str(), utils::t2s(pem).c_str());
                    sslServer.Get(".*", Get);
                    sslServer.Post(".*", Post);
                    sslServer.set_basic_auth(utils::t2s(basicUserName), utils::t2s(basicPassword));
                    sslServer.set_mount_point("/", utils::replace_t(utils::t2s(directory), PATH_SEPARATORA, '/'));
                    sslServer.listen(utils::t2s(host).c_str(), port);
                }

                static inline void create_static_file_server(_tstring directory,
                                                             std::function<void(const ::httplib::Request &, ::httplib::Response &)> Get,
                                                             std::function<void(const ::httplib::Request &, ::httplib::Response &)> Post) {
                    create_static_file_server(directory, _tstring(), _tstring(), HOST_ANY, SSL_PORT, _tstring(), _tstring(), Get, Post);
                }

                static inline void create_static_file_server(_tstring directory,
                                                             std::function<void(const ::httplib::Request &, ::httplib::Response &)> Post) {
                    create_static_file_server(directory, _tstring(), _tstring(), HOST_ANY, SSL_PORT, _tstring(), _tstring(), [](const ::httplib::Request &, ::httplib::Response &) {}, Post);
                }

                static inline void create_static_file_server(_tstring directory) {
                    create_static_file_server(directory, _tstring(), _tstring(), HOST_ANY, SSL_PORT, _tstring(), _tstring(),
                                              [](const ::httplib::Request &, ::httplib::Response &) {},
                                              [](const ::httplib::Request &, ::httplib::Response &) {});
                }

                static inline ::httplib::Result Post(std::string host, std::string url, std::string body, const REQUEST &req) {
                    ::httplib::Client cli(host);
                    cli.set_read_timeout(timeout());
                    cli.set_write_timeout(timeout());
                    return cli.Post(url,
                                    req.headers.m_headers,
                                    body,
                                    req.contentType.empty() ? "application/json" : utils::t2s(req.contentType));
                }

            private:
                static int m_timeout;
            };
        };

        class datetime {
        public:
#ifdef WIN32

            static inline _tstring now() {
                SYSTEMTIME systemTime = {0};
                GetLocalTime(&systemTime);
                return utils::format(_T("%04d/%02d/%02d %02d:%02d:%02d.%03d"),
                                     systemTime.wYear,
                                     systemTime.wMonth,
                                     systemTime.wDay,
                                     systemTime.wHour,
                                     systemTime.wMinute,
                                     systemTime.wSecond,
                                     systemTime.wMilliseconds);
            }

#endif
        };

        _wstring() : std::wstring() {}

        _wstring(int i) : std::wstring() {
            this->clear();
            this->append(utils::format(L"%d", i));
        }

#ifdef WIN32

        _wstring(DWORD dw) : std::wstring() {
            this->clear();
            this->append(utils::format(L"%d", dw));
        }

        _wstring(HANDLE h) : std::wstring() {
            this->clear();
            this->append(utils::format(L"0x%llx", h));
        }

#endif

        _wstring(const wchar_t *s) : std::wstring(s) {}

        _wstring(std::wstring s) : std::wstring(s) {}

        _wstring(char* s, UINT cp = CP_ACP) {
            this->clear();
            this->append(s2w(s, cp));
        }

        _wstring(std::string s, UINT cp = CP_ACP) {
            this->clear();
            this->append(s2w(s, cp));
        }

        inline std::shared_ptr<utils::http::curl> to_curl(std::wstring proxyHost = std::wstring(), std::wstring proxyUserName = std::wstring(), std::wstring proxyPassword = std::wstring()) {
            return std::shared_ptr<utils::http::curl>(new http::curl(this->s(), w2s(proxyHost), w2s(proxyUserName), w2s(proxyPassword)));
        }

        inline std::shared_ptr<utils::system::io::directory> to_directory() {
            return std::shared_ptr<utils::system::io::directory>(new utils::system::io::directory(*this));
        }

        inline std::shared_ptr<utils::system::io::path> to_path() {
            return std::shared_ptr<utils::system::io::path>(new utils::system::io::path(*this));
        }

        inline void println(std::wstring prefix, std::wstring suffix) {
#ifdef WIN32
            wprintf(L"[%s] %s%s%s\n", utils::datetime::now().c_str(), prefix.c_str(), this->c_str(), suffix.c_str());
#endif
        }

        inline void println(std::wstring prefix) {
#ifdef WIN32
            this->println(prefix, _tstring());
#endif
        }

        inline void println() {
#ifdef WIN32
            this->println(_tstring(), _tstring());
#endif
        }

        inline wchar_t *allocate_new() {
            wchar_t *buffer;
            if (this->empty()) {
                buffer = reinterpret_cast<LPWSTR>(new wchar_t[sizeof(wchar_t)]);
                memset(buffer, 0, sizeof(wchar_t));
            } else {
                auto buffer_size = (this->size() + 1) * sizeof(wchar_t);
                buffer = reinterpret_cast<LPWSTR>(malloc(buffer_size));
                memset(buffer, 0, buffer_size);
                memcpy(buffer, reinterpret_cast<const char *>(this->c_str()), buffer_size - sizeof(wchar_t));
            }
            return buffer;
        }

        inline std::string s() {
            return w2s(*this);
        }

        inline std::vector<std::wstring> find(std::wstring prefix, std::wstring suffix) {
            auto v = std::vector<std::wstring>();
            auto prefix_ = std::wstring::find(prefix);
            while (prefix_ != std::wstring::npos) {
                auto suffix_ = suffix.empty() ? std::wstring::npos : std::wstring::find(suffix, prefix_ + prefix.size());
                if (suffix_ == std::wstring::npos) {
                    v.push_back(this->substr(prefix_));
                    prefix_ = std::wstring::npos;
                } else {
                    v.push_back(this->substr(prefix_, suffix_ - prefix_ + suffix.size()));
                    prefix_ = std::wstring::find(prefix, suffix_ + suffix.size());
                }
            }
            return v;
        }

        inline std::vector<std::wstring> truncate(std::wstring prefix, std::wstring suffix) {
            auto v = std::vector<std::wstring>();
            for (auto &e: this->find(prefix, suffix)) {
                auto s = _wstring(e);
                s = s.right(s.size() - prefix.size());
                if (!suffix.empty()) {
                    s = s.left(s.size() - suffix.size());
                }
                v.push_back(s);
            }
            return v;
        }

        inline std::wstring right(size_t size) {
            return size >= this->size() ? std::wstring() : this->substr(this->size() - size);
        }

        inline std::wstring left(size_t size) {
            return size >= this->size() ? std::wstring() : this->substr(0, size);
        }

        inline std::shared_ptr<utils::system::io::file> file() {
            return std::shared_ptr<utils::system::io::file>(new utils::system::io::file(*this));
        }

        inline std::shared_ptr<utils::system::io::image> image() {
            return std::shared_ptr<utils::system::io::image>(new utils::system::io::image(*this));
        }

        inline std::shared_ptr<utils::system::io::ofstream> ofstream() {
            return std::shared_ptr<utils::system::io::ofstream>(new utils::system::io::ofstream(*this));
        }

        static inline std::wstring random(int size = 6) {
            static const char alphanum[] = "0123456789"
                                           "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                           "abcdefghijklmnopqrstuvwxyz";
            std::wstring s;
            s.reserve(size);
            for (int i = 0; i < size; ++i) {
                s += alphanum[rand() % (sizeof(alphanum) - 1)];
            }
            return s;
        }

        inline std::wstring replace(std::wstring s, const std::wstring target, const std::wstring replacement, bool once) {
            if (s.empty() || target.empty()) {
                return s;
            }
            using S = std::wstring;
            using C = std::wstring::value_type;
            using N = std::wstring::size_type;
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
                N n = s.size() + replacement_length * (1 + s.size());
                s.reserve(!once ? n : s.size() + replacement_length);
                for (N i = 0; i < n; i += 1 + replacement_length) {
                    util.ins(&s, replacement, i);
                    if (once) break;
                }
                return s;
            }

            N pos = 0;
            while ((pos = s.find(target, pos)) != std::wstring::npos) {
                util.sub(&s, replacement, pos, target_length);
                if (once) return s;
                pos += replacement_length;
            }
            return s;
        }

        inline std::wstring replace(const std::wstring target, const std::wstring replacement, bool once) {
            return replace_t(*this, target, replacement, once);
        }

        inline std::wstring replace(const std::wstring target, const std::wstring replacement) {
            return replace_t(*this, target, replacement, false);
        }
    };
};

class __utils_init__ {
public:
    __utils_init__() {
#ifdef WIN32
        setbuf(stdout, 0);
#endif
    }
} utilsInit;

int utils::http::httplib::m_timeout = CPPHTTPLIB_READ_TIMEOUT_SECOND;
#endif
