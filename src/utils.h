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
    inline std::string sha1(const std::string& input){
        unsigned char hash[SHA_DIGEST_LENGTH];
        SHA1(
            reinterpret_cast<const unsigned char*>(input.c_str()),input.size(),hash
        );

        std::stringstream ss;
        for(unsigned char i:hash){
            ss<<std::hex<<std::setw(2)<<std::setfill('0')<<static_cast<int>(i);
        }
        return ss.str();
    }
}

#endif