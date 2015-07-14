#ifndef _EXTRACTORFACTORY_H_
#define _EXTRACTORFACTORY_H_

#include <sstream>

#include "Extractor/IExtractor.hpp"

namespace Factory
{
    namespace ex
    {
        FloatExtractorUPtr getFloatExtractor(std::stringstream& code);
        UnitExtractorUPtr getUnitExtractor(std::stringstream& code);
        PointExtractorUPtr getPointExtractor(std::stringstream& code);
        SetExtractorUPtr getSetExtractor(std::stringstream& code);
    }
}

#endif //_EXTRACTORFACTORY_H_