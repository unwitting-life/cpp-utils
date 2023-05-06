#pragma once

namespace _utils {
	namespace path {
        class path {
        public:
            path(_tstring path) {
                this->m_path = path;
            }
            _tstring name() {
                return _utils::path::name(this->m_path);
            }
            _tstring name_without_extension() {
                return _utils::path::name_without_extension(this->m_path);
            }
            _tstring combine(_tstring path) {
                return _utils::path::combine(this->m_path, path);
            }
        protected:
            _tstring m_path;
        };
	}
}
