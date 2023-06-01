#pragma once

#include "__utils_init__.hpp"

#ifndef UTILS_HPP
#define UTILS_HPP
#pragma message ("* ********************************************************************************* *")
#pragma message ("* URL    : https://github.com/unwitting-life/cpp-utils                              *")
#pragma message ("* License: https://creativecommons.org/licenses/by-nc/4.0/                          *")
#pragma message ("* Author : Yuanlei Huang                                                            *")
#pragma message ("* Latest : 2023/05/30                                                               *")
#pragma message ("* ********************************************************************************* *")

#define synchronsize(x) std::lock_guard<std::recursive_mutex> _recursive_mutex(*utils::system::threading::find_recursive_mutex((x)))

class _utils {
public:
    class _wstring : public std::wstring {
    public:
        static inline std::wstring s2w(std::string s, UINT cp = CP_ACP) {
            std::wstring w;
            if (!s.empty()) {
                auto size = MultiByteToWideChar(cp, 0, s.c_str(), -1, nullptr, 0);
                if (size) {
                    auto p = std::shared_ptr<wchar_t>(new wchar_t[size + 1]);
                    memset(p.get(), 0, (static_cast<size_t>(size) + 1) * sizeof(wchar_t));
                    MultiByteToWideChar(cp, 0, s.c_str(), -1, p.get(), size);
                    w.append(p.get(), size);
                }
            }
            return w;
        }

        static inline std::string w2s(std::wstring w) {
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
        }

        static inline std::string t2s(std::wstring s) {
            return w2s(s);
        }

        static inline std::string t2s(std::string s) {
            return s;
        }

        static inline std::wstring t2t(std::wstring s) {
            return s;
        }

        static inline std::wstring t2t(std::string s) {
            return s2w(s);
        }

        static inline std::string _replace(std::string s, const std::string target, const std::string replacement, bool once) {
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

        static inline std::wstring _replace(std::wstring s, const std::wstring target, const std::wstring replacement, bool once) {
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

        static inline std::wstring _replace(std::wstring s, const std::wstring target, const std::wstring replacement) {
            return _replace(s, target, replacement, false);
        }

        static inline std::wstring _replace(std::wstring s, const wchar_t *target, const wchar_t replacement) {
            return _replace(s, target, std::wstring({replacement}), false);
        }

        static inline std::wstring _replace(std::wstring s, const wchar_t find, const wchar_t replacement) {
            return _replace(s, std::wstring({ find }), std::wstring({ replacement }), false);
        }

        static inline std::string format(std::string f, ...) {
            //@formatter:off
            std::string s;
            va_list args;
            va_start(args, f);
            size_t size = (_vscprintf(f.c_str(), args) + 1) * sizeof(char);
            va_end(args);
            auto p = std::shared_ptr<char>(new char[size]);
            memset(p.get(), 0, size);
            va_start(args, f);
            _vsnprintf_s(p.get(), size, size - 1, f.c_str(), args);
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
            size_t size = (_vscwprintf(f.c_str(), args) + 1) * sizeof(wchar_t);
            va_end(args);
            auto p = std::shared_ptr<wchar_t>(new wchar_t[size]);
            memset(p.get(), 0, size);
            va_start(args, f);
            _vsnwprintf_s(p.get(), size, size - 1, f.c_str(), args);
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

        static inline int _atoi(std::string s) {
            std::string number;
            for (auto& e : s) {
                if (e >= '0' && e <= '9') {
                    number += e;
                }
            }
            return ::atoi(number.c_str());
        }

        static inline int _wtoi(std::wstring s) {
            std::wstring number;
            for (auto& e : s) {
                if (e >= L'0' && e <= L'9') {
                    number += e;
                }
            }
            return ::_wtoi(number.c_str());
        }

        static inline std::wstring _lower(std::wstring s) {
            auto upper = std::wstring(s);
            std::transform(upper.begin(), upper.end(), upper.begin(), ::towlower);
            return upper;
        }

        static inline std::string _lower(std::string s) {
            auto upper = std::string(s);
            std::transform(upper.begin(), upper.end(), upper.begin(), ::tolower);
            return upper;
        }

        static inline std::wstring _upper(std::wstring s) {
            auto upper = std::wstring(s);
            std::transform(upper.begin(), upper.end(), upper.begin(), ::towupper);
            return upper;
        }

        class _base_wstring {
        public:
            _base_wstring(std::wstring s) : m_str(s) {}

            const LPCWSTR c_str() { return this->m_str.c_str(); };

        protected:
            std::wstring m_str;
        };

        class _base_string {
        public:
            _base_string(std::string s) : m_str(s) {}

            const LPCSTR c_str() { return this->m_str.c_str(); };

        protected:
            std::string m_str;
        };

        class system {
        public:
            class process {
            public:
                static inline DWORD get_parent_process_id() {
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
                }

                static inline long start(std::wstring exe, std::wstring params, std::wstring workingDirectory, std::function<void(std::string _stdout)> _stdout) {
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

                static inline long start(std::wstring exe, std::wstring params, std::wstring workingDirectory) {
                    return start(exe, params, workingDirectory, [](std::string _stdout) {});
                }

                static inline long start(std::wstring exe, std::wstring params) {
                    return start(exe, params, std::wstring(), [](std::string _stdout) {});
                }

                static inline long start(std::wstring cmd) {
                    return start(std::wstring(), cmd, std::wstring(), [](std::string _stdout) {});
                }

                static inline HANDLE shell_execute(std::wstring exe, std::wstring params, std::wstring workingDirectory) {
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

                static inline HANDLE shell_execute(std::wstring exe, std::wstring params) {
                    return shell_execute(exe, params, std::wstring());
                }

                static inline HANDLE shell_execute(std::wstring exe) {
                    return shell_execute(exe, std::wstring(), std::wstring());
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
                class path : public _base_wstring {
                public:
                    static inline bool exists(std::wstring path) {
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

                    static inline bool is_file(std::wstring path) {
                        return exists(path) && !std::filesystem::is_directory(path);
                    }

                    static inline bool _is_directory(std::wstring path) {
                        return exists(path) && std::filesystem::is_directory(path);
                    }

                    static inline uintmax_t _remove_directory(std::wstring path, bool recursive) {
                        return recursive ? std::filesystem::remove_all(path) : (std::filesystem::remove(path) ? 1 : 0);
                    }

                    static inline std::wstring _name(std::wstring path) {
                        auto s = _replace(path, _T("/"), PATH_SEPARATOR);
                        auto pos = s.find_last_of(PATH_SEPARATOR);
                        return (pos == std::string::npos ? s : s.substr(pos + 1));
                    }

                    static inline std::wstring combine_t(std::wstring path1, std::wstring path2) {
                        std::wstring s1 = _replace(path1, _T("/"), PATH_SEPARATOR);
                        std::wstring s2 = _replace(path2, _T("/"), PATH_SEPARATOR);
                        if (!s1.empty() && s1.at(s1.size() - 1) != PATH_SEPARATOR) {
                            if (s2.empty() || s2.at(0) != PATH_SEPARATOR) {
                                s1 += PATH_SEPARATOR;
                            }
                        }
                        s1 += s2;
                        return s1;
                    }

                    static inline std::wstring _directory(std::wstring path) {
                        std::wstring directoryName_ = path;
                        auto pos = path.find_last_of(PATH_SEPARATOR);
                        if (pos == std::wstring::npos) {
                            directoryName_ = _T(".");
                        } else {
                            directoryName_ = directoryName_.substr(0, pos);
                        }
                        return directoryName_;
                    }

                    static inline std::wstring executable_file_directory() {
                        TCHAR buffer[MAX_PATH] = {0};
                        GetModuleFileName(nullptr, buffer, _countof(buffer) - 1);
                        auto exe = std::wstring(buffer);
                        auto pos = exe.find_last_of(PATH_SEPARATOR);
                        if (pos != std::wstring::npos) {
                            exe = exe.substr(0, pos);
                        }
                        return exe;
                    }

                    static inline std::wstring working_directory() {
                        auto workingDirectory = new TCHAR[MAX_PATH];
                        GetCurrentDirectory(MAX_PATH, workingDirectory);
                        return workingDirectory;
                    }

                    static inline std::wstring _extension(std::wstring path) {
                        std::wstring s = path;
                        auto pos = path.find_last_of(PATH_SEPARATOR);
                        if (pos != std::wstring::npos) {
                            s = s.substr(pos + 1);
                        }
                        pos = s.find_last_of(_T("."));
                        if (pos != std::wstring::npos) {
                            s = s.substr(pos);
                        }
                        return s;
                    }

                    static inline std::wstring _name_without_extension(std::wstring path) {
                        std::wstring s = path;
                        auto pos = path.find_last_of(PATH_SEPARATOR);
                        if (pos != std::wstring::npos) {
                            s = s.substr(pos + 1);
                        }
                        pos = s.find_last_of(_T("."));
                        if (pos != std::wstring::npos) {
                            s = s.substr(0, pos);
                        }
                        return s;
                    }

                public:
                    path(std::wstring path) : _base_wstring(path) { }

                    std::wstring combine(std::wstring path) {
                        return utils::system::io::path::combine_t(this->m_str, path);
                    }

                    std::wstring name() {
                        return utils::system::io::path::_name(this->m_str);
                    }

                    std::wstring extension() {
                        return utils::system::io::path::_extension(this->m_str);
                    }

                    std::wstring name_without_extension() {
                        return utils::system::io::path::_name_without_extension(this->m_str);
                    }

                    std::wstring directory() {
                        return utils::system::io::path::_directory(this->m_str);
                    }
                };

                class file : public _base_wstring {
                public:
                    file(std::wstring path) : _base_wstring(path) { }

                    bool exists() {
                        auto b = false;
                        auto path = std::wstring(this->m_str);
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

                    bool move(std::wstring newPath) {
                        auto b = true;
                        auto dst = std::wstring(newPath);
                        try {
                            if (std::filesystem::exists(dst) && std::filesystem::is_directory(dst)) {
                                dst = utils::system::io::path::combine_t(newPath, utils::system::io::path::_name(this->m_str));
                            }
                            if (std::filesystem::exists(dst)) {
                                if (std::filesystem::is_directory(dst)) {
                                    b = false;
                                } else {
                                    b = std::filesystem::remove(dst);
                                }
                            }
                        } catch (...) {
                            b = false;
                        }
                        if (b) {
                            b = ::rename(t2s(this->m_str).c_str(), t2s(dst).c_str()) == 0;
                            if (b) {
                                this->m_str = dst;
                            }
                        }
                        return b;
                    }

                    bool rename(std::wstring newPath) {
                        return this->move(newPath);
                    }

                    inline unsigned long long size() {
                        return this->exists() ? std::filesystem::file_size(this->m_str) : -1;
                    }

                    inline std::string content() {
                        auto s = std::string();
                        if (this->exists()) {
                            FILE* file = nullptr;
                            _wfopen_s(&file, this->m_str.c_str(), L"rb+");
                            if (file) {
                                fseek(file, 0, SEEK_END);
                                auto size = ftell(file);
                                if (size > 0) {
                                    fseek(file, 0, SEEK_SET);
                                    auto p = std::shared_ptr<char>( new char[size]);
                                    fread(p.get(), size, sizeof(p.get()[0]), file);
                                    s.append(p.get(), size);
                                }
                                fclose(file);
                            }
                        }
                        return s;
                    }
                };

                class ofstream : public file {
                public:
                    static inline void writeBytes(std::wstring file, const char *data, size_t size, bool createNew = true) {
                        auto f = ofstream(file, createNew);
                        f.write(data, size);
                        f.close();
                    }

                    static inline void appendBytes(std::wstring file, const char *data, size_t size) {
                        ofstream::writeBytes(file, data, size, false);
                    }

                    ofstream(std::wstring path, bool createNew = false) : file(path), m_closed(false) {
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

                    inline void write(const std::string& data) {
                        this->write(data.c_str(), data.size());
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
                    image(std::wstring file) : file(file)  {}
                    bool png(const std::wstring dst = std::wstring()) {
                        auto path = std::wstring(dst);
                        if (path.empty()) {
                            auto png = utils::format(_T("%s.png"), utils::system::io::path::_name_without_extension(this->m_str));
                            path = utils::system::io::path::combine_t(utils::system::io::path::_directory(this->m_str), png.c_str());
                        }
                        CImage image;
                        image.Load(this->m_str.c_str());
                        return image.Save(path.c_str(), Gdiplus::ImageFormatPNG) == S_OK;
                    }
                };

                class directory : public _base_wstring {
                public :
                    static inline std::vector<std::wstring> enumerate_t(std::wstring path, bool recursive = true) {
                        std::vector<std::wstring> directories;
                        if (path::_is_directory(path)) {
                            for (const auto &entry: std::filesystem::directory_iterator(path)) {
                                std::wstring p = reinterpret_cast<LPCTSTR>(entry.path().u16string().c_str());
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

                    directory(std::wstring path) : _base_wstring(path) {}

                    inline std::wstring mkdir() {
                        if (!this->m_str.empty()) {
                            std::wstring::size_type tmp_pos_begin = 0;
                            std::wstring::size_type tmp_pos;
                            if (this->m_str.find(UNC) == 0) {
                                tmp_pos = this->m_str.find(PATH_SEPARATOR, _tcslen(UNC));
                            } else {
                                tmp_pos = this->m_str.find(PATH_SEPARATOR, tmp_pos_begin);
                            }
                            while (tmp_pos != std::wstring::npos) {
                                auto tmpdir = this->m_str.substr(0, tmp_pos);
                                if (tmpdir.empty()) {
                                    return this->m_str;
                                }
                                if (!std::filesystem::exists(tmpdir)) {
                                    ::CreateDirectory(tmpdir.c_str(), nullptr);
                                }
                                tmp_pos_begin = tmp_pos + 1;
                                tmp_pos = this->m_str.find(PATH_SEPARATOR, tmp_pos_begin);
                            }
                            if (!std::filesystem::exists(this->m_str)) {
                                ::CreateDirectory(this->m_str.c_str(), nullptr);
                            }
                        }
                        return this->m_str;
                    }

                    bool move(std::wstring newPath) {
                        auto b = ::rename(t2s(this->m_str).c_str(), t2s(newPath).c_str()) == 0;
                        this->m_str = newPath;
                        return b;
                    }

                    std::wstring name() {
                        std::wstring name = utils::_replace(this->m_str, _T("/"), PATH_SEPARATOR);
                        auto pos = name.find_last_of(PATH_SEPARATOR);
                        if (pos != std::wstring::npos) {
                            name = name.substr(pos + 1);
                        }
                        return name;
                    }

                    bool rename(std::wstring newPath) {
                        return this->move(newPath);
                    }

                    std::vector<std::wstring> enumerate(bool recursive = true) {
                        return enumerate_t(this->m_str, recursive);
                    }

                    bool exists() {
                        return utils::system::io::path::_is_directory(this->m_str);
                    }

                    uintmax_t remove(bool recursive = true) {
                        return utils::system::io::path::_remove_directory(this->m_str, true);
                    }
                };
            };

            class threading {
            public:
                static constexpr auto mutex = _T("utils::threading");
                static inline std::recursive_mutex* find_recursive_mutex(std::wstring recursive_mutex_name) {
                    std::lock_guard<std::recursive_mutex> recursive_mutex(_utils_init_.mutex_singleton);
                    std::recursive_mutex* p = nullptr;
                    if (_utils_init_.mutexes.contains(recursive_mutex_name)) {
                        p = _utils_init_.mutexes[recursive_mutex_name];
                    } else {
                        p = new std::recursive_mutex();
                        _utils_init_.mutexes[recursive_mutex_name] = p;
                    }
                    return p;
                }

                static inline std::recursive_mutex* find_recursive_mutex(std::string recursive_mutex_name) {
                    return find_recursive_mutex(utils::s2w(recursive_mutex_name));
                }
            };

            class log {
            public:
                static constexpr auto mutex = _T("utils::log");

                static inline void write(std::wstring log) {
                    synchronsize(mutex);

                    TCHAR buffer[MAX_PATH] = { 0 };
                    GetModuleFileName(nullptr, buffer, _countof(buffer) - 1);
                    auto logFilePath = std::wstring(buffer);
                    auto pos = logFilePath.find_last_of('.');
                    if (pos != std::wstring::npos) {
                        logFilePath = logFilePath.substr(0, pos);
                    }
                    logFilePath += _T(".log");
                    FILE* file = nullptr;
                    _tfopen_s(&file, logFilePath.c_str(), _T("ab+"));
                    if (file) {
                        fseek(file, 0, SEEK_END);
                        std::string text;
                        text = utils::t2s(log);
                        fwrite(text.c_str(), sizeof(text.c_str()[0]), text.size(), file);
                        fclose(file);
                        file = nullptr;
                    }
                }
            };

            class console {
            public:
                static constexpr auto mutex = _T("utils::system::console");
                struct text {
                    std::string s;
                    WORD color;
                    text() {
                        this->color = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
                    }

                    text(std::string s) {
                        this->s = s;
                        this->color = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
                    }

                    text(std::string s, WORD color) {
                        this->s = s;
                        this->color = color;
                    }
                };

                static inline void println(std::vector<text> words, bool break_line) {
                    synchronsize(utils::system::console::mutex);
                    static HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
                    if (hConsole != INVALID_HANDLE_VALUE) {
                        SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);

                        std::wstring line;
                        auto datetime = utils::format(_T("[%s] "), utils::datetime::now().c_str());
                        _tprintf(_T("%s"), datetime.c_str());
                        line += datetime;
                        for (auto& w : words) {
                            SetConsoleTextAttribute(hConsole, w.color);
                            printf("%s", w.s.c_str());
                            line += utils::s2w(w.s);
                        }
                        if (break_line) {
                            _tprintf(_T("\n"));
                            line += _T("\n");
                        }
                        log::write(line);
                    }
                }

                static inline void println(std::vector<text> words) {
                    println(words, true);
                }

                static inline void println(std::string s) {
                    std::vector<text> v;
                    v.push_back(text(s));
                    println(v, true);
                }

                static inline void println(std::string s, WORD color) {
                    std::vector<text> v;
                    v.push_back(text(s, color));
                    println(v, true);
                }

                static inline void println_red(std::string s) {
                    std::vector<text> v;
                    v.push_back(text(s, FOREGROUND_RED | FOREGROUND_INTENSITY));
                    println(v, true);
                }

                static inline void println_green(std::string s) {
                    std::vector<text> v;
                    v.push_back(text(s, FOREGROUND_GREEN | FOREGROUND_INTENSITY));
                    println(v, true);
                }

                static inline void println_gray(std::string s) {
                    std::vector<text> v;
                    v.push_back(text(s, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE));
                    println(v, true);
                }

                static inline void print(std::string s, WORD color) {
                    std::vector<text> v;
                    v.push_back(text(s, color));
                    println(v, false);
                }

                static inline void print_red(std::string s) {
                    print(s, FOREGROUND_RED | FOREGROUND_INTENSITY);
                }

                static inline void print_green(std::string s) {
                    print(s, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                }

                static inline void print_blue(std::string s) {
                    print(s, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
                }
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
                static constexpr auto Ok = 200;
                static constexpr auto PermanentlyMoved = 301;
                static constexpr auto TemporarilyMoved = 302;
                static constexpr auto Unauthorized = 401;
                static constexpr auto NotFound = 404;
                static constexpr auto InternalServerError = 500;
            };

            class method {
            public:
                static constexpr auto GET = "GET";
                static constexpr auto POST = "POST";
                static constexpr auto HEAD = "HEAD";
            };

            class schema {
            public:
                static constexpr auto HTTP = "http://";
                static constexpr auto HTTPS = "https://";
            };

            class mime {
            public:
                static inline std::string find(std::wstring file) {  
                    auto name = std::string("application/octet-stream");
                    auto extension = utils::_lower(utils::system::io::path::_extension(file));
                    for (auto& e : _utils_init_.mime_types) {
                        if (extension == e.extension) {
                            name = e.name;
                            break;
                        }
                    }
                    return name;
                }
            };

            struct Proxy {
                Proxy() : port(0) {}

                Proxy(std::string uri) : port(0) {
                    this->parse(uri);
                }

                Proxy(std::string host, int port) {
                    this->host = host;
                    this->port = port;
                }

                void parse(std::string uri) {
                    auto host = std::string();
                    auto port = 0;
                    auto pos = uri.find(":");
                    if (pos != std::string::npos) {
                        host = uri.substr(0, pos);
                        port = utils::_atoi(uri.substr(pos + 1));
                    }
                    if (!host.empty()) {
                        auto lower = utils::_lower(host);
                        if (lower.starts_with(utils::http::schema::HTTP)) {
                            host = host.substr(strlen(utils::http::schema::HTTP));
                        }
                        if (lower.starts_with(utils::http::schema::HTTPS)) {
                            host = host.substr(strlen(utils::http::schema::HTTPS));
                        }
                    }
                    this->host = host;
                    this->port = port;
                }

                bool check() {
                    return !this->host.empty() && this->port > 0 && this->port <= 65535;
                }

                std::string host;
                int port;
                std::string userName;
                std::string password;
            };

            struct CURLDATA {
            public:
                std::string uri;
                std::wstring fileName;
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
                            params->fileName = std::wstring(t2t(params->uri));
                            auto pos = params->fileName.find_last_of(_T('?'));
                            if (pos != std::wstring::npos) {
                                params->fileName = params->fileName.substr(0, pos);
                            }
                            pos = params->fileName.find_last_of(_T('/'));
                            if (pos != std::wstring::npos) {
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
                    this->m_progress = false;
                    this->m_cookie.clear();
                }

                inline void set_uri(std::string uri) {
                    this->m_uri = uri;
                }

                inline void set_uri(std::wstring uri) {
                    this->m_uri = w2s(uri);
                }

                inline void set_cookie(std::string cookie) {
                    this->m_cookie = cookie;
                }

                inline void set_proxy(std::string proxyHost, std::string proxyUserName = std::string(), std::string proxyPassword = std::string()) {
                    this->m_proxyHost = proxyHost;
                    this->m_proxyUserName = proxyUserName;
                    this->m_proxyPassword = proxyPassword;
                }

                inline std::string get() {
                    CURLDATA params(this->m_uri);
                    auto curl = curl_easy_init();
                    if (curl) {
                        curl_easy_setopt(curl, CURLOPT_URL, this->m_uri.c_str());
                        this->setOptions(curl);
                        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, this->m_progress ? 0L : 1L);
                        curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, http::write_header);
                        curl_easy_setopt(curl, CURLOPT_HEADERDATA, &params);
                        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, http::write_string);
                        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &params);
                        curl_easy_perform(curl);
                        curl_easy_cleanup(curl);
                    }
                    return params.body;
                }

                inline std::vector<std::string> query() {
                    CURLDATA params(this->m_uri);
                    auto curl = curl_easy_init();
                    if (curl) {
                        this->setOptions(curl);
                        if (CURLE_OK == curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_null) &&
                            CURLE_OK == curl_easy_setopt(curl, CURLOPT_FILE, nullptr) &&
                            CURLE_OK == curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, write_header) &&
                            CURLE_OK == curl_easy_setopt(curl, CURLOPT_HEADERDATA, &params) &&
                            CURLE_OK == curl_easy_setopt(curl, CURLOPT_NOPROGRESS, this->m_progress ? 0L : 1L) &&
                            CURLE_OK == curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 0L) &&
                            CURLE_OK == curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L) &&
                            CURLE_OK == curl_easy_setopt(curl, CURLOPT_URL, this->m_uri.c_str())) {
                            curl_easy_perform(curl);
                        }
                        curl_easy_cleanup(curl);
                    }
                    return params.headers;
                }

                inline std::wstring download(std::function<bool(CURLDATA *)> f) {
                    CURLDATA params(this->m_uri);
                    params.f = f;
                    auto curl = curl_easy_init();
                    if (curl) {
                        this->setOptions(curl);
                        if (CURLE_OK == curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_file) &&
                            CURLE_OK == curl_easy_setopt(curl, CURLOPT_FILE, &params) &&
                            CURLE_OK == curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, write_header) &&
                            CURLE_OK == curl_easy_setopt(curl, CURLOPT_HEADERDATA, &params) &&
                            CURLE_OK == curl_easy_setopt(curl, CURLOPT_NOPROGRESS, this->m_progress ? 0L : 1L) &&
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

                inline std::wstring download() {
                    return this->download([](CURLDATA *) { return true; });
                }

                inline void enable_progress() {
                    this->m_progress = true;
                }

                inline void disable_progress() {
                    this->m_progress = false;
                }

            private:
                std::string m_uri;
                std::string m_proxyHost;
                std::string m_proxyUserName;
                std::string m_proxyPassword;
                std::string m_cookie;
                bool m_progress;
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

                    std::wstring contentType;
                };

                static inline int timeout() { return m_timeout; }

                static inline void setTimeout(int timeout) { m_timeout = timeout; }

                static inline void create_new_certificate(bool overwrite = true) {
                    utils::system::io::ofstream::writeBytes(utils::system::io::path::combine_t(utils::system::io::path::executable_file_directory(), LOCALHOST_CRT_FILE_NAME), CERTIFICATE, strlen(CERTIFICATE), overwrite);
                    utils::system::io::ofstream::writeBytes(utils::system::io::path::combine_t(utils::system::io::path::executable_file_directory(), LOCALHOST_KEY_FILE_NAME), PRIVATE_KEY, strlen(PRIVATE_KEY), overwrite);
                }

                static inline void create_static_file_server(std::wstring directory, std::wstring cert_path, std::wstring private_key_path,
                                                             std::wstring host, int port, std::wstring basicUserName, std::wstring basicPassword,
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
                    sslServer.set_mount_point("/", utils::w2s(utils::_replace(directory, PATH_SEPARATORA, L'/')));
                    sslServer.listen(utils::t2s(host).c_str(), port);
                }

                static inline void create_static_file_server(std::wstring directory,
                                                             std::function<void(const ::httplib::Request &, ::httplib::Response &)> Get,
                                                             std::function<void(const ::httplib::Request &, ::httplib::Response &)> Post) {
                    create_static_file_server(directory, std::wstring(), std::wstring(), HOST_ANY, SSL_PORT, std::wstring(), std::wstring(), Get, Post);
                }

                static inline void create_static_file_server(std::wstring directory,
                                                             std::function<void(const ::httplib::Request &, ::httplib::Response &)> Post) {
                    create_static_file_server(directory, std::wstring(), std::wstring(), HOST_ANY, SSL_PORT, std::wstring(), std::wstring(), [](const ::httplib::Request &, ::httplib::Response &) {}, Post);
                }

                static inline void create_static_file_server(std::wstring directory) {
                    create_static_file_server(directory, std::wstring(), std::wstring(), HOST_ANY, SSL_PORT, std::wstring(), std::wstring(),
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

            http():m_uri(std::wstring()) {}
            http(std::wstring uri) :m_uri(uri) {}

            inline std::wstring trunk() {
                auto uri = std::wstring(this->m_uri);
                auto pos = uri.find(_T("?"));
                if (pos != std::wstring::npos) {
                    uri = uri.substr(0, pos);
                }
                return uri;
            }

            protected:
                std::wstring m_uri;
        };

        class datetime {
        public:
            static inline std::wstring now(std::wstring format) {
                SYSTEMTIME systemTime = {0};
                GetLocalTime(&systemTime);
                return utils::format(format.c_str(),
                                     systemTime.wYear,
                                     systemTime.wMonth,
                                     systemTime.wDay,
                                     systemTime.wHour,
                                     systemTime.wMinute,
                                     systemTime.wSecond,
                                     systemTime.wMilliseconds);
            }

            static inline std::wstring now() {
                return utils::datetime::now(L"%04d/%02d/%02d %02d:%02d:%02d.%03d");
            }
        };

        _wstring() : std::wstring() {}

        _wstring(int i) : std::wstring() {
            this->clear();
            this->append(utils::format(L"%d", i));
        }

        _wstring(DWORD dw) : std::wstring() {
            this->clear();
            this->append(utils::format(L"%d", dw));
        }

        _wstring(HANDLE h) : std::wstring() {
            this->clear();
            this->append(utils::format(L"0x%llx", h));
        }

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

        inline std::shared_ptr<utils::http> to_http() {
            return std::shared_ptr<utils::http>(new utils::http(*this));
        }

        inline void println(std::wstring prefix, std::wstring suffix) {
            wprintf(L"[%s] %s%s%s\n", utils::datetime::now().c_str(), prefix.c_str(), this->c_str(), suffix.c_str());
        }

        inline void println(std::wstring prefix) {
            this->println(prefix, std::wstring());
        }

        inline void println() {
            this->println(std::wstring(), std::wstring());
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

        inline std::shared_ptr<utils::system::io::file> to_file() {
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
            return utils::_lower(*this);
        }

        inline std::wstring upper() {
            return utils::_upper(*this);
        }
    };
};

#endif
