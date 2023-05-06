#pragma once
/****************************** Module Header ******************************\
Module Name:  dllmain.cpp
Project:      CppShellExtContextMenuHandler
Copyright (c) Microsoft Corporation.

The file implements DllMain, and the DllGetClassObject, DllCanUnloadNow,
DllRegisterServer, DllUnregisterServer functions that are necessary for a COM
DLL.

DllGetClassObject invokes the class factory defined in ClassFactory.h/cpp and
queries to the specific interface.

DllCanUnloadNow checks if we can unload the component from the memory.

DllRegisterServer registers the COM server and the context menu handler in
the registry by invoking the helper functions defined in Reg.h/cpp. The
context menu handler is associated with the .cpp file class.

DllUnregisterServer unregisters the COM server and the context menu handler.

This source is subject to the Microsoft Public License.
See http://www.microsoft.com/opensource/licenses.mspx#Ms-PL.
All other rights reserved.

THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
\***************************************************************************/

#include "../utils.hpp"
#include "shell32.h"
#include "Reg.hpp"
#include "classFactory.hpp"
#include <Shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID FAR* ppv) {
    HRESULT hr = CLASS_E_CLASSNOTAVAILABLE;
    if (IsEqualCLSID(utils::shell32::CLSID_Shell32, rclsid)) {
        hr = E_OUTOFMEMORY;
        auto pClassFactory = new utils::shell32::ClassFactory();
        if (pClassFactory) {
            hr = pClassFactory->QueryInterface(riid, ppv);
            pClassFactory->Release();
        }
    }
    return hr;
}

STDAPI DllCanUnloadNow(void) {
    return utils::shell32::refCount > 0 ? S_FALSE : S_OK;
}

STDAPI DllRegisterServer(void) {
    auto hr = HRESULT(NULL);
    wchar_t szModuleFilePath[MAX_PATH];
    if (GetModuleFileName(utils::shell32::hModule, szModuleFilePath, ARRAYSIZE(szModuleFilePath)) == 0) {
        hr = HRESULT_FROM_WIN32(GetLastError());
        return hr;
    }
    hr = utils::shell32::RegisterInprocServerW(
        szModuleFilePath,
        utils::shell32::CLSID_Shell32,
        utils::shell32::CLSID_CLASS_NAME,
        L"Apartment");
    if (SUCCEEDED(hr)) {
        // Computer\HKEY_CLASSES_ROOT\*\shellex\ContextMenuHandlers\cpp-shell32.Filter
        // Computer\HKEY_CLASSES_ROOT\*\shellex\ContextMenuHandlers\{A52373F2-28BC-4D99-BF44-83E504CB0588}
        // Change key name will also change the menu item present position in menu 
        hr = utils::shell32::RegisterShellExtContextMenuHandler(L"*",
            utils::shell32::CONTEXT_MENU_HANDLERS_NAME,
            utils::shell32::CLSID_Shell32);
    }
    return hr;
}

STDAPI DllUnregisterServer(void) {
    HRESULT hr = S_OK;
    wchar_t szModuleFilePath[MAX_PATH];
    if (GetModuleFileName(utils::shell32::hModule, szModuleFilePath, ARRAYSIZE(szModuleFilePath)) == 0) {
        hr = HRESULT_FROM_WIN32(GetLastError());
        return hr;
    }
    hr = utils::shell32::UnregisterInprocServer(utils::shell32::CLSID_Shell32);
    if (SUCCEEDED(hr)) {
        hr = utils::shell32::UnregisterShellExtContextMenuHandler(L"*", utils::shell32::CONTEXT_MENU_HANDLERS_NAME);
    }
    return hr;
}
