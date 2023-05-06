#pragma once
#pragma warning (disable: 4267)

#include <string>
#include <vector>
#include <fstream>

#define BASE64_TAIL ('=')
static std::string cvt = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

namespace easy_encryption {
    inline std::string encode(const std::vector<char> &data) {
        std::string::size_type i;
        char c;
        unsigned int len = data.size();
        std::string ret;

        for (i = 0; i < len; ++i) {
            c = (data[i] >> 2) & 0x3f;
            ret.append(1, cvt[c]);
            c = (data[i] << 4) & 0x3f;
            if (++i < len)
                c |= (data[i] >> 4) & 0x0f;

            ret.append(1, cvt[c]);
            if (i < len) {
                c = (data[i] << 2) & 0x3f;
                if (++i < len)
                    c |= (data[i] >> 6) & 0x03;

                ret.append(1, cvt[c]);
            } else {
                ++i;
                ret.append(1, BASE64_TAIL);
            }

            if (i < len) {
                c = data[i] & 0x3f;
                ret.append(1, cvt[c]);
            } else {
                ret.append(1, BASE64_TAIL);
            }
        }

        return (ret);
    }

    inline std::vector<char> decode(const std::string &data) {
        std::string::size_type i;
        char c;
        char c1;
        std::string::size_type len = data.length();
        std::vector<char> ret;

        for (i = 0; i < len; ++i) {
            c = (char) cvt.find(data[i]);
            ++i;
            c1 = (char) cvt.find(data[i]);
            c = (c << 2) | ((c1 >> 4) & 0x3);
            ret.push_back(c);
            if (++i < len) {
                c = data[i];
                if (BASE64_TAIL == c)
                    break;
                c = (char) cvt.find(c);
                c1 = ((c1 << 4) & 0xf0) | ((c >> 2) & 0xf);
                ret.push_back(c1);
            }
            if (++i < len) {
                c1 = data[i];
                if (BASE64_TAIL == c1)
                    break;
                c1 = (char) cvt.find(c1);
                c = ((c << 6) & 0xc0) | c1;
                ret.push_back(c);
            }
        }
        return (ret);
    }

    inline std::string encodeFromFile(const std::string &inFileName) {
        std::ifstream in;
        in.open(inFileName.c_str(), std::ios::binary);
        if (!in.good()) {
            throw std::invalid_argument(std::string("can't open file ") + inFileName);
        }
        std::vector<char> fBytes;
        while (in.good())
            fBytes.push_back(in.get());
        fBytes.pop_back();
        in.close();

        return easy_encryption::encode(fBytes);
    }

    inline void decodeToFile(const std::string &outFileName, const std::string &encodedString) {
        std::ofstream out;
        out.open(outFileName.c_str(), std::ios::binary);
        if (!out.good()) {
            throw std::invalid_argument(std::string("can't open file ") + outFileName);
        }
        std::vector<char> bytes = easy_encryption::decode(encodedString);
        for (unsigned int i = 0; i < bytes.size(); ++i)
            out.put(bytes[i]);
        out.close();
    }
}