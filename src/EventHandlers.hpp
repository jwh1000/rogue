#ifndef EVENT_HANDLER_HPP
#define EVENT_HANDLER_HPP

#include <SDL2/SDL.h>
//#include "actions.hpp"

class EventHandler {
public:
    std::unique_ptr<Action> handle_event(SDL_Event& event) {
        switch (event.type) {
            case SDL_QUIT:
                return std::make_unique<EscapeAction>();
            case SDL_KEYDOWN:
                return event_keydown(event.key);
            default:
                return nullptr;
        }
    }

private:
    std::unique_ptr<Action> event_keydown(SDL_KeyboardEvent& keyEvent) {
        switch (keyEvent.keysym.sym) {
            case SDLK_UP:
            case SDLK_k:
                return std::make_unique<MovementAction>(0, -1);
            case SDLK_DOWN:
            case SDLK_j:
                return std::make_unique<MovementAction>(0, 1);
            case SDLK_LEFT:
            case SDLK_h:
                return std::make_unique<MovementAction>(-1, 0);
            case SDLK_RIGHT:
            case SDLK_l:
                return std::make_unique<MovementAction>(1, 0);
            case SDLK_y:
                return std::make_unique<MovementAction>(-1, -1);
            case SDLK_u:
                return std::make_unique<MovementAction>(1, -1);
            case SDLK_b:
                return std::make_unique<MovementAction>(-1, 1);
            case SDLK_n:
                return std::make_unique<MovementAction>(1, 1);

            case SDLK_ESCAPE:
                return std::make_unique<EscapeAction>();

            default:
                return nullptr;
        }
    }
};

#endif // EVENT_HANDLER_HPP
