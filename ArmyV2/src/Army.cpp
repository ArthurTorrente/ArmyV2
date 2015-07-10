#pragma warning(push, 0)
#include <stdexcept>
#include <limits>
#include <memory>
#include <iostream>
#pragma warning(pop)

#include "Army.hpp"


void Army::copyUnits_(const std::vector<UnitSPtr >& units)
{
    units_.clear();
    for(auto it = units.begin(); it != units.end(); ++it) {
        units_.push_back(UnitSPtr(new Unit(*(it->get()))));
    }
}

Army::Army(int size, int level)
{
    while(size--) {
        units_.push_back(UnitSPtr(new Unit(level)));
    }
}

Army::Army(std::vector<UnitSPtr >& units)
{
    copyUnits_(units);
}


Army::Army(const Army& army)
{
    copyUnits_(army.units_);
}

void Army::swap(Army& army)
{
    std::swap(units_, army.units_);
}

Army& Army::operator=(Army army)
{
    swap(army);
    return *this;
}


Unit& Army::getNearestUnit(const Point& p)
{
    if(units_.empty())throw std::invalid_argument("empty army");
    Unit* result = nullptr;
    float minDist = std::numeric_limits<float>::max();
    for(auto it = units_.begin(); it != units_.end(); ++it) {
        float d = (*it)->getPosition().distance(p);
        if(d < minDist) {
            minDist = d;
            result = it->get();
        }
    }
    return *result;
}

Unit& Army::getFurthestUnit(const Point& p)
{
    if(units_.empty())throw std::invalid_argument("empty army");    
    Unit* result = nullptr;
    
    float maxDist = -1.0f;

    for(auto it = units_.begin(); it != units_.end(); ++it) {
        float d = (*it)->getPosition().distance(p);
        if(d > maxDist) {
            maxDist = d;
            result = it->get();
        }
    }
    return *result;
}

Unit& Army::getLowestUnit(unsigned int capa_index)
{
    if(units_.empty())throw std::invalid_argument("empty army");

    return **std::min_element(units_.begin(), units_.end(),
        [&capa_index](const UnitSPtr& a, const UnitSPtr& b) {
        return a->getCapacity(capa_index)->getLevel() < b->getCapacity(capa_index)->getLevel();
    });
}

Unit& Army::getHigestUnit(unsigned int capa_index)
{
    if(units_.empty())throw std::invalid_argument("empty army");

    return **std::max_element(units_.begin(), units_.end(),
        [&capa_index](const UnitSPtr& a, const UnitSPtr& b) {
        return a->getCapacity(capa_index)->getLevel() < b->getCapacity(capa_index)->getLevel();
    });
}

void Army::purge()
{
    units_.erase(std::remove_if(units_.begin(), units_.end(), [](const UnitSPtr& u) {
        return !(u->isAlive());
    }), units_.end());
}



Army Army::mutate()const
{
    if(units_.size()==0)
        return Army(*this);

    Army mutated(*this);
    unsigned int count = 1 + std::rand() % units_.size();
    
    while(count--) {
        if(std::rand() % 3 == 0 || units_.size() == 1)
            mutated.units_[std::rand() % units_.size()] = UnitSPtr(new Unit(units_[0]->getLevel()));

        else if(std::rand()%2)
        {
            unsigned int i1 = std::rand() % units_.size();
            unsigned int i2 = std::rand() % units_.size();
            
            while(i1 == i2)
                i2 = std::rand() % units_.size();

            mutated.units_[i1] = UnitSPtr(new Unit(*(units_[i2])));
        }
        else
        {
            unsigned int i1 = std::rand()%units_.size();
            unsigned int i2 = std::rand()%units_.size();
            
            while(i1 == i2)
                i2 = std::rand() % units_.size();
            
            mutated.units_[i1] = UnitSPtr(new Unit((*(units_[i2]))*(*(units_[i1]))));

        }
    }
    return mutated;
}


Army Army::operator*(const Army& army)const
{
    if(this->size() == 0 && army.size() == 0)
        return Army(*this);

    unsigned int s = std::min(this->size(),army.size());
    std::vector<UnitSPtr > crossing;
    
    for(unsigned int i = 0; i < s; i++)
    {
        switch(std::rand() % 4)
        {
        case 0 :
            crossing.push_back(UnitSPtr(new Unit(*(units_[i]))));
            break;
        case 1 :
            crossing.push_back(UnitSPtr(new Unit(*(army.units_[i]))));
            break;
        case 2 :
            crossing.push_back(UnitSPtr(new Unit(*(units_[i])**(army.units_[std::rand() % army.units_.size()]))));
            break;
        case 3 :
            crossing.push_back(UnitSPtr(new Unit(*(army.units_[i])**(units_[std::rand() % units_.size()]))));
            break;
        }
    }
    return Army(crossing);
}


void Army::save(std::ostream& out)const
{
    std::for_each(units_.begin(), units_.end(), [&out](const UnitSPtr& unit) {
        unit->save(out);
    });
}

Army Army::load(std::istream& in)
{
    std::vector<UnitSPtr > units;

    in.exceptions(std::istream::failbit | std::istream::eofbit);
    while(in) {
        try {
            units.push_back(UnitSPtr(new Unit(Unit::load(in))));
        } catch(...) {}
    }
    return Army(units);
}