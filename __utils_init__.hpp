#pragma once 
#ifndef UNICODE
#error "THIS FILE IS BUILD FOR UNICODE ONLY"
#endif

#ifndef _WIN64
#error "THIS FILE IS BUILD FOR WINDOWS x64 ONLY"
#endif

#include <atlimage.h>
#include <WinSock2.h>

#define _WINSOCKAPI_    // stops windows.h including winsock.h

#include <windows.h>

#define wstr _utils::_wstring
#define stringstream_t std::wstringstream
#define ofstream_t std::wofstream
#define utils wstr

#define UNC_W LR"(\\)"
#define UNC_A R"(\\)"
#define PATH_SEPARATOR_W L'\\'
#define PATH_SEPARATOR_STRING_W L"\\"
#define PATH_SEPARATORA '\\'
#define PATH_SEPARATOR_STRING_A "\\"

#define UNC UNC_W
#define PATH_SEPARATOR PATH_SEPARATOR_W
#define PATH_SEPARATOR_STRING PATH_SEPARATOR_STRING_W

// https://blog.csdn.net/panjunnn/article/details/115750622
#ifdef _HAS_STD_BYTE
#undef _HAS_STD_BYTE
#endif
#define _HAS_STD_BYTE 0

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <sys/stat.h>
#include <sstream>
#include <codecvt>
#include <unordered_map>
#include <thread>
#include <mutex>
#include <regex>
#include <functional>

#include <iostream>
#include <stdio.h>
#include <conio.h>
#include <iostream>
#include <direct.h>
#include <io.h>
#include <shlobj_core.h>
#include <Shlwapi.h>
#include <comutil.h>
#include <tchar.h>
#include <wincrypt.h>
#include <tlhelp32.h>
#include <processthreadsapi.h>
#include <shellapi.h>

#pragma comment(lib, "comsuppw.lib")
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "Normaliz.lib")
#pragma comment(lib, "wldap32.lib" )
#pragma comment(lib, "crypt32.lib" )

#include "base64/base64.hpp"

#define CONTENT_DISPOSITION "Content-Disposition"
#define AUTHORIZATION "Authorization"
#define CONTENT_TYPE "Content-Type"
#define CURL_STATICLIB

#include "curl/include/curl/curl.h"

#define HOST_ANY (_T("0.0.0.0"))
#define SSL_PORT (443)
#define LOCALHOST_CRT_FILE_NAME (_T("localhost.crt"))
#define LOCALHOST_KEY_FILE_NAME (_T("localhost.key"))
#define CERTIFICATE "-----BEGIN CERTIFICATE-----\n"                                        \
                    "MIIDDTCCAfWgAwIBAgIJAKG8cwnciIStMA0GCSqGSIb3DQEBCwUAMBQxEjAQBgNV\n"   \
                    "BAMTCWxvY2FsaG9zdDAeFw0yMjEwMTQwNTE0MjhaFw0yMzEwMTQwNTE0MjhaMBQx\n"   \
                    "EjAQBgNVBAMTCWxvY2FsaG9zdDCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoC\n"   \
                    "ggEBAOSC/CsB7nBNBuPmHlV9pBUKBQIeT+s38d5aHNr+xzOV2DsienLEGbdIT11o\n"   \
                    "ztqQ73wqKcWqXWoYJM6oj7P2XIed33RkWF4PrC0uWy1RC9YEBtxitfxUr+KKFq2O\n"   \
                    "r7FBLkqH5EImzGsZfEtajhh4kTElhxDW7G6xR7uMaZbdZ9sCP79aESKr/MRkdbyf\n"   \
                    "rE/tRzka9VSbFsRUjJEJZVqdLyHauTV3t69O/Z1kan+Wsd4fclgYidVn9WLJ/4EB\n"   \
                    "jE7bPJnibMYaY6haD0AMUkYeXc3C+HTmIFRAjr6NTgrVa9NJUQE88vVVSG8NZfJu\n"   \
                    "U4Hzu0zIusddQcNgPWLDJozRTWECAwEAAaNiMGAwDAYDVR0TAQH/BAIwADAOBgNV\n"   \
                    "HQ8BAf8EBAMCBaAwFgYDVR0lAQH/BAwwCgYIKwYBBQUHAwEwFwYDVR0RAQH/BA0w\n"   \
                    "C4IJbG9jYWxob3N0MA8GCisGAQQBgjdUAQEEAQIwDQYJKoZIhvcNAQELBQADggEB\n"   \
                    "AIseuRktMe0gdqnXhNwSCausn97UPvEcV5b4NOG14jMomn8oYGRXQPRkypBFoug6\n"   \
                    "vnkL+mBi0PBslMhaZGB4RVnpTd4NHuRt52XuOr6t1bPHvZN4+2uBmo6ACnocSrZ1\n"   \
                    "PxVGjMa3kI5GeyOKw+c8la5WRrqyDI27PWjo7fpNzUwfKC6jAEZmoYfyF2ZeSiod\n"   \
                    "+j6SueaQHJF/FrT0qhDWnfC0z7nEGvwMkE9Eqpl5Zu2Q7BRnKoCnjzRv85o+XZsr\n"   \
                    "Y6TMEWiWoViC3wjPKVE1qC62VRP3caIYOrL99YgWSrHeaoGuNDyo4PUNsu3VwE95\n"   \
                    "L6LkjC+maAA4+rUexyFvLfc=\n"                                           \
                    "-----END CERTIFICATE-----\n"

#define PRIVATE_KEY "-----BEGIN PRIVATE KEY-----\n"                                        \
                    "MIIEvgIBADANBgkqhkiG9w0BAQEFAASCBKgwggSkAgEAAoIBAQDkgvwrAe5wTQbj\n"   \
                    "5h5VfaQVCgUCHk/rN/HeWhza/sczldg7InpyxBm3SE9daM7akO98KinFql1qGCTO\n"   \
                    "qI+z9lyHnd90ZFheD6wtLlstUQvWBAbcYrX8VK/iihatjq+xQS5Kh+RCJsxrGXxL\n"   \
                    "Wo4YeJExJYcQ1uxusUe7jGmW3WfbAj+/WhEiq/zEZHW8n6xP7Uc5GvVUmxbEVIyR\n"   \
                    "CWVanS8h2rk1d7evTv2dZGp/lrHeH3JYGInVZ/Viyf+BAYxO2zyZ4mzGGmOoWg9A\n"   \
                    "DFJGHl3Nwvh05iBUQI6+jU4K1WvTSVEBPPL1VUhvDWXyblOB87tMyLrHXUHDYD1i\n"   \
                    "wyaM0U1hAgMBAAECggEAcC7V+Y8zLbLDBTM+BR5vZVzL5KRY+9M1TMHUlLaruOmB\n"   \
                    "hFaC4sDfFxnKzMLdenPY/o38f+wJfEJqH4KlOeZiurmIsC1rLJJpJq0xWegF9YV4\n"   \
                    "kM1tJnm093RvebkpC85CGZjpOIivpoyWdq/4Po9CSGxyCJRa8mTaIHrog4YirGNa\n"   \
                    "hfxG4rY8cU+0+QbP52bDL7DGHVJ7dAR4UuQDOi+M8ymzhNIR6VLX2Ax3HlkFOJkp\n"   \
                    "aW0JAGP2jSQib22MQqDJR+60hYKFqcuCZv+eVWrHX6FgyvpPLpF2ZO78ZiHmbcP1\n"   \
                    "Y7VVRyzM/nF15pgWymhiXA5GX2L6FcAHNMrRXBDquQKBgQDmwWjilIZmk/1ljyLI\n"   \
                    "1jZW8YwV9neGNlHanuoF4yTesn2f5iY3ga/KcaimV1wc8JaRTHEPOkabQVkAl/t3\n"   \
                    "ZLhQlz/gw9vHzDoTo9MhxLAoVxhebXZXxnFCHl5wOEyEOEHegIoJ01YlYXB3j8nT\n"   \
                    "ZOljbKyz0p9h9uA6UUp3xUdzZwKBgQD9grvRKspEGFcZh2qk8yELgP15AvMyjMUX\n"   \
                    "YiKBxkiAR/j/ahqgDsaLa3r5JHzxwRHERPBsQbBb42Op41fAijvHi/v2CdvbPZ4Q\n"   \
                    "T0QEFTXF/IZIpQ0B2fNaAXujbU6LqQOSNHvHUAieuQeAP20YOfIh/SmVAJo3YXsm\n"   \
                    "mLQyqctD9wKBgQCHyJ+TVJqRmdut+0AYthRbrN8lvVYv357i6kgDFtoHQTiy8pbi\n"   \
                    "aqmqkp8O4rENRnUTIswUjdImYZmhw8zxQI9EvPuVoyqcswwpqmj+oeKWmkl4pyjQ\n"   \
                    "FbFnmr8vgbFkzEXWTATuKzGx5VhswmucvPCo1LX7SqEG4GKpblRHjgqxWQKBgQCZ\n"   \
                    "GYoKAViUFUsMzN5X7ckoJGdv3Js5DRC+VXLXgwMoDc9PmSmpPiMj8e4bZrDaY6oy\n"   \
                    "KRw1lNpINCRtZjIJwrHliiHJbhzL5AOAgA5olNKb1izW1G7tjk3MLpcXlJzZ9WGF\n"   \
                    "u5KBRVFfYgkop6Vf+ybC5hxlsy5vgbyZC6HP8wEh8wKBgHA3ZMU7s6ZcBDXneKuk\n"   \
                    "NZ1GGscGV0WRgqsaHPlHKlMSZ3Sy3MtHSoh1aTTKJiF7ESwVR+B350gV8JK77baC\n"   \
                    "yaPYwGgV2rxhvrtfLhG5wQ6EFh9OO63iLCPv8JvPugehQn917WjuiiYKfn9jEi1d\n"   \
                    "qSov5MqodQK1DhdgI10IfiuX\n"                                           \
                    "-----END PRIVATE KEY-----\n"
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "cpp-httplib/v0.11.4+/httplib.h"
#pragma comment(lib, __FILE__ "/../OpenSSL/3.0.6+/VC-WIN64A/lib/libcrypto.lib")
#pragma comment(lib, __FILE__ "/../OpenSSL/3.0.6+/VC-WIN64A/lib/libssl.lib")
#pragma comment(lib, __FILE__ "/../curl/builds/libcurl-vc-x64-release-dll-ipv6-sspi-schannel/lib/libcurl.lib")

static class __utils_init__ {
public:
    struct mime_entry {
        std::wstring extension;
        std::string name;
        mime_entry(const wchar_t* extension, const char* name) {
            this->extension = extension;
            this->name = name;
            if (this->extension.back() != L'\0') {
                this->extension += L'\0';
            }
            if (this->name.back() != '\0') {
                this->name += '\0';
            }
        }
    };

    __utils_init__() {
        setbuf(stdout, 0);
        this->http_timeout = CPPHTTPLIB_READ_TIMEOUT_SECOND;
        this->init_mime_mime_types();
    }

    void init_mime_mime_types() {
        mime_types.emplace_back(L".3gpp", "audio/3gpp");
        mime_types.emplace_back(L".jpm", "video/jpm");
        mime_types.emplace_back(L".mp3", "audio/mp3");
        mime_types.emplace_back(L".rtf", "text/rtf");
        mime_types.emplace_back(L".wav", "audio/wave");
        mime_types.emplace_back(L".xml", "text/xml");
        mime_types.emplace_back(L".3g2", "video/3gpp2");
        mime_types.emplace_back(L".3gp", "video/3gpp");
        mime_types.emplace_back(L".3gpp", "video/3gpp");
        mime_types.emplace_back(L".ac", "application/pkix-attr-cert");
        mime_types.emplace_back(L".adp", "audio/adpcm");
        mime_types.emplace_back(L".ai", "application/postscript");
        mime_types.emplace_back(L".apng", "image/apng");
        mime_types.emplace_back(L".appcache", "text/cache-manifest");
        mime_types.emplace_back(L".asc", "application/pgp-signature");
        mime_types.emplace_back(L".atom", "application/atom+xml");
        mime_types.emplace_back(L".atomcat", "application/atomcat+xml");
        mime_types.emplace_back(L".atomsvc", "application/atomsvc+xml");
        mime_types.emplace_back(L".au", "audio/basic");
        mime_types.emplace_back(L".aw", "application/applixware");
        mime_types.emplace_back(L".bdoc", "application/bdoc");
        mime_types.emplace_back(L".bin", "application/octet-stream");
        mime_types.emplace_back(L".bmp", "image/bmp");
        mime_types.emplace_back(L".bpk", "application/octet-stream");
        mime_types.emplace_back(L".buffer", "application/octet-stream");
        mime_types.emplace_back(L".ccxml", "application/ccxml+xml");
        mime_types.emplace_back(L".cdmia", "application/cdmi-capability");
        mime_types.emplace_back(L".cdmic", "application/cdmi-container");
        mime_types.emplace_back(L".cdmid", "application/cdmi-domain");
        mime_types.emplace_back(L".cdmio", "application/cdmi-object");
        mime_types.emplace_back(L".cdmiq", "application/cdmi-queue");
        mime_types.emplace_back(L".cer", "application/pkix-cert");
        mime_types.emplace_back(L".cgm", "image/cgm");
        mime_types.emplace_back(L".class", "application/java-vm");
        mime_types.emplace_back(L".coffee", "text/coffeescript");
        mime_types.emplace_back(L".conf", "text/plain");
        mime_types.emplace_back(L".cpt", "application/mac-compactpro");
        mime_types.emplace_back(L".crl", "application/pkix-crl");
        mime_types.emplace_back(L".css", "text/css");
        mime_types.emplace_back(L".csv", "text/csv");
        mime_types.emplace_back(L".cu", "application/cu-seeme");
        mime_types.emplace_back(L".davmount", "application/davmount+xml");
        mime_types.emplace_back(L".dbk", "application/docbook+xml");
        mime_types.emplace_back(L".deb", "application/octet-stream");
        mime_types.emplace_back(L".def", "text/plain");
        mime_types.emplace_back(L".deploy", "application/octet-stream");
        mime_types.emplace_back(L".disposition-notification", "message/disposition-notification");
        mime_types.emplace_back(L".dist", "application/octet-stream");
        mime_types.emplace_back(L".distz", "application/octet-stream");
        mime_types.emplace_back(L".dll", "application/octet-stream");
        mime_types.emplace_back(L".dmg", "application/octet-stream");
        mime_types.emplace_back(L".dms", "application/octet-stream");
        mime_types.emplace_back(L".doc", "application/msword");
        mime_types.emplace_back(L".dot", "application/msword");
        mime_types.emplace_back(L".drle", "image/dicom-rle");
        mime_types.emplace_back(L".dssc", "application/dssc+der");
        mime_types.emplace_back(L".dtd", "application/xml-dtd");
        mime_types.emplace_back(L".dump", "application/octet-stream");
        mime_types.emplace_back(L".ear", "application/java-archive");
        mime_types.emplace_back(L".ecma", "application/ecmascript");
        mime_types.emplace_back(L".elc", "application/octet-stream");
        mime_types.emplace_back(L".emf", "image/emf");
        mime_types.emplace_back(L".eml", "message/rfc822");
        mime_types.emplace_back(L".emma", "application/emma+xml");
        mime_types.emplace_back(L".eps", "application/postscript");
        mime_types.emplace_back(L".epub", "application/epub+zip");
        mime_types.emplace_back(L".es", "application/ecmascript");
        mime_types.emplace_back(L".exe", "application/octet-stream");
        mime_types.emplace_back(L".exi", "application/exi");
        mime_types.emplace_back(L".exr", "image/aces");
        mime_types.emplace_back(L".ez", "application/andrew-inset");
        mime_types.emplace_back(L".fits", "image/fits");
        mime_types.emplace_back(L".g3", "image/g3fax");
        mime_types.emplace_back(L".gbr", "application/rpki-ghostbusters");
        mime_types.emplace_back(L".geojson", "application/geo+json");
        mime_types.emplace_back(L".gif", "image/gif");
        mime_types.emplace_back(L".glb", "model/gltf-binary");
        mime_types.emplace_back(L".gltf", "model/gltf+json");
        mime_types.emplace_back(L".gml", "application/gml+xml");
        mime_types.emplace_back(L".gpx", "application/gpx+xml");
        mime_types.emplace_back(L".gram", "application/srgs");
        mime_types.emplace_back(L".grxml", "application/srgs+xml");
        mime_types.emplace_back(L".gxf", "application/gxf");
        mime_types.emplace_back(L".gz", "application/gzip");
        mime_types.emplace_back(L".h261", "video/h261");
        mime_types.emplace_back(L".h263", "video/h263");
        mime_types.emplace_back(L".h264", "video/h264");
        mime_types.emplace_back(L".heic", "image/heic");
        mime_types.emplace_back(L".heics", "image/heic-sequence");
        mime_types.emplace_back(L".heif", "image/heif");
        mime_types.emplace_back(L".heifs", "image/heif-sequence");
        mime_types.emplace_back(L".hjson", "application/hjson");
        mime_types.emplace_back(L".hlp", "application/winhlp");
        mime_types.emplace_back(L".hqx", "application/mac-binhex40");
        mime_types.emplace_back(L".htm", "text/html");
        mime_types.emplace_back(L".html", "text/html");
        mime_types.emplace_back(L".ico", "image/x-icon");
        mime_types.emplace_back(L".ics", "text/calendar");
        mime_types.emplace_back(L".ief", "image/ief");
        mime_types.emplace_back(L".ifb", "text/calendar");
        mime_types.emplace_back(L".iges", "model/iges");
        mime_types.emplace_back(L".igs", "model/iges");
        mime_types.emplace_back(L".img", "application/octet-stream");
        mime_types.emplace_back(L".in", "text/plain");
        mime_types.emplace_back(L".ini", "text/plain");
        mime_types.emplace_back(L".ink", "application/inkml+xml");
        mime_types.emplace_back(L".inkml", "application/inkml+xml");
        mime_types.emplace_back(L".ipfix", "application/ipfix");
        mime_types.emplace_back(L".iso", "application/octet-stream");
        mime_types.emplace_back(L".jade", "text/jade");
        mime_types.emplace_back(L".jar", "application/java-archive");
        mime_types.emplace_back(L".jls", "image/jls");
        mime_types.emplace_back(L".jp2", "image/jp2");
        mime_types.emplace_back(L".jpe", "image/jpeg");
        mime_types.emplace_back(L".jpeg", "image/jpeg");
        mime_types.emplace_back(L".jpf", "image/jpx");
        mime_types.emplace_back(L".jpg", "image/jpeg");
        mime_types.emplace_back(L".jpg2", "image/jp2");
        mime_types.emplace_back(L".jpgm", "video/jpm");
        mime_types.emplace_back(L".jpgv", "video/jpeg");
        mime_types.emplace_back(L".jpm", "image/jpm");
        mime_types.emplace_back(L".jpx", "image/jpx");
        mime_types.emplace_back(L".js", "application/javascript");
        mime_types.emplace_back(L".json", "application/json");
        mime_types.emplace_back(L".json5", "application/json5");
        mime_types.emplace_back(L".jsonld", "application/ld+json");
        mime_types.emplace_back(L".jsonml", "application/jsonml+json");
        mime_types.emplace_back(L".jsx", "text/jsx");
        mime_types.emplace_back(L".kar", "audio/midi");
        mime_types.emplace_back(L".ktx", "image/ktx");
        mime_types.emplace_back(L".less", "text/less");
        mime_types.emplace_back(L".list", "text/plain");
        mime_types.emplace_back(L".litcoffee", "text/coffeescript");
        mime_types.emplace_back(L".log", "text/plain");
        mime_types.emplace_back(L".lostxml", "application/lost+xml");
        mime_types.emplace_back(L".lrf", "application/octet-stream");
        mime_types.emplace_back(L".m1v", "video/mpeg");
        mime_types.emplace_back(L".m21", "application/mp21");
        mime_types.emplace_back(L".m2a", "audio/mpeg");
        mime_types.emplace_back(L".m2v", "video/mpeg");
        mime_types.emplace_back(L".m3a", "audio/mpeg");
        mime_types.emplace_back(L".m4a", "audio/mp4");
        mime_types.emplace_back(L".m4p", "application/mp4");
        mime_types.emplace_back(L".ma", "application/mathematica");
        mime_types.emplace_back(L".mads", "application/mads+xml");
        mime_types.emplace_back(L".man", "text/troff");
        mime_types.emplace_back(L".manifest", "text/cache-manifest");
        mime_types.emplace_back(L".map", "application/json");
        mime_types.emplace_back(L".mar", "application/octet-stream");
        mime_types.emplace_back(L".markdown", "text/markdown");
        mime_types.emplace_back(L".mathml", "application/mathml+xml");
        mime_types.emplace_back(L".mb", "application/mathematica");
        mime_types.emplace_back(L".mbox", "application/mbox");
        mime_types.emplace_back(L".md", "text/markdown");
        mime_types.emplace_back(L".me", "text/troff");
        mime_types.emplace_back(L".mesh", "model/mesh");
        mime_types.emplace_back(L".meta4", "application/metalink4+xml");
        mime_types.emplace_back(L".metalink", "application/metalink+xml");
        mime_types.emplace_back(L".mets", "application/mets+xml");
        mime_types.emplace_back(L".mft", "application/rpki-manifest");
        mime_types.emplace_back(L".mid", "audio/midi");
        mime_types.emplace_back(L".midi", "audio/midi");
        mime_types.emplace_back(L".mime", "message/rfc822");
        mime_types.emplace_back(L".mj2", "video/mj2");
        mime_types.emplace_back(L".mjp2", "video/mj2");
        mime_types.emplace_back(L".mjs", "application/javascript");
        mime_types.emplace_back(L".mml", "text/mathml");
        mime_types.emplace_back(L".mods", "application/mods+xml");
        mime_types.emplace_back(L".mov", "video/quicktime");
        mime_types.emplace_back(L".mp2", "audio/mpeg");
        mime_types.emplace_back(L".mp21", "application/mp21");
        mime_types.emplace_back(L".mp2a", "audio/mpeg");
        mime_types.emplace_back(L".mp3", "audio/mpeg");
        mime_types.emplace_back(L".mp4", "video/mp4");
        mime_types.emplace_back(L".mp4a", "audio/mp4");
        mime_types.emplace_back(L".mp4s", "application/mp4");
        mime_types.emplace_back(L".mp4v", "video/mp4");
        mime_types.emplace_back(L".mpd", "application/dash+xml");
        mime_types.emplace_back(L".mpe", "video/mpeg");
        mime_types.emplace_back(L".mpeg", "video/mpeg");
        mime_types.emplace_back(L".mpg", "video/mpeg");
        mime_types.emplace_back(L".mpg4", "video/mp4");
        mime_types.emplace_back(L".mpga", "audio/mpeg");
        mime_types.emplace_back(L".mrc", "application/marc");
        mime_types.emplace_back(L".mrcx", "application/marcxml+xml");
        mime_types.emplace_back(L".ms", "text/troff");
        mime_types.emplace_back(L".mscml", "application/mediaservercontrol+xml");
        mime_types.emplace_back(L".msh", "model/mesh");
        mime_types.emplace_back(L".msi", "application/octet-stream");
        mime_types.emplace_back(L".msm", "application/octet-stream");
        mime_types.emplace_back(L".msp", "application/octet-stream");
        mime_types.emplace_back(L".mxf", "application/mxf");
        mime_types.emplace_back(L".mxml", "application/xv+xml");
        mime_types.emplace_back(L".n3", "text/n3");
        mime_types.emplace_back(L".nb", "application/mathematica");
        mime_types.emplace_back(L".oda", "application/oda");
        mime_types.emplace_back(L".oga", "audio/ogg");
        mime_types.emplace_back(L".ogg", "audio/ogg");
        mime_types.emplace_back(L".ogv", "video/ogg");
        mime_types.emplace_back(L".ogx", "application/ogg");
        mime_types.emplace_back(L".omdoc", "application/omdoc+xml");
        mime_types.emplace_back(L".onepkg", "application/onenote");
        mime_types.emplace_back(L".onetmp", "application/onenote");
        mime_types.emplace_back(L".onetoc", "application/onenote");
        mime_types.emplace_back(L".onetoc2", "application/onenote");
        mime_types.emplace_back(L".opf", "application/oebps-package+xml");
        mime_types.emplace_back(L".otf", "font/otf");
        mime_types.emplace_back(L".owl", "application/rdf+xml");
        mime_types.emplace_back(L".oxps", "application/oxps");
        mime_types.emplace_back(L".p10", "application/pkcs10");
        mime_types.emplace_back(L".p7c", "application/pkcs7-mime");
        mime_types.emplace_back(L".p7m", "application/pkcs7-mime");
        mime_types.emplace_back(L".p7s", "application/pkcs7-signature");
        mime_types.emplace_back(L".p8", "application/pkcs8");
        mime_types.emplace_back(L".pdf", "application/pdf");
        mime_types.emplace_back(L".pfr", "application/font-tdpfr");
        mime_types.emplace_back(L".pgp", "application/pgp-encrypted");
        mime_types.emplace_back(L".pkg", "application/octet-stream");
        mime_types.emplace_back(L".pki", "application/pkixcmp");
        mime_types.emplace_back(L".pkipath", "application/pkix-pkipath");
        mime_types.emplace_back(L".pls", "application/pls+xml");
        mime_types.emplace_back(L".png", "image/png");
        mime_types.emplace_back(L".prf", "application/pics-rules");
        mime_types.emplace_back(L".ps", "application/postscript");
        mime_types.emplace_back(L".pskcxml", "application/pskc+xml");
        mime_types.emplace_back(L".qt", "video/quicktime");
        mime_types.emplace_back(L".raml", "application/raml+yaml");
        mime_types.emplace_back(L".rdf", "application/rdf+xml");
        mime_types.emplace_back(L".rif", "application/reginfo+xml");
        mime_types.emplace_back(L".rl", "application/resource-lists+xml");
        mime_types.emplace_back(L".rld", "application/resource-lists-diff+xml");
        mime_types.emplace_back(L".rmi", "audio/midi");
        mime_types.emplace_back(L".rnc", "application/relax-ng-compact-syntax");
        mime_types.emplace_back(L".rng", "application/xml");
        mime_types.emplace_back(L".roa", "application/rpki-roa");
        mime_types.emplace_back(L".roff", "text/troff");
        mime_types.emplace_back(L".rq", "application/sparql-query");
        mime_types.emplace_back(L".rs", "application/rls-services+xml");
        mime_types.emplace_back(L".rsd", "application/rsd+xml");
        mime_types.emplace_back(L".rss", "application/rss+xml");
        mime_types.emplace_back(L".rtf", "application/rtf");
        mime_types.emplace_back(L".rtx", "text/richtext");
        mime_types.emplace_back(L".s3m", "audio/s3m");
        mime_types.emplace_back(L".sbml", "application/sbml+xml");
        mime_types.emplace_back(L".scq", "application/scvp-cv-request");
        mime_types.emplace_back(L".scs", "application/scvp-cv-response");
        mime_types.emplace_back(L".sdp", "application/sdp");
        mime_types.emplace_back(L".ser", "application/java-serialized-object");
        mime_types.emplace_back(L".setpay", "application/set-payment-initiation");
        mime_types.emplace_back(L".setreg", "application/set-registration-initiation");
        mime_types.emplace_back(L".sgi", "image/sgi");
        mime_types.emplace_back(L".sgm", "text/sgml");
        mime_types.emplace_back(L".sgml", "text/sgml");
        mime_types.emplace_back(L".shex", "text/shex");
        mime_types.emplace_back(L".shf", "application/shf+xml");
        mime_types.emplace_back(L".shtml", "text/html");
        mime_types.emplace_back(L".sig", "application/pgp-signature");
        mime_types.emplace_back(L".sil", "audio/silk");
        mime_types.emplace_back(L".silo", "model/mesh");
        mime_types.emplace_back(L".slim", "text/slim");
        mime_types.emplace_back(L".slm", "text/slim");
        mime_types.emplace_back(L".smi", "application/smil+xml");
        mime_types.emplace_back(L".smil", "application/smil+xml");
        mime_types.emplace_back(L".snd", "audio/basic");
        mime_types.emplace_back(L".so", "application/octet-stream");
        mime_types.emplace_back(L".spp", "application/scvp-vp-response");
        mime_types.emplace_back(L".spq", "application/scvp-vp-request");
        mime_types.emplace_back(L".spx", "audio/ogg");
        mime_types.emplace_back(L".sru", "application/sru+xml");
        mime_types.emplace_back(L".srx", "application/sparql-results+xml");
        mime_types.emplace_back(L".ssdl", "application/ssdl+xml");
        mime_types.emplace_back(L".ssml", "application/ssml+xml");
        mime_types.emplace_back(L".stk", "application/hyperstudio");
        mime_types.emplace_back(L".styl", "text/stylus");
        mime_types.emplace_back(L".stylus", "text/stylus");
        mime_types.emplace_back(L".svg", "image/svg+xml");
        mime_types.emplace_back(L".svgz", "image/svg+xml");
        mime_types.emplace_back(L".t", "text/troff");
        mime_types.emplace_back(L".t38", "image/t38");
        mime_types.emplace_back(L".tei", "application/tei+xml");
        mime_types.emplace_back(L".teicorpus", "application/tei+xml");
        mime_types.emplace_back(L".text", "text/plain");
        mime_types.emplace_back(L".tfi", "application/thraud+xml");
        mime_types.emplace_back(L".tfx", "image/tiff-fx");
        mime_types.emplace_back(L".tif", "image/tiff");
        mime_types.emplace_back(L".tiff", "image/tiff");
        mime_types.emplace_back(L".tr", "text/troff");
        mime_types.emplace_back(L".ts", "video/mp2t");
        mime_types.emplace_back(L".tsd", "application/timestamped-data");
        mime_types.emplace_back(L".tsv", "text/tab-separated-values");
        mime_types.emplace_back(L".ttc", "font/collection");
        mime_types.emplace_back(L".ttf", "font/ttf");
        mime_types.emplace_back(L".ttl", "text/turtle");
        mime_types.emplace_back(L".txt", "text/plain");
        mime_types.emplace_back(L".u8dsn", "message/global-delivery-status");
        mime_types.emplace_back(L".u8hdr", "message/global-headers");
        mime_types.emplace_back(L".u8mdn", "message/global-disposition-notification");
        mime_types.emplace_back(L".u8msg", "message/global");
        mime_types.emplace_back(L".uri", "text/uri-list");
        mime_types.emplace_back(L".uris", "text/uri-list");
        mime_types.emplace_back(L".urls", "text/uri-list");
        mime_types.emplace_back(L".vcard", "text/vcard");
        mime_types.emplace_back(L".vrml", "model/vrml");
        mime_types.emplace_back(L".vtt", "text/vtt");
        mime_types.emplace_back(L".vxml", "application/voicexml+xml");
        mime_types.emplace_back(L".war", "application/java-archive");
        mime_types.emplace_back(L".wasm", "application/wasm");
        mime_types.emplace_back(L".wav", "audio/wav");
        mime_types.emplace_back(L".weba", "audio/webm");
        mime_types.emplace_back(L".webm", "video/webm");
        mime_types.emplace_back(L".webmanifest", "application/manifest+json");
        mime_types.emplace_back(L".webp", "image/webp");
        mime_types.emplace_back(L".wgt", "application/widget");
        mime_types.emplace_back(L".wmf", "image/wmf");
        mime_types.emplace_back(L".woff", "font/woff");
        mime_types.emplace_back(L".woff2", "font/woff2");
        mime_types.emplace_back(L".wrl", "model/vrml");
        mime_types.emplace_back(L".wsdl", "application/wsdl+xml");
        mime_types.emplace_back(L".wspolicy", "application/wspolicy+xml");
        mime_types.emplace_back(L".x3d", "model/x3d+xml");
        mime_types.emplace_back(L".x3db", "model/x3d+binary");
        mime_types.emplace_back(L".x3dbz", "model/x3d+binary");
        mime_types.emplace_back(L".x3dv", "model/x3d+vrml");
        mime_types.emplace_back(L".x3dvz", "model/x3d+vrml");
        mime_types.emplace_back(L".x3dz", "model/x3d+xml");
        mime_types.emplace_back(L".xaml", "application/xaml+xml");
        mime_types.emplace_back(L".xdf", "application/xcap-diff+xml");
        mime_types.emplace_back(L".xdssc", "application/dssc+xml");
        mime_types.emplace_back(L".xenc", "application/xenc+xml");
        mime_types.emplace_back(L".xer", "application/patch-ops-error+xml");
        mime_types.emplace_back(L".xht", "application/xhtml+xml");
        mime_types.emplace_back(L".xhtml", "application/xhtml+xml");
        mime_types.emplace_back(L".xhvml", "application/xv+xml");
        mime_types.emplace_back(L".xm", "audio/xm");
        mime_types.emplace_back(L".xml", "application/xml");
        mime_types.emplace_back(L".xop", "application/xop+xml");
        mime_types.emplace_back(L".xpl", "application/xproc+xml");
        mime_types.emplace_back(L".xsd", "application/xml");
        mime_types.emplace_back(L".xsl", "application/xml");
        mime_types.emplace_back(L".xslt", "application/xslt+xml");
        mime_types.emplace_back(L".xspf", "application/xspf+xml");
        mime_types.emplace_back(L".xvm", "application/xv+xml");
        mime_types.emplace_back(L".xvml", "application/xv+xml");
        mime_types.emplace_back(L".yaml", "text/yaml");
        mime_types.emplace_back(L".yang", "application/yang");
        mime_types.emplace_back(L".yin", "application/yin+xml");
        mime_types.emplace_back(L".yml", "text/yaml");
        mime_types.emplace_back(L".zip", "application/zip");
    }

    int http_timeout;
    std::vector<mime_entry> mime_types;
    std::recursive_mutex mutex_singleton;
    std::unordered_map<std::wstring, std::recursive_mutex*> mutexes;
} _utils_init_;