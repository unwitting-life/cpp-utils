//
// Created by huangyl on 2023/5/5.
//

#pragma once

#include "utils.hpp"
#include "http.hpp"
#include "file.hpp"
#include "dir.hpp"
#include "path.hpp"

namespace _utils {
    namespace strings {
  /*      class _string : public std::string {
        public:
            inline std::shared_ptr<http::curl> curl() {
                return std::shared_ptr<http::curl>(new http::curl(*this));
            }

            inline std::wstring w() {
                return strings::s2w(*this);
            }

            inline std::shared_ptr <std::string> s() {
                return std::shared_ptr<std::string>(new std::string());
            }
        };*/

        class _wstring : public std::wstring {
        public:
            _wstring() : std::wstring() { }

            _wstring(const wchar_t* s) : std::wstring(s) {}

            _wstring(std::wstring s) : std::wstring(s) {}

            _wstring(std::string s, UINT cp = CP_ACP) {
                this->clear();
                this->append(s2w(s, cp));
            }

            inline std::shared_ptr<http::curl> curl(std::wstring proxyHost = std::wstring(), std::wstring proxyUserName = std::wstring(), std::wstring proxyPassword = std::wstring()) {
                return std::shared_ptr<http::curl>(new http::curl(this->s(), strings::w2s(proxyHost), strings::w2s(proxyUserName), strings::w2s(proxyPassword)));
            }

            inline std::shared_ptr<dir::dir> dir() {
                return std::shared_ptr<dir::dir>(new dir::dir(*this));
            }

            inline std::shared_ptr<path::path> path() {
                return std::shared_ptr<path::path>(new path::path(*this));
            }

            inline std::string s() {
                return strings::w2s(*this);
            }

            inline std::vector <std::wstring> find(std::wstring prefix, std::wstring suffix) {
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

            inline std::vector <std::wstring> truncate(std::wstring prefix, std::wstring suffix) {
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
            inline std::shared_ptr<file::file> file() {
                return std::shared_ptr<file::file>(new file::file(*this));
            }
            inline std::shared_ptr<file::image> image() {
                return std::shared_ptr<file::image>(new file::image(*this));
            }
            inline std::shared_ptr<file::ofstream> ofstream() {
                return std::shared_ptr<file::ofstream>(new file::ofstream(*this));
            }
            static inline _wstring format(std::wstring f, ...) {
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
            static inline std::wstring random(int size = 6) {
                static const char alphanum[] =
                    "0123456789"
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
                return strings::replace(*this, target, replacement, once);
            }

            inline std::wstring replace(const std::wstring target, const std::wstring replacement) {
                return strings::replace(*this, target, replacement, false);
            }
        };
    }
}
