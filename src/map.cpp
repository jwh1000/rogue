/*
* map.cpp
* created by ironhands
* 
* tools for creating the map (placing rooms, etc.) and drawing
* it onto the console
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

    for (int i = 0; i < 400; i++) {
        int x = rng->getInt(1, 79);
        int y = rng->getInt(1, 49);
        size_t idx = xy_idx(x, y);
        if (idx != xy_idx(40, 25)) {
            map.at(idx) = TileType::Wall;
        }
    }

    return map;
}

void apply_room_to_map(Rect& room, std::vector<TileType>& map) {
    for (int y = room.y1; y < room.y2; y++) {
        for (int x = room.x1; x < room.x2; x++) {
            map.at(xy_idx(x, y)) = TileType::Floor;
        }
    }
}

void apply_horizontal_tunnel(std::vector<TileType>& map, int x1, int x2, int y) {
    for (int x = std::min(x1, x2); x <= std::max(x1, x2); x++) {
        size_t idx = xy_idx(x, y);
        if (idx > 0 && idx < 80 * 50) {
            map.at(idx) = TileType::Floor;
        }
    }
}

void apply_vertical_tunnel(std::vector<TileType>& map, int y1, int y2, int x) {
    for (int y = std::min(y1, y2); y <= std::max(y1, y2); y++) {
        size_t idx = xy_idx(x, y);
        if (idx > 0 && idx < 80 * 50) {
            map.at(idx) = TileType::Floor;
        }
    }
}

flecs::entity new_map_rooms_and_corridors(flecs::world& ecs) {
    std::vector<TileType> map((80 * 50), TileType::Wall);

    std::vector<Rect> rooms;
    constexpr int MAX_ROOMS = 30;
    constexpr int MIN_SIZE = 6;
    constexpr int MAX_SIZE = 12;

    TCODRandom* rng = TCODRandom::getInstance();

    for (int i = 0; i < MAX_ROOMS; i++) {
        int w = rng->getInt(MIN_SIZE, MAX_SIZE);
        int h = rng->getInt(MIN_SIZE, MAX_SIZE);
        int x = rng->getInt(1, 80 - w - 1) - 1;
        int y = rng->getInt(1, 50 - h - 1) - 1;
        Rect new_room = Rect(x, y, w, h);
        bool ok = true;
        for (auto other_room : rooms) {
            if (new_room.intersect(other_room)) { ok = false; }
        }
        if (ok) {
            apply_room_to_map(new_room, map);

            if (!rooms.empty()) {
                std::pair<int, int> new_center = new_room.center();
                std::pair<int, int> previous_center = rooms.back().center();

                if (rng->getInt(0, 2) == 1) {
                    apply_horizontal_tunnel(map, previous_center.first, new_center.first, previous_center.second);
                    apply_vertical_tunnel(map, previous_center.second, new_center.second, new_center.first);
                }
                else {
                    apply_vertical_tunnel(map, previous_center.second, new_center.second, previous_center.first);
                    apply_horizontal_tunnel(map, previous_center.first, new_center.first, new_center.second);
                }
            }
            rooms.push_back(new_room);
        }
    }

    // create the map and add it to the world
    auto map_entity = ecs.entity("Map")
        .set<MapTiles>({ map })
        .set<Rooms>({ rooms });

    return map_entity;
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

