/*
* procgen.cpp
* 
* contains utilities for procedural generation in the dungeon
*/

#include "procgen.hpp"

bool Rect::intersect(const Rect& other) const {
	return x1 <= other.x2 && x2 >= other.x1 && y1 <= other.y2 && y2 >= other.y1;
}

std::pair<int, int> Rect::center() const {
	return std::make_pair((x1 + x2) / 2, (y1 + y2) / 2);
}