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
#include "data.hpp"
#include "actions.hpp"
#include "InputHandlers.hpp"

struct Position {
    int x, y;
};

struct Renderable {
    char glyph;
    TCOD_ColorRGB fg;
    TCOD_ColorRGB bg;
};

struct LeftMover {};

struct Player {};

int main(int argc, char* argv[]) {
    flecs::world ecs;

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
    
    auto tileset = tcod::load_tilesheet(get_data_dir() / "dejavu10x10_gs_tc.png", { 32, 8 }, tcod::CHARMAP_TCOD);
    params.tileset = tileset.get();

    auto console = tcod::Console(screen_width, screen_height);
    params.console = console.get();

    auto context = tcod::Context(params);  

    ecs.component<Position>();
    ecs.component<Renderable>();

    for (int i = 0; i < 11; i++) {
        ecs.entity()
            .set<Position>({ i * 7, 20 })
            .set<Renderable>({ '@', TCOD_red, TCOD_black })
            .add<LeftMover>();
    }

    auto player = ecs.entity()
        .set<Position>({ player_x, player_y })
        .set<Renderable>({ '@', TCOD_white, TCOD_black })
        .add<Player>();

    ecs.system<const Position, const Renderable>("Render")
        .each([&](const Position& pos, const Renderable& ren) {
            tcod::print(console, { pos.x, pos.y }, std::string(1, ren.glyph), ren.fg, ren.bg);
    });


    // LeftWalker system using .iter
    ecs.system<LeftMover, Position>("LeftWalker")
        .iter([](flecs::iter& it, LeftMover* lm, Position* pos) {
        for (auto i : it) {
            pos[i].x -= 1; // Move the entity left
            if (pos[i].x < 0) pos[i].x = 79; // Wrap around the screen
        }
    });

    EventHandler eventHandler;

    bool running = true;
    while (running) {
        TCOD_console_clear(console.get());
        
        ecs.progress();

        context.present(console);

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
