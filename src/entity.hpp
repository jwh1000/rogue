/*
* entity.hpp
* 
* the base component information for an entity
* an entity is anything that belongs in the world
*/

#include <flecs.h>
#include <libtcod.hpp>

struct Position {
    int x, y;
};

struct Renderable {
    char glyph;
    TCOD_ColorRGB fg;
};

inline flecs::entity CreatePlayerPrefab(flecs::world& ecs) {
    return ecs.prefab("PlayerPrefab")
        .set<Position>({ 0, 0 })
        .set<Renderable>({ '@', TCOD_white });
}

inline flecs::entity CreateEnemyPrefab(flecs::world & ecs) {
    return ecs.prefab("EnemyPrefab")
        .set<Position>({ 0, 0 })
        .set<Renderable>({ 'E', TCOD_red });
}