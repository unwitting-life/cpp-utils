//
// Created by huangyl on 2023/5/5.
//

#pragma once
#include "utils.hpp"
#include "file.hpp"

#ifdef WIN32
/* https://stackoverflow.com/questions/12530947/unresolved-external-symbol-idntoascii */
#pragma comment(lib, "Normaliz.lib")

/* https://stackoverflow.com/questions/41762859/visual-studio-2015-curl-will-not-statically-link */
#pragma comment(lib, "wldap32.lib" )
#pragma comment(lib, "crypt32.lib" )
#pragma comment(lib, "Ws2_32.lib")
#define CURL_STATICLIB
#endif
#include "curl/include/curl/curl.h"

#define CONTENT_DISPOSITION "Content-Disposition"
namespace _utils {
    namespace http {
        struct CURLDATA {
        public:
            std::string uri;
            _tstring fileName;
            std::vector<std::string> headers;
            std::string body;
            file::ofstream* ofstream;
            std::function<bool(CURLDATA* data)> f;
            CURLDATA(std::string uri) {
                this->uri = uri;
                this->fileName.clear();
                this->headers.clear();
                this->body.clear();
                this->ofstream = nullptr;
            }
            ~CURLDATA() {
                if (this->ofstream) {
                    this->ofstream->close();
                    this->ofstream = nullptr;
                }
            }
        };
        inline size_t write_header(char* buffer, size_t size, size_t nmemb, CURLDATA* params) {
            auto numberOfBytesWritten = size_t(0);
            if (params) {
                numberOfBytesWritten = size * nmemb;
                params->headers.push_back(std::string(buffer, numberOfBytesWritten));
            }
            return numberOfBytesWritten;
        }
        inline size_t write_string(void* p, size_t size, size_t nmemb, CURLDATA* params) {
            auto numberOfBytesWritten = size_t(0);
            if (params) {
                numberOfBytesWritten = size * nmemb;
                params->body.append(reinterpret_cast<char*>(p), numberOfBytesWritten);
            }
            return numberOfBytesWritten;
        }
        inline size_t write_file(void* p, size_t size, size_t nmemb, CURLDATA* params) {
            auto numberOfBytesWritten = size_t(0);
            if (params) {
                if (!params->ofstream) {
                    params->fileName.clear();
                    for (auto& e : params->headers) {
                        if (e.starts_with(CONTENT_DISPOSITION)) {
                            auto filename = strings::truncate(strings::t2t(e), _T("filename=\""), _T("\""));
                            if (!filename.empty()) {
                                params->fileName = filename[0];
                            }
                            break;
                        }
                    }
                    if (params->fileName.empty()) {
                        params->fileName = _tstring(strings::t2t(params->uri));
                        auto pos = params->fileName.find_last_of(_T('?'));
                        if (pos != _tstring::npos) {
                            params->fileName = params->fileName.substr(0, pos);
                        }
                        pos = params->fileName.find_last_of(_T('/'));
                        if (pos != _tstring::npos) {
                            params->fileName = params->fileName.substr(pos + 1);
                        }
                    }
                    if (!params->fileName.empty()) {
                        if (params->f(params)) {
                            params->ofstream = new file::ofstream(params->fileName, true);
                        }
                    }
                }
                if (params->ofstream) {
                    numberOfBytesWritten = size * nmemb;
                    params->ofstream->write(reinterpret_cast<char*>(p), numberOfBytesWritten);
                } else {
                    numberOfBytesWritten = 0;
                }
            }
            return numberOfBytesWritten;
        }
        inline size_t write_null(void* p, size_t size, size_t nmemb, CURLDATA* params) {
            return 0;
        }

        class curl {
        public:
            curl(std::string uri, std::string proxyHost = std::string(), std::string proxyUserName = std::string(), std::string proxyPassword = std::string()) {
                this->m_uri = uri;
                this->m_proxyHost = proxyHost;
                this->m_proxyUserName = proxyUserName;
                this->m_proxyPassword = proxyPassword;
                this->m_cookie.clear();
            }
            inline void setUri(std::string uri) {
                this->m_uri = uri;
            }
            inline void setUri(std::wstring uri) {
                this->m_uri = strings::w2s(uri);
            }
            inline void setCookie(std::string cookie) {
                this->m_cookie = cookie;
            }
            inline void setProxy(std::string proxyHost, std::string proxyUserName = std::string(), std::string proxyPassword = std::string()) {
                this->m_proxyHost = proxyHost;
                this->m_proxyUserName = proxyUserName;
                this->m_proxyPassword = proxyPassword;
            }
            inline std::string Get() {
                CURLDATA params(this->m_uri);
                auto curl = curl_easy_init();
                if (curl) {
                    curl_easy_setopt(curl, CURLOPT_URL, this->m_uri.c_str());
                    this->setOptions(curl);

                    /* https://gist.github.com/whoshuu/2dc858b8730079602044 */
                    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
                    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, http::write_header);
                    curl_easy_setopt(curl, CURLOPT_HEADERDATA, &params);
                    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, http::write_string);
                    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &params);
                    curl_easy_perform(curl);
                    curl_easy_cleanup(curl);
                }
                return params.body;
            }
            inline std::vector<std::string> Query() {
                CURLDATA params(this->m_uri);
                auto code = CURLcode(CURLE_FAILED_INIT);
                auto curl = curl_easy_init();
                if (curl) {
                    this->setOptions(curl);
                    if (CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_null)) &&
                        CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_FILE, nullptr)) &&
                        CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, write_header)) &&
                        CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_HEADERDATA, &params)) &&
                        CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L)) &&
                        CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 0L)) &&
                        CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L)) &&
                        CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_URL, this->m_uri.c_str()))) {
                        code = curl_easy_perform(curl);
                    }
                    curl_easy_cleanup(curl);
                }
                return params.headers;
            }
            inline _tstring Download(std::function<bool(CURLDATA*)> f) {
                CURLDATA params(this->m_uri);
                params.f = f;
                auto code = CURLcode(CURLE_FAILED_INIT);
                auto curl = curl_easy_init();
                if (curl) {
                    this->setOptions(curl);
                    if (CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_file)) &&
                        CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_FILE, &params)) &&
                        CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, write_header)) &&
                        CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_HEADERDATA, &params)) &&
                        CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L)) &&
                        CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 0L)) &&
                        CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L)) &&
                        CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_URL, this->m_uri.c_str()))) {
                        code = curl_easy_perform(curl);
                    }
                    curl_easy_cleanup(curl);
                }
                if (params.ofstream) {
                    params.ofstream->close();
                }
                return params.fileName;
            }
            inline _tstring Download() {
                return this->Download([](CURLDATA*) { return true; });
            }
        private:
            std::string m_uri;
            std::string m_proxyHost;
            std::string m_proxyUserName;
            std::string m_proxyPassword;
            std::string m_cookie;
            inline CURLcode setOptions(CURL* curl) {
                auto code = CURLcode(CURLE_OK);
                if (code == CURLE_OK && !this->m_proxyHost.empty()) {
                    code = curl_easy_setopt(curl, CURLOPT_PROXY, this->m_proxyHost.c_str());
                    if (code == CURLE_OK && !this->m_proxyUserName.empty()) {
                        code = curl_easy_setopt(curl, CURLOPT_PROXYUSERNAME, this->m_proxyUserName.c_str());
                    }
                    if (code == CURLE_OK && !this->m_proxyPassword.empty()) {
                        code = curl_easy_setopt(curl, CURLOPT_PROXYPASSWORD, this->m_proxyPassword.c_str());
                    }
                }
                if (code == CURLE_OK && !this->m_cookie.empty()) {
                    code = curl_easy_setopt(curl, CURLOPT_COOKIE, this->m_cookie.c_str());
                }
                return code;
            }
        };
    }
}
