/*
* data.hpp
* created by ironhands
* 
* Shamelessly stolen from HexDecimal's roguelike engine
* used to get the path to the data directory, which holds things like
* the tileset
* 
* Code is in an hpp for inline stuff (I had to google this)
*/

#pragma once
#include <filesystem>

inline auto get_data_dir() -> std::filesystem::path {
    // Begin at the working directory.
    static auto root_directory = std::filesystem::path{ "." };  

    while (!std::filesystem::exists(root_directory / "data")) {
        // If the current working directory is missing the data dir then it will assume it exists in any parent directory.
        // this should always eventually get the path to it as the ./data directory is in the project root
        root_directory /= "..";
        if (!std::filesystem::exists(root_directory)) {
            throw std::runtime_error("Could not find the data directory.");
        }
    }
    return root_directory / "data";
};