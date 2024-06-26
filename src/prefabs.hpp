/*
* prefabs.hpp
* created by ironhands
* 
* Prefabs for common entities
*/

#ifndef PREFABS_HPP
#define PREFABS_HPP

#include "components.hpp"
#include "tags.hpp"

inline flecs::entity CreatePlayerPrefab(flecs::world& ecs) {
    return ecs.prefab("PlayerPrefab")
        .set<Position>({ 0, 0 })
        .set<Renderable>({ '@', TCOD_brass })
        .add<Actor>();
}

inline flecs::entity CreateEnemyPrefab(flecs::world& ecs) {
    return ecs.prefab("EnemyPrefab")
        .set<Position>({ 0, 0 })
        .set<Renderable>({ 'E', TCOD_red })
        .add<Actor>()
        .add<LeftMover>();
}

#endif 