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

template < typename OUT, typename ... Args >
class IFilter 
{
public:
    virtual OUT operator()(Args) = 0;
};

class FloatFliter : public IFilter < float, int, int, int >
{
public:
    float operator()(int a, int b, int c)
    {
        return static_cast<float>(a + b + c);
    }
};

#endif //_IFILTERS_H_h