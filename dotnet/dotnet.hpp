#pragma once
// https://www.shuzhiduo.com/A/Gkz1D8lGdR/
// https://xz.aliyun.com/t/3050

#include "../utils.hpp"
#include "../process/process.hpp"
#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include <mscoree.h>
#include <metahost.h>
#include <assert.h>

#pragma comment(lib, "mscoree.lib")

namespace utils {
    namespace dotnet {
        namespace clr {
            constexpr auto MICROSOFT_NET = _T("Microsoft.NET");
            constexpr auto FRAMEWORK = _T("Framework");
            constexpr auto FRAMEWORK64 = _T("Framework64");
            int execute(LPCWSTR runtimeVersion, LPCWSTR libFilePath, LPCWSTR className, LPCWSTR methodName, LPCWSTR args) {
                auto retVal = 0;
                HRESULT hr = E_FAIL;
                static ICLRMetaHost* pMetaHost = nullptr;
                static ICLRRuntimeInfo* pRuntimeInfo = nullptr;
                static ICLRRuntimeHost* pRuntimeHost = nullptr;
                if (!pMetaHost && !pRuntimeInfo && !pRuntimeHost) {
                    if (SUCCEEDED(hr = CLRCreateInstance(CLSID_CLRMetaHost, IID_ICLRMetaHost, (LPVOID*)&pMetaHost)) &&
                        SUCCEEDED(hr = pMetaHost->GetRuntime(runtimeVersion, IID_ICLRRuntimeInfo, (LPVOID*)&pRuntimeInfo)) &&
                        SUCCEEDED(hr = pRuntimeInfo->GetInterface(CLSID_CLRRuntimeHost, IID_ICLRRuntimeHost, (LPVOID*)&pRuntimeHost))) {
                        hr = pRuntimeHost->Start();
                    }
                }
                if (pRuntimeHost) {
                    auto dwReturnValue = DWORD(0);
                    hr = pRuntimeHost->ExecuteInDefaultAppDomain(libFilePath, className, methodName, args, &dwReturnValue);
                    retVal = dwReturnValue;
                }
#if 0
                pRuntimeHost ? pRuntimeHost->Release() : E_FAIL;
                pRuntimeInfo ? pRuntimeInfo->Release() : E_FAIL;
                pMetaHost ? pMetaHost->Release() : E_FAIL;
#endif
                return retVal;
            }

            int invokeW(std::wstring libFilePath, std::wstring className, std::wstring methodName, std::wstring args) {
                auto retVal = 0;
                wchar_t buffer[MAX_PATH] = { 0 };
                if (GetWindowsDirectory(buffer, _countof(buffer) - 1)) {
                    auto frameworkDirectory = std::wstring();
                    frameworkDirectory += buffer;
                    frameworkDirectory = utils::io::path::combine(frameworkDirectory, utils::strings::t2w(MICROSOFT_NET));
                    frameworkDirectory = utils::io::path::combine(frameworkDirectory, utils::io::process::Is64BitWindows() ? utils::strings::t2w(FRAMEWORK64) : utils::strings::t2w(FRAMEWORK));
                    auto directories = strings::sort(io::directory::directories(frameworkDirectory, false), true);
                    if (!directories.empty()) {
                        auto frameworkVersion = io::path::GetFileName(directories.at(0));
                        retVal = execute(frameworkVersion.c_str(), libFilePath.c_str(), className.c_str(), methodName.c_str(), args.c_str());
                    }
                }
                return retVal;
            }

            int invokeA(std::string libFilePath, std::string className, std::string methodName, std::string args) {
                return invokeW(utils::strings::t2w(libFilePath), utils::strings::t2w(className), utils::strings::t2w(methodName), utils::strings::t2w(args));
            }

            int invoke(string_t libFilePath, string_t className, string_t methodName, string_t args) {
#ifdef UNICODE
                return invokeW(libFilePath, className, methodName, args);
#else
                return invokeA(libFilePath, className, methodName, args);
#endif
            }
        }
    }
}
