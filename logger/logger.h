#pragma once
#ifdef STDAFX_H
#include "stdafx.h"
#endif
#include <tuple>
#include <stdlib.h>
#include <unordered_map>

#ifdef _MSC_VER
#ifndef string_t
    #ifdef UNICODE
        #define string_t std::wstring
    #else
        #define string_t std::string
    #endif
#endif
#define VARIABLE std::tuple<string_t, string_t>
#define MAKE_VARIABLE(param) std::make_tuple(string_t((_T(#param))), (utils::log::a2s(param)))
#define EMPTY_VARIABLE std::make_tuple(string_t(_T("")), string_t(_T("")))
#ifdef N_LOG
    #define LOG0(l, f)
    #define LOG1(l, f, p1)
    #define LOG2(l, f, p1, p2)
    #define LOG3(l, f, p1, p2, p3)
    #define LOG4(l, f, p1, p2, p3, p4)
    #define LOG5(l, f, p1, p2, p3, p4, p5)
    #define LOG6(l, f, p1, p2, p3, p4, p5, p6)
#else
    #define LOG0(l, f)                          utils::log::logger __logger__(l, _T(__FILE__), __LINE__, _T(#f))
    #define LOG1(l, f, p1)                      utils::log::logger __logger__(l, _T(__FILE__), __LINE__, _T(#f), MAKE_VARIABLE(p1))
    #define LOG2(l, f, p1, p2)                  utils::log::logger __logger__(l, _T(__FILE__), __LINE__, _T(#f), MAKE_VARIABLE(p1), MAKE_VARIABLE(p2))
    #define LOG3(l, f, p1, p2, p3)              utils::log::logger __logger__(l, _T(__FILE__), __LINE__, _T(#f), MAKE_VARIABLE(p1), MAKE_VARIABLE(p2), MAKE_VARIABLE(p3))
    #define LOG4(l, f, p1, p2, p3, p4)          utils::log::logger __logger__(l, _T(__FILE__), __LINE__, _T(#f), MAKE_VARIABLE(p1), MAKE_VARIABLE(p2), MAKE_VARIABLE(p3), MAKE_VARIABLE(p4))
    #define LOG5(l, f, p1, p2, p3, p4, p5)      utils::log::logger __logger__(l, _T(__FILE__), __LINE__, _T(#f), MAKE_VARIABLE(p1), MAKE_VARIABLE(p2), MAKE_VARIABLE(p3), MAKE_VARIABLE(p4), MAKE_VARIABLE(p5))
    #define LOG6(l, f, p1, p2, p3, p4, p5, p6)  utils::log::logger __logger__(l, _T(__FILE__), __LINE__, _T(#f), MAKE_VARIABLE(p1), MAKE_VARIABLE(p2), MAKE_VARIABLE(p3), MAKE_VARIABLE(p4), MAKE_VARIABLE(p5), MAKE_VARIABLE(p6))
#endif
#define ERR0(f)                          LOG0(utils::log::LOGLEVEL::ERR, f)
#define ERR1(f, p1)                      LOG1(utils::log::LOGLEVEL::ERR, f, p1)
#define ERR2(f, p1, p2)                  LOG2(utils::log::LOGLEVEL::ERR, f, p1, p2)
#define ERR3(f, p1, p2, p3)              LOG3(utils::log::LOGLEVEL::ERR, f, p1, p2, p3)
#define ERR4(f, p1, p2, p3, p4)          LOG4(utils::log::LOGLEVEL::ERR, f, p1, p2, p3, p4)
#define ERR5(f, p1, p2, p3, p4, p5)      LOG5(utils::log::LOGLEVEL::ERR, f, p1, p2, p3, p4, p5)
#define ERR6(f, p1, p2, p3, p4, p5, p6)  LOG6(utils::log::LOGLEVEL::ERR, f, p1, p2, p3, p4, p5, p6)
#define WARN0(f)                         LOG0(utils::log::LOGLEVEL::WARNING, f)
#define WARN1(f, p1)                     LOG1(utils::log::LOGLEVEL::WARNING, f, p1)
#define WARN2(f, p1, p2)                 LOG2(utils::log::LOGLEVEL::WARNING, f, p1, p2)
#define WARN3(f, p1, p2, p3)             LOG3(utils::log::LOGLEVEL::WARNING, f, p1, p2, p3)
#define WARN4(f, p1, p2, p3, p4)         LOG4(utils::log::LOGLEVEL::WARNING, f, p1, p2, p3, p4)
#define WARN5(f, p1, p2, p3, p4, p5)     LOG5(utils::log::LOGLEVEL::WARNING, f, p1, p2, p3, p4, p5)
#define WARN6(f, p1, p2, p3, p4, p5, p6) LOG6(utils::log::LOGLEVEL::WARNING, f, p1, p2, p3, p4, p5, p6)
#define INFO0(f)                         LOG0(utils::log::LOGLEVEL::INFORMATION, f)
#define INFO1(f, p1)                     LOG1(utils::log::LOGLEVEL::INFORMATION, f, p1)
#define INFO2(f, p1, p2)                 LOG2(utils::log::LOGLEVEL::INFORMATION, f, p1, p2)
#define INFO3(f, p1, p2, p3)             LOG3(utils::log::LOGLEVEL::INFORMATION, f, p1, p2, p3)
#define INFO4(f, p1, p2, p3, p4)         LOG4(utils::log::LOGLEVEL::INFORMATION, f, p1, p2, p3, p4)
#define INFO5(f, p1, p2, p3, p4, p5)     LOG5(utils::log::LOGLEVEL::INFORMATION, f, p1, p2, p3, p4, p5)
#define INFO6(f, p1, p2, p3, p4, p5, p6) LOG6(utils::log::LOGLEVEL::INFORMATION, f, p1, p2, p3, p4, p5, p6)
#define DBG0(f)                          LOG0(utils::log::LOGLEVEL::DEBUG, f)
#define DBG1(f, p1)                      LOG1(utils::log::LOGLEVEL::DEBUG, f, p1)
#define DBG2(f, p1, p2)                  LOG2(utils::log::LOGLEVEL::DEBUG, f, p1, p2)
#define DBG3(f, p1, p2, p3)              LOG3(utils::log::LOGLEVEL::DEBUG, f, p1, p2, p3)
#define DBG4(f, p1, p2, p3, p4)          LOG4(utils::log::LOGLEVEL::DEBUG, f, p1, p2, p3, p4)
#define DBG5(f, p1, p2, p3, p4, p5)      LOG5(utils::log::LOGLEVEL::DEBUG, f, p1, p2, p3, p4, p5)
#define DBG6(f, p1, p2, p3, p4, p5, p6)  LOG6(utils::log::LOGLEVEL::DEBUG, f, p1, p2, p3, p4, p5, p6)
#ifdef N_LOG
    #define V_ERR(x)
    #define V_WARN(x)
    #define V_INFO(x)
    #define V_DBG(x)
    #define ERR(x)
    #define WARN(x)
    #define INFO(x)
    #define DBG(x)
    #define M_ERR
    #define return_l(x) return x
#else
    #define V_ERR(x)  utils::log::v_err (_T(__FILE__), __LINE__, MAKE_VARIABLE(x))
    #define V_WARN(x) utils::log::v_warn(_T(__FILE__), __LINE__, MAKE_VARIABLE(x))
    #define V_INFO(x) utils::log::v_info(_T(__FILE__), __LINE__, MAKE_VARIABLE(x))
    #define V_DBG(x)  utils::log::v_dbg (_T(__FILE__), __LINE__, MAKE_VARIABLE(x))
    #define ERR(x)    utils::log::err   (_T(__FILE__), __LINE__, x)
    #define WARN(x)   utils::log::warn  (_T(__FILE__), __LINE__, x)
    #define INFO(x)   utils::log::info  (_T(__FILE__), __LINE__, x)
    #define DBG(x)    utils::log::dbg   (_T(__FILE__), __LINE__, x)
    #define M_ERR     ERR(_T("ERROR"))
    #define return_l(x) { auto __return_value__ = (x); __logger__.dispose(std::make_tuple(string_t((_T(#x))), (utils::log::a2s(__return_value__)))); return __return_value__; }
#endif
#define CONSOLE_FOREGROUND_DEFAULT      _T("\033[39m")
#define CONSOLE_FOREGROUND_BLACK        _T("\033[30m")
#define CONSOLE_FOREGROUND_DARK_RED     _T("\033[31m")
#define CONSOLE_FOREGROUND_DARK_GREEN   _T("\033[32m")
#define CONSOLE_FOREGROUND_DARK_YELLOW  _T("\033[33m")
#define CONSOLE_FOREGROUND_DARK_BLUE    _T("\033[34m")
#define CONSOLE_FOREGROUND_DARK_MAGENTA _T("\033[35m")
#define CONSOLE_FOREGROUND_DARK_CYAN    _T("\033[36m")
#define CONSOLE_FOREGROUND_LIGHT_GRAY   _T("\033[37m")
#define CONSOLE_FOREGROUND_DARK_GRAY    _T("\033[90m")
#define CONSOLE_FOREGROUND_RED          _T("\033[91m")
#define CONSOLE_FOREGROUND_GREEN        _T("\033[92m")
#define CONSOLE_FOREGROUND_YELLOW       _T("\033[93m")
#define CONSOLE_FOREGROUND_BLUE         _T("\033[94m")
#define CONSOLE_FOREGROUND_MAGENTA      _T("\033[95m")
#define CONSOLE_FOREGROUND_CYAN         _T("\033[96m")
#define CONSOLE_FOREGROUND_WHITE        _T("\033[97m")
#define CONSOLE_BACKOUND_DEFAULT        _T("\033[49m")
#define CONSOLE_BACKOUND_BLACK          _T("\033[40m")
#define CONSOLE_BACKOUND_DARK_RED       _T("\033[41m")
#define CONSOLE_BACKOUND_DARK_GREEN     _T("\033[42m")
#define CONSOLE_BACKOUND_DARK_YELLOW    _T("\033[43m")
#define CONSOLE_BACKOUND_DARK_BLUE      _T("\033[44m")
#define CONSOLE_BACKOUND_DARK_MAGENTA   _T("\033[45m")
#define CONSOLE_BACKOUND_DARK_CYAN      _T("\033[46m")
#define CONSOLE_BACKOUND_LIGHT_GRAY     _T("\033[47m")
#define CONSOLE_BACKOUND_DARK_GRAY      _T("\033[100m")
#define CONSOLE_BACKOUND_RED            _T("\033[101m")
#define CONSOLE_BACKOUND_GREEN          _T("\033[102m")
#define CONSOLE_BACKOUND_YELLOW         _T("\033[103m")
#define CONSOLE_BACKOUND_BLUE           _T("\033[104m")
#define CONSOLE_BACKOUND_MAGENTA        _T("\033[105m")
#define CONSOLE_BACKOUND_CYAN           _T("\033[106m")
#define CONSOLE_BACKOUND_WHITE          _T("\033[107m")
#define CONSOLE_COLOR_RESET             _T("\033[0m")

namespace utils {
    namespace log {
        constexpr auto TEXT_BUFFER_SIZE = 1000;
        constexpr auto DEFAULT_PADDING_SIZE = 80;
        constexpr auto TAB_PADDING_SIZE = 3;
        constexpr auto SPACE = _T(' ');
        constexpr auto HOROSCOPE = _T('<');
        namespace LOGLEVEL {
            constexpr auto NONE = 0;
            constexpr auto ERR = 1;
            constexpr auto WARNING = 2;
            constexpr auto INFORMATION = 3;
            constexpr auto DEBUG = 4;
        }
        static std::unordered_map<DWORD, int> TAB_INDENTATION;
        static std::vector<string_t> KEYWORD;
        static auto C_KEYWORD = { _T("DWORD"), _T("static"), _T("inline"), _T("void"), _T("extern"), _T("__declspec"), _T("dllexport"), _T("return"),
                                  _T("int"), _T("bool"), _T("__stdcall") };
        static auto SDK_KEYWORD = { _T("S_OK") };
        static int LEVEL = LOGLEVEL::DEBUG;
        inline string_t format(LPCTSTR f, ...) {
            auto s = string_t();
            va_list args = { 0 };
            va_start(args, f);
            auto size = _vsctprintf(f, args) * sizeof(TCHAR) + sizeof(TCHAR);
            va_end(args);
            auto p = new TCHAR[size];
            if (p) {
                memset(p, 0, size);
                va_start(args, f);
                _vsntprintf_s(p, size, size - 1, f, args);
                va_end(args);
                s = p;
                delete[] p;
            }
            return s;
        }
        inline LPCWSTR checkW(LPCWSTR value) {
            auto ptr = value ? value : L"";
            __try {
                ptr = !IsBadReadPtr(ptr, sizeof(wchar_t)) && wcslen(ptr) > 0 ? ptr : L"";
            } __except (EXCEPTION_CONTINUE_EXECUTION) {
                ptr = L"";
            }
            return ptr;
        }
        inline LPCSTR checkA(LPCSTR value) {
            auto ptr = value ? value : "";
            __try {
                ptr = !IsBadReadPtr(ptr, sizeof(char)) && strlen(ptr) > 0 ? ptr : "";
            } __except (EXCEPTION_CONTINUE_EXECUTION) {
                ptr = "";
            }
            return ptr;
        }
        inline LPCTSTR check(LPCTSTR value) {
#ifdef UNICODE
            return checkW(value);
#else
            return checkA(value);
#endif
        }
        inline string_t replace(string_t s, const string_t target, const string_t replacement, bool replace_first = 0, bool replace_empty = 0) {
            if (s.empty() || target.empty()) {
                return s;
            }
            using S = string_t;
            using C = string_t::value_type;
            using N = string_t::size_type;
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
        inline string_t now() {
            SYSTEMTIME time = { 0 };
            GetLocalTime(&time);
            return format(_T("%04d/%02d/%02d %02d:%02d:%02d.%03d"), time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);
        }
        inline string_t removeColors(string_t s) {
            auto w = s;
            w = replace(w, CONSOLE_FOREGROUND_DEFAULT, _T(""));
            w = replace(w, CONSOLE_FOREGROUND_BLACK, _T(""));
            w = replace(w, CONSOLE_FOREGROUND_DARK_RED, _T(""));
            w = replace(w, CONSOLE_FOREGROUND_DARK_GREEN, _T(""));
            w = replace(w, CONSOLE_FOREGROUND_DARK_YELLOW, _T(""));
            w = replace(w, CONSOLE_FOREGROUND_DARK_BLUE, _T(""));
            w = replace(w, CONSOLE_FOREGROUND_DARK_MAGENTA, _T(""));
            w = replace(w, CONSOLE_FOREGROUND_DARK_CYAN, _T(""));
            w = replace(w, CONSOLE_FOREGROUND_LIGHT_GRAY, _T(""));
            w = replace(w, CONSOLE_FOREGROUND_DARK_GRAY, _T(""));
            w = replace(w, CONSOLE_FOREGROUND_RED, _T(""));
            w = replace(w, CONSOLE_FOREGROUND_GREEN, _T(""));
            w = replace(w, CONSOLE_FOREGROUND_YELLOW, _T(""));
            w = replace(w, CONSOLE_FOREGROUND_BLUE, _T(""));
            w = replace(w, CONSOLE_FOREGROUND_MAGENTA, _T(""));
            w = replace(w, CONSOLE_FOREGROUND_CYAN, _T(""));
            w = replace(w, CONSOLE_FOREGROUND_WHITE, _T(""));
            w = replace(w, CONSOLE_BACKOUND_DEFAULT, _T(""));
            w = replace(w, CONSOLE_BACKOUND_BLACK, _T(""));
            w = replace(w, CONSOLE_BACKOUND_DARK_RED, _T(""));
            w = replace(w, CONSOLE_BACKOUND_DARK_GREEN, _T(""));
            w = replace(w, CONSOLE_BACKOUND_DARK_YELLOW, _T(""));
            w = replace(w, CONSOLE_BACKOUND_DARK_BLUE, _T(""));
            w = replace(w, CONSOLE_BACKOUND_DARK_MAGENTA, _T(""));
            w = replace(w, CONSOLE_BACKOUND_DARK_CYAN, _T(""));
            w = replace(w, CONSOLE_BACKOUND_LIGHT_GRAY, _T(""));
            w = replace(w, CONSOLE_BACKOUND_DARK_GRAY, _T(""));
            w = replace(w, CONSOLE_BACKOUND_RED, _T(""));
            w = replace(w, CONSOLE_BACKOUND_GREEN, _T(""));
            w = replace(w, CONSOLE_BACKOUND_YELLOW, _T(""));
            w = replace(w, CONSOLE_BACKOUND_BLUE, _T(""));
            w = replace(w, CONSOLE_BACKOUND_MAGENTA, _T(""));
            w = replace(w, CONSOLE_BACKOUND_CYAN, _T(""));
            w = replace(w, CONSOLE_BACKOUND_WHITE, _T(""));
            w = replace(w, CONSOLE_COLOR_RESET, _T(""));
            return w;
        }
        inline string_t padding(string_t s, TCHAR t, int size = DEFAULT_PADDING_SIZE) {
            auto w = removeColors(s);
            for (auto i = w.size(); i < size; i++) {
                s += t;
            }
            return s;
        }
        inline bool isNum(string_t s) {
            auto b = false;
            if (!s.empty()) {
                if (s[0] == _T('+') || s[0] == _T('-')) {
                    auto bad = false;
                    for (auto i = 0; i < s.size(); i++) {
                        if (s[i] <= _T('0') || s[i] >= _T('9')) {
                            bad = true;
                            break;
                        }
                    }
                    if (!bad) {
                        b = true;
                    }
                }
            }
            return b;
        }
        inline std::string w2s(std::wstring w) {
            auto s = std::string();
            if (!w.empty()) {
                auto size = WideCharToMultiByte(CP_ACP, 0, w.c_str(), static_cast<int>(w.size()), nullptr, 0, nullptr, nullptr) + 1;
                auto p = std::shared_ptr<char>(static_cast<char*>(malloc(size * sizeof(wchar_t))));
                if (p) {
                    auto numberOfBytesWrite = WideCharToMultiByte(CP_ACP, 0, w.c_str(), static_cast<int>(w.size()), p.get(), size, nullptr, nullptr);
                    if (numberOfBytesWrite > 0) {
                        s = std::string(p.get(), numberOfBytesWrite);
                    }
                }
            }
            return s;
        }
        inline std::string w2s(LPCWSTR w) {
            return w2s(std::wstring(checkW(w)));
        }
        inline std::wstring s2w(std::string s) {
            auto w = std::wstring();
            if (!s.empty()) {
                auto size = MultiByteToWideChar(CP_ACP, 0, s.c_str(), static_cast<int>(s.size()), nullptr, 0) + 1;
                auto p = std::shared_ptr<wchar_t>(static_cast<wchar_t*>(malloc(size * sizeof(wchar_t))));
                if (p) {
                    auto numberOfBytesWrite = MultiByteToWideChar(CP_ACP, 0, s.c_str(), static_cast<int>(s.size()), p.get(), size);
                    if (numberOfBytesWrite > 0) {
                        w = std::wstring(p.get(), numberOfBytesWrite);
                    }
                }
            }
            return w;
        }
        inline std::wstring s2w(LPCSTR s) {
            return s2w(std::string(checkA(s)));
        }
        inline bool isEmptyVariable(VARIABLE var) {
            return std::get<0>(var) == _T("");
        }
#ifndef synchronized_thread
        static class critical_sections {
        public:
            critical_sections() : m_pCritical(nullptr) {
                m_pCritical = new CRITICAL_SECTION();
                InitializeCriticalSection(m_pCritical);
            }

            ~critical_sections() {
                DeleteCriticalSection(m_pCritical);
                delete m_pCritical;
                m_pCritical = nullptr;
            }

            void Lock() {
                EnterCriticalSection(m_pCritical);
            }

            void Unlock() {
                LeaveCriticalSection(m_pCritical);
            }

        private:
            LPCRITICAL_SECTION m_pCritical;

        } __critical_section;

        class _synchronized_thread {
        public:
            _synchronized_thread() {
                __critical_section.Lock();
            }

            ~_synchronized_thread() {
                __critical_section.Unlock();
            }
        };

#define synchronized_thread class _synchronized_thread __synchronized_thread
#endif
        inline string_t white(string_t s) { return format(CONSOLE_FOREGROUND_WHITE _T("%s") CONSOLE_COLOR_RESET, s.c_str()); }
        inline string_t red(string_t s) { return format(CONSOLE_FOREGROUND_RED _T("%s") CONSOLE_COLOR_RESET, s.c_str()); }
        inline string_t green(string_t s) { return format(CONSOLE_FOREGROUND_GREEN _T("%s") CONSOLE_COLOR_RESET, s.c_str()); }
        inline string_t yellow(string_t s) { return format(CONSOLE_FOREGROUND_YELLOW _T("%s") CONSOLE_COLOR_RESET, s.c_str()); }
        inline string_t magenta(string_t s) { return format(CONSOLE_FOREGROUND_MAGENTA _T("%s") CONSOLE_COLOR_RESET, s.c_str()); }
        inline string_t cyan(string_t s) { return format(CONSOLE_FOREGROUND_CYAN _T("%s") CONSOLE_COLOR_RESET, s.c_str()); }
        inline string_t darkGray(string_t s) { return format(CONSOLE_FOREGROUND_DARK_GRAY _T("%s") CONSOLE_COLOR_RESET, s.c_str()); }
        inline string_t _tp2s(std::tuple<string_t, string_t> param, bool comma, string_t(color)(string_t)) {
            auto s = string_t();
            auto keyword = false;
            auto& key = std::get<0>(param);
            for (auto& e : SDK_KEYWORD) {
                if (key == e) {
                    keyword = true;
                    break;
                }
            }
            if (keyword) {
                s = format(comma ? _T(", %s") : _T("%s"), color(std::get<0>(param)).c_str());
            } else if (isNum(key)) {
                s = format(comma ? _T(", %s") : _T("%s"), darkGray(std::get<1>(param)).c_str());
            } else {
                s = format(comma ? _T(", %s = %s") : _T("%s = %s"), color(std::get<0>(param)).c_str(), darkGray(std::get<1>(param)).c_str());
            }
            return s;
        }
        inline string_t tp2s(std::tuple<string_t, string_t> param, bool comma) { return _tp2s(param, comma, white); }
        inline string_t tp2se(std::tuple<string_t, string_t> param) { return _tp2s(param, false, red); }
        inline string_t tp2sw(std::tuple<string_t, string_t> param) { return _tp2s(param, false, yellow); }
        inline string_t tp2si(std::tuple<string_t, string_t> param) { return _tp2s(param, false, green); }
        inline string_t tp2sd(std::tuple<string_t, string_t> param) { return _tp2s(param, false, darkGray); }
        inline string_t a2s(string_t value) { return format(_T("\"%s\""), value.c_str()); }
        inline string_t a2s(LPCTSTR value) { return a2s(string_t(check(value))); }
        inline string_t a2s(int value) { return format(_T("%d"), value); }
        inline string_t a2s(HRESULT value) { return format(_T("%d"), value); }
        inline string_t a2s(long long value) { return format(_T("%lld"), value); }
        inline string_t a2s(LPTSTR value[]) { return format(_T("%s"), value ? a2s(value[0]).c_str() : _T("nullptr")); }
        inline string_t a2s(bool(__stdcall* p)(LPCTSTR)) { return p ? format(_T("FARPROC* 0x%08llx"), reinterpret_cast<long long>(p)) : _T("nullptr"); }
#ifdef __AFX_H__
        inline string_t a2s(CStringArray& strArray) { auto s = string_t(); for (auto i = 0; i < strArray.GetCount(); i++) { s += s.empty() ? _T("") : _T(", "); s += strArray.GetAt(i); } return s; }
#endif
        inline string_t a2s(ULONGLONG value) { return format(_T("%ulld"), value); }
        inline string_t highlight(string_t text, bool isFunc) {
            string_t t = text;
            auto hit = false;
            for (auto s : { _T("::"), _T(" ") }) {
                auto pos = text.find_last_of(s);
                if (pos != string_t::npos) {
                    auto temp = text.substr(0, pos + 1);
                    temp += isFunc ? CONSOLE_FOREGROUND_MAGENTA : CONSOLE_FOREGROUND_WHITE;
                    temp += text.substr(pos + 1);
                    temp += CONSOLE_COLOR_RESET;
                    t = temp;
                    hit = true;
                    break;
                }
            }
            if (hit) {
                for (auto& s : log::KEYWORD) {
                    auto pos = t.find(s);
                    if (pos != string_t::npos) {
                        t = replace(t, s, format(_T("%s%s%s"), CONSOLE_FOREGROUND_CYAN, s.c_str(), CONSOLE_COLOR_RESET).c_str());
                    }
                }
                for (auto s : C_KEYWORD) {
                    auto pos = t.find(s);
                    if (pos != string_t::npos) {
                        t = replace(t, s, format(_T("%s%s%s"), CONSOLE_FOREGROUND_BLUE, s, CONSOLE_COLOR_RESET).c_str());
                    }
                }
                t = replace(t, _T("\"C\""), format(_T("%s\"C\"%s"), CONSOLE_FOREGROUND_YELLOW, CONSOLE_COLOR_RESET).c_str());
            } else {
                t = magenta(t);
            }
            return t;
        }
        inline void __stdout(string_t s) {
#ifdef UNICODE
            printf("%s\n", w2s(s).c_str());
#else
            printf("%s\n", s.c_str());
#endif
            OutputDebugString(format(_T("%s\n"), removeColors(s).c_str()).c_str());
        }
        inline void println(LPCTSTR cpp, int line, LPCTSTR text) {
            int indentation = 0;
            auto threadId = GetCurrentThreadId();
            {
                synchronized_thread;
                indentation = (TAB_INDENTATION.find(threadId) == TAB_INDENTATION.end() ? 0 : TAB_INDENTATION[threadId]);
                indentation = indentation < 0 ? 0 : indentation;
            }
            TCHAR drive[TEXT_BUFFER_SIZE] = { 0 };
            TCHAR directory[TEXT_BUFFER_SIZE] = { 0 };
            TCHAR file[TEXT_BUFFER_SIZE] = { 0 };
            TCHAR extension[TEXT_BUFFER_SIZE] = { 0 };
            TCHAR buffer[TEXT_BUFFER_SIZE] = { 0 };
            _tsplitpath_s(cpp, drive, directory, file, extension);
            auto t = padding(format(_T("[\033[97m%s\033[0m][\033[97m%06d\033[0m][\033[97m0x%08X\033[0m][%s%s:%s "), now().c_str(), GetCurrentProcessId(), GetCurrentThreadId(), white(file).c_str(), white(extension).c_str(), white(a2s(line)).c_str()), HOROSCOPE);
            for (auto i = 0; i < indentation * TAB_PADDING_SIZE; i++) { t += log::SPACE; }
            t += _T(" -> ");
            t += highlight(check(text), false);
            __stdout(t);
        }
        inline void v_err(LPCTSTR cpp, int line, std::tuple<string_t, string_t> param) {
            if (LEVEL >= LOGLEVEL::ERR) {
                println(cpp, line, tp2se(param).c_str());
            }
        }
        inline void v_warn(LPCTSTR cpp, int line, std::tuple<string_t, string_t> param) {
            if (LEVEL >= LOGLEVEL::WARNING) {
                println(cpp, line, tp2si(param).c_str());
            }
        }
        inline void v_info(LPCTSTR cpp, int line, std::tuple<string_t, string_t> param) {
            if (LEVEL >= LOGLEVEL::WARNING) {
                println(cpp, line, tp2si(param).c_str());
            }
        }
        inline void v_dbg(LPCTSTR cpp, int line, std::tuple<string_t, string_t> param) {
            if (LEVEL >= LOGLEVEL::WARNING) {
                println(cpp, line, tp2sd(param).c_str());
            }
        }
        inline void err(LPCTSTR cpp, int line, LPCTSTR text) {
            if (LEVEL >= LOGLEVEL::WARNING) {
                println(cpp, line, format(_T("%s"), red(check(text)).c_str()).c_str());
            }
        }
        inline void warn(LPCTSTR cpp, int line, LPCTSTR text) {
            if (LEVEL >= LOGLEVEL::WARNING) {
                println(cpp, line, format(_T("%s"), yellow(check(text)).c_str()).c_str());
            }
        }
        inline void info(LPCTSTR cpp, int line, LPCTSTR text) {
            if (LEVEL >= LOGLEVEL::WARNING) {
                println(cpp, line, format(_T("%s"), green(check(text)).c_str()).c_str());
            }
        }
        inline void dbg(LPCTSTR cpp, int line, LPCTSTR text) {
            if (LEVEL >= LOGLEVEL::WARNING) {
                println(cpp, line, format(_T("%s"), darkGray(check(text)).c_str()).c_str());
            }
        }
        static class _LOGGER_INITIALIZATION {
        public:
            _LOGGER_INITIALIZATION() {
                TCHAR level[100] = { 0 };
                GetEnvironmentVariable(_T("LOG_LEVEL"), level, ARRAYSIZE(level) - 1);
                if (_tcsicmp(level, _T("NONE")) == 0) {
                    LEVEL = LOGLEVEL::NONE;
                } else if (_tcsicmp(level, _T("ERR")) == 0 || _tcsicmp(level, _T("ERROR")) == 0) {
                    LEVEL = LOGLEVEL::ERR;
                } else if (_tcsicmp(level, _T("WARN")) == 0 || _tcsicmp(level, _T("WARNING")) == 0) {
                    LEVEL = LOGLEVEL::WARNING;
                } else if (_tcsicmp(level, _T("INFO")) == 0 || _tcsicmp(level, _T("INFORMATION")) == 0) {
                    LEVEL = LOGLEVEL::INFORMATION;
                } else if (_tcsicmp(level, _T("DBG")) == 0 || _tcsicmp(level, _T("DEBUG")) == 0) {
                    LEVEL = LOGLEVEL::DEBUG;
                } else {
                    LEVEL = LOGLEVEL::DEBUG;
                }
            }
        } __init__logger_;
        class logger {
        public:
            logger(int level, LPCTSTR file, int line, LPCTSTR func,
                std::tuple<string_t, string_t> param1 = EMPTY_VARIABLE,
                std::tuple<string_t, string_t> param2 = EMPTY_VARIABLE,
                std::tuple<string_t, string_t> param3 = EMPTY_VARIABLE,
                std::tuple<string_t, string_t> param4 = EMPTY_VARIABLE,
                std::tuple<string_t, string_t> param5 = EMPTY_VARIABLE,
                std::tuple<string_t, string_t> param6 = EMPTY_VARIABLE,
                std::tuple<string_t, string_t> param7 = EMPTY_VARIABLE,
                std::tuple<string_t, string_t> param8 = EMPTY_VARIABLE,
                std::tuple<string_t, string_t> param9 = EMPTY_VARIABLE) :
                m_level(level), m_isDisposed(false), m_file(file),
                m_line(line), m_func(func), m_text(string_t()),
                m_param1(param1), m_param2(param2), m_param3(param3),
                m_param4(param4), m_param5(param5), m_param6(param6),
                m_param7(param7), m_param8(param8), m_param9(param9) {

                this->enter();
            };
            ~logger() { this->dispose(EMPTY_VARIABLE); };
            void SetText(const string_t& text) { this->m_text = text; }
        private:
            int m_level;
            bool m_isDisposed;
            string_t m_text;
            string_t m_file;
            int m_line;
            string_t m_func;
            std::tuple<string_t, string_t>& m_param1;
            std::tuple<string_t, string_t>& m_param2;
            std::tuple<string_t, string_t>& m_param3;
            std::tuple<string_t, string_t>& m_param4;
            std::tuple<string_t, string_t>& m_param5;
            std::tuple<string_t, string_t>& m_param6;
            std::tuple<string_t, string_t>& m_param7;
            std::tuple<string_t, string_t>& m_param8;
            std::tuple<string_t, string_t>& m_param9;
        public:
            inline void enter() {
                int indentation = 0;
                auto threadId = GetCurrentThreadId();
                {
                    synchronized_thread;
                    indentation = (TAB_INDENTATION.find(threadId) == TAB_INDENTATION.end() ? 0 : TAB_INDENTATION[threadId]);
                    indentation = indentation < 0 ? 0 : indentation;
                    TAB_INDENTATION[threadId] = indentation + 1;
                }
                if (this->m_level >= LEVEL) {
                    string_t text;
                    TCHAR drive[TEXT_BUFFER_SIZE] = { 0 };
                    TCHAR directory[TEXT_BUFFER_SIZE] = { 0 };
                    TCHAR file[TEXT_BUFFER_SIZE] = { 0 };
                    TCHAR extension[TEXT_BUFFER_SIZE] = { 0 };
                    _tsplitpath_s(this->m_file.c_str(), drive, directory, file, extension);
                    auto t = padding(format(_T("[\033[97m%s\033[0m][\033[97m%06d\033[0m][\033[97m0x%08X\033[0m][%s%s:%s "), now().c_str(), GetCurrentProcessId(), GetCurrentThreadId(), white(file).c_str(), white(extension).c_str(), white(a2s(this->m_line)).c_str()), HOROSCOPE);
                    for (auto i = 0; i < indentation * TAB_PADDING_SIZE; i++) { t += log::SPACE; }
                    t += _T(" + ");
                    t += highlight(this->m_func, true);
                    errno_t err = ERROR_NO_MORE_ITEMS;
                    t += _T("(");
                    if (!std::get<0>(this->m_param1).empty()) { t += tp2s(this->m_param1, false); err = S_OK; }
                    if (!std::get<0>(this->m_param2).empty()) { t += tp2s(this->m_param2, true); err = S_OK; }
                    if (!std::get<0>(this->m_param3).empty()) { t += tp2s(this->m_param3, true); err = S_OK; }
                    if (!std::get<0>(this->m_param4).empty()) { t += tp2s(this->m_param4, true); err = S_OK; }
                    if (!std::get<0>(this->m_param5).empty()) { t += tp2s(this->m_param5, true); err = S_OK; }
                    if (!std::get<0>(this->m_param6).empty()) { t += tp2s(this->m_param6, true); err = S_OK; }
                    if (!std::get<0>(this->m_param7).empty()) { t += tp2s(this->m_param7, true); err = S_OK; }
                    if (!std::get<0>(this->m_param8).empty()) { t += tp2s(this->m_param8, true); err = S_OK; }
                    if (!std::get<0>(this->m_param9).empty()) { t += tp2s(this->m_param9, true); err = S_OK; }
                    if (err == ERROR_NO_MORE_ITEMS) {
                        t += _T("void");
                    }
                    t += _T(")");
                    __stdout(t);
                }
            }
            inline void dispose(VARIABLE retVal) {
                int indentation = 0;
                auto threadId = GetCurrentThreadId();
                auto disposed = false;
                {
                    synchronized_thread;
                    disposed = m_isDisposed;
                    if (!disposed) {
                        indentation = (TAB_INDENTATION.find(threadId) == TAB_INDENTATION.end() ? 0 : TAB_INDENTATION[threadId]) - 1;
                        indentation = indentation < 0 ? 0 : indentation;
                        TAB_INDENTATION[threadId] = indentation;
                    }
                    m_isDisposed = true;
                }
                if (this->m_level >= LEVEL && !disposed) {
                    TCHAR drive[TEXT_BUFFER_SIZE] = { 0 };
                    TCHAR directory[TEXT_BUFFER_SIZE] = { 0 };
                    TCHAR file[TEXT_BUFFER_SIZE] = { 0 };
                    TCHAR extension[TEXT_BUFFER_SIZE] = { 0 };
                    TCHAR buffer[TEXT_BUFFER_SIZE] = { 0 };
                    _tsplitpath_s(this->m_file.c_str(), drive, directory, file, extension);
                    auto t = padding(format(_T("[\033[97m%s\033[0m][\033[97m%06d\033[0m][\033[97m0x%08X\033[0m][%s%s:%s "), now().c_str(), GetCurrentProcessId(), GetCurrentThreadId(), white(file).c_str(), white(extension).c_str(), white(a2s(this->m_line)).c_str()), HOROSCOPE);
                    for (auto i = 0; i < indentation * TAB_PADDING_SIZE; i++) { t += log::SPACE; }
                    t += _T(" - ");
                    if (isEmptyVariable(retVal)) {
                        t += highlight(_T("return;"), false);
                    } else {
                        t += highlight(format(_T("return %s;"), tp2s(retVal, false).c_str()), false);
                    }
                    __stdout(t);
                }
            }
        };
    }
}
#endif
