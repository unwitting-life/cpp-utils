/****************************** Module Header ******************************\
Module Name:  Filter.h
Project:      CppShellExtContextMenuHandler
Copyright (c) Microsoft Corporation.

The code sample demonstrates creating a Shell context menu handler with C++. 

A context menu handler is a shell extension handler that adds commands to an 
existing context menu. Context menu handlers are associated with a particular 
file class and are called any time a context menu is displayed for a member 
of the class. While you can add items to a file class context menu with the 
registry, the items will be the same for all members of the class. By 
implementing and registering such a handler, you can dynamically add items to 
an object's context menu, customized for the particular object.

The example context menu handler adds the menu item "Display File Name (C++)"
to the context menu when you right-click a .cpp file in the Windows Explorer. 
Clicking the menu item brings up a message box that displays the full path 
of the .cpp file.

This source is subject to the Microsoft Public License.
See http://www.microsoft.com/opensource/licenses.mspx#Ms-PL.
All other rights reserved.

THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, 
EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
\***************************************************************************/

#pragma once

#include "../utils.hpp"

#ifdef WIN32
#include "shell32.h"
#include "../gdi32/gdi32.hpp"
#include <windows.h>
#include <shlobj.h>
#include <vector>
#include <string>
#include <strsafe.h>
#include <Shlwapi.h>
#include <shellapi.h>
#pragma comment(lib, "shlwapi.lib")

namespace utils {
    namespace shell32 {
        static auto refCount = long(0);
        class Filter : public IShellExtInit, public IContextMenu {
        public:
            IFACEMETHODIMP QueryInterface(REFIID, void**);
            IFACEMETHODIMP_(ULONG) AddRef();
            IFACEMETHODIMP_(ULONG) Release();
            IFACEMETHODIMP Initialize(LPCITEMIDLIST, LPDATAOBJECT, HKEY);
            IFACEMETHODIMP QueryContextMenu(HMENU, UINT, UINT, UINT, UINT);
            IFACEMETHODIMP InvokeCommand(LPCMINVOKECOMMANDINFO);
            IFACEMETHODIMP GetCommandString(UINT_PTR, UINT, UINT*, LPSTR, UINT);

            Filter();
            Filter(string_t);

        protected:
            ~Filter(void);

        private:
            long m_cRef;
            std::vector<std::wstring> m_files;
        };

        Filter::Filter() {
            this->m_cRef = 1;
            InterlockedIncrement(&shell32::refCount);
        }

        Filter::Filter(string_t menuText) {
            this->m_cRef = 1;
            InterlockedIncrement(&shell32::refCount);
        }

        Filter::~Filter(void) {
            InterlockedDecrement(&shell32::refCount);
        }

        IFACEMETHODIMP Filter::QueryInterface(REFIID riid, void** ppv) {
            static const QITAB qit[] =
            {
                QITABENT(Filter, IContextMenu),
                QITABENT(Filter, IShellExtInit),
                { 0 },
            };
            return QISearch(this, qit, riid, ppv);
        }

        IFACEMETHODIMP_(ULONG) Filter::AddRef() {
            return InterlockedIncrement(&m_cRef);
        }

        IFACEMETHODIMP_(ULONG) Filter::Release() {
            return InterlockedDecrement(&m_cRef);
        }

        IFACEMETHODIMP Filter::Initialize(LPCITEMIDLIST pidlFolder, LPDATAOBJECT pDataObj, HKEY hKeyProgID) {
            auto hr = E_INVALIDARG;
            if (pDataObj) {
                FORMATETC fe = { CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
                STGMEDIUM stm = { 0 };
                HDROP hDrop = nullptr;
                if (SUCCEEDED(pDataObj->GetData(&fe, &stm)) &&
                    (hDrop = static_cast<HDROP>(GlobalLock(stm.hGlobal)))) {
                    m_files.clear();
                    UINT n = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);
                    for (size_t i = 0; i < n; i++) {
                        wchar_t sz[MAX_PATH] = { 0 };
                        if (DragQueryFile(hDrop, i, sz, ARRAYSIZE(sz))) {
                            auto hit = false;
                            for (auto& assoricatedType : associatedTypes) {
                                if (strings::equalsIgnoreCaseW(assoricatedType, io::path::GetFileExtensionName(sz))) {
                                    hit = true;
                                    break;
                                }
                            }
                            if (associatedTypes.empty() || hit) {
                                m_files.push_back(sz);
                            }
                        }
                    }
                    hr = m_files.empty() ? E_FAIL : S_OK;
                    GlobalUnlock(stm.hGlobal);
                    ReleaseStgMedium(&stm);
                }
            }
            if (SUCCEEDED(hr)) {
                hr = shell32::OnInitialized(m_files);
            }
            return hr;
        }

        IFACEMETHODIMP Filter::QueryContextMenu(HMENU hMenu, UINT indexMenu, UINT idCmdFirst, UINT idCmdLast, UINT uFlags) {
            return shell32::OnQueryContextMenu(hMenu, indexMenu, idCmdFirst, idCmdLast, uFlags, this->m_files);
        }

        IFACEMETHODIMP Filter::InvokeCommand(LPCMINVOKECOMMANDINFO pici) {
            auto hr = HRESULT(E_FAIL);
            auto isUnicode = (pici->cbSize == sizeof(CMINVOKECOMMANDINFOEX)) && (pici->fMask & CMIC_MASK_UNICODE);
            auto isVerbA = !isUnicode && HIWORD(pici->lpVerb);
            auto isVerbW = isUnicode && HIWORD(((CMINVOKECOMMANDINFOEX*)pici)->lpVerbW);
            if (!isVerbA && !isVerbW) {
                hr = shell32::OnCommand(pici, m_files);
            }
            return hr;
        }

        IFACEMETHODIMP Filter::GetCommandString(UINT_PTR idCommand, UINT uFlags, UINT* pwReserved, LPSTR pszName, UINT cchMax) {
            return shell32::OnGetCommandString(idCommand, uFlags, pwReserved, pszName, cchMax);
        }
    }
}

#endif
