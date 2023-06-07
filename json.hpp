#pragma once
#include "nlohmann/json/3.11.2+/json.hpp"

namespace utils {
    class json : _base_string {
    public:
        json(std::string s) : _base_string(s) {}

        json(std::wstring s) : _base_string(s) {}

        std::string operator[](std::string key) {
            auto value = std::string();

            // https://github.com/nlohmann/json
            auto o = nlohmann::json::parse(this->m_str);
            for (nlohmann::json::iterator it = o.begin(); it != o.end(); ++it) {
                if (it.key() == key) {
                    if (it->is_string()) {
                        value = it.value().get<std::string>();
                    }
                    break;
                }
            }
            return value;
        }
    };
}
