#ifndef _EXTRACTORFACTORY_H_
#define _EXTRACTORFACTORY_H_

#include <sstream>

#include "Extractor/IExtractor.hpp"

namespace Factory
{
    namespace ex
    {
        /**
         * Make a float extractor with stringstream
         */
        FloatExtractorUPtr getFloatExtractor(std::stringstream& code);

        /**
         * Make an unit extractor with stringstream
         */
        UnitExtractorUPtr getUnitExtractor(std::stringstream& code);

        /**
         * Make a point extractor with stringstream
         */
        PointExtractorUPtr getPointExtractor(std::stringstream& code);

        /**
         * Make a set extractor with stringstream
         */
        SetExtractorUPtr getSetExtractor(std::stringstream& code);
    }
}

#endif //_EXTRACTORFACTORY_H_