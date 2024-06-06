/*
*/

#include "engine.hpp"

GameState::GameState(flecs::world& world) : ecs(world) {}

void GameState::tick(tcod::Console& console, tcod::Context& context) {
    TCOD_console_clear(console.get());
    auto map = ecs.get<Map>()->map;
    draw_map(map, console);
    ecs.progress();
    context.present(console);
}

Engine::Engine(flecs::world& world) : ecs(world) {
    auto playerPrefab = CreatePlayerPrefab(ecs);
    player = ecs.entity("Player")
        .is_a(playerPrefab)
        .set<Position>({ 40, 25 })
        .add<Player>();
}

void Engine::initSystems(tcod::Console& console) {
    // phase 1: perform all actions
    flecs::entity actionPhase = ecs.entity()
        .add(flecs::Phase);

    // phase 2: render entities after actions are taken
    flecs::entity renderPhase = ecs.entity()
        .add(flecs::Phase)
        .depends_on(actionPhase);

    ecs.system<LeftMover, Position>("LeftWalker")
        .kind(actionPhase)
        .iter([](flecs::iter& it, LeftMover* lm, Position* pos) {
            for (auto i : it) {
                pos[i].x -= 1; // Move the entity left
                if (pos[i].x < 0) pos[i].x = 79; // Wrap around the screen
            }
         });

    ecs.system<const Position, const Renderable>("Render")
        .kind(renderPhase)
        .each([&](const Position& pos, const Renderable& ren) {
            tcod::print(console, { pos.x, pos.y }, std::string(1, ren.glyph), ren.fg, TCOD_black);
           });

    ecs.progress();
}

void Engine::handle_events(SDL_Event event) {
    auto action = eventHandler.convert_event(event);
    if (action) {
        if (dynamic_cast<EscapeAction*>(action.get())) {
            exit(0);
        }
        else if (auto moveAction = dynamic_cast<MovementAction*>(action.get())) {
            auto playerPos = player.get<Position>();
            player.set<Position>({ playerPos->x + moveAction->dx, playerPos->y + moveAction->dy });
        }
    }
}
