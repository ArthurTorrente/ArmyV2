#ifndef _SHOT_ACTION_HPP_
#define _SHOT_ACTION_HPP_

#pragma warning(push, 0)
#include <iomanip>
#include <iostream>
#pragma warning(pop)

#include "Unit.hpp"
#include "Action.hpp"
//Class representing the action of attacking one unit with another
class ShootAction : public Action {
private:
    Unit* unit_;
    Unit* opponent_;

public:
    //Constructor with as first paramter the unit attacking, and as second, its target
    ShootAction(Unit& unit, Unit& opponent)
        : unit_(&unit),
        opponent_(&opponent)
    {}

    ShootAction(UnitSPtr unit, UnitSPtr opponent)
        : unit_(unit.get()),
        opponent_(opponent.get())
    {}

    //Run the action
    //Log parameter indicate if we write something or not on the standard output
    void execute(bool log = false)
    {
        opponent_->takeDamage(unit_->getDamage().getValue());
        unit_->shoot();
        if(log) {
            std::cout<<"Unit "<<unit_->getId()<<" shoot Unit "<<opponent_->getId();
            float hp = opponent_->getLife().getValue();
            if(hp>0) {
                std::cout<<" ("<<hp<<"hp remaining)"<<std::endl;
            } else {
                std::cout<<" (dead !)"<<std::endl;
            }
        }
    }
};

#endif