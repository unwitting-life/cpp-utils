#pragma once

namespace utils {
    namespace system {
        class process {
        public:
            static inline DWORD get_parent_process_id() {
                auto ppid = DWORD(0);
                auto pid = GetCurrentProcessId();
                PROCESSENTRY32 pe32 = { 0 };
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
                SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES) };
                sa.bInheritHandle = TRUE;
                sa.lpSecurityDescriptor = nullptr;
                if (CreatePipe(&pipeReadHandle, &pipeWriteHandle, &sa, 0) && SetHandleInformation(pipeReadHandle, HANDLE_FLAG_INHERIT, FALSE)) {
                    STARTUPINFOEX si = { 0 };
                    si.StartupInfo.cb = sizeof(si);
                    si.StartupInfo.hStdOutput = pipeWriteHandle;
                    si.StartupInfo.hStdError = pipeReadHandle;
                    si.StartupInfo.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
                    si.StartupInfo.wShowWindow = SW_SHOW;
                    HANDLE inheritHandles[] = { pipeWriteHandle };
                    auto size = SIZE_T(0);
                    InitializeProcThreadAttributeList(nullptr, _countof(inheritHandles), 0, &size);
                    if (size > 0) {
                        auto lpAttributeList = reinterpret_cast<LPPROC_THREAD_ATTRIBUTE_LIST>(malloc(size));
                        if (lpAttributeList) {
                            if (InitializeProcThreadAttributeList(lpAttributeList, _countof(inheritHandles), 0, &size) &&
                                UpdateProcThreadAttribute(lpAttributeList, 0, PROC_THREAD_ATTRIBUTE_HANDLE_LIST, inheritHandles, sizeof(inheritHandles), nullptr, nullptr)) {
                                si.lpAttributeList = lpAttributeList;
                                PROCESS_INFORMATION pi = { nullptr };
                                std::thread pipe_thread([&]() {
                                    auto numberOfBytesRead = DWORD(0);
                                    char buffer[8192] = { 0 };
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
                SHELLEXECUTEINFO se = { 0 };
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
    }
}
