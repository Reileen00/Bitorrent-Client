#ifndef BITTORRENT_STARTER_CPP_UTILS_H
#define BITTORRENT_STARTER_CPP_UTILS_H

#include<fstream>
#include<sstream>
#include<string>

#include<openssl/sha.h>

namespace utils::file{
    inline std::string read_file(const std::string& filename){
        std::ifstream file(filename,std::ifstream::binary);

        if(!file.is_open()){
            throw std::runtime_error("Could not open file: "+filename);
        }

        std::stringstream ss;
        ss << file.rdbuf();
        return ss.str();
    }
}

namespace utils::crypto{

    inline std::string url_encode(const std::string &value){
        std::ostringstream escaped;
        escaped.fill('0');
        escaped<<std::hex;

        for(char ch:value){
            if (isalnum(ch) || ch == '-' || ch == '_' || ch == '.' || ch == '~') {
                escaped << ch;
                continue;
            }
            escaped << std::uppercase;
            escaped << '%' << std::setw(2) << int((unsigned char) ch);
            escaped << std::nouppercase;
        }
        return escaped.str();        
    }

    inline std::string sha1(const std::string& input)
    {
        unsigned char hash[SHA_DIGEST_LENGTH];

        SHA1(
          reinterpret_cast<const unsigned char*>(input.c_str()), input.size(), hash
        );

        return {reinterpret_cast<char*>(hash), SHA_DIGEST_LENGTH};
    }

    inline std::string sha1_and_hex(const std::string& input)
    {
        std::string hash = sha1(input);

        std::stringstream ss;
        for (unsigned char i : hash) {
            ss << std::hex << std::setw(2) << std::setfill('0')
               << static_cast<int>(i);
        }
        return ss.str();
    }
}

#endif