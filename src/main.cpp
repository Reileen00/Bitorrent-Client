#include<cstdlib>
#include<iostream>
#include<string>
#include<tuple>

#include<nlohmann/json.hpp>
#include<spdlog/spdlog.h>

#include "torrent.h"

using json = nlohmann::json;

int main(int argc, char* argv[]) {
    std::string command = argv[1];
    
    if(command == "decode"){
        if(argc<3){
            std::cerr<<"Usage: "<<argv[0]<<" decode <encoded_value>" << std::endl;
            return EXIT_FAILURE;
        }

        std::string encoded_value = argv[2];
        json decoded_value;
        std::tie(decoded_value,std::ignore)=bencode::decode_bencode_value(encoded_value);
        std::cout<<decoded_value.dump()<<std::endl;
    }
    else if(command == "info"){
        if(argc<3){
            std::cerr<<"Usage: "<<argv[0]<<" info <torrent_file>" <<std::endl;
            return EXIT_FAILURE;
        }
        std::string torrent_file = argv[2];

        torrent::Torrent torrent = torrent::decode_torrent(torrent_file);

        spdlog::info("Tracker URL: {}", torrent.announce);
        spdlog::info("Length: {}",torrent.info.length);

        std::string info_hash = torrent::calculate_info_hash(torrent);

        spdlog::info("Info Hash: {}",info_hash);

        spdlog::info("Piece Length: {}",torrent.info.piece_length);

        std::list<std::string> pieces = torrent::split_pieces(torrent.info.pieces);

        for(const auto& item:pieces){
            std::cout<<item<<std::endl;
        }
    }
    else{
        std::cerr<<"Unknown command: "<<command<<std::endl;
        return 1;
    }
    return 0;
}