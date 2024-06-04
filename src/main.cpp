/*
* main.cpp
* created by ironhands
* 
* Main file, probably going to change a lot!
* program starts here
*/

#include <libtcod.hpp>
#include <SDL2/SDL.h>
#include "data.hpp"

int main(int argc, char* argv[]) {
    const int screen_width = 80;
    const int screen_height = 50;

    TCOD_ContextParams params = {};
    params.argc = argc;
    params.argv = argv;
    params.tcod_version = TCOD_COMPILEDVERSION;
    params.window_x = screen_width;
    params.window_y = screen_height;
    params.window_title = "rogue thing";
    params.vsync = true;
    params.sdl_window_flags = SDL_WINDOW_RESIZABLE;

    auto tileset = tcod::load_tilesheet(get_data_dir() / "dejavu10x10_gs_tc.png", { 32, 8 }, tcod::CHARMAP_TCOD);
    params.tileset = tileset.get();

    auto console = tcod::Console(screen_width, screen_height);
    params.console = console.get();

    auto context = tcod::new_context(params);   
    
    bool running = true;
    while (running) {
        TCOD_console_clear(console.get());
        tcod::print(console, { 0, 0 }, "some text", TCOD_white, std::nullopt);
        context->present(console);

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
}
