/*
* procgen.cpp
* 
* contains utilities for procedural generation in the dungeon
*/

#include "procgen.hpp"

class Rect {
public:
	int x1;
	int y1;
	int x2;
	int y2;

	Rect(int x1, int y1, int w, int h) : x1(x1), y1(y1), x2(x1+w), y2(y1+h) {}

	bool intersect(const Rect& other) const;
	std::pair<int, int> center() const;
};

bool Rect::intersect(const Rect& other) const {
	return x1 <= other.x2 && x2 >= other.x1 && y1 <= other.y2 && y2 >= other.y1;
}

std::pair<int, int> Rect::center() const {
	return std::make_pair((x1 + x2) / 2, (y1 + y2) / 2);
}