#pragma once

namespace utils {
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
                text = t2s(log);
                fwrite(text.c_str(), sizeof(text.c_str()[0]), text.size(), file);
                fclose(file);
                file = nullptr;
            }
        }
    };
}
