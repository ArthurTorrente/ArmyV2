#include "Factory/ExtractorFactory.hpp"

namespace Factory
{
    namespace ex
    {
        FloatExtractorUPtr getFloatExtractor(std::stringstream& code)
        {
            return FloatExtractorUPtr(nullptr);
        }

        UnitExtractorUPtr getUnitExtractor(std::stringstream& code)
        {
            return UnitExtractorUPtr(nullptr);
        }

        PointExtractorUPtr getPointExtractor(std::stringstream& code)
        {
            return PointExtractorUPtr(nullptr);
        }

        SetExtractorUPtr getSetExtractor(std::stringstream& code)
        {
            return SetExtractorUPtr(nullptr);
        }
    }
}