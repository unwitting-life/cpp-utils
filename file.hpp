//
// Created by huangyl on 2023/5/5.
//
#pragma once
#include "utils.hpp"
#include <iostream>
#include <cstdio>
#ifdef WIN32
#include <atlimage.h>
#endif

namespace _utils {
    namespace file {
        class file {
        public:
            file(_tstring path) {
                this->m_path = path;
            }
            bool exists() {
                auto b = false;
                auto path = _tstring(this->m_path);
                if (!path.empty() && path[path.size() - 1] == PATH_SEPARATOR &&
                    (!(path.size() >= 2 && path[path.size() - 2] == PATH_SEPARATOR))) {
                    path = path.substr(0, path.size() - 1);
                }
                try {
                    b = std::filesystem::exists(path) && !std::filesystem::is_directory(path);
                } catch (...) {
                    b = false;
                }
                return b;
            }
            bool remove() {
                auto b = true;
                if (this->exists()) {
                    try {
                        b = std::filesystem::remove(this->m_path);
                    } catch (...) {

                    }
                }
                return b;
            }
            bool move(_tstring newPath) {
                auto b = true;
                auto path = _tstring(newPath);
                try {
                    if (std::filesystem::exists(path) && std::filesystem::is_directory(path)) {
                        path = path::combine(newPath, path::name(this->m_path));
                    }
                    if (std::filesystem::exists(path)) {
                        if (std::filesystem::is_directory(path)) {
                            b = false;
                        } else {
                            b = std::filesystem::remove(path);
                        }
                    }
                } catch (...) {
                    b = false;
                }
                if (b) {
                    b = ::rename(strings::t2s(this->m_path).c_str(), strings::t2s(path).c_str()) == 0;
                    if (b) {
                        this->m_path = path;
                    }
                }
                return b;
            }
            bool rename(_tstring newPath) {
                return this->move(newPath);
            }
            inline unsigned long long size() {
                return this->exists() ? std::filesystem::file_size(this->m_path) : -1;
            }
        protected:
            _tstring m_path;
        };
        class ofstream : public file {
        public:
            ofstream(_tstring path, bool createNew = false) : file(path) {
                if (createNew) {
                    this->remove();
                }
                this->m_file.open(strings::t2s(path).c_str(), std::ios::app | std::ios::binary);
            }
            ~ofstream() {
                this->close();
            }
            inline void write(const char* data, size_t size) {
                this->m_file.write(data, size);
            }
            inline void close() {
                this->m_file.flush();
                this->m_file.close();
            }
        private:
            std::ofstream m_file;
        };
        class image : public file {
        public:
#ifdef WIN32
            bool png(const _tstring dst = _tstring()) {
                auto path = _tstring(dst);
                if (path.empty()) {
                    path = _utils::path::combine(_utils::path::directory(this->m_path),
                        strings::format(_T("%s.png"), _utils::path::name_without_extension(this->m_path).c_str()));
                }
                CImage image;
                image.Load(this->m_path.c_str());
                return image.Save(path.c_str(), Gdiplus::ImageFormatPNG) == S_OK;
            }
#endif
        };
    }
}