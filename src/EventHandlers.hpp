#ifndef EVENT_HANDLER_HPP
#define EVENT_HANDLER_HPP

#include <SDL2/SDL.h>
#include "actions.hpp"

class EventHandler {
public:
    std::unique_ptr<Action> convertEvent(SDL_Event& event) {
        switch (event.type) {
            case SDL_QUIT:
                return std::make_unique<EscapeAction>();
            case SDL_KEYDOWN:
                return eventKeydown(event.key);
            default:
                return nullptr;
        }
    }

private:
    std::unique_ptr<Action> eventKeydown(SDL_KeyboardEvent& keyEvent) {
        switch (keyEvent.keysym.sym) {
            case SDLK_UP:
            case SDLK_k:
            case SDLK_KP_8:
                return std::make_unique<MovementAction>(0, -1);
            case SDLK_DOWN:
            case SDLK_j:
            case SDLK_KP_2:
                return std::make_unique<MovementAction>(0, 1);
            case SDLK_LEFT:
            case SDLK_h:
            case SDLK_KP_4:
                return std::make_unique<MovementAction>(-1, 0);
            case SDLK_RIGHT:
            case SDLK_l:
            case SDLK_KP_6:
                return std::make_unique<MovementAction>(1, 0);
            case SDLK_y:
            case SDLK_KP_7:
                return std::make_unique<MovementAction>(-1, -1);
            case SDLK_u:
            case SDLK_KP_9:
                return std::make_unique<MovementAction>(1, -1);
            case SDLK_b:
            case SDLK_KP_1:
                return std::make_unique<MovementAction>(-1, 1);
            case SDLK_n:
            case SDLK_KP_3:
                return std::make_unique<MovementAction>(1, 1);

            case SDLK_ESCAPE:
                return std::make_unique<EscapeAction>();

            default:
                return nullptr;
        }
    }
};

#endif // EVENT_HANDLER_HPP
