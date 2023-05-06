#pragma once
/*
 * CMakeLists.txt Darwin
 *
 * include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../common/github.com/unwitting.life/cpp-utils/OpenSSL/3.0.6+/darwin64-x86_64-cc/include)
 * link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../common/github.com/unwitting.life/cpp-utils/OpenSSL/3.0.6+/darwin64-x86_64-cc/lib)
 * add_executable(${PROJECT_NAME} main.cpp)
 * target_link_libraries(${PROJECT_NAME} crypto ssl)
 */

#include "utils.hpp"
#define CPPHTTPLIB_OPENSSL_SUPPORT

#include "cpp-httplib/v0.11.4+/httplib.h"
#include "MimeTypes/MimeTypes.h"
#ifdef _MSC_VER

#ifdef _WIN64
#pragma comment(lib, __FILE__ "/../OpenSSL/3.0.6+/VC-WIN64A/lib/libcrypto.lib")
#pragma comment(lib, __FILE__ "/../OpenSSL/3.0.6+/VC-WIN64A/lib/libssl.lib")
#else
#pragma comment(lib, __FILE__ "/../OpenSSL/3.0.6+/VC-WIN32/lib/libcrypto.lib")
#pragma comment(lib, __FILE__ "/../OpenSSL/3.0.6+/VC-WIN32/lib/libssl.lib")
#endif

#endif

namespace _utils {
    namespace httplib {
        namespace header {
            constexpr auto Location ="Location";
            constexpr auto Referer = "referer";
        }

        namespace schema {
            constexpr auto HTTP = _T("http://");
            constexpr auto HTTPS = _T("https://");
        }

        namespace status {
            constexpr int UNKNOWN = 0;
            constexpr int CONNECTION_BREAK = -1;
            constexpr int SYSTEM_ERROR = -2;
            constexpr int OK = 200;
            constexpr int PERMANENTLY_MOVED = 301;
            constexpr int TEMPORARILY_MOVED = 302;
            constexpr int NOT_FOUND = 404;
        }

        namespace method {
            constexpr auto GET = _T("GET");
            constexpr auto POST = _T("POST");
            constexpr auto HEAD = _T("HEAD");
        }

        struct PROXY {
            std::string host;
            int port;
            std::string userName;
            std::string password;

            PROXY() {}

            PROXY(std::string host) {
                this->setProxy(host);
            }

            PROXY(std::wstring host) {
                this->setProxy(host);
            }

            PROXY(std::string host, int port) {
                this->host = host;
                this->port = port;
            }

            PROXY(std::wstring host, int port) {
                this->host = _utils::strings::w2s(host);
                this->port = port;
            }

            void setProxy(std::string proxy) {
                auto host = std::string();
                auto port = 0;
                auto pos = proxy.find(":");
                if (pos != std::string::npos) {
                    host = proxy.substr(0, pos);
                    port = _utils::strings::atoiA(proxy.substr(pos + 1));
                }
                if (!host.empty()) {
                    auto lower = _utils::strings::lowerA(host);
                    if (_utils::strings::startsWithA(lower, _utils::strings::t2s(httplib::schema::HTTP))) {
                        host = host.substr(_utils::strings::t2s(httplib::schema::HTTP).size());
                    }
                    if (_utils::strings::startsWithA(lower, _utils::strings::t2s(httplib::schema::HTTPS))) {
                        host = host.substr(_utils::strings::t2s(httplib::schema::HTTPS).size());
                    }
                }
                this->host = host;
                this->port = port;
            }

            void setProxy(std::wstring proxy) {
                this->setProxy(_utils::strings::w2s(proxy));
            }

            bool check() {
                return !this->host.empty() && this->port > 0 && this->port <= 65535;
            }
        };

        static int retry = 3;
        static int retryWait = 1000;

        inline void __init() {
            static bool b = false;
#ifdef WIN32
            if (!b) {
                b = true;
            }
#else
            if (!b) {
                b = true;
                signal(SIGPIPE, SIG_IGN);
            }
#endif
        }

        inline std::string GetUrlA(std::string uri) {
            auto uri_ = uri;
            auto pos = uri_.find("?");
            if (pos != std::string::npos) {
                uri_ = uri_.substr(0, pos);
            }
            return uri_;
        }

        inline std::wstring GetUrlW(std::wstring uri) {
            auto uri_ = uri;
            auto pos = uri_.find(L"?");
            if (pos != std::wstring::npos) {
                uri_ = uri_.substr(0, pos);
            }
            return uri_;
        }

        inline _w GetUrl(_w uri) {
#ifdef UNICODE
            return GetUrlW(uri);
#else
            return GetUrlA(uri);
#endif
        }

        inline int invoke(::httplib::Result result, std::string& body, std::string& location) {
            auto err = status::CONNECTION_BREAK;
            switch (result.error()) {
            case ::httplib::Error::ConnectionTimeout:
            case ::httplib::Error::Read:
            case ::httplib::Error::Write:
#ifdef _MSC_VER
                Sleep(retryWait);
#else
                sleep(retryWait / 1000);
#endif
                break;
            case ::httplib::Error::Success:
                err = result->status;
                switch (result->status) {
                case status::PERMANENTLY_MOVED:
                case status::TEMPORARILY_MOVED: {
                    for (auto& header : result->headers) {
                        if (_utils::strings::equalsIgnoreCaseA(header.first, header::Location)) {
                            location = header.second;
                            break;
                        }
                    }
                }
                case status::OK:
                    body = result->body;
                    break;
                }
                break;
            default:
                break;
            }
            return err;
        }

        inline std::string Get(_w uri, ::httplib::Headers* headers, struct PROXY* proxy, int* err) {
            std::string result;
            auto host = _utils::strings::lower(uri);
            if (!host.empty()) {
                auto http = _utils::strings::find(host, schema::HTTP, _T("/"));
                auto https = _utils::strings::find(host, schema::HTTPS, _T("/"));
                if (_utils::strings::endsWith(http, _T("/"))) {
                    http = _utils::strings::decrease(http);
                }
                if (_utils::strings::endsWith(https, _T("/"))) {
                    https = _utils::strings::decrease(https);
                }
                auto retry_ = retry;
                auto location = std::string();
                auto err_ = status::CONNECTION_BREAK;
                while (retry_-- > 0) {
                    if (!http.empty()) {
                        ::httplib::Client client(strings::t2s(http));
                        if (proxy && proxy->check()) {
                            client.set_proxy(proxy->host, proxy->port);
                            if (!proxy->userName.empty()) {
                                client.set_basic_auth(proxy->userName, proxy->password);
                            }
                        }
                        auto trunk = _utils::strings::replace(uri.substr(http.size()), _T("//"), _T("/"));
                        try {
                            err_ = invoke(client.Get(strings::t2s(trunk), headers ? *headers : ::httplib::Headers()), result, location);
                        } catch (std::system_error&) {
                            err_ = status::SYSTEM_ERROR;
                        }
                    } else if (!https.empty()) {
                        ::httplib::SSLClient sslClient(_utils::strings::t2s(https));
                        if (proxy && proxy->check()) {
#ifdef WIN32
                            OutputDebugString(_T("* v0.11.4+ does not support ssl throught proxy yet, proxy was ignored."));
#endif
#if 0
                            sslClient.set_proxy(proxy->host, proxy->port);
                            if (!proxy->userName.empty()) {
                                sslClient.set_basic_auth(proxy->userName, proxy->password);
                            }
#endif
                        }
                        sslClient.enable_server_certificate_verification(false);
                        auto trunk = uri.substr(https.size());
                        if (trunk.empty()) {
                            trunk = _T("/");
                        }
                        try {
                            err_ = invoke(sslClient.Get(strings::t2s(trunk), headers ? *headers : ::httplib::Headers()), result, location);
                        } catch (std::system_error&) {
                            err_ = status::SYSTEM_ERROR;
                        }
                    }
                    if (location.empty()) {
                        if (err) {
                            *err = err_;
                        }
                    } else {
                        result = Get(strings::t2t(location), headers, proxy, err);
                    }
                    retry_ = (err_ == status::CONNECTION_BREAK ? (location.empty() ? retry_ - 1 : 0) : 0);
                }
            }
            return result;
        }

        inline std::string Get(_w uri) {
            return Get(uri, nullptr, nullptr, nullptr);
        }

        inline std::string Get(_w uri, int* err) {
            return Get(uri, nullptr, nullptr, err);
        }

        inline std::string Get(_w uri, _w referer) {
            ::httplib::Headers headers;
            headers.emplace(header::Referer, _utils::strings::t2s(referer).c_str());
            return Get(uri, &headers, nullptr, nullptr);
        }

        inline std::string Get(_w uri, PROXY* proxy) {
            return Get(uri, nullptr, proxy, nullptr);
        }

        inline std::string Get(_w uri, PROXY* proxy, int* err) {
            return Get(uri, nullptr, proxy, err);
        }

        inline std::string Get(_w uri, _w proxyHost, int proxyPort, int* err) {
            auto proxy = PROXY(proxyHost, proxyPort);
            return Get(uri, nullptr, &proxy, err);
        }

        inline std::string Get(_w uri, _w proxyHost, int proxyPort) {
            return Get(uri, proxyHost, proxyPort, nullptr);
        }

        inline std::string Post(_w uri,
            const ::httplib::Headers headers,
            const std::string body,
            const std::string contentType) {

            std::string result;
            auto host = _utils::strings::lower(uri);
            if (!host.empty()) {
                if (host.at(host.size() - 1) != _T('/')) {
                    host += _T("/");
                }
                auto http = _utils::strings::find(host, schema::HTTP, _T("/"));
                auto https = _utils::strings::find(host, schema::HTTPS, _T("/"));
                auto retry_ = retry;
                auto location = std::string();
                auto b = false;
                while (retry_-- > 0) {
                    if (!http.empty()) {
#ifdef UNICODE
                        ::httplib::Client client(strings::t2s(http));
#else
                        ::httplib::Client client(strings::t2s(http));
#endif
                        b = invoke(client.Post(strings::t2s(uri.substr(http.size())), headers, body, contentType), result, location);
                    }
                    else if (!https.empty()) {
                        ::httplib::SSLClient sslClient(strings::t2s(strings::replace(https, schema::HTTPS, _T(""))));
                        sslClient.enable_server_certificate_verification(false);
                        b = invoke(sslClient.Post(strings::t2s(uri.substr(https.size())), headers, body, contentType), result, location);
                    }
                    if (!location.empty()) {
                        result = Post(strings::t2t(location), headers, body, contentType);
                    }
                    retry_ = b ? 0 : (location.empty() ? retry_ - 1 : 0);
                }
            }
            return result;
        }

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

        inline void CreateCertificate(bool overwrite) {
#ifdef WIN32
            io::file::write(io::path::combine(io::path::GetExeFileDirectory(), LOCALHOST_CRT_FILE_NAME), CERTIFICATE, overwrite);
            io::file::write(io::path::combine(io::path::GetExeFileDirectory(), LOCALHOST_KEY_FILE_NAME), PRIVATE_KEY, overwrite);
#endif
        }

        inline void CreateCertificate() {
            CreateCertificate(true);
        }

        inline void listen(_w cert_path, _w private_key_path, _w host, int port,
            std::function<void(const ::httplib::Request&, ::httplib::Response&)> Get,
            std::function<void(const ::httplib::Request&, ::httplib::Response&)> Post) {
            CreateCertificate();
            auto sslServer = ::httplib::SSLServer(strings::t2s(cert_path).c_str(), strings::t2s(private_key_path).c_str());
            sslServer.Get(".*", Get);
            sslServer.Post(".*", Post);
            sslServer.listen(strings::t2s(host).c_str(), port);
        }

        inline void listen(
            std::function<void(const ::httplib::Request&, ::httplib::Response&)> Get,
            std::function<void(const ::httplib::Request&, ::httplib::Response&)> Post) {
            listen(LOCALHOST_CRT_FILE_NAME, LOCALHOST_KEY_FILE_NAME, HOST_ANY, SSL_PORT, Get, Post);
        }

        inline void listen(std::function<void(const ::httplib::Request&, ::httplib::Response&)> Get) {
            listen(Get, [](const ::httplib::Request&, ::httplib::Response&) {
                // eg. res.set_content("Hello World!", "text/plain");
                });
        }
    }
}