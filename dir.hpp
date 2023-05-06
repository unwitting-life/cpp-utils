//
// Created by huangyl on 2023/5/6.
//

#pragma once
#include "utils.hpp"

namespace _utils{
    namespace dir {
        class dir {
        public :
            dir(_tstring path) {
                this->m_path = path;
            }
            inline _tstring Create() {
                if (!this->m_path.empty()) {
                    _tstring::size_type tmp_pos_begin = 0;
                    _tstring::size_type tmp_pos;
#ifdef WIN32
                    if (this->m_path.find(UNC) == 0) {
                        tmp_pos = this->m_path.find(PATH_SEPARATOR, wcslen(UNC));
                    }
                    else {
                        tmp_pos = this->m_path.find(PATH_SEPARATOR, tmp_pos_begin);
                    }
#else
                    tmp_pos = this->m_path.find(PATH_SEPARATOR_W, tmp_pos_begin);
#endif
                    while (tmp_pos != _tstring::npos) {
                        std::wstring tmpdir = this->m_path.substr(0, tmp_pos);
                        if (tmpdir.empty()) {
                            return this->m_path;
                        }
                        if (!std::filesystem::exists(tmpdir)) {
#ifdef WIN32
                            ::CreateDirectoryW(tmpdir.c_str(), nullptr);
#else
                            ::mkdir(tmpdir.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
#endif
                        }
                        tmp_pos_begin = tmp_pos + 1;
                        tmp_pos = this->m_path.find(PATH_SEPARATOR, tmp_pos_begin);
                    }
                    if (!std::filesystem::exists(this->m_path)) {
#ifdef WIN32
                        ::CreateDirectoryW(this->m_path.c_str(), nullptr);
#else
                        ::mkdir(this->m_path.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
#endif
                    }
                }
                return this->m_path;
            }
            bool move(_tstring newPath) {
                auto b = ::rename(strings::t2s(this->m_path).c_str(), strings::t2s(newPath).c_str()) == 0;
                this->m_path = newPath;
                return b;
            }
            bool rename(_tstring newPath) {
                return this->move(newPath);
            }
        private:
            _tstring m_path;
        };
    }
}
