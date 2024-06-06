/*
* main.cpp
* created by ironhands
* 
* Main file, probably going to change a lot!
* program starts here
*/

#include <libtcod.hpp>
#include <SDL2/SDL.h>
#include <flecs.h>
#include <vector>

#include "data.hpp"
#include "actions.hpp"
#include "EventHandlers.hpp"

// ecs stuffs
#include "tags.hpp"
#include "components.hpp"
#include "prefabs.hpp"


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
size_t xy_idx(int x, int y) {
    return static_cast<size_t>(y) * 80 + static_cast<size_t>(x);
}


std::vector<TileType> new_map() {
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


void draw_map(std::vector<TileType> map, tcod::Console& console) {
    int x = 0;
    int y = 0;
    for (auto tile : map) {
        switch (tile) {
            case TileType::Floor :
                tcod::print(console, { x, y }, std::string(1,'.'), TCOD_darker_gray, TCOD_black);
                break;
            case TileType::Wall :
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


class GameState {
public:
    flecs::world& ecs;

    GameState(flecs::world& world) : ecs(world) {}

    void tick(tcod::Console& console, tcod::Context& context) {
        TCOD_console_clear(console.get());
        auto map = ecs.get<Map>()->map;
        draw_map(map, console);
        ecs.progress();
        context.present(console);
    }
};


void init_systems(flecs::world& ecs, tcod::Console& console) {
    ecs.system<LeftMover, Position>("LeftWalker")
        .iter([](flecs::iter& it, LeftMover* lm, Position* pos) {
        for (auto i : it) {
            pos[i].x -= 1; // Move the entity left
            if (pos[i].x < 0) pos[i].x = 79; // Wrap around the screen
        }
    });

    ecs.system<const Position, const Renderable>("Render")
        .each([&](const Position& pos, const Renderable& ren) {
        tcod::print(console, { pos.x, pos.y }, std::string(1, ren.glyph), ren.fg, TCOD_black);
    });
}


int main(int argc, char* argv[]) {
    flecs::world ecs;
    GameState gameState(ecs);
    
    constexpr int screen_width = 80;
    constexpr int screen_height = 50;

    int player_x = int(screen_width / 2);
    int player_y = int(screen_height / 2);

    TCOD_ContextParams params = {};
    params.argc = argc;
    params.argv = argv;
    params.vsync = true;
    params.renderer_type = TCOD_RENDERER_SDL2;
    params.sdl_window_flags = SDL_WINDOW_RESIZABLE;
    params.tcod_version = TCOD_COMPILEDVERSION;
    params.window_title = "rogue thing";
    params.window_x = screen_width;
    params.window_y = screen_height;
    
    auto tileset = tcod::load_tilesheet(get_data_dir() / "Alloy_curses_12x12.png", { 16, 16 }, tcod::CHARMAP_CP437);
    
    params.tileset = tileset.get();

    auto console = tcod::Console(screen_width, screen_height);
    params.console = console.get();

    auto context = tcod::Context(params);  

    ecs.component<Position>();
    ecs.component<Renderable>();

    auto playerPrefab = CreatePlayerPrefab(ecs);
    auto enemyPrefab = CreateEnemyPrefab(ecs);

    for (int i = 0; i < 11; i++) {
        ecs.entity()
            .is_a(enemyPrefab)
            .set<Position>({ i * 7, 20 });
    }

    auto player = ecs.entity()
        .is_a(playerPrefab)
        .set<Position>({ player_x, player_y })
        .add<Player>();

    init_systems(ecs, console);
    EventHandler eventHandler;
    ecs.set<Map>({ new_map() });

    bool running = true;
    while (running) {
        TCOD_console_clear(console.get());

        gameState.tick(console, context);

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            auto action = eventHandler.handle_event(event);
            if (action) {
                if (dynamic_cast<EscapeAction*>(action.get())) {
                    running = false;
                }
                else if (auto moveAction = dynamic_cast<MovementAction*>(action.get())) {
                    auto playerPos = player.get<Position>();
                    player.set<Position>({ playerPos->x + moveAction->dx, playerPos->y + moveAction->dy });
                }
            }
        }
    }

    return 0;
}

