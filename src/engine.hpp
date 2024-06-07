#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <libtcod.h>
#include <flecs.h>
#include <vector>
#include <SDL2/SDL.h>

#include "EventHandlers.hpp"
#include "map.hpp"
#include "components.hpp"
#include "tags.hpp"
#include "prefabs.hpp"

class GameState {
public:
    flecs::world& ecs;

    GameState(flecs::world& world);

    void tick(tcod::Console& console, tcod::Context& context);
};

class Engine {
public:
    flecs::world& ecs;
    flecs::entity player;

    Engine(flecs::world& world);

    void initSystems(tcod::Console& console);

    void handleEvents(SDL_Event event);

private:
    EventHandler eventHandler;
};

#endif // GAME_HPP
