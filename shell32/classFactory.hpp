/****************************** Module Header ******************************\
Module Name:  ClassFactory.cpp
Project:      CppShellExtContextMenuHandler
Copyright (c) Microsoft Corporation.

The file implements the class factory for the FileContextMenuExt COM class.

This source is subject to the Microsoft Public License.
See http://www.microsoft.com/opensource/licenses.mspx#Ms-PL.
All other rights reserved.

THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
\***************************************************************************/

#include "../utils.hpp"
#include "shell32.hpp"
#include "filter.hpp"
#include <unknwn.h>
#include <windows.h>

namespace utils {
    namespace shell32 {
        class ClassFactory : public IClassFactory {
        public:
            IFACEMETHODIMP QueryInterface(REFIID riid, void** ppv);
            IFACEMETHODIMP_(ULONG) AddRef();
            IFACEMETHODIMP_(ULONG) Release();
            IFACEMETHODIMP CreateInstance(IUnknown* pUnkOuter, REFIID riid, void** ppv);
            IFACEMETHODIMP LockServer(BOOL fLock);
            ClassFactory();

            string_t m_menuText;
            string_t m_verb;
            string_t m_verbCanonicalName;
            string_t m_verbHelpText;

        protected:
            ~ClassFactory();

        private:
            long m_cRef;
        };

        ClassFactory::ClassFactory() : m_cRef(1) {
            InterlockedIncrement(&refCount);
        }

        ClassFactory::~ClassFactory() {
            InterlockedDecrement(&refCount);
        }

        IFACEMETHODIMP ClassFactory::QueryInterface(REFIID riid, void** ppv)  {
            static const QITAB qit[] =  {
                QITABENT(ClassFactory, IClassFactory),
                { 0 },
            };
            return QISearch(this, qit, riid, ppv);
        }

        IFACEMETHODIMP_(ULONG) ClassFactory::AddRef()  {
            return InterlockedIncrement(&m_cRef);
        }

        IFACEMETHODIMP_(ULONG) ClassFactory::Release()  {
            ULONG cRef = InterlockedDecrement(&m_cRef);
            if (0 == cRef)  {
                delete this;
            }
            return cRef;
        }

        IFACEMETHODIMP ClassFactory::CreateInstance(IUnknown* pUnkOuter, REFIID riid, void** ppv) {
            HRESULT hr = CLASS_E_NOAGGREGATION;
            if (pUnkOuter == NULL) {
                hr = E_OUTOFMEMORY;
                static Filter* pExt = new (std::nothrow) Filter();
                if (pExt) {
                    hr = pExt->QueryInterface(riid, ppv);
                }
            }
            return hr;
        }

        IFACEMETHODIMP ClassFactory::LockServer(BOOL fLock) {
            if (fLock) {
                InterlockedIncrement(&refCount);
            } else {
                InterlockedDecrement(&refCount);
            }
            return S_OK;
        }
    }
}
