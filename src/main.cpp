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

struct Position {
    int x, y;
};

struct Renderable {
    char glyph;
    TCOD_ColorRGB fg;
    TCOD_ColorRGB bg;
};

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

    auto enemy = ecs.entity()
        .set([](Position& p) {
            p = { 1, 1 };
        })
        .set([](Renderable& r) {
        r = { '@', TCOD_red, TCOD_black };
            });

    auto player = ecs.entity()
        .set([player_x, player_y](Position& p) {
        p = { player_x, player_y };
            })
        .set([](Renderable& r) {
        r = { '@', TCOD_white, TCOD_black };
            });

    ecs.system<const Position, const Renderable>()
        .each([&](const Position& pos, const Renderable& ren) {
        tcod::print(console, { pos.x, pos.y }, std::string(1, ren.glyph), ren.fg, ren.bg);
            });

    bool running = true;
    while (running) {
        TCOD_console_clear(console.get());
        
        ecs.progress();

        context.present(console);

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    running = false;
                }
            }
        }
    }

    return 0;
}
