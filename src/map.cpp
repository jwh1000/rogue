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

void tunnel_rooms(std::pair<int, int> start, std::pair<int, int> end, std::vector<TileType>& map) {
    int x1 = start.first; 
    int y1 = start.second;
    int x2 = end.first;
    int y2 = end.second; 

    TCODRandom* rng = TCODRandom::getInstance();

    int corner_x;
    int corner_y;
    if (rng->getFloat(0, 1) < 0.5f) {
        corner_x = x2;  
        corner_y = y1; 
    }
    else {
        corner_x = x1; 
        corner_y = y2; 
    } 

    std::vector<std::tuple<int, int>> tunnel;
    TCOD_bresenham_data_t bresenham_data;

    TCOD_line_init_mt(x1, y1, corner_x, corner_y, &bresenham_data);
    int x = x1, y = y1;
    do {
        size_t idx = xy_idx(x, y);
        map.at(idx) = TileType::Floor;
    } while (!TCOD_line_step_mt(&x, &y, &bresenham_data));

    TCOD_line_init_mt(corner_x, corner_y, x2, y2, &bresenham_data);
    x = corner_x;
    y = corner_y;
    do {
        size_t idx = xy_idx(x, y);
        map.at(idx) = TileType::Floor;
    } while (!TCOD_line_step_mt(&x, &y, &bresenham_data));
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

                tunnel_rooms(previous_center, new_center, map);
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

