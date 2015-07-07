#ifndef _IEXTRACTOR_H_
#define _IEXTRACTOR_H_

#include <memory>
#include <vector>

#include "Army.hpp"

template < typename OUT >
class Extractor
{
public:
    /* Return value of Filter */
    virtual OUT operator()(const UnitPtr&, const ArmyPtr&, const ArmyPtr&) = 0;
};

typedef Extractor<float> FloatExtractor;
typedef Extractor<UnitPtr> UnitExtractor;
typedef Extractor<UnitVector> SetExtractor;
typedef Extractor<Point> PointExtractor;

#endif //_IEXTRACTOR_H_