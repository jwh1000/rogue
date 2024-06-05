// action.hpp
#ifndef ACTION_HPP
#define ACTION_HPP

class Action {
public:
    virtual ~Action() = default;
};

class EscapeAction : public Action {
public:
    ~EscapeAction() override = default;
};

class MovementAction : public Action {
public:
    int dx;
    int dy;

    MovementAction(int dx, int dy) : dx(dx), dy(dy) {}
    ~MovementAction() override = default;
};

#endif // ACTION_HPP
