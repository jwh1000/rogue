/*
* main.cpp
* created by ironhands
* 
* Main file, probably going to change a lot!
* program starts here
*/

#include <flecs.h>
#include <iostream>
#include <libtcod.hpp>
#include <SDL2/SDL.h>

#include "actions.hpp"
#include "data.hpp"
#include "engine.hpp"
#include "EventHandlers.hpp"

// ecs stuffs
#include "components.hpp"
#include "map.hpp"
#include "prefabs.hpp"
#include "tags.hpp"


tcod::Context initContext(tcod::Tileset& tileset, tcod::Console& console) {

    int screen_width = console.get_width();
    int screen_height = console.get_height();

    TCOD_ContextParams params = {};
    params.vsync = true;
    params.renderer_type = TCOD_RENDERER_SDL2;
    params.sdl_window_flags = SDL_WINDOW_RESIZABLE;
    params.tcod_version = TCOD_COMPILEDVERSION;
    params.window_title = "rogue thing";
    params.window_x = screen_width;
    params.window_y = screen_height;
    params.tileset = tileset.get();
    params.console = console.get();

    return tcod::Context(params);
}

int main(int argc, char* argv[]) {
    flecs::world ecs;
    GameState gameState(ecs);
    Engine engine(ecs);
    
    constexpr int screen_width = 80;
    constexpr int screen_height = 50;
    auto console = tcod::Console(screen_width, screen_height);
    auto tileset = tcod::load_tilesheet(get_data_dir() / "Alloy_curses_12x12.png", { 16, 16 }, tcod::CHARMAP_CP437);
    auto context = initContext(tileset, console);

    // for testing
    auto enemyPrefab = CreateEnemyPrefab(ecs);
    for (int i = 0; i < 11; i++) {
        ecs.entity()
            .is_a(enemyPrefab)
            .set<Position>({ i * 7, 20 });
    }

    // add systems to world
    engine.initSystems(console);

    // create the map and add it to the world
    ecs.set<Map>({ new_map_rooms_and_corridors() });

    while (true) {
        TCOD_console_clear(console.get());

        gameState.tick(console, context);

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            engine.handle_events(event);
        }
    }

    return 0;
}

