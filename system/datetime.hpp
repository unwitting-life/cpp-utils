#pragma once

namespace utils {
    namespace system {
        class datetime {
        public:
            static inline std::wstring now(std::wstring format) {
                SYSTEMTIME systemTime = { 0 };
                GetLocalTime(&systemTime);
                return _format(format.c_str(),
                    systemTime.wYear,
                    systemTime.wMonth,
                    systemTime.wDay,
                    systemTime.wHour,
                    systemTime.wMinute,
                    systemTime.wSecond,
                    systemTime.wMilliseconds);
            }

            static inline std::wstring now() {
                return now(L"%04d/%02d/%02d %02d:%02d:%02d.%03d");
            }
        };
    }
}
