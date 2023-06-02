#pragma once

#include "__utils_init__.hpp"
#include "system/thread.hpp"
#include "system/datetime.hpp"
#include "system/process.hpp"
#include "system/console.hpp"
#include "system/io/path.hpp"
#include "http.hpp"
#include "logger.hpp"

namespace utils {
    class _wstring : public std::wstring {
    public:
        _wstring() : std::wstring() {}

        _wstring(int i) : std::wstring() {
            this->clear();
            this->append(_format(L"%d", i));
        }

        _wstring(DWORD dw) : std::wstring() {
            this->clear();
            this->append(_format(L"%d", dw));
        }

        _wstring(HANDLE h) : std::wstring() {
            this->clear();
            this->append(_format(L"0x%llx", h));
        }

        _wstring(const wchar_t* s) : std::wstring(s) {}

        _wstring(std::wstring s) : std::wstring(s) {}

        _wstring(char* s, UINT cp = CP_ACP) {
            this->clear();
            this->append(s2w(s, cp));
        }

        _wstring(std::string s, UINT cp = CP_ACP) {
            this->clear();
            this->append(s2w(s, cp));
        }

        inline std::shared_ptr<http::session> to_curl(std::wstring proxyHost = std::wstring(), std::wstring proxyUserName = std::wstring(), std::wstring proxyPassword = std::wstring()) {
            return std::shared_ptr<http::session>(new http::session(this->s(), w2s(proxyHost), w2s(proxyUserName), w2s(proxyPassword)));
        }

        inline std::shared_ptr<system::io::directory> to_directory() {
            return std::shared_ptr<system::io::directory>(new system::io::directory(*this));
        }

        inline std::shared_ptr<system::io::path> to_path() {
            return std::shared_ptr<system::io::path>(new system::io::path(*this));
        }

        inline std::shared_ptr<http> to_http() {
            return std::shared_ptr<http>(new http(*this));
        }

        inline void println(std::wstring prefix, std::wstring suffix) {
            wprintf(L"[%s] %s%s%s\n", system::datetime::now().c_str(), prefix.c_str(), this->c_str(), suffix.c_str());
        }

        inline void println(std::wstring prefix) {
            this->println(prefix, std::wstring());
        }

        inline void println() {
            this->println(std::wstring(), std::wstring());
        }

        inline wchar_t* allocate_new() {
            wchar_t* buffer;
            if (this->empty()) {
                buffer = reinterpret_cast<LPWSTR>(new wchar_t[sizeof(wchar_t)]);
                memset(buffer, 0, sizeof(wchar_t));
            } else {
                auto buffer_size = (this->size() + 1) * sizeof(wchar_t);
                buffer = reinterpret_cast<LPWSTR>(malloc(buffer_size));
                memset(buffer, 0, buffer_size);
                memcpy(buffer, reinterpret_cast<const char*>(this->c_str()), buffer_size - sizeof(wchar_t));
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
            for (auto& e : this->find(prefix, suffix)) {
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

        inline std::shared_ptr<system::io::file> to_file() {
            return std::shared_ptr<system::io::file>(new system::io::file(*this));
        }

        inline std::shared_ptr<system::io::image> to_image() {
            return std::shared_ptr<system::io::image>(new system::io::image(*this));
        }

        inline std::shared_ptr<system::io::ofstream> to_ofstream() {
            return std::shared_ptr<system::io::ofstream>(new system::io::ofstream(*this));
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

        inline std::wstring replace(const std::wstring find, const std::wstring replacement, bool once) {
            return _replace(*this, find, replacement, once);
        }

        inline std::wstring replace(const std::wstring find, const std::wstring replacement) {
            return _replace(*this, find, replacement);
        }

        inline std::wstring replace(const wchar_t find, const wchar_t replacement) {
            return _replace(*this, find, replacement);
        }

        inline std::wstring lower() {
            return _lower(*this);
        }

        inline std::wstring upper() {
            return _upper(*this);
        }

        inline std::wstring format(int param1) { return _format(this->c_str(), param1); }
        inline std::wstring format(std::string param1) { return _format(this->c_str(), s2w(param1).c_str()); }
        inline std::wstring format(std::wstring param1) { return _format(this->c_str(), param1.c_str()); }
        inline std::wstring format(std::wstring param1, std::wstring param2) { return _format(this->c_str(), param1.c_str(), param2.c_str()); }
        inline std::wstring format(std::wstring param1, int param2) { return _format(this->c_str(), param1.c_str(), param2); }
    };
};
