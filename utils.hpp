#pragma once
#include "utils.h"

namespace utils {
    namespace strings {
        inline std::wstring t2w(const std::string &s) {
            std::wstring w;
            if (!s.empty()) {
                size_t pos;
                size_t begin = 0;
#ifdef WIN32
                int size = 0;
                pos = s.find(static_cast<char>(0), begin);
                while (pos != std::string::npos) {
                    auto segment = std::string(&s[begin], pos - begin);
                    auto converted = std::wstring(segment.size() + 1, 0);
                    size = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, &segment[0], segment.size(), &converted[0], converted.length());
                    converted.resize(size);
                    w.append(converted);
                    w.append({ 0 });
                    begin = pos + 1;
                    pos = s.find(static_cast<char>(0), begin);
                }
                if (begin < s.length()) {
                    auto segment = std::string(&s[begin], s.length() - begin);
                    auto converted = std::wstring(segment.size() + 1, 0);
                    size = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, segment.c_str(), segment.size(), &converted[0], converted.length());
                    converted.resize(size);
                    w.append(converted);
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
                    w.append({0});
                    begin = pos + 1;
                    pos = s.find(static_cast<char>(0), begin);
                }
                if (begin < s.length()) {
                    auto segment = std::string(&s[begin], s.length() - begin);
                    auto converted = std::wstring(segment.size(), 0);
                    size = mbstowcs(&converted[0], &segment[0], converted.size());
                    converted.resize(size);
                    w.append(converted);
                }
#endif
            }
            return w;
        }

        inline std::wstring t2w(const std::wstring &s) {
            return s;
        }

        inline std::string t2s(const std::string &s) {
            return s;
        }

        inline std::string t2s(const std::wstring &s) {
#ifdef WIN32
            return (char*)_bstr_t(s.c_str());
#else
            // https://stackoverflow.com/questions/11512656/how-to-print-c-wstring-utf-8-characters-to-mac-os-or-unix-terminal
            std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> convert;
            return convert.to_bytes(s);
#endif
        }

        inline string_t t2t(const std::string &s) {
#ifdef UNICODE
            return t2w(s);
#else
            return t2s(s);
#endif
        }

        inline string_t t2t(const std::wstring &s) {
#ifdef UNICODE
            return t2w(s);
#else
            return t2s(s);
#endif
        }

#ifdef WIN32
        inline std::string u2s(const std::string utf8) {
            std::string s;

            // https://blog.csdn.net/luofeixiongsix/article/details/80245351
            auto size = MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, nullptr, 0);
            auto buffer = new wchar_t[size + 1];
            if (buffer) {
                ZeroMemory(buffer, sizeof(buffer[0]) * (size + 1));
                MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), static_cast<int>(utf8.length()), buffer, size);
                int length = WideCharToMultiByte(CP_ACP, 0, buffer, -1, nullptr, 0, nullptr, nullptr);
                char* p = new char[length + 1];
                if (p) {
                    ZeroMemory(p, sizeof(p[0]) * (length + 1));
                    WideCharToMultiByte(CP_ACP, 0, buffer, size, p, length, nullptr, nullptr);
                    s = p;
                    delete[] p;
                    p = nullptr;
                }
                delete[] buffer;
                buffer = nullptr;
            }
            return s;
        }
#endif

        inline string_t replace(string_t s, const string_t target, const string_t replacement, bool replace_first = 0, bool replace_empty = 0) {
            if (s.empty() || target.empty()) {
                return s;
            }
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

        inline string_t replace(string_t s, const TCHAR target, const TCHAR replacement, bool replace_first = 0, bool replace_empty = 0) {
            string_t target_;
            target_.push_back(target);

            string_t replacement_;
            replacement_.push_back(replacement);

            return replace(s, target_, replacement_, replace_first, replace_empty);
        }

        inline string_t replace(string_t s, const string_t target, const TCHAR replacement, bool replace_first = 0, bool replace_empty = 0) {
            string_t replacement_;
            replacement_.push_back(replacement);

            return replace(s, target, replacement_, replace_first, replace_empty);
        }

        inline string_t replace(string_t s, const TCHAR target, const string_t replacement, bool replace_first = 0, bool replace_empty = 0) {
            string_t target_;
            target_.push_back(target);

            return replace(s, target_, replacement, replace_first, replace_empty);
        }

        inline string_t format(string_t f, ...) {
            string_t s;
            va_list args;
            va_start(args, f);

            // https://stackoverflow.com/questions/16351523/vscwprintf-on-mac-os-x-linux
#ifdef WIN32
            size_t size = (_vsctprintf(f.c_str(), args) + 1) * sizeof(TCHAR);
#else
            size_t size = (vsnprintf(nullptr, 0, f.c_str(), args) + 1) * sizeof(TCHAR);
#endif
            va_end(args);
            auto p = new TCHAR[size];
            if (p) {
                memset(p, 0, size);
                va_start(args, f);
#if WIN32
                _vsntprintf_s(p, size, size - 1, f.c_str(), args);
#else
                vsnprintf(p, size, f.c_str(), args);
#endif
                va_end(args);
                s = p;
                delete[] p;
            }
            return s;
        }

        inline string_t lower(string_t s) {
            string_t lower = s;
            std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
            return lower;
        }

        inline string_t upper(string_t s) {
            string_t upper = s;
            std::transform(upper.begin(), upper.end(), upper.begin(), ::toupper);
            return upper;
        }

        inline bool startsWith(string_t s, string_t find) {
            return s.find(find) == 0;
        }

        inline bool endsWith(string_t s, string_t find) {
            return s.size() >= find.size() && s.substr(s.size() - find.size()) == find;
        }

        inline std::vector<string_t> findAll(string_t s, string_t prefix, string_t suffix) {
            std::vector<string_t> find_;
            auto prefix_ = s.find(prefix);
            while (prefix_ != string_t::npos) {
                auto suffix_ = suffix.empty() ? string_t::npos : s.find(suffix, prefix_ + prefix.size());
                if (suffix_ == string_t::npos) {
                    find_.push_back(s.substr(prefix_));
                } else {
                    find_.push_back(s.substr(prefix_, suffix_ - prefix_ + suffix.size()));
                    prefix_ = s.find(prefix, suffix_ + suffix.size());
                }
            }
            return find_;
        }

        inline string_t find(string_t s, string_t prefix, string_t suffix) {
            auto find_ = findAll(s, prefix, suffix);
            return find_.empty() ? string_t() : find_.at(0);
        }

        inline string_t find(string_t s, string_t prefix) {
            return find(s, prefix, string_t());
        }

        inline string_t left(string_t s, size_t size) {
            return size >= s.size() ? string_t() : s.substr(0, size);
        }

        inline string_t right(string_t s, size_t size) {
            return size >= s.size() ? string_t() : s.substr(s.size() - size);
        }

        inline string_t truncate(string_t s, string_t prefix, string_t suffix) {
            auto truncate_ = string_t();
            truncate_ = find(s, prefix, suffix);
            truncate_ = strings::right(truncate_, truncate_.size() - prefix.size());
            truncate_ = strings::left(truncate_, truncate_.size() - suffix.size());
            return truncate_;
        }

        inline string_t decrease(string_t s, size_t size = 1) {
            return left(s, s.size() >= size ? s.size() - size : 0);
        }

        inline string_t trim(const string_t s) {
            auto trim_ = s;
            while (!trim_.empty() && trim_.at(0) == _T(' ')) {
                trim_ = trim_.substr(1);
            }
            while (!trim_.empty() && trim_.at(trim_.size() - 1) == _T(' ')) {
                trim_ = trim_.substr(0, trim_.size() - 1);
            }
            return trim_;
        }

        inline std::vector<string_t> split(string_t s, string_t delim, bool ignoreEmpty = false) {
            std::vector<string_t> split_;
            std::replace_if(s.begin(), s.end(), [&](const TCHAR &c) {
                return delim.find(c) != string_t::npos;
            }, delim.at(0));
            size_t pos = s.find(delim.at(0));
            string_t substr;
            while (pos != string_t::npos) {
                substr = s.substr(0, pos);
                if (!substr.empty() || !ignoreEmpty) {
                    split_.push_back(substr);
                }
                s.erase(s.begin(), s.begin() + pos + 1);
                pos = s.find(delim.at(0));
            }
            substr = s;
            if (!substr.empty() || !ignoreEmpty) {
                split_.push_back(substr);
            }
            return split_;
        }

        inline bool equalsIgnoreCase(string_t s, string_t compare) {
            return lower(s) == lower(compare);
        }

        inline std::vector<string_t> sort(std::vector<string_t> s, bool descend = false) {
            for (size_t i = 0; i < s.size(); i++) {
                for (size_t j = i + 1; j < s.size(); j++) {
                    if ((descend && s[i].compare(s[j]) < 0) ||
                        (!descend && s[i].compare(s[j]) > 0)) {
                        string_t tmp = s[i];
                        s[i] = s[j];
                        s[j] = tmp;
                    }
                }
            }
            return s;
        }

        inline std::vector<string_t> numbers(string_t s) {
            std::vector<string_t> numbers_;
            string_t number;
            for (auto &e: s) {
                if (e >= '0' && e <= '9') {
                    number += e;
                } else {
                    if (!number.empty()) {
                        numbers_.push_back(number);
                        number.clear();
                    }
                }
            }
            if (!number.empty()) {
                numbers_.push_back(number);
            }
            return numbers_;
        }

        inline int atoi(string_t s) {
            string_t number;
            for (auto &e: s) {
                if (e >= '0' && e <= '9') {
                    number += e;
                }
            }
            return ::atoi(t2s(number).c_str());
        }

        inline string_t number(string_t s) {
            auto number_ = string_t();
            auto vector = numbers(s);
            if (!vector.empty()) {
                number_ = vector[0];
            }
            return number_;
        }

        inline string_t itoa16(int i) {
            std::string s;
            TCHAR hex[100] = {0};
#ifdef WIN32
            _sntprintf_s(hex, _countof(hex) - 1, _countof(hex) - 1, _T("%x"), i);
#else
            snprintf(hex, _countof(hex) - 1, "%x", i);
#endif
            return hex;
        }

        inline string_t hex(int i) {
            return itoa16(i);
        }

        inline std::string encrypt(std::string &data, std::string &key) {
            return easy_encryption::encrypt(data, key);
        }

        inline std::string decrypt(std::string &data, std::string &key) {
            return easy_encryption::decrypt(data, key);
        }
    }

    namespace datetime {
#ifdef WIN32
        inline string_t now() {
            SYSTEMTIME time = { 0 };
            GetLocalTime(&time);
            TCHAR buffer[MAX_PATH] = { 0 };
            _sntprintf_s(buffer, _countof(buffer) - 1, _T("%04d/%02d/%02d %02d:%02d:%02d.%03d"),
                time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);
            return buffer;
        }
#endif
    }

    namespace io {        
        inline bool exists(string_t path) {
            return io::directory::exists(path) || io::file::exists(path);
        }

        inline void remove(string_t path) {
            if (io::directory::exists(path)) {
                io::directory::remove(path);
            }
            if (io::file::exists(path)) {
                io::file::remove(path);
            }
        }
        namespace path {
            inline string_t combine(string_t path1, string_t path2) {
                string_t s1 = strings::replace(path1, "/", PATH_SEPARATOR);
                string_t s2 = strings::replace(path2, "/", PATH_SEPARATOR);
                if (!s1.empty() && s1.at(s1.size() - 1) != PATH_SEPARATOR) {
                    if (s2.empty() || s2.at(0) != PATH_SEPARATOR) {
                        s1 += PATH_SEPARATOR;
                    }
                }
                s1 += s2;
                return s1;
            }

#ifdef WIN32
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

            inline string_t GetExeFileDirectory() {
                TCHAR buffer[MAX_PATH] = { 0 };
                GetModuleFileName(nullptr, buffer, _countof(buffer) - 1);
                auto exe = string_t(buffer);
                auto pos = exe.find_last_of(PATH_SEPARATOR);
                if (pos != string_t::npos) {
                    exe = exe.substr(0, pos);
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
#endif

            inline string_t GetFileName(string_t path) {
                string_t fileName;
                string_t path_ = strings::replace(path, "/", PATH_SEPARATOR);
                auto pos = path_.find_last_of(PATH_SEPARATOR);
                if (pos != string_t::npos) {
                    fileName = path_.substr(pos + 1);
                }
                return fileName;
            }

            inline string_t GetDirectoryPath(string_t path) {
                string_t directoryPath;
                string_t path_ = strings::replace(path, "/", PATH_SEPARATOR);
                auto pos = path_.find_last_of(PATH_SEPARATOR);
                if (pos != string_t::npos) {
                    directoryPath = path_.substr(0, pos);
                }
                return directoryPath;
            }

            inline string_t GetDirectoryName(string_t path) {
                string_t path_ = strings::replace(path, "/", PATH_SEPARATOR);
                string_t directoryName = GetDirectoryPath(path_);
                auto pos = directoryName.find_last_of(PATH_SEPARATOR);
                if (pos != string_t::npos) {
                    directoryName = path_.substr(0, pos);
                }
                return directoryName;
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

            inline string_t GetSpecialFolderPath(string_t path) {
                char buffer[MAX_PATH] = { 0 };
                SHGetSpecialFolderPath(0, buffer, CSIDL_PERSONAL, false);
                return io::path::combine(buffer, path);
            }
        }

        namespace file {
            inline std::vector<string_t> allLines(string_t path) {
                std::vector<string_t> lines;
                std::ifstream in(path);
                if (in) {
                    std::string line;
                    while (getline(in, line)) {
                        lines.push_back(strings::t2t(line));
                    }
                }
                return lines;
            }

            inline string_t allText(string_t path) {
                string_t text;
                std::ifstream in(path);
                if (in) {
                    stringstream_t buffer;
                    buffer << in.rdbuf();
                    text = buffer.str();
                }
                return text;
            }

            inline unsigned long long size(string_t path) {
                return io::file::exists(path) ? std::filesystem::file_size(path) : -1;
            }

            inline void append(string_t path, std::string data) {
                auto access = _T("rb+");
                if (!io::file::exists(path)) {
                    access = _T("wb+");
                }
#ifdef WIN32
                FILE *file = nullptr;
                _tfopen_s(&file, path.c_str(), access);
#else
                auto file = fopen(path.c_str(), access);
#endif
                if (file) {
                    fseek(file, 0, SEEK_END);
                    fwrite(data.c_str(), sizeof(data.c_str()[0]), data.size(), file);
                    fclose(file);
#ifdef WIN32
                    file = nullptr;
#endif
                }
            }

            inline void write(string_t path, std::string data) {
                if (io::directory::exists(io::directory::mkdir(io::directoryPath(path)))) {
#ifdef WIN32
                    FILE *file = nullptr;
                    _tfopen_s(&file, path.c_str(), _T("wb+"));
#else
                    auto file = fopen(path.c_str(), "wb+");
#endif
                    if (file) {
                        fwrite(data.c_str(), sizeof(data.c_str()[0]), data.size(), file);
                        fclose(file);
#ifdef WIN32
                        file = nullptr;
#endif
                    }
                }
            }

            inline string_t name(string_t path) {
                return io::fileName(path);
            }

            inline bool remove(string_t path) {
                auto b = false;
                if (io::file::exists(path)) {
                    b = std::filesystem::remove(path);
                }
                return b;
            }

            inline bool copy(string_t src, string_t dst, bool overwrite = false) {
                auto b = false;
                if (io::file::exists(src)) {
                    if (overwrite && io::file::exists(dst)) {
                        remove(dst);
                    }
                    std::ifstream src_(src, std::ios::binary);
                    std::ofstream dst_(dst, std::ios::binary);
                    dst_ << src_.rdbuf();
                    src_.close();
                    dst_.close();
                    b = true;
                }
                return b;
            }

            inline string_t base64(string_t path) {
                auto s = string_t();
                if (io::file::exists(path)) {
                    char buffer[8192] = {0};
                    FILE *file = nullptr;
#ifdef WIN32
                    _tfopen_s(&file, path.c_str(), _T("rb+"));
#else
                    file = ::fopen(path.c_str(), _T("rb+"));
#endif
                    if (file) {
                        while (!::feof(file)) {
                            auto numberOfBytesRead = ::fread(buffer, sizeof(buffer[0]), _countof(buffer), file);
                            if (numberOfBytesRead > 0) {
                                s.append(buffer, numberOfBytesRead);
                            }
                        }
                        fclose(file);
                        file = nullptr;
                        s = strings::t2t(base64::to_base64(s));
                    }
                }
                return s;
            }

            inline std::vector<string_t> base64(string_t path, int limit) {
                auto s = std::vector<string_t>();
                auto encoded = file::base64(path);
                if (limit > 0) {
                    auto p = (size_t)0;
                    while (p < encoded.size()) {
                        auto snippet = encoded.substr(p, limit);
                        s.push_back(strings::t2t(snippet));
                        p += snippet.size();
                    }
                } else {
                    s.push_back(strings::t2t(encoded));
                }
                return s;
            }

            inline void fwrite(string_t __ptr, FILE *__stream) {
                if (!__ptr.empty() && __stream) {
                    ::fwrite(__ptr.c_str(), __ptr.size(), sizeof(string_t::value_type), __stream);
                    ::fflush(__stream);
                }
            }

            inline bool exists(string_t path) {
                return std::filesystem::exists(path) && !std::filesystem::is_directory(path);
            }
        }

        namespace directory {
            inline std::vector<string_t> files(string_t path, bool recursive = true) {
                std::vector<string_t> files_;
                if (exists(path)) {
                    for (const auto &entry: std::filesystem::directory_iterator(path)) {
#ifdef UNICODE
                        string_t p = reinterpret_cast<LPCWSTR>(entry.path().u16string().c_str());
#else
                        string_t p = entry.path().string();
#endif
                        if (entry.is_directory()) {
                            if (recursive) {
                                for (auto &file: files(p, recursive)) {
                                    files_.push_back(file);
                                }
                            }
                        } else {
                            files_.push_back(p);
                        }
                    }
                }
                return files_;
            }

            inline void pack(string_t src, string_t dst = "") {
                if (io::directory::exists(src)) {
                    auto hpp = dst;
                    if (hpp.empty()) {
                        hpp = _T("auto_gen.hpp");
                    }
                    io::file::remove(hpp);
                    FILE *p = nullptr;
#ifdef WIN32
                    _tfopen_s(&p, hpp.c_str(), _T("ab+"));
#else
                    p = ::fopen(hpp.c_str(), _T("ab+"));
#endif
                    if (p) {
                        auto files = io::directory::files(src);
                        io::file::fwrite(_T("#pragma once\n"), p);
                        io::file::fwrite(_T("#include <string>\n"), p);
                        io::file::fwrite(_T("namespace auto_gen {\n"), p);
                        io::file::fwrite(_T("   inline std::string get_base64_chars() {\n"), p);
                        io::file::fwrite(_T("       static std::string base64_chars = \"ABCDEFGHIJKLMNOPQRSTUVWXYZ\"\n"), p);
                        io::file::fwrite(_T("                                         \"abcdefghijklmnopqrstuvwxyz\"\n"), p);
                        io::file::fwrite(_T("                                         \"0123456789+/\";\n"), p);
                        io::file::fwrite(_T("       return base64_chars;\n"), p);
                        io::file::fwrite(_T("   }\n"), p);
                        io::file::fwrite(_T(""), p);
                        io::file::fwrite(_T("   inline std::string from_base64(std::string const &data) {\n"), p);
                        io::file::fwrite(_T("       int counter = 0;\n"), p);
                        io::file::fwrite(_T("       size_t bit_stream = 0;\n"), p);
                        io::file::fwrite(_T("       std::string decoded;\n"), p);
                        io::file::fwrite(_T("       size_t offset;\n"), p);
                        io::file::fwrite(_T("       const std::string base64_chars = get_base64_chars();\n"), p);
                        io::file::fwrite(_T("       for (unsigned char c : data) {\n"), p);
                        io::file::fwrite(_T("           auto num_val = base64_chars.find(c);\n"), p);
                        io::file::fwrite(_T("           if (num_val != std::string::npos) {\n"), p);
                        io::file::fwrite(_T("               offset = 18 - counter % 4 * 6;\n"), p);
                        io::file::fwrite(_T("               bit_stream += num_val << offset;\n"), p);
                        io::file::fwrite(_T("               if (offset == 12) {\n"), p);
                        io::file::fwrite(_T("                   decoded += static_cast<char>(bit_stream >> 16 & 0xff);\n"), p);
                        io::file::fwrite(_T("               }\n"), p);
                        io::file::fwrite(_T("               if (offset == 6) {\n"), p);
                        io::file::fwrite(_T("                   decoded += static_cast<char>(bit_stream >> 8 & 0xff);\n"), p);
                        io::file::fwrite(_T("               }\n"), p);
                        io::file::fwrite(_T("               if (offset == 0 && counter != 4) {\n"), p);
                        io::file::fwrite(_T("                   decoded += static_cast<char>(bit_stream & 0xff);\n"), p);
                        io::file::fwrite(_T("                   bit_stream = 0;\n"), p);
                        io::file::fwrite(_T("               }\n"), p);
                        io::file::fwrite(_T("           } else if (c != '=') {\n"), p);
                        io::file::fwrite(_T("               return std::string();\n"), p);
                        io::file::fwrite(_T("           }\n"), p);
                        io::file::fwrite(_T("           counter++;\n"), p);
                        io::file::fwrite(_T("       }\n"), p);
                        io::file::fwrite(_T("       return decoded;\n"), p);
                        io::file::fwrite(_T("   }\n"), p);
                        io::file::fwrite(_T(""), p);
                        io::file::fwrite(_T("   std::string res(const char* name) {\n"), p);
                        for (auto &f: files) {
                            auto name = f.substr(src.size());
                            io::file::fwrite(strings::format("        if (strcmp(name, \"%s\") == 0) {\n", name.c_str()), p);
                            auto count = 0;
                            for (auto &base64: io::file::base64(f, 100)) {
                                if (count == 0) {
                                    io::file::fwrite(strings::format("            auto s = \"%s\"", base64.c_str()), p);
                                } else {
                                    io::file::fwrite(strings::format("\n                     \"%s\"", base64.c_str()), p);
                                }
                                count++;
                            }
                            if (count > 0) {
                                io::file::fwrite(_T(";\n"), p);
                            }
                            io::file::fwrite(strings::format("            return from_base64(s);\n", io::fileName(f).c_str()), p);
                            io::file::fwrite(strings::format("        }\n"), p);
                        }
                        io::file::fwrite(_T("        return std::string();\n"), p);
                        io::file::fwrite(_T("    }\n"), p);
                        io::file::fwrite(_T("    std::vector<std::string> files() {\n"), p);
                        io::file::fwrite(_T("        static std::vector<std::string> f;\n"), p);
                        io::file::fwrite(_T("        if (f.empty()) {\n"), p);
                        for (auto &f: files) {
                            auto name = f.substr(src.size());
                            io::file::fwrite(strings::format(_T("            f.push_back(\"%s\");\n"), name.c_str()), p);
                        }
                        io::file::fwrite(_T("        }\n"), p);
                        io::file::fwrite(_T("        return f;\n"), p);
                        io::file::fwrite(_T("    }\n"), p);
                        io::file::fwrite(_T("}\n"), p);
                        fclose(p);
                    }
                }
            }

            inline string_t mkdir(const string_t path) {
                if (!path.empty()) {
                    string_t::size_type tmp_pos_begin = 0;
                    string_t::size_type tmp_pos;
#ifdef WIN32
                    if (path.find(UNC) == 0) {
                        tmp_pos = path.find(PATH_SEPARATOR, _tcslen(UNC));
                    }
                    else {
                        tmp_pos = path.find(PATH_SEPARATOR, tmp_pos_begin);
                    }
#else
                    tmp_pos = path.find(PATH_SEPARATOR, tmp_pos_begin);
#endif
                    while (tmp_pos != path.npos) {
                        string_t tmpdir = path.substr(0, tmp_pos);
                        if (tmpdir.empty()) {
                            return path;
                        }
                        if (!std::filesystem::exists(tmpdir)) {
#ifdef WIN32
                            ::CreateDirectory(tmpdir.c_str(), nullptr);
#else
                            ::mkdir(tmpdir.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
#endif
                        }
                        tmp_pos_begin = tmp_pos + 1;
                        tmp_pos = path.find(PATH_SEPARATOR, tmp_pos_begin);
                    }
                    if (!std::filesystem::exists(path)) {
#ifdef WIN32
                        ::CreateDirectory(path.c_str(), nullptr);
#else
                        ::mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
#endif
                    }
                }
                return path;
            }

            inline bool exists(string_t path) {
                return std::filesystem::exists(path) && std::filesystem::is_directory(path);
            }

            inline bool is_directory(string_t path) {
                return exists(path);
            }

            inline uintmax_t remove(string_t path, bool recursive) {
                return recursive ? std::filesystem::remove_all(path) : (std::filesystem::remove(path) ? 1 : 0);
            }
        }

        inline std::vector<string_t> GetDirectories(string_t path, bool recursive = false) {
            std::vector<string_t> directories_;
            if (directory::exists(path)) {
                for (const auto &entry: std::filesystem::directory_iterator(path)) {
                    if (entry.is_directory()) {
#ifdef UNICODE
                        directories_.push_back(entry.path().wstring());
#else
                        directories_.push_back(entry.path().string());
#endif
                        if (recursive) {
#ifdef UNICODE
                            for (auto& directory : GetDirectories(entry.path().wstring(), recursive)) {
#else
                            for (auto& directory : GetDirectories(entry.path().string(), recursive)) {
#endif
                                directories_.push_back(directory);
                            }
                        }
                    }
                }
            }
            return directories_;
        }

        inline string_t directoryPath(string_t path) {
            string_t directoryName_ = path;
            auto pos = path.find_last_of(PATH_SEPARATOR);
            if (pos == string_t::npos) {
                directoryName_ = _T(".");
            } else {
                directoryName_ = directoryName_.substr(0, pos);
            }
            return directoryName_;
        }

        inline string_t fileName(string_t path) {
            string_t fileName_ = path;
            auto pos = path.find_last_of(PATH_SEPARATOR);
            if (pos != string_t::npos) {
                fileName_ = fileName_.substr(pos + 1);
            }
            return fileName_;
        }

        inline string_t GetFileNameWithoutExtension(string_t path) {
            string_t fileNameWithoutExtension = path;
            auto pos = path.find_last_of(PATH_SEPARATOR);
            if (pos != string_t::npos) {
                fileNameWithoutExtension = fileNameWithoutExtension.substr(pos + 1);
            }
            pos = fileNameWithoutExtension.find_last_of(_T("."));
            if (pos != string_t::npos) {
                fileNameWithoutExtension = fileNameWithoutExtension.substr(0, pos);
            }
            return fileNameWithoutExtension;
        }

        inline string_t GetFileExtensionName(string_t path) {
            string_t fileExtensionName;
            auto pos = path.find_last_of(_T("."));
            if (pos != string_t::npos) {
                fileExtensionName = path.substr(pos + 1);
            }
            return fileExtensionName;
        }

        inline string_t rename(string_t __old, string_t __new) {
            ::rename(strings::t2s(__old).c_str(), strings::t2s(__new).c_str());
            return __new;
        }
    }

#ifdef WIN32
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
                        hash_ += strings::upper(strings::hex(p[i]));
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

    namespace threading {
        static LPCRITICAL_SECTION pCriticalSection = nullptr;
        inline LPCRITICAL_SECTION GetCriticalSection() {
            if (!pCriticalSection) {
                pCriticalSection = new CRITICAL_SECTION();
                InitializeCriticalSection(pCriticalSection);
            }
            return pCriticalSection;
        }
    }

    namespace console {
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

        inline void println(std::vector<CONSOLE_TEXT> words, bool crlf = true) {
            static HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            if (hConsole != INVALID_HANDLE_VALUE) {
                EnterCriticalSection(utils::threading::GetCriticalSection());
                SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);

                string_t line;
                auto datetime = strings::format(_T("[%s] "), datetime::now().c_str());
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
                log::write(line);
                LeaveCriticalSection(utils::threading::GetCriticalSection());
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
    }

    namespace log {
#ifdef WIN32
        inline void write(string_t log) {
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
                text = strings::t2s(log);
                fwrite(log.c_str(), sizeof(log.c_str()[0]), log.size(), file);
                fclose(file);
                file = nullptr;
            }
        }
#endif
    }
#endif
}
#ifdef JSON_SUPPORT
#include "nlohmann/json/3.11.2+/json.hpp"
#define JSON_INT(json, key) (json.contains(key) && json[key].is_number() ? json[key].get<int>() : 0)
#define JSON_STRING(json, key) (json.contains(key) && json[key].is_string() ? json[key].get<std::string>() : std::string())
#endif

#ifdef CPPHTTPLIB_HTTPLIB_SUPPORT
#include "httpLib.hpp"
#endif

#ifdef KEYBOARD_SUPPORT
#include "keyboard.hpp"
#endif

namespace utils {
    class __init {
    public:
        __init() {
#ifdef CPPHTTPLIB_HTTPLIB_SUPPORT
            httplib::__init();
#endif
        }

        ~__init() {

        }
    };
    static class __init __init;
}