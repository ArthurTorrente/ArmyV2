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

typedef Extractor<float> FloatExtractor;
typedef Extractor<Unit> UnitExtractor;
typedef Extractor<UnitVector> UnitVectorExtractor;

#endif //_IFILTERS_H_h