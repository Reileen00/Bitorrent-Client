#ifndef BITTORRENT_STARTER_CPP_TORRENT_H
#define BITTORRENT_STARTER_CPP_TORRENT_H

#include<string>

#include<nholmann/json.hpp>

#include "bencode.h"
#include "utils.h"

using json = nholmann::json;

namespace torrent{
    struct Info{
        std::string name;
        int64_t piece_length{};
        std::string pieces;
        int64_t length{};
    };

    void from_json(const json& j,Info& info){
        j.at("name").get_to(info.name);
        j.at("piece length").get_to(info.piece_length);
        j.at("pieces").get_to(info.pieces);
        if(j.contains("length")){
            j.at("length").get_to(info.length);
        }
    }

    void to_json(json& j,const Info& info){
        j=json{
            {"name",info.name},
            {"piece length",info.piece_length},
            {"pieces",info.pieces},
            {"length",info.length},
        };
    }

    struct Torrent{
        std::string announce;
        Info info;
    };

    void from_json(const json& j,Torrent& torrent){
        j.at("anounce").get_to(torrent.announce);
        j.at("info").get_to(torrent.info);
    }

    void to_json(json& j,const Torrent& torrent){
        j=json{
            {"announce",torrent.announce},
            {"info":torrent.info},
        };
    }

    inline Torrent decode_torrent(const std::string& torrent_file){
        std::string torrent_content = utils::file::read_file(torrent_file);

        json decoded_content;

        std::tie(decoded_content,std::ignore)=bencode::decode_bencoded_value(torrent_content);
        auto torrent = decoded_content.template get<torrent::Torrent>();

        return torrent;
    }

    inline std::string calculate_info_hash(const Torrent& torrent){
        json info = torrent.info;

        std::string encoded_info = bencode::encode_bencode_value(info);

        return utils::crypto::sha1(encoded_info);
    }
}

#endif