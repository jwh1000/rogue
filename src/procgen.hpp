/*
* 
*/

#include <utility>

class Rect {
public:
	int x1;
	int y1;
	int x2;
	int y2;

	Rect(int x1, int y1, int w, int h) : x1(x1), y1(y1), x2(x1 + w), y2(y1 + h) {}

	bool intersect(const Rect& other) const;
	std::pair<int, int> center() const;
};