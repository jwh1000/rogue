/*
* components.hpp
* created by ironhands
* 
* the base component information for an entity
* an entity is anything that belongs in the world
*/

#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include <flecs.h>
#include <libtcod.hpp>

struct Position {
    int x, y;
};

struct Renderable {
    char glyph;
    TCOD_ColorRGB fg;
};

struct Movement {
    int dx;
    int dy;
};

#endif
