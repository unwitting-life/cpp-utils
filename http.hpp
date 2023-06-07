#pragma once

namespace utils {
    class http {
    public:
        class header {
        public:
            static constexpr auto Basic = "Basic ";
            static constexpr auto Location = "Location";
            static constexpr auto ContentType = "Content-Type";
            static constexpr auto LocalAddr = "LOCAL_ADDR";
            static constexpr auto ContentDisposition = "Content-Disposition";
            static constexpr auto Authorization = "Authorization";
        };

        class status {
        public:
            static constexpr auto None = 0;
            static constexpr auto Ok = 200;
            static constexpr auto PermanentlyMoved = 301;
            static constexpr auto TemporarilyMoved = 302;
            static constexpr auto Unauthorized = 401;
            static constexpr auto NotFound = 404;
            static constexpr auto InternalServerError = 500;
        };

        class method {
        public:
            static constexpr auto GET = "GET";
            static constexpr auto POST = "POST";
            static constexpr auto HEAD = "HEAD";
        };

        class schema {
        public:
            static constexpr auto HTTP = "http://";
            static constexpr auto HTTPS = "https://";
        };

        class mime {
        public:
            static inline std::string get(std::wstring file) {
                auto name = std::string("application/octet-stream");
                auto extension = _lower(system::io::path::_extension(file));
                for (auto& e : configurations.mime_types) {
                    if (extension == e.extension) {
                        name = e.name;
                        break;
                    }
                }
                return name;
            }
        };

        struct Proxy {
            Proxy() : port(0) {}

            Proxy(std::string uri) : port(0) {
                this->parse(uri);
            }

            Proxy(std::string host, int port) {
                this->host = host;
                this->port = port;
            }

            void parse(std::string uri) {
                auto host = std::string();
                auto port = 0;
                auto pos = uri.find(":");
                if (pos != std::string::npos) {
                    host = uri.substr(0, pos);
                    port = _atoi(uri.substr(pos + 1));
                }
                if (!host.empty()) {
                    auto lower = _lower(host);
                    if (lower.starts_with(http::schema::HTTP)) {
                        host = host.substr(strlen(http::schema::HTTP));
                    }
                    if (lower.starts_with(http::schema::HTTPS)) {
                        host = host.substr(strlen(http::schema::HTTPS));
                    }
                }
                this->host = host;
                this->port = port;
            }

            void parse(std::wstring uri) {
                this->parse(w2s(uri));
            }

            bool check() {
                return !this->host.empty() && this->port > 0 && this->port <= 65535;
            }

            std::string host;
            int port;
            std::string userName;
            std::string password;
        };

        struct CURLDATA {
        public:
            std::string uri;
            std::wstring fileName;
            std::vector<std::string> headers;
            long status_code;
            std::string body;
            system::io::ofstream* ofstream;
            std::function<bool(CURLDATA* data)> f;

            CURLDATA(std::string uri) {
                this->uri = uri;
                this->fileName.clear();
                this->headers.clear();
                this->status_code = http::status::None;
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

        static inline size_t write_header(char* buffer, size_t size, size_t nmemb, CURLDATA* params) {
            auto number_of_bytes_written = size_t(0);
            if (params) {
                number_of_bytes_written = size * nmemb;
                auto n = number_of_bytes_written;
                while (n > 0 && (buffer[n - 1] == '\r' || buffer[n - 1] == '\n')) {
                    n--;
                }
                params->headers.push_back(std::string(buffer, n));
            }
            return number_of_bytes_written;
        }

        static inline size_t write_string(void* p, size_t size, size_t nmemb, CURLDATA* params) {
            auto numberOfBytesWritten = size_t(0);
            if (params) {
                numberOfBytesWritten = size * nmemb;
                params->body.append(reinterpret_cast<char*>(p), numberOfBytesWritten);
            }
            return numberOfBytesWritten;
        }

        static inline size_t write_file(void* p, size_t size, size_t nmemb, CURLDATA* params) {
            auto numberOfBytesWritten = size_t(0);
            if (params) {
                if (!params->ofstream) {
                    params->fileName.clear();
                    for (auto& e : params->headers) {
                        if (e.starts_with(header::ContentDisposition)) {
                            auto filename = _truncate(t2t(e), _T("filename=\""), _T("\""));
                            if (!filename.empty()) {
                                params->fileName = filename[0];
                            }
                            break;
                        }
                    }
                    if (params->fileName.empty()) {
                        params->fileName = std::wstring(t2t(params->uri));
                        auto pos = params->fileName.find_last_of(_T('?'));
                        if (pos != std::wstring::npos) {
                            params->fileName = params->fileName.substr(0, pos);
                        }
                        pos = params->fileName.find_last_of(_T('/'));
                        if (pos != std::wstring::npos) {
                            params->fileName = params->fileName.substr(pos + 1);
                        }
                    }
                    if (!params->fileName.empty()) {
                        if (params->f(params)) {
                            params->ofstream = new system::io::ofstream(params->fileName, true);
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

        static inline size_t write_null(void* p, size_t size, size_t nmemb, CURLDATA* params) {
            UNREFERENCED_PARAMETER(p);
            UNREFERENCED_PARAMETER(size);
            UNREFERENCED_PARAMETER(nmemb);
            UNREFERENCED_PARAMETER(params);
            return 0;
        }

        class session {
        public:
            session(std::string uri, std::string proxyHost = std::string(), std::string proxyUserName = std::string(), std::string proxyPassword = std::string()) {
                this->m_uri = uri;
                this->m_proxyHost = proxyHost;
                this->m_proxyUserName = proxyUserName;
                this->m_proxyPassword = proxyPassword;
                this->m_progress = false;
                this->m_cookie.clear();
            }

            inline void set_uri(std::string uri) {
                this->m_uri = uri;
            }

            inline void set_uri(std::wstring uri) {
                this->m_uri = w2s(uri);
            }

            inline void set_cookie(std::string cookie) {
                this->m_cookie = cookie;
            }

            inline void set_proxy(std::string proxyHost, std::string proxyUserName = std::string(), std::string proxyPassword = std::string()) {
                this->m_proxyHost = proxyHost;
                this->m_proxyUserName = proxyUserName;
                this->m_proxyPassword = proxyPassword;
            }

            inline CURLDATA get() {
                CURLDATA params(this->m_uri);
                auto session = curl_easy_init();
                if (session) {
                    curl_easy_setopt(session, CURLOPT_URL, this->m_uri.c_str());
                    this->setOptions(session);
                    curl_easy_setopt(session, CURLOPT_NOPROGRESS, this->m_progress ? 0L : 1L);
                    curl_easy_setopt(session, CURLOPT_HEADERFUNCTION, http::write_header);
                    curl_easy_setopt(session, CURLOPT_HEADERDATA, &params);
                    curl_easy_setopt(session, CURLOPT_WRITEFUNCTION, http::write_string);
                    curl_easy_setopt(session, CURLOPT_WRITEDATA, &params);
                    curl_easy_perform(session);
                    curl_easy_getinfo(session, CURLINFO_RESPONSE_CODE, &params.status_code);
                    curl_easy_cleanup(session);
                }
                return params;
            }

            inline std::string download_data() {
                return this->get().body;
            }

            inline std::vector<std::string> query() {
                CURLDATA params(this->m_uri);
                auto session = curl_easy_init();
                if (session) {
                    this->setOptions(session);
                    if (CURLE_OK == curl_easy_setopt(session, CURLOPT_WRITEFUNCTION, write_null) &&
                        CURLE_OK == curl_easy_setopt(session, CURLOPT_FILE, nullptr) &&
                        CURLE_OK == curl_easy_setopt(session, CURLOPT_HEADERFUNCTION, write_header) &&
                        CURLE_OK == curl_easy_setopt(session, CURLOPT_HEADERDATA, &params) &&
                        CURLE_OK == curl_easy_setopt(session, CURLOPT_NOPROGRESS, this->m_progress ? 0L : 1L) &&
                        CURLE_OK == curl_easy_setopt(session, CURLOPT_FOLLOWLOCATION, 0L) &&
                        CURLE_OK == curl_easy_setopt(session, CURLOPT_TIMEOUT, 30L) &&
                        CURLE_OK == curl_easy_setopt(session, CURLOPT_URL, this->m_uri.c_str())) {
                        curl_easy_perform(session);
                        curl_easy_getinfo(session, CURLINFO_RESPONSE_CODE, &params.status_code);
                    }
                    curl_easy_cleanup(session);
                }
                return params.headers;
            }

            inline std::wstring download(std::function<bool(CURLDATA*)> f) {
                CURLDATA params(this->m_uri);
                params.f = f;
                auto session = curl_easy_init();
                if (session) {
                    this->setOptions(session);
                    if (CURLE_OK == curl_easy_setopt(session, CURLOPT_WRITEFUNCTION, write_file) &&
                        CURLE_OK == curl_easy_setopt(session, CURLOPT_FILE, &params) &&
                        CURLE_OK == curl_easy_setopt(session, CURLOPT_HEADERFUNCTION, write_header) &&
                        CURLE_OK == curl_easy_setopt(session, CURLOPT_HEADERDATA, &params) &&
                        CURLE_OK == curl_easy_setopt(session, CURLOPT_NOPROGRESS, this->m_progress ? 0L : 1L) &&
                        CURLE_OK == curl_easy_setopt(session, CURLOPT_FOLLOWLOCATION, 0L) &&
                        CURLE_OK == curl_easy_setopt(session, CURLOPT_TIMEOUT, 30L) &&
                        CURLE_OK == curl_easy_setopt(session, CURLOPT_URL, this->m_uri.c_str())) {
                        curl_easy_perform(session);
                        curl_easy_getinfo(session, CURLINFO_RESPONSE_CODE, &params.status_code);
                    }
                    curl_easy_cleanup(session);
                }
                if (params.ofstream) {
                    params.ofstream->close();
                }
                return params.fileName;
            }

            inline std::wstring download() {
                return this->download([](CURLDATA*) { return true; });
            }

            inline void enable_progress() {
                this->m_progress = true;
            }

            inline void disable_progress() {
                this->m_progress = false;
            }

        private:
            std::string m_uri;
            std::string m_proxyHost;
            std::string m_proxyUserName;
            std::string m_proxyPassword;
            std::string m_cookie;
            bool m_progress;
            inline CURLcode setOptions(CURL* curl) {
                auto code = CURLcode(CURLE_OK);
                if (!this->m_proxyHost.empty()) {
                    code = curl_easy_setopt(curl, CURLOPT_PROXY, this->m_proxyHost.c_str());
                    if (code == CURLE_OK && !this->m_proxyUserName.empty()) {
                        code = curl_easy_setopt(curl, CURLOPT_PROXYUSERNAME, this->m_proxyUserName.c_str());
                    }
                    if (code == CURLE_OK && !this->m_proxyPassword.empty()) {
                        code = curl_easy_setopt(curl, CURLOPT_PROXYPASSWORD, this->m_proxyPassword.c_str());
                    }
                }
                if (!this->m_cookie.empty()) {
                    code = curl_easy_setopt(curl, CURLOPT_COOKIE, this->m_cookie.c_str());
                }
                return code;
            }
        };

        class httplib {
        public:
            struct REQUEST {
                struct HEADERS {
                    void add(std::string key, std::string value) {
                        this->m_headers.insert(std::make_pair(key, value));
                    }

                    ::httplib::Headers m_headers;
                } headers;

                std::wstring contentType;
            };

            static inline int timeout() { return configurations.http_timeout; }

            static inline void set_timeout(int timeout) { configurations.http_timeout = timeout; }

            static inline void create_new_certificate(bool overwrite = true) {
                system::io::ofstream::writeBytes(system::io::path::_combine(system::io::path::executable_file_directory(), LOCALHOST_CRT_FILE_NAME), CERTIFICATE, strlen(CERTIFICATE), overwrite);
                system::io::ofstream::writeBytes(system::io::path::_combine(system::io::path::executable_file_directory(), LOCALHOST_KEY_FILE_NAME), PRIVATE_KEY, strlen(PRIVATE_KEY), overwrite);
            }

            static inline void create_static_file_server(std::wstring directory, std::wstring cert_path, std::wstring private_key_path,
                std::wstring host, int port, std::wstring basicUserName, std::wstring basicPassword,
                std::function<void(const ::httplib::Request&, ::httplib::Response&)> Get,
                std::function<void(const ::httplib::Request&, ::httplib::Response&)> Post) {
                auto cert = cert_path;
                auto pem = private_key_path;
                if (cert.empty() || pem.empty()) {
                    create_new_certificate();
                    cert = system::io::path::_combine(system::io::path::executable_file_directory(), LOCALHOST_CRT_FILE_NAME);
                    pem = system::io::path::_combine(system::io::path::executable_file_directory(), LOCALHOST_KEY_FILE_NAME);;
                }
                auto sslServer = ::httplib::SSLServer(t2s(cert).c_str(), t2s(pem).c_str());
                sslServer.Get(".*", Get);
                sslServer.Post(".*", Post);
                sslServer.set_basic_auth(t2s(basicUserName), t2s(basicPassword));
                sslServer.set_mount_point("/", w2s(_replace(directory, PATH_SEPARATORA, L'/')));
                sslServer.listen(t2s(host).c_str(), port);
            }

            static inline void create_static_file_server(std::wstring directory, std::wstring cert_path, std::wstring private_key_path,
                                                         std::wstring host, int port,
                                                         std::function<void(const ::httplib::Request&, ::httplib::Response&)> Get,
                                                         std::function<void(const ::httplib::Request&, ::httplib::Response&)> Post) {
                create_static_file_server(directory, cert_path, private_key_path, host, port, std::wstring(), std::wstring(), Get, Post);
            }

            static inline void create_static_file_server(std::wstring directory,
                std::function<void(const ::httplib::Request&, ::httplib::Response&)> Get,
                std::function<void(const ::httplib::Request&, ::httplib::Response&)> Post) {
                create_static_file_server(directory, std::wstring(), std::wstring(), HOST_ANY, SSL_PORT, std::wstring(), std::wstring(), Get, Post);
            }

            static inline void create_static_file_server(std::wstring directory,
                std::function<void(const ::httplib::Request&, ::httplib::Response&)> Post) {
                create_static_file_server(directory, std::wstring(), std::wstring(), HOST_ANY, SSL_PORT, std::wstring(), std::wstring(), [](const ::httplib::Request&, ::httplib::Response&) {}, Post);
            }

            static inline void create_static_file_server(std::wstring directory) {
                create_static_file_server(directory, std::wstring(), std::wstring(), HOST_ANY, SSL_PORT, std::wstring(), std::wstring(),
                    [](const ::httplib::Request&, ::httplib::Response&) {},
                    [](const ::httplib::Request&, ::httplib::Response&) {});
            }

            static inline std::string Get(std::string host, std::string url) {
                auto body = std::string();
                ::httplib::Client cli(host);
                cli.set_read_timeout(timeout());
                cli.set_write_timeout(timeout());
                auto result = cli.Get(url);
                if (result) {
                    body = result->body;
                }
                return body;
            }

            static inline ::httplib::Result Post(std::string host, std::string url, std::string body, const REQUEST& req) {
                ::httplib::Client cli(host);
                cli.set_read_timeout(timeout());
                cli.set_write_timeout(timeout());
                return cli.Post(url,
                    req.headers.m_headers,
                    body,
                    req.contentType.empty() ? "application/json" : t2s(req.contentType));
            }
        };

        http() :m_uri(std::wstring()) {}
        http(std::wstring uri) :m_uri(uri) {}

        inline std::wstring trunk() {
            auto uri = std::wstring(this->m_uri);
            auto pos = uri.find(_T("?"));
            if (pos != std::wstring::npos) {
                uri = uri.substr(0, pos);
            }
            return uri;
        }

        struct ip_region {
            std::string status;
            std::string country;
            std::string countryCode;
            std::string regionName;
            std::string city;
            std::string zip;
            std::string lat;
            std::string lon;
            std::string timezone;
            std::string isp;
            std::string org;
            std::string as;
            std::string query;
        };

        inline ip_region ip_region() {
            auto j  = json(http::httplib::Get("http://ip-api.com", utils::_format("/json/%s?lang=zh-CN", utils::w2s(this->m_uri).c_str())));
            return {
                    j["status"],
                    j["country"],
                    j["countryCode"],
                    j["regionName"],
                    j["city"],
                    j["zip"],
                    j["lat"],
                    j["lon"],
                    j["timezone"],
                    j["isp"],
                    j["org"],
                    j["as"],
                    j["query"],
            };
        }

    protected:
        std::wstring m_uri;
    };
}
