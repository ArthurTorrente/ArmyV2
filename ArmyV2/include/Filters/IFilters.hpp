#ifndef _IFILTERS_H_h
#define _IFILTERS_H_h

#include <memory>
#include <vector>

#include "Army.hpp"

template < typename OUT >
class Extractor
{
public:
    /* Return value of Filter */
    virtual OUT operator()(const std::shared_ptr<Unit>&, const std::shared_ptr<Army>&, const std::shared_ptr<Army>&) = 0;
};

class FloatExtractor : public Extractor < float >
{
public:
    virtual float operator()(const std::shared_ptr<Unit>& u, const std::shared_ptr<Army>& a, const std::shared_ptr<Army>& b) = 0;
}; 

class CapacityValueExtractor : public FloatExtractor
{
public:
    float operator()(const std::shared_ptr<Unit>& u, const std::shared_ptr<Army>& a, const std::shared_ptr<Army>& b)
    {
        return u->getCapacity(m_capacity)->getValue();
    }

protected:
    int m_capacity;
};

class UnitVectorExtractor : public Extractor< std::vector<std::shared_ptr<Unit>> >
{
public:
    virtual std::vector<std::shared_ptr<Unit>> operator()(const std::shared_ptr<Unit>&, const std::shared_ptr<Army>&, const std::shared_ptr<Army>&) = 0;
};

#endif //_IFILTERS_H_h