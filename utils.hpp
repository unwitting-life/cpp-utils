#ifndef UTILS_H
#define UTILS_H
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
#include <WinSock2.h>
#define wINSOCKAPI_    // stops windows.h including winsock.h
#include <windows.h>
#endif

#ifdef UNICODE
#define string_t _utils::strings::_wstring
#define _tstring std::wstring
#define stringstream_t std::wstringstream
#define ofstream_t std::wofstream
#else
#define string_t utils::strings::_string
#define _tstring std::string
#define stringstream_t std::stringstream
#define ofstream_t std::ofstream
#endif
#define utils string_t

#ifdef WIN32
#define UNCw LR"(\\)"
#define UNC_A R"(\\)"
#define PATH_SEPARATORw L'\\'
#define PATH_SEPARATOR_STRINGw L"\\"
#define PATH_SEPARATOR_A '\\'
#define PATH_SEPARATOR_STRING_A "\\"

#ifdef UNICODE
#define UNC UNCw
#define PATH_SEPARATOR PATH_SEPARATORw
#define PATH_SEPARATOR_STRING PATH_SEPARATOR_STRINGw
#else
#define UNC UNC_A
#define PATH_SEPARATOR PATH_SEPARATOR_A
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
#include <Guiddef.h>
#pragma comment(lib, "comsuppw.lib")
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, __FILE__ "/../curl/builds/libcurl-vc-x64-debug-static-ipv6-sspi-schannel/lib/libcurl_a_debug.lib")
#else
#define L
#define _T(x) x
#define TCHAR char
#define _countof(x) (sizeof(x) / sizeof(x[0]))
#endif

namespace _utils {
    namespace strings {
        inline std::wstring s2w(std::string s, UINT cp = CP_ACP) {
            std::wstring w;
            if (!s.empty()) {
#ifdef WIN32
                auto size = MultiByteToWideChar(cp, 0, s.c_str(), -1, nullptr, 0);
                if (size) {
                    auto p = new wchar_t[size + 1];
                    if (p) {
                        memset(p, 0, (static_cast<size_t>(size) + 1) * sizeof(wchar_t));
                        MultiByteToWideChar(cp, 0, s.c_str(), -1, p, size);
                        w.append(p, size);
                        delete[] p;
                        p = nullptr;
                    }
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
        inline std::string w2s(std::wstring w) {
            // error C4996 : 'std::codecvt_utf8<wchar_t,1114111,(std::codecvt_mode)0>' : warning STL4017 : std::wbuffer_convert, std::wstring_convert,
            // and the <codecvt> header(containing std::codecvt_mode, std::codecvt_utf8, std::codecvt_utf16, and std::codecvt_utf8_utf16) are deprecated in C++17.
            // (The std::codecvt class template is NOT deprecated.) The C++ Standard doesn't provide equivalent non-deprecated functionality; consider using
            // MultiByteToWideChar() and WideCharToMultiByte() from <Windows.h> instead. You can define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATIONwARNING or
            // _SILENCE_ALL_CXX17_DEPRECATIONwARNINGS to suppress this warning.
#ifdef WIN32
            auto s = std::string();
            if (!w.empty()) {
                auto size = WideCharToMultiByte(CP_ACP, 0, w.c_str(), -1, nullptr, 0, nullptr, nullptr);
                char* p = new char[size + 1];
                if (p) {
                    memset(p, 0, sizeof(p[0]) * (size + 1));
                    size = WideCharToMultiByte(CP_ACP, 0, w.c_str(), -1, p, size + 1, nullptr, nullptr);
                    if (size >= 1) {
                        s = std::string(p, static_cast<size_t>(size));
                    }
                    delete[] p;
                    p = nullptr;
                }
            }
            return s;
#else
    // https://stackoverflow.com/questions/11512656/how-to-print-c-wstring-utf-8-characters-to-mac-os-or-unix-terminal
            std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> convert;
            return convert.to_bytes(s);
#endif
        }
        inline std::string t2s(std::wstring s) {
            return w2s(s);
        }
        inline std::string t2s(std::string s) {
            return s;
        }
        inline _tstring t2t(std::wstring s) {
#ifdef UNICODE
            return s;
#else
            return s2w(s);
#endif
        }
        inline _tstring t2t(std::string s) {
#ifdef UNICODE
            return s2w(s);
#else
            return s;
#endif
        }

        inline std::wstring replace(std::wstring s, const std::wstring target, const std::wstring replacement, bool once) {
            if (s.empty() || target.empty()) {
                return s;
            }
            using S = std::wstring;
            using C = std::wstring::value_type;
            using N = std::wstring::size_type;
            struct {
                auto len(const S& s) { return s.size(); }

                auto len(const C* p) { return std::char_traits<C>::length(p); }

                auto len(const C c) { return sizeof(c); }

                auto sub(S* s, const S& t, N pos, N len) { s->replace(pos, len, t); }

                auto sub(S* s, const C* t, N pos, N len) { s->replace(pos, len, t); }

                auto sub(S* s, const C t, N pos, N len) { s->replace(pos, len, 1, t); }

                auto ins(S* s, const S& t, N pos) { s->insert(pos, t); }

                auto ins(S* s, const C* t, N pos) { s->insert(pos, t); }

                auto ins(S* s, const C t, N pos) { s->insert(pos, 1, t); }
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

        inline std::wstring replace(std::wstring s, const std::wstring target, const std::wstring replacement) {
            return replace(s, target, replacement, false);
        }

        inline std::wstring replace(std::wstring s, const wchar_t* target, const wchar_t replacement) {
            return replace(s, target, std::wstring({ replacement }), false);
        }

        inline std::string format(std::string f, ...) {
            std::string s;
            va_list args;
            va_start(args, f);

            // https://stackoverflow.com/questions/16351523/vscwprintf-on-mac-os-x-linux
#ifdef WIN32
            size_t size = (_vscprintf(f.c_str(), args) + 1) * sizeof(char);
#else
            size_t size = (vsnprintf(nullptr, 0, f.c_str(), args) + 1) * sizeof(TCHAR);
#endif
            va_end(args);
            auto p = new char[size];
            if (p) {
                memset(p, 0, size);
                va_start(args, f);
#ifdef WIN32
                _vsnprintf_s(p, size, size - 1, f.c_str(), args);
#else
                vsnprintf(p, size, f.c_str(), args);
#endif
                va_end(args);
                s = p;
                delete[] p;
            }
            return s;
        }

        inline std::wstring format(std::wstring f, ...) {
            std::wstring s;
            va_list args;
            va_start(args, f);

            // https://stackoverflow.com/questions/16351523/vscwprintf-on-mac-os-x-linux
#ifdef WIN32
            size_t size = (_vscwprintf(f.c_str(), args) + 1) * sizeof(wchar_t);
#else
            size_t size = (vsnprintf(nullptr, 0, f.c_str(), args) + 1) * sizeof(TCHAR);
#endif
            va_end(args);
            auto p = new wchar_t[size];
            if (p) {
                memset(p, 0, size);
                va_start(args, f);
#ifdef WIN32
                _vsnwprintf_s(p, size, size - 1, f.c_str(), args);
#else
                vsnprintf(p, size, f.c_str(), args);
#endif
                va_end(args);
                s = p;
                delete[] p;
            }
            return s;
        }

        inline std::wstring right(std::wstring s, size_t size) {
            return size >= s.size() ? std::wstring() : s.substr(s.size() - size);
        }

        inline std::wstring left(std::wstring s, size_t size) {
            return size >= s.size() ? std::wstring() : s.substr(0, size);
        }

        inline std::vector <std::wstring> find(std::wstring s, std::wstring prefix, std::wstring suffix) {
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

        inline std::vector <std::wstring> truncate(std::wstring s, std::wstring prefix, std::wstring suffix) {
            auto v = std::vector<std::wstring>();
            for (auto& e : find(s, prefix, suffix)) {
                auto w = std::wstring(e);
                w = right(w, w.size() - prefix.size());
                if (!suffix.empty()) {
                    w = left(w, w.size() - suffix.size());
                }
                v.push_back(w);
            }
            return v;
        }
    }

    namespace path {
        inline _tstring name(_tstring path) {
            auto s = strings::replace(path, _T("/"), PATH_SEPARATOR);
            auto pos = s.find_last_of(PATH_SEPARATOR);
            return (pos == std::string::npos ? s : s.substr(pos + 1));
        }
        _tstring combine(_tstring path1, _tstring path2) {
            _tstring s1 = strings::replace(path1, _T("/"), PATH_SEPARATOR);
            _tstring s2 = strings::replace(path2, _T("/"), PATH_SEPARATOR);
            if (!s1.empty() && s1.at(s1.size() - 1) != PATH_SEPARATOR) {
                if (s2.empty() || s2.at(0) != PATH_SEPARATOR) {
                    s1 += PATH_SEPARATOR;
                }
            }
            s1 += s2;
            return s1;
        }
        inline _tstring directory(_tstring path) {
            _tstring directoryName_ = path;
            auto pos = path.find_last_of(PATH_SEPARATOR);
            if (pos == _tstring::npos) {
                directoryName_ = _T(".");
            } else {
                directoryName_ = directoryName_.substr(0, pos);
            }
            return directoryName_;
        }
        inline _tstring name_without_extension(_tstring path) {
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
    }
}

#endif