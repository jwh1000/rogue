/*
* 
*/

#include "map.hpp"

std::vector<TileType> new_map_test() {
    std::vector<TileType> map((80 * 50), TileType::Floor);

    for (int x = 0; x < 80; x++) {
        map.at(xy_idx(x, 0)) = TileType::Wall;
        map.at(xy_idx(x, 49)) = TileType::Wall;
    }
    for (int y = 0; y < 50; y++) {
        map.at(xy_idx(0, y)) = TileType::Wall;
        map.at(xy_idx(79, y)) = TileType::Wall;
    }

    TCODRandom* rng = TCODRandom::getInstance();

    int x;
    int y;
    for (int i = 0; i < 400; i++) {
        x = rng->getInt(1, 79);
        y = rng->getInt(1, 49);
        size_t idx = xy_idx(x, y);
        if (idx != xy_idx(40, 25)) {
            map.at(idx) = TileType::Wall;
        }
    }

    return map;
}


std::vector<TileType> new_map_rooms_and_corridors() {
    std::vector<TileType> map((80 * 50), TileType::Wall);
    




    return map;
}


void draw_map(std::vector<TileType> map, tcod::Console& console) {
    int x = 0;
    int y = 0;
    for (auto tile : map) {
        switch (tile) {
        case TileType::Floor:
            tcod::print(console, { x, y }, std::string(1, '.'), TCOD_darker_gray, TCOD_black);
            break;
        case TileType::Wall:
            tcod::print(console, { x, y }, std::string(1, '#'), TCOD_white, TCOD_black);
            break;
        }

        x += 1;
        if (x > 79) {
            x = 0;
            y += 1;
        }
    }
}

