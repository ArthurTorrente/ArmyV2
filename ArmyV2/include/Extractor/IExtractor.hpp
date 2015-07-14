#ifndef _IEXTRACTOR_H_
#define _IEXTRACTOR_H_

#pragma warning(push, 0)
#include <memory>
#include <vector>
#pragma warning(pop)

#include "Army.hpp"

/**
 * Base class of Extractor
 * Return type of fonctor is template because we want use different return value from Extractor
 */
template < typename OUT >
class Extractor
{
public:
    /* Return value of Filter */
    virtual OUT operator()(const UnitSPtr&, const ArmyPtr&, const ArmyPtr&) = 0;
};

/**
 * Declare all possible type of Extractor with typedef
 */
typedef Extractor<float> FloatExtractor;
typedef Extractor<UnitSPtr> UnitExtractor;
typedef Extractor<UnitVector> SetExtractor;
typedef Extractor<Point> PointExtractor;


/**
 * Shared pointer of all type of extractor
 */
typedef std::shared_ptr<FloatExtractor> FloatExtractorSPtr;
typedef std::shared_ptr<UnitExtractor> UnitExtractorSPtr;
typedef std::shared_ptr<SetExtractor> SetExtractorSPtr;
typedef std::shared_ptr<PointExtractor> PointExtractorSPtr;

/**
* Unique pointer of all type of extractor
*/
typedef std::unique_ptr<FloatExtractor> FloatExtractorUPtr;
typedef std::unique_ptr<UnitExtractor> UnitExtractorUPtr;
typedef std::unique_ptr<SetExtractor> SetExtractorUPtr;
typedef std::unique_ptr<PointExtractor> PointExtractorUPtr;

#endif //_IEXTRACTOR_H_