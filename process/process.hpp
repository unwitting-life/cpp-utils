#pragma once

#include "../utils.hpp"

namespace utils {
    namespace io {
        namespace process {
#ifdef WIN32
            typedef BOOL(WINAPI* LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);

            LPFN_ISWOW64PROCESS fnIsWow64Process;

            inline BOOL IsWow64() {
                BOOL bIsWow64 = FALSE;

                //IsWow64Process is not available on all supported versions of Windows.
                //Use GetModuleHandle to get a handle to the DLL that contains the function
                //and GetProcAddress to get a pointer to the function if available.

                fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(
                    GetModuleHandle(TEXT("kernel32")), "IsWow64Process");

                if (NULL != fnIsWow64Process) {
                    if (!fnIsWow64Process(GetCurrentProcess(), &bIsWow64)) {
                        //handle error
                    }
                }
                return bIsWow64;
            }

            // https://devblogs.microsoft.com/oldnewthing/20050201-00/?p=36553
            inline BOOL Is64BitWindows() {
#if defined(_WIN64)
                return TRUE;  // 64-bit programs run only on Win64
#elif defined(_WIN32)
                // 32-bit programs run on both 32-bit and 64-bit Windows
                // so must sniff
                return IsWow64();
#else
                return FALSE; // Win64 does not support Win16
#endif
            }
        }
    }
#endif
}
