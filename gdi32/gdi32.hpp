#pragma once
#include "../utils.hpp"

#ifndef GDI32_SUPPORT
#define GDI32_SUPPORT
#endif

#ifdef WIN32
#pragma warning(push)
#pragma warning(disable : 4458) // declaration of 'xxx' hides class member
#include <wingdi.h>
#include <Uxtheme.h>
#include <gdiplus.h>

#pragma comment (lib, "Msimg32.lib")
#pragma comment (lib, "UxTheme.lib")

namespace utils {
    namespace gdi32 {
        inline void replaceColor(HBITMAP hBitmap, COLORREF src, COLORREF dst) {
            if (hBitmap) {
                HDC hDC = GetDC(HWND_DESKTOP);
                if (hDC) {
                    BITMAP bm = { 0 };
                    if (GetObject(hBitmap, sizeof(bm), &bm) && bm.bmWidth > 0 && bm.bmHeight > 0) {
                        BITMAPINFO bi = { 0 };
                        bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
                        bi.bmiHeader.biWidth = bm.bmWidth;
                        bi.bmiHeader.biHeight = bm.bmHeight;
                        bi.bmiHeader.biPlanes = bm.bmPlanes;
                        bi.bmiHeader.biBitCount = bm.bmBitsPixel;

                        std::vector<BYTE> pixels(bm.bmWidthBytes * bm.bmHeight);
                        GetDIBits(hDC, hBitmap, 0, bm.bmHeight, &pixels[0], &bi, DIB_RGB_COLORS);

                        auto a = LOBYTE((dst) >> 24);
                        auto r = GetRValue(dst);
                        auto g = GetGValue(dst);
                        auto b = GetBValue(dst);
                        auto src24 = src & 0x00FFFFFF;
                        auto bytesCount = bm.bmBitsPixel / 8;
                        for (auto y = 0; y < bm.bmHeight; y++) {
                            for (auto x = 0; x < bm.bmWidth; x++) {
                                auto i = x * bytesCount + y * bm.bmWidthBytes;
                                if (RGB(pixels.at(i + 0), pixels.at(i + 1), pixels.at(i + 2)) == src24) {
                                    if (bm.bmBitsPixel == 32) {
                                        if (pixels.at(i + 3) == a) {
                                            pixels[i + 0] = r;
                                            pixels[i + 1] = g;
                                            pixels[i + 2] = b;
                                        }
                                    } else {
                                        pixels[i + 0] = r;
                                        pixels[i + 1] = g;
                                        pixels[i + 2] = b;
                                    }
                                }
                            }
                        }
                        SetDIBits(hDC, hBitmap, 0, bm.bmHeight, &pixels[0], &bi, DIB_RGB_COLORS);
                    }
                    ReleaseDC(HWND_DESKTOP, hDC);
                }
            }
        }

        inline COLORREF GetPixel(HBITMAP hBitmap, int x, int y) {
            COLORREF pixel = 0;
            if (hBitmap) {
                HDC hDC = GetDC(HWND_DESKTOP);
                if (hDC) {
                    BITMAP bm = { 0 };
                    if (GetObject(hBitmap, sizeof(bm), &bm)) {
                        BITMAPINFO bi = { 0 };
                        bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
                        bi.bmiHeader.biWidth = bm.bmWidth;
                        bi.bmiHeader.biHeight = bm.bmHeight;
                        bi.bmiHeader.biPlanes = bm.bmPlanes;
                        bi.bmiHeader.biBitCount = bm.bmBitsPixel;

                        std::vector<BYTE> pixels(bm.bmWidthBytes * bm.bmHeight);
                        GetDIBits(hDC, hBitmap, 0, bm.bmHeight, &pixels[0], &bi, DIB_RGB_COLORS);
                        auto position = x + y * bm.bmWidthBytes;
                        if (position < pixels.size()) {
                            if (bm.bmBitsPixel == 32) {
                                pixel = RGB(pixels[position + 0], pixels[position + 1], pixels[position + 2]) | (pixels[position + 3] << 24);
                            } else {
                                pixel = RGB(pixels[position + 0], pixels[position + 1], pixels[position + 2]) | 0xFF000000;
                            }
                        }
                    }
                    ReleaseDC(HWND_DESKTOP, hDC);
                }
            }
            return pixel;
        }

        inline HBITMAP CreateTransparentBitmap(HBITMAP hBitmap) {
            HBITMAP bitmap = nullptr;
            if (hBitmap) {
                HDC hDC = GetDC(HWND_DESKTOP);
                if (hDC) {
                    BITMAP bm = { 0 };
                    if (GetObject(hBitmap, sizeof(bm), &bm) && bm.bmWidth > 0 && bm.bmHeight > 0) {
                        BITMAPINFO bi = { 0 };
                        bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
                        bi.bmiHeader.biWidth = bm.bmWidth;
                        bi.bmiHeader.biHeight = bm.bmHeight;
                        bi.bmiHeader.biPlanes = bm.bmPlanes;
                        bi.bmiHeader.biBitCount = bm.bmBitsPixel;

                        std::vector<BYTE> pixels(bm.bmWidthBytes * bm.bmHeight);
                        std::vector<BYTE> pixels32(bm.bmWidth * bm.bmHeight * 4);
                        GetDIBits(hDC, hBitmap, 0, bm.bmHeight, &pixels[0], &bi, DIB_RGB_COLORS);
                        if (bm.bmBitsPixel == 32) {
                            bitmap = ::CreateBitmap(bm.bmWidth, bm.bmHeight, 1, pixels.size(), &pixels[0]);
                        } else {
                            COLORREF pixel = 0;
                            auto bytesCount = bm.bmBitsPixel / 8;
                            auto j = 0;
                            for (auto y = bm.bmHeight - 1; y >= 0; y--) {
                                for (auto x = 0; x < bm.bmWidth; x++) {
                                    auto i = x * bytesCount + y * bm.bmWidthBytes;
                                    if (i == 0) {
                                        pixel = RGB(pixels[i + 0], pixels[i + 1], pixels[i + 2]);
                                    }
                                    pixels32[j++] = pixels[i + 0];
                                    pixels32[j++] = pixels[i + 1];
                                    pixels32[j++] = pixels[i + 2];
                                    if (RGB(pixels[i + 0], pixels[i + 1], pixels[i + 2]) == pixel) {
                                        pixels32[j++] = 0;
                                    } else {
                                        pixels32[j++] = pixels[i + 3];
                                    }
                                }
                            }
                            bitmap = ::CreateBitmap(bm.bmWidth, bm.bmHeight, 1, 32, &pixels32[0]);
                        }
                    }
                    ReleaseDC(HWND_DESKTOP, hDC);
                }
            }
            return bitmap;
        }

        inline bool isAlphaExists(Gdiplus::ARGB* p, SIZE& size, int cxRow) {
            bool b = false;
            auto offset = cxRow - size.cx;
            for (auto y = size.cy; y && !b; --y) {
                for (auto x = size.cx; x && !b; --x) {
                    if (*p++ & 0xFF000000) {
                        b = true;
                    }
                }
                p += offset;
            }
            return b;
        }

        inline bool CreateArgb32(HDC hdc, Gdiplus::ARGB* pArgb, HBITMAP hBitmap, SIZE& size, int cxRow) {
            auto b = false;
            BITMAPINFO bmi = { 0 };
            bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
            bmi.bmiHeader.biPlanes = 1;
            bmi.bmiHeader.biCompression = BI_RGB;
            bmi.bmiHeader.biWidth = size.cx;
            bmi.bmiHeader.biHeight = size.cy;
            bmi.bmiHeader.biBitCount = 32;

            auto hHeap = GetProcessHeap();
            void* pvBits = HeapAlloc(hHeap, 0, bmi.bmiHeader.biWidth * 4 * bmi.bmiHeader.biHeight);
            if (pvBits) {
                if (GetDIBits(hdc, hBitmap, 0, bmi.bmiHeader.biHeight, pvBits, &bmi, DIB_RGB_COLORS) == bmi.bmiHeader.biHeight) {
                    auto offset = cxRow - bmi.bmiHeader.biWidth;
                    Gdiplus::ARGB* p = static_cast<Gdiplus::ARGB*>(pvBits);
                    for (auto y = bmi.bmiHeader.biHeight; y; --y) {
                        for (auto x = bmi.bmiHeader.biWidth; x; --x) {
                            if (*p++) {
                                *pArgb++ = 0;
                            } else {
                                *pArgb++ |= 0xFF000000;
                            }
                        }
                        pArgb += offset;
                    }
                    b = true;
                }
                HeapFree(hHeap, 0, pvBits);
            }
            return b;
        }

        inline bool CreateArgb32(HPAINTBUFFER paint, HDC hDC, HICON hIcon, SIZE& size) {
            bool b = false;
            RGBQUAD* prgbQuad = nullptr;
            int cxRow = 0;
            if (SUCCEEDED(GetBufferedPaintBits(paint, &prgbQuad, &cxRow))) {
                Gdiplus::ARGB* p = reinterpret_cast<Gdiplus::ARGB*>(prgbQuad);
                ICONINFO icon = { 0 };
                if (!isAlphaExists(p, size, cxRow) &&
                    GetIconInfo(hIcon, &icon) && icon.hbmMask) {
                    b = CreateArgb32(hDC, p, icon.hbmMask, size, cxRow);
                } else {
                    b = true;
                }
            }
            return b;
        }

        inline bool CreateIconBitmap(HDC hDC, const SIZE* pSize, void** ppvBits, HBITMAP* phBitmap) {
            bool b = false;
            if (pSize && phBitmap) {
                *phBitmap = nullptr;

                BITMAPINFO bi = { 0 };
                bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
                bi.bmiHeader.biPlanes = 1;
                bi.bmiHeader.biCompression = BI_RGB;
                bi.bmiHeader.biWidth = pSize->cx;
                bi.bmiHeader.biHeight = pSize->cy;
                bi.bmiHeader.biBitCount = 32;

                HDC hMemDC = hDC ? hDC : GetDC(nullptr);
                if (hMemDC) {
                    *phBitmap = CreateDIBSection(hMemDC, &bi, DIB_RGB_COLORS, ppvBits, nullptr, 0);
                    if (hDC != hMemDC) {
                        ReleaseDC(nullptr, hMemDC);
                    }
                }
                b = (*phBitmap != nullptr);
            }
            return b;
        }

        inline HBITMAP CreateIconBitmap(HICON hIcon, int width, int height) {
            HBITMAP hBitmap = nullptr;
            if (hIcon) {
                SIZE size = { width, height };
                RECT rect{ 0, 0, width, height };
                auto hMemDC = CreateCompatibleDC(nullptr);
                if (hMemDC) {
                    if (CreateIconBitmap(hMemDC, &size, NULL, &hBitmap)) {
                        auto hGdiObj = reinterpret_cast<HBITMAP>(SelectObject(hMemDC, hBitmap));
                        if (hGdiObj) {
                            BLENDFUNCTION alpha = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
                            BP_PAINTPARAMS params = { 0 };
                            params.cbSize = sizeof(params);
                            params.dwFlags = BPPF_ERASE;
                            params.pBlendFunction = &alpha;
                            HDC hDC = nullptr;
                            auto paint = BeginBufferedPaint(hMemDC, &rect, BPBF_DIB, &params, &hDC);
                            if (paint) {
                                if (DrawIconEx(hDC, 0, 0, hIcon, size.cx, size.cy, 0, nullptr, DI_NORMAL)) {
                                    CreateArgb32(paint, hMemDC, hIcon, size);
                                }
                                EndBufferedPaint(paint, TRUE);
                            }
                            SelectObject(hMemDC, hGdiObj);
                        }
                    }
                    DeleteDC(hMemDC);
                }
            }
            return hBitmap;
        }

        inline HBITMAP CreateIconBitmap(int iconId, int width, int height) {
            auto hBitmap = HBITMAP(nullptr);
            auto hIcon = LoadIcon(reinterpret_cast<HINSTANCE>(shell32::hModule), MAKEINTRESOURCE(iconId));
            if (hIcon) {
                hBitmap = CreateIconBitmap(hIcon, width, height);
                DeleteObject(hIcon);
            }
            return hBitmap;
        }

        inline HBITMAP CreateIconBitmap(int iconId) {
            return CreateIconBitmap(iconId, 24, 24);
        }

        inline void DeleteObject(HICON hIcon) {
            ICONINFO icon = { 0 };
            if (GetIconInfo(hIcon, &icon)) {
                if (icon.hbmColor) {
                    DeleteObject(icon.hbmColor);
                }
                if (icon.hbmMask) {
                    DeleteObject(icon.hbmMask);
                }
            }
        }
    }
}

#pragma warning(pop)

#endif
