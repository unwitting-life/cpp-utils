#pragma once

#include "../logger.hpp"

namespace utils {
    namespace system {
#define FOREGROUND_GRAY FOREGROUND_INTENSITY
        class console {
        public:
            static constexpr auto mutex = _T("system::console");
            struct text {
                std::wstring content;
                WORD color;
                text() {
                    this->color = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
                }

                text(std::wstring s) {
                    this->content = s;
                    this->color = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
                }

                text(std::string s) {
                    this->content = s2w(s);
                    this->color = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
                }

                text(std::wstring s, WORD color) {
                    this->content = s;
                    this->color = color;
                }

                text(std::string s, WORD color) {
                    this->content = s2w(s);
                    this->color = color;
                }
            };

            static inline void println(std::vector<text> words, bool break_line) {
                synchronsize(system::console::mutex);
                static HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
                if (hConsole != INVALID_HANDLE_VALUE) {
                    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);

                    std::wstring line;
                    auto datetime = _format(L"[%s] ", system::datetime::now().c_str());
                    wprintf(L"%s", datetime.c_str());
                    line += datetime;
                    for (auto& w : words) {
                        SetConsoleTextAttribute(hConsole, w.color);
                        printf("%s", w2s(w.content).c_str());
                        line += w.content;
                    }
                    if (break_line) {
                        wprintf(L"\n");
                        line += L"\n";
                    }
                    log::write(line);
                }
            }

            static inline void println(std::vector<text> words) {
                println(words, true);
            }

            static inline void println(std::wstring s) {
                std::vector<text> v;
                v.push_back(text(s));
                println(v, true);
            }

            static inline void println(std::wstring s, WORD color) {
                std::vector<text> v;
                v.push_back(text(s, color));
                println(v, true);
            }

            static inline void println_red(std::wstring s) {
                std::vector<text> v;
                v.push_back(text(s, FOREGROUND_RED | FOREGROUND_INTENSITY));
                println(v, true);
            }

            static inline void println_green(std::wstring s) {
                std::vector<text> v;
                v.push_back(text(s, FOREGROUND_GREEN | FOREGROUND_INTENSITY));
                println(v, true);
            }

            static inline void println_gray(std::wstring s) {
                std::vector<text> v;
                v.push_back(text(s, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE));
                println(v, true);
            }

            static inline void print(std::wstring s, WORD color) {
                std::vector<text> v;
                v.push_back(text(s, color));
                println(v, false);
            }

            static inline void print_red(std::wstring s) {
                print(s, FOREGROUND_RED | FOREGROUND_INTENSITY);
            }

            static inline void print_green(std::wstring s) {
                print(s, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            }

            static inline void print_blue(std::wstring s) {
                print(s, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
            }
        };
    };
}