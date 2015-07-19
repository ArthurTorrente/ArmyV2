#include "Factory/ExtractorFactory.hpp"

#include "Factory/FactoryException.hpp"

#include "Extractor/ValueExtractor.hpp"
#include "Extractor/UnitExtractor.hpp"
#include "Extractor/PointExtractor.hpp"
#include "Extractor/SetExtractor.hpp"

namespace Factory
{
    namespace ex
    {
        FloatExtractorUPtr getFloatExtractor(std::stringstream& code)
        {
            char first;
            code.get(first);

            switch (first)
            {
            case 'C':
            {
                char second;
                code.get(second);

                unsigned int capIndex = second - '0';

                if (capIndex > 6)
                    capIndex = 6;

                auto unitExtractor = getUnitExtractor(code);
                return FloatExtractorUPtr(new CapacityValueExtractor(capIndex, unitExtractor));
            }
                break;

            case 'D':
            {
                auto unitEx = getUnitExtractor(code);
                auto pointEx = getPointExtractor(code);

                return FloatExtractorUPtr(new DistanceExtractor(pointEx, unitEx));
            }
                break;

            case 'M':
            {
                char second;
                code.get(second);

                if (second == 'D')
                {
                    auto sex = getSetExtractor(code);
                    auto pex = getPointExtractor(code);

                    return FloatExtractorUPtr(
                        new MinMaxAverageDistanceValueExtractor(MinMaxAverageDistanceValueExtractor::MAX, sex, pex)
                        );
                }
                else
                {
                    unsigned int capIndex = second - '0';

                    if (capIndex > 6)
                        capIndex = 6;
                    
                    auto sex = getSetExtractor(code);

                    return FloatExtractorUPtr(
                        new MinMaxAverageCapacityValueExtractor(MinMaxAverageCapacityValueExtractor::MAX, capIndex, sex)
                        );
                }

            }
                break;

            case 'm':
            {
                char second;
                code.get(second);

                if (second == 'D')
                {
                    auto sex = getSetExtractor(code);
                    auto pex = getPointExtractor(code);

                    return FloatExtractorUPtr(
                        new MinMaxAverageDistanceValueExtractor(MinMaxAverageDistanceValueExtractor::MIN, sex, pex)
                        );
                }
                else
                {
                    unsigned int capIndex = second - '0';

                    if (capIndex > 6)
                        capIndex = 6;

                    auto sex = getSetExtractor(code);

                    return FloatExtractorUPtr(
                        new MinMaxAverageCapacityValueExtractor(MinMaxAverageCapacityValueExtractor::MIN, capIndex, sex)
                        );
                }
            }
                break;

            case 'a':
            {
                char second;
                code.get(second);

                if (second == 'D')
                {
                    auto sex = getSetExtractor(code);
                    auto pex = getPointExtractor(code);

                    return FloatExtractorUPtr(
                        new MinMaxAverageDistanceValueExtractor(MinMaxAverageDistanceValueExtractor::AVERAGE, sex, pex)
                        );
                }
                else
                {
                    unsigned int capIndex = second - '0';

                    if (capIndex > 6)
                        capIndex = 6;

                    auto sex = getSetExtractor(code);

                    return FloatExtractorUPtr(
                        new MinMaxAverageCapacityValueExtractor(MinMaxAverageCapacityValueExtractor::AVERAGE, capIndex, sex)
                        );
                }
            }
                break;

            case 'V':
            {
                float second;
                
                code << std::setprecision(4) << std::fixed;
                code >> second;

                return FloatExtractorUPtr(
                    new PreDefineValueExtractor(second)
                    );
            }

            default:
                throw FactoryException("Extractor not found");
            }
        }

        UnitExtractorUPtr getUnitExtractor(std::stringstream& code)
        {
            char first;

            code.get(first);

            switch (first)
            {
            case 'U':
            {
                return UnitExtractorUPtr(
                    new IAUnitExtractor()
                    );
            }
                break;

            case 'L':
            {
                char second;
                code.get(second);

                if (second == 'D')
                {
                    auto sex = getSetExtractor(code);
                    auto pex = getPointExtractor(code);

                    return UnitExtractorUPtr(
                        new FarNearExtractor(false, sex, pex)
                        );
                }
                else
                {
                    unsigned int capIndex = second - '0';

                    if (capIndex > 6)
                        capIndex = 6;

                    auto sex = getSetExtractor(code);

                    return UnitExtractorUPtr(
                        new MinMaxCapacityUnitExtractor(true, capIndex, sex)
                        );
                }
            }
            break;

            case 'H':
            {
                char second;
                code.get(second);

                if (second == 'D')
                {
                    auto sex = getSetExtractor(code);
                    auto pex = getPointExtractor(code);

                    return UnitExtractorUPtr(
                        new FarNearExtractor(true, sex, pex)
                        );
                }
                else
                {
                    unsigned int capIndex = second - '0';

                    if (capIndex > 6)
                        capIndex = 6;

                    auto sex = getSetExtractor(code);

                    return UnitExtractorUPtr(
                        new MinMaxCapacityUnitExtractor(false, capIndex, sex)
                        );
                }
            }
            break;

            default:
                throw FactoryException("UnitExtractor code not found");
            }
        }

        PointExtractorUPtr getPointExtractor(std::stringstream& code)
        {
            char first;
            code.get(first);

            switch (first)
            {
            case 'B':
            {
                auto sex = getSetExtractor(code);

                return PointExtractorUPtr(
                    new BaryCenterExtractor(sex)
                    );
            }
                break;

            case 'P':
            {
                auto uex = getUnitExtractor(code);

                return PointExtractorUPtr(
                    new UnitPositionExtractor(uex)
                    );
            }
                break;

            default:
                throw FactoryException("PointExtractor code not found");
            }
        }

        SetExtractorUPtr getSetExtractor(std::stringstream& code)
        {
            char first;
            code.get(first);

            switch (first)
            {
            case 'A':
            {
                return SetExtractorUPtr(
                    new AlliesArmyExtractor()
                    );
            }
                break;

            case 'O':
            {
                return SetExtractorUPtr(
                    new OpponentArmyExtractor()
                    );
            }
            break;

            case 'N':
            {
                char comp;
                char second;
                unsigned int number;

                code.get(comp);
                code.get(second);
                code >> number;

                auto sex = getSetExtractor(code);

                if (second == 'D')
                {
                    auto pex = getPointExtractor(code);
                    return SetExtractorUPtr(new NFarNearExtractor(comp == 'L' ? false : true, number, sex, pex));
                }
                else
                {
                    unsigned int capIndex = second - '0';

                    if (capIndex > 6)
                        capIndex = 6;

                    return SetExtractorUPtr(new NMinMaxCapacityExtractor(comp == 'L' ? NMinMaxCapacityExtractor::MIN : NMinMaxCapacityExtractor::MAX, number, capIndex, sex));
                }
            }
            break;

            case 'T':
            {
                char comp;
                char second;
                float threshold;

                code.get(comp);
                code.get(second);
                
                code << std::setprecision(4) << std::fixed;
                code >> threshold;

                auto sex = getSetExtractor(code);

                if (second == 'D')
                {
                    auto pex = getPointExtractor(code);
                    return SetExtractorUPtr(new ThresholdDistanceExtractor(comp == 'L' ? true : false, threshold, sex, pex));
                }
                else
                {
                    unsigned int capIndex = second - '0';

                    if (capIndex > 6)
                        capIndex = 6;

                    return SetExtractorUPtr(new ThresholdCapacityExtractor(comp == 'L' ? true : false, threshold, capIndex, sex));
                }
            }
            break;

            default:
                throw FactoryException("SetExtractor code not found");
            }
        }
    }
}