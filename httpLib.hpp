#pragma once
#include "utils.hpp"

/*
 * CMakeLists.txt Darwin
 *
 * include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../common/github.com/unwitting.life/cpp-utils/OpenSSL/3.0.6+/darwin64-x86_64-cc/include)
 * link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../common/github.com/unwitting.life/cpp-utils/OpenSSL/3.0.6+/darwin64-x86_64-cc/lib)
 * add_executable(${PROJECT_NAME} main.cpp)
 * target_link_libraries(${PROJECT_NAME} crypto ssl)
 */
#define CPPHTTPLIB_OPENSSL_SUPPORT

#include "cpp-httplib/v0.11.2+/httplib.h"

#ifdef _MSC_VER

#ifndef UTILS_SOURCE_DIRECTORY
#define UTILS_SOURCE_DIRECTORY __FILE__
#endif

#ifdef _WIN64
#pragma comment(lib, UTILS_SOURCE_DIRECTORY "/../OpenSSL/3.0.6+/VC-WIN64A/lib/libcrypto.lib")
#pragma comment(lib, UTILS_SOURCE_DIRECTORY "/../OpenSSL/3.0.6+/VC-WIN64A/lib/libssl.lib")
#endif

#endif

namespace httplib {
    namespace header {
        constexpr auto Location = _T("Location");
        constexpr auto Referer = _T("referer");
    }

    namespace schema {
        constexpr auto HTTP = _T("http://");
        constexpr auto HTTPS = _T("https://");
    }
    namespace status {
        constexpr int UNKNOWN = 0;
        constexpr int OK = 200;
        constexpr int PERMANENTLY_MOVED = 301;
        constexpr int TEMPORARILY_MOVED = 302;
    }
    static int retry = 3;
    static int retryWait = 1000;

    inline void init() {

        static bool b = false;
        if (!b) {
            b = true;
#ifndef _MSC_VER
            signal(SIGPIPE, SIG_IGN);
#endif
        }
    }

    inline bool invoke(::httplib::Result result, std::string& body, std::string& location) {
        bool b = true;
        switch (result.error()) {
        case ::httplib::Error::ConnectionTimeout:
        case ::httplib::Error::Read:
        case ::httplib::Error::Write:
#ifdef _MSC_VER
            Sleep(retryWait);
#else
            sleep(retryWait / 1000);
#endif
            b = false;
            break;
        case ::httplib::Error::Success:
            switch (result->status) {
            case status::PERMANENTLY_MOVED:
            case status::TEMPORARILY_MOVED: {
                for (auto header : result->headers) {
                    if (strings::equalsIgnoreCase(header.first, header::Location)) {
                        location = header.second;
                        break;
                    }
                }
            }
            default:
                body = result->body;
                break;
            }
            break;
        default:
            break;
        }
        return b;
    }

    inline std::string Get(string_t uri, ::httplib::Headers* headers) {
        init();

        std::string result;
        auto host = strings::lower(uri);
        if (!host.empty()) {
            if (host.at(host.size() - 1) != _T('/')) {
                host += _T("/");
            }
            auto http = strings::decrease(strings::find(host, schema::HTTP, _T("/")));
            auto https = strings::decrease(strings::find(host, schema::HTTPS, _T("/")));
            auto retry_ = retry;
            auto location = std::string();
            auto b = false;
            while (retry_-- > 0) {
                if (!http.empty()) {
                    ::httplib::Client client(http);
                    b = invoke(client.Get(strings::replace(uri.substr(http.size()), _T("//"), _T("/")), headers ? *headers : ::httplib::Headers()), result, location);
                }
                else if (!https.empty()) {
                    ::httplib::SSLClient sslClient(strings::replace(https, schema::HTTPS, ""));
                    sslClient.enable_server_certificate_verification(false);
                    b = invoke(sslClient.Get(strings::replace(uri.substr(https.size()), _T("//"), _T("/")), headers ? *headers : ::httplib::Headers()), result, location);
                }
                if (!location.empty()) {
                    result = Get(location, headers);
                }
                retry_ = b ? 0 : (location.empty() ? retry_ - 1 : 0);
            }
        }
        return result;
    }

    inline std::string Get(std::string uri) {
        return Get(uri, nullptr);
    }

    inline std::string Post(const std::string uri,
        const ::httplib::Headers headers,
        const std::string body,
        const std::string contentType) {
        init();

        std::string result;
        auto host = strings::lower(uri);
        if (!host.empty()) {
            if (host.at(host.size() - 1) != _T('/')) {
                host += _T("/");
            }
            auto http = strings::find(host, schema::HTTP, _T("/"));
            auto https = strings::find(host, schema::HTTPS, _T("/"));
            auto retry_ = retry;
            auto location = std::string();
            auto b = false;
            while (retry_-- > 0) {
                if (!http.empty()) {
#ifdef UNICODE
                    ::httplib::Client client(http);
#else
                    ::httplib::Client client(http);
#endif
                    b = invoke(client.Post(uri.substr(http.size()), headers, body, contentType), result, location);
                }
                else if (!https.empty()) {
                    ::httplib::SSLClient sslClient(strings::replace(https, schema::HTTPS, _T("")));
                    sslClient.enable_server_certificate_verification(false);
                    b = invoke(sslClient.Post(uri.substr(https.size()), headers, body, contentType), result, location);
                }
                if (!location.empty()) {
                    result = Post(location, headers, body, contentType);
                }
                retry_ = b ? 0 : (location.empty() ? retry_ - 1 : 0);
            }
        }
        return result;
    }
}