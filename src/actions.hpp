/*
* actions.hpp
* created by ironhands
*
* Actions taken by the player
*/

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

// Maybe add a "takes time" bool? Like some actions will cause the
// turn to progress or something while others wont (like opening inventory)
class MovementAction : public Action {
public:
    int dx;
    int dy;

    MovementAction(int dx, int dy) : dx(dx), dy(dy) {}
    ~MovementAction() override = default;
};

#endif // ACTION_HPP
