#pragma once

namespace utils {
    namespace system {
        class thread {
        public:
            static constexpr auto mutex = _T("utils::thread");
            static inline std::recursive_mutex* find_recursive_mutex(std::wstring recursive_mutex_name) {
                std::lock_guard<std::recursive_mutex> recursive_mutex(configurations.mutex_singleton);
                std::recursive_mutex* p = nullptr;
                if (configurations.mutexes.contains(recursive_mutex_name)) {
                    p = configurations.mutexes[recursive_mutex_name];
                } else {
                    p = new std::recursive_mutex();
                    configurations.mutexes[recursive_mutex_name] = p;
                }
                return p;
            }

            static inline std::recursive_mutex* find_recursive_mutex(std::string recursive_mutex_name) {
                return find_recursive_mutex(s2w(recursive_mutex_name));
            }
        };
    };
}