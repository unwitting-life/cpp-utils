#pragma once

namespace utils{
    namespace system {
        namespace io {
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
                    auto removed_items_count = uintmax_t(0);
                    try {
                        if (_is_directory(path)) {
                            for (auto& e : std::filesystem::directory_iterator(std::filesystem::path(path))) {
                                if (recursive && _is_directory(e.path())) {
                                    _remove_directory(e.path(), true);
                                }
                            }
                            removed_items_count += std::filesystem::remove_all(path);
                            removed_items_count += std::filesystem::remove(path) ? 1 : 0;
                        }
                    } catch (std::exception& e) {
                        OutputDebugString(t2w(e.what()).c_str());
                    }
                    return removed_items_count;
                }

                static inline std::wstring _name(std::wstring path) {
                    auto s = _replace(path, _T("/"), PATH_SEPARATOR);
                    auto pos = s.find_last_of(PATH_SEPARATOR);
                    return (pos == std::string::npos ? s : s.substr(pos + 1));
                }

                static inline std::wstring _combine(std::wstring path1, std::wstring path2) {
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
                    TCHAR buffer[MAX_PATH] = { 0 };
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
                    auto s = std::wstring();
                    auto name = _name(path);
                    auto pos = name.find_last_of(L".");
                    if (pos != std::wstring::npos) {
                        s = name.substr(pos);
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
                path(std::wstring path) : _base_wstring(path) {}

                std::wstring combine(std::wstring path) {
                    return system::io::path::_combine(this->m_str, path);
                }

                std::wstring name() {
                    return system::io::path::_name(this->m_str);
                }

                std::wstring extension() {
                    return system::io::path::_extension(this->m_str);
                }

                std::wstring name_without_extension() {
                    return system::io::path::_name_without_extension(this->m_str);
                }

                std::wstring directory() {
                    return system::io::path::_directory(this->m_str);
                }
            };

            class file : public _base_wstring {
            public:
                file(std::wstring path) : _base_wstring(path) {}

                bool exists() {
                    auto b = false;
                    auto path = std::wstring(this->m_str);
                    if (!path.empty()) {
                        if (path[path.size() - 1] == PATH_SEPARATOR &&
                            (!(path.size() >= 2 && path[path.size() - 2] == PATH_SEPARATOR))) {
                            path = path.substr(0, path.size() - 1);
                        }
                        try {
                            b = std::filesystem::exists(path) && !std::filesystem::is_directory(path);
                        } catch (...) {
                        }
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
                            dst = system::io::path::_combine(newPath, system::io::path::_name(this->m_str));
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
                                auto p = std::shared_ptr<char>(new char[size]);
                                fread(p.get(), size, sizeof(p.get()[0]), file);
                                s.append(p.get(), size);
                            }
                            fclose(file);
                        }
                    }
                    return s;
                }

                inline uintmax_t size() {
                    auto u = uintmax_t(0);
                    try {
                        if (this->exists()) {
                            u = std::filesystem::file_size(this->m_str);
                        }
                    } catch (...) {

                    }
                    return u;
                }

                inline std::wstring hash(ALG_ID hash_type) {
                    auto content = this->content();
                    return _hash(reinterpret_cast<BYTE*>(const_cast<char*>(content.c_str())), content.size(), hash_type);
                }

                inline std::wstring md5() {
                    return this->hash(CALG_MD5);
                }

                inline std::wstring sha1() {
                    return this->hash(CALG_SHA1);
                }
            };

            class ofstream : public file {
            public:
                static inline void writeBytes(std::wstring file, const char* data, size_t size, bool createNew = true) {
                    auto f = ofstream(file, createNew);
                    f.write(data, size);
                    f.close();
                }

                static inline void appendBytes(std::wstring file, const char* data, size_t size) {
                    ofstream::writeBytes(file, data, size, false);
                }

                ofstream(std::wstring path, bool truncate = false) : file(path) {
                    this->m_truncate = truncate;
                }

                ~ofstream() {
                    this->close();
                }

                inline void write(const char* data, size_t size) {
                    if (!this->m_file.is_open()) {
                        system::io::path::_remove_directory(this->m_str, true);
                        if (this->m_truncate) {
                            this->remove();
                        }
                        this->m_file.open(t2s(this->m_str).c_str(), std::ios::app | std::ios::binary);
                        this->m_file.seekp(0, std::ios::end);
                    }
                    if (data && size > 0) {
                        this->m_file.write(data, size);
                        this->m_file.flush();
                    }
                }

                inline void write(const std::string& data) {
                    this->write(data.c_str(), data.size());
                }

                inline void close() {
                    if (this->m_file.is_open()) {
                        this->m_file.flush();
                        this->m_file.close();
                    }
                }

            private:
                std::ofstream m_file;
                bool m_truncate;
            };

            class image : public file {
            public:
                image(std::wstring file) : file(file) {}
                bool png(const std::wstring dst = std::wstring()) {
                    auto path = std::wstring(dst);
                    if (path.empty()) {
                        auto png = _format(L"%s.png", system::io::path::_name_without_extension(this->m_str).c_str());
                        path = system::io::path::_combine(system::io::path::_directory(this->m_str), png.c_str());
                    }
                    CImage image;
                    image.Load(this->m_str.c_str());
                    return image.Save(path.c_str(), Gdiplus::ImageFormatPNG) == S_OK;
                }
            };

            class directory : public _base_wstring {
            public:
                static inline std::vector<std::wstring> _enumerate(std::wstring path, bool recursive = true) {
                    std::vector<std::wstring> directories;
                    if (path::_is_directory(path)) {
                        for (const auto& entry : std::filesystem::directory_iterator(path)) {
                            std::wstring p = reinterpret_cast<LPCTSTR>(entry.path().u16string().c_str());
                            if (entry.is_directory()) {
                                if (recursive) {
                                    for (auto& directory : _enumerate(p, recursive)) {
                                        directories.push_back(directory);
                                    }
                                }
                                directories.push_back(p);
                            }
                        }
                    }
                    return directories;
                }

                static inline std::vector<std::wstring> _files(std::wstring path, bool recursive = true) {
                    std::vector<std::wstring> files;
                    if (path::_is_directory(path)) {
                        for (const auto& entry : std::filesystem::directory_iterator(path)) {
                            std::wstring w = reinterpret_cast<LPCTSTR>(entry.path().u16string().c_str());
                            if (entry.is_directory()) {
                                if (recursive) {
                                    for (auto& file : _files(w, recursive)) {
                                        files.push_back(file);
                                    }
                                }
                            } else {
                                files.push_back(w);
                            }
                        }
                    }
                    return files;
                }

                directory(std::wstring path) : _base_wstring(path) {}

                inline std::wstring mkdir(bool force = false) {
                    auto w = this->m_str;
                    auto _mkdir = [&](std::wstring path) -> bool {
                        auto b = false;
                        if (path::exists(path)) {
                            try {
                                if (path::_is_directory(path)) {
                                    b = true;
                                } else {
                                    if (force) {
                                        std::filesystem::remove(path);
                                        b = CreateDirectory(path.c_str(), nullptr);
                                    }
                                }
                            } catch (...) {
                            }
                        } else {
                            b = CreateDirectory(path.c_str(), nullptr);
                        }
                        return b;
                    };
                    if (!this->m_str.empty()) {
                        std::wstring::size_type next_child_directory_begin_position = 0;
                        std::wstring::size_type next_child_directory_position = 0;
                        if (this->m_str.find(UNC) == 0) {
                            next_child_directory_position = this->m_str.find(PATH_SEPARATOR, _tcslen(UNC));
                        } else {
                            next_child_directory_position = this->m_str.find(PATH_SEPARATOR, next_child_directory_begin_position);
                        }
                        while (!w.empty() && next_child_directory_position != std::wstring::npos) {
                            auto child_directory = this->m_str.substr(0, next_child_directory_position);
                            if (child_directory.empty() || !_mkdir(child_directory)) {
                                w = std::wstring();
                            }
                            next_child_directory_begin_position = next_child_directory_position + 1;
                            next_child_directory_position = this->m_str.find(PATH_SEPARATOR, next_child_directory_begin_position);
                        }
                        if (!_mkdir(this->m_str)) {
                            w = std::wstring();
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
                    std::wstring name = _replace(this->m_str, _T("/"), PATH_SEPARATOR);
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
                    return _enumerate(this->m_str, recursive);
                }

                std::vector<std::wstring> files(bool recursive = true) {
                    return _files(this->m_str, recursive);
                }

                bool exists() {
                    return system::io::path::_is_directory(this->m_str);
                }

                uintmax_t remove(bool recursive = true) {
                    return system::io::path::_remove_directory(this->m_str, true);
                }
            };
        };
    };
}