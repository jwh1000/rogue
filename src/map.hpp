#ifndef MAP_HPP
#define MAP_HPP


enum class TileType {
    Wall,
    Floor
};


struct Map {
    std::vector<TileType> map;
};


/**
* Gets an index from a vector (1 dimensional) for a tile in the map
* Each row is offset by 80, hence the *80
*
* @param x
* @param y
* @return size_t value for the index in the map vector
*/
inline size_t xy_idx(int x, int y) {
    return static_cast<size_t>(y) * 80 + static_cast<size_t>(x);
}


std::vector<TileType> new_map();


void draw_map(std::vector<TileType> map, tcod::Console& console);





#endif