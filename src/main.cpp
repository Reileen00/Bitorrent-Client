#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <cctype>
#include <cstdlib>

#include "lib/nlohmann/json.hpp"

using json = nlohmann::json;

json decode_bencoded_list(const std::string& encoded_value, size_t& index) {
    json result;
    while(encoded_value[index] != 'e') {
        result.push_back(decode_bencoded_value(encoded_value, index));
    }
    return result;
}

json decode_bencoded_dict(const std::string& encoded_value, size_t& index) {
    json result;
    while(encoded_value[index] != 'e') {
        result[decode_bencoded_value(encoded_value, index)] = decode_bencoded_value(encoded_value, index);
    }
    return result;
}

json decode_bencoded_value(const std::string& encoded_value) {
    if (std::isdigit(encoded_value[0])) {
        // Example: "5:hello" -> "hello"
        size_t colon_index = encoded_value.find(':');
        if (colon_index != std::string::npos) {
            std::string number_string = encoded_value.substr(0, colon_index);
            int64_t number = std::atoll(number_string.c_str());
            std::string str = encoded_value.substr(colon_index + 1, number);
            return json(str);
        } else {
            throw std::runtime_error("Invalid encoded value: " + encoded_value);
        }
    } else if(encoded_value[0] == 'i' && encoded_value.back() == 'e') {
        return json(std::stol(encoded_value.substr(1, encoded_value.length()-2)));
    } else if(encoded_value[0] == 'l' && encoded_value.back() == 'e') {
        return decode_bencoded_list(encoded_value, index);
    } else if(encoded_value[0] == 'd' && encoded_value.back() == 'e') {
        return decode_bencoded_dict(encoded_value, index);
    }
     else {
        throw std::runtime_error("Unhandled encoded value: " + encoded_value);
    }
}

int main(int argc, char* argv[]) {
    // Flush after every std::cout / std::cerr
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " decode <encoded_value>" << std::endl;
        return 1;
    }

    std::string command = argv[1];

    if (command == "decode") {
        if (argc < 3) {
            std::cerr << "Usage: " << argv[0] << " decode <encoded_value>" << std::endl;
            return 1;
        }
        // You can use print statements as follows for debugging, they'll be visible when running tests.
        //std::cerr << "Logs from your program will appear here!" << std::endl;

        // Uncomment this block to pass the first stage
         std::string encoded_value = argv[2];
         json decoded_value = decode_bencoded_value(encoded_value);
         std::cout << decoded_value.dump() << std::endl;
    }else if(command == "info"){
        std::string file_name = argv[2];
        std::ifstream file(file_name,std::ios::binary);

        std::filesystem::path p{argv[2]};

        std::string info(std::filesystem::file_size(p), '_');
        file.read(info.data(), std::filesystem::file_size(p));

        json torrent_info = decode_bencoded_value(info).first;
        std::cout << "Tracker URL: " << torrent_info["announce"].dump().substr(1, torrent_info["announce"].size()-2) << std::endl;
        std::cout << "Length: " << torrent_info["info"]["length"] << std::endl;


    }
     else {
        std::cerr << "unknown command: " << command << std::endl;
        return 1;
    }

    return 0;
}
