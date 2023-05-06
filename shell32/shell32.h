#pragma once

#include <functional>

namespace utils {
    namespace shell32 {
        const CLSID CLSID_Shell32 = { 0xa52373f2, 0x28bc, 0x4d99, { 0xbf, 0x44, 0x83, 0xe5, 0x4, 0xcb, 0x5, 0x88 } };
        static auto CONTEXT_MENU_HANDLERS_NAME = L"cpp-shell32.Filter";
        static auto CLSID_CLASS_NAME = L"cpp-shell32.Filter.Class";
        static auto hModule = HMODULE(nullptr);
        static auto associatedTypes = std::vector<std::wstring>();
        static auto OnInitialized = std::function<HRESULT(std::vector<std::wstring>&)>();
        static auto OnCommand = std::function<HRESULT(LPCMINVOKECOMMANDINFO, std::vector<std::wstring>&)>();
        static auto OnGetCommandString = std::function<HRESULT(UINT_PTR, UINT, UINT*, LPSTR, UINT)>();
        static auto OnQueryContextMenu = std::function<HRESULT(HMENU, UINT, UINT, UINT, UINT, std::vector<std::wstring>&)>();
        inline void init(HMODULE hModule) {
            shell32::hModule = hModule;
            shell32::associatedTypes.clear();
            if (shell32::OnInitialized) {
                shell32::OnInitialized = [](std::vector<std::wstring>&) { return S_OK; };
            }
            if (!shell32::OnCommand) {
                shell32::OnCommand = [](LPCMINVOKECOMMANDINFO, std::vector<std::wstring>&) { return E_FAIL; };
            }
            if (!shell32::OnGetCommandString) {
                shell32::OnGetCommandString = [](UINT_PTR, UINT, UINT*, LPSTR, UINT) { return S_OK; };
            }
            if (shell32::OnQueryContextMenu) {
                shell32::OnQueryContextMenu = [](HMENU, UINT, UINT, UINT, UINT, std::vector<std::wstring>&) {return E_FAIL; };
            }
            DisableThreadLibraryCalls(hModule);
        }
    }
}
