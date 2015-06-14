#ifndef _IFILTERS_H_h
#define _IFILTERS_H_h

#include <memory>
#include <vector>

#include "Army.hpp"

/*
template<typename OUT>
class IFilter
{
public:
OUT operator()(const std::vector<Unit>&) const;
}*/

template < typename OUT >
class IFilter
{
public:
    /* Return value of Filter */
    virtual OUT operator()() = 0;

protected:
    std::shared_ptr<Unit> m_unit;
    std::shared_ptr<Army> m_coop;
    std::shared_ptr<Army> m_opponent;
};

class FloatFliter : public IFilter < float >
{
public:
    float operator()()
    {
        return m_unit->getCapacity(1)->getValue();
    }
};

class UnitVectorFilter : public IFilter< std::vector<std::shared_ptr<Unit>> >
{
public:
    std::vector<std::shared_ptr<Unit>> operator()()
    {
        return std::vector<std::shared_ptr<Unit>>();
    }
};

#endif //_IFILTERS_H_h