/****************************** Module Header ******************************\
Module Name:  Reg.cpp
Project:      CppShellExtContextMenuHandler
Copyright (c) Microsoft Corporation.

The file implements the reusable helper functions to register and unregister 
in-process COM components and shell context menu handlers in the registry.

RegisterInprocServer - register the in-process component in the registry.
UnregisterInprocServer - unregister the in-process component in the registry.
RegisterShellExtContextMenuHandler - register the context menu handler.
UnregisterShellExtContextMenuHandler - unregister the context menu handler.

This source is subject to the Microsoft Public License.
See http://www.microsoft.com/opensource/licenses.mspx#Ms-PL.
All other rights reserved.

THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, 
EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
\***************************************************************************/

#pragma warning (disable: 6031)
#include <strsafe.h>

namespace utils {
    namespace shell32 {
        inline HRESULT SetHKCRRegistryKeyAndValue(PCWSTR pszSubKey, PCWSTR pszValueName,
            PCWSTR pszData) {
            HRESULT hr;
            HKEY hKey = NULL;
            hr = HRESULT_FROM_WIN32(RegCreateKeyEx(HKEY_CLASSES_ROOT, pszSubKey, 0,
                NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL));

            if (SUCCEEDED(hr)) {
                if (pszData != NULL) {
                    DWORD cbData = lstrlen(pszData) * sizeof(*pszData);
                    hr = HRESULT_FROM_WIN32(RegSetValueEx(hKey, pszValueName, 0,
                        REG_SZ, reinterpret_cast<const BYTE*>(pszData), cbData));
                }

                RegCloseKey(hKey);
            }
            return hr;
        }

        inline HRESULT GetHKCRRegistryKeyAndValue(PCWSTR pszSubKey, PCWSTR pszValueName,
            PWSTR pszData, DWORD cbData) {
            HRESULT hr;
            HKEY hKey = NULL;
            hr = HRESULT_FROM_WIN32(RegOpenKeyEx(HKEY_CLASSES_ROOT, pszSubKey, 0,
                KEY_READ, &hKey));
            if (SUCCEEDED(hr)) {
                hr = HRESULT_FROM_WIN32(RegQueryValueEx(hKey, pszValueName, NULL,
                    NULL, reinterpret_cast<LPBYTE>(pszData), &cbData));

                RegCloseKey(hKey);
            }
            return hr;
        }

        inline HRESULT RegisterInprocServerW(PCWSTR pszModule, const CLSID& clsid, PCWSTR pszFriendlyName, PCWSTR pszThreadModel) {
            HRESULT hr = E_INVALIDARG;
            if (pszModule && pszThreadModel) {
                wchar_t szCLSID[MAX_PATH] = { 0 };
                StringFromGUID2(clsid, szCLSID, ARRAYSIZE(szCLSID));
                wchar_t szSubkey[MAX_PATH] = { 0 };
                hr = StringCchPrintf(szSubkey, ARRAYSIZE(szSubkey), L"CLSID\\%s", szCLSID);
                if (SUCCEEDED(hr = SetHKCRRegistryKeyAndValue(szSubkey, NULL, pszFriendlyName)) &&
                    SUCCEEDED(hr = StringCchPrintf(szSubkey, ARRAYSIZE(szSubkey), L"CLSID\\%s\\InprocServer32", szCLSID)) &&
                    SUCCEEDED(hr = SetHKCRRegistryKeyAndValue(szSubkey, NULL, pszModule))) {
                    hr = SetHKCRRegistryKeyAndValue(szSubkey, L"ThreadingModel", pszThreadModel);
                }
            }
            return hr;
        }

        inline HRESULT UnregisterInprocServer(const CLSID& clsid) {
            HRESULT hr = S_OK;
            wchar_t szCLSID[MAX_PATH];
            StringFromGUID2(clsid, szCLSID, ARRAYSIZE(szCLSID));
            wchar_t szSubkey[MAX_PATH];
            hr = StringCchPrintf(szSubkey, ARRAYSIZE(szSubkey), L"CLSID\\%s", szCLSID);
            if (SUCCEEDED(hr)) {
                hr = HRESULT_FROM_WIN32(RegDeleteTree(HKEY_CLASSES_ROOT, szSubkey));
            }
            return hr;
        }

        inline HRESULT RegisterShellExtContextMenuHandler(PCWSTR pszFileType, PCWSTR pszFriendlyName, const CLSID& clsid) {
            if (pszFileType == NULL) {
                return E_INVALIDARG;
            }
            HRESULT hr;
            wchar_t szCLSID[MAX_PATH];
            StringFromGUID2(clsid, szCLSID, ARRAYSIZE(szCLSID));
            wchar_t szSubkey[MAX_PATH];
            if (*pszFileType == L'.') {
                wchar_t szDefaultVal[260];
                hr = GetHKCRRegistryKeyAndValue(pszFileType, NULL, szDefaultVal, sizeof(szDefaultVal));
                if (SUCCEEDED(hr) && szDefaultVal[0] != L'\0') {
                    pszFileType = szDefaultVal;
                }
            }
            if (SUCCEEDED(hr = StringCchPrintf(szSubkey, ARRAYSIZE(szSubkey), L"%s\\shellex\\ContextMenuHandlers\\%s", pszFileType, pszFriendlyName))) {
                hr = SetHKCRRegistryKeyAndValue(szSubkey, NULL, szCLSID);
            }
            if (SUCCEEDED(hr = StringCchPrintf(szSubkey, ARRAYSIZE(szSubkey), L"Directory\\shellex\\ContextMenuHandlers\\%s", pszFriendlyName))) {
                hr = SetHKCRRegistryKeyAndValue(szSubkey, NULL, szCLSID);
            }
            return hr;
        }

        inline HRESULT UnregisterShellExtContextMenuHandler(PCWSTR pszFileType, PCWSTR pszFriendlyName) {
            if (pszFileType == NULL) {
                return E_INVALIDARG;
            }
            HRESULT hr;
            wchar_t szSubkey[MAX_PATH];
            if (*pszFileType == L'.') {
                wchar_t szDefaultVal[260];
                hr = GetHKCRRegistryKeyAndValue(pszFileType, NULL, szDefaultVal,
                    sizeof(szDefaultVal));
                if (SUCCEEDED(hr) && szDefaultVal[0] != L'\0') {
                    pszFileType = szDefaultVal;
                }
            }
            if (SUCCEEDED(hr = StringCchPrintf(szSubkey, ARRAYSIZE(szSubkey), L"%s\\shellex\\ContextMenuHandlers\\%s", pszFileType, pszFriendlyName))) {
                hr = HRESULT_FROM_WIN32(RegDeleteTree(HKEY_CLASSES_ROOT, szSubkey));
            }
            if (SUCCEEDED(hr = StringCchPrintf(szSubkey, ARRAYSIZE(szSubkey), L"Directory\\shellex\\ContextMenuHandlers\\%s", pszFriendlyName))) {
                hr = HRESULT_FROM_WIN32(RegDeleteTree(HKEY_CLASSES_ROOT, szSubkey));
            }
            return hr;
        }
    }
}
