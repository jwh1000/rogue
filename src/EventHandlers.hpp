#ifndef EVENT_HANDLER_HPP
#define EVENT_HANDLER_HPP

#include <SDL2/SDL.h>
#include "actions.hpp"

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
                return std::make_unique<MovementAction>(0, -1);
            case SDLK_DOWN:
                return std::make_unique<MovementAction>(0, 1);
            case SDLK_LEFT:
                return std::make_unique<MovementAction>(-1, 0);
            case SDLK_RIGHT:
                return std::make_unique<MovementAction>(1, 0);

            case SDLK_ESCAPE:
                return std::make_unique<EscapeAction>();

            default:
                return nullptr;
        }
    }
};

#endif // EVENT_HANDLER_HPP
