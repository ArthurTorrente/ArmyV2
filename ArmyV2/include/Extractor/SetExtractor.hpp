#ifndef _SETEXTRACTOR_H_
#define _SETEXTRACTOR_H_

#pragma warning(push, 0)
#include <numeric>
#include <algorithm>
#include <functional>
#include <sstream>
#pragma warning(pop)

#include "IExtractor.hpp"



/**
 * Returns the unit army of Allies army
 */
class AlliesArmyExtractor : public SetExtractor
{
public:
    AlliesArmyExtractor()
        : SetExtractor()
    {}

    UnitSPtrVector operator()(Unit& unit, Army& allies, Army& opponent)
    {
        tools::unusedArg(unit, opponent);

        return allies.getUnitsList();
    }

    std::string getCode() const
    {
        return std::string("A");
    }
};

/**
* Returns the unit army of Opponent army
*/

class OpponentArmyExtractor : public SetExtractor
{
public:
    OpponentArmyExtractor()
        : SetExtractor()
    {}

    UnitSPtrVector operator()(Unit& unit, Army& allies, Army& opponent)
    {
        tools::unusedArg(unit, allies);

        return opponent.getUnitsList();
    }

    std::string getCode() const
    {
        return std::string("O");
    }
};

/**
 * Return the first N units that have the best capacity or worse
 * It use 3 parameters, CapacityIndex for choose the capacity, N is the number of unit returns, and a SetExtractor
 */

class NMinMaxCapacityExtractor : public SetExtractor
{
public:
    enum Type
    {
        MIN,
        MAX
    };

    NMinMaxCapacityExtractor(Type type, unsigned int N, unsigned int capacityIndex, SetExtractorUPtr& sex)
        : SetExtractor(),
        m_type(type),
        m_n(N),
        m_capacityIndex(capacityIndex),
        m_setExtractor(std::move(sex))
    {
        if (type == Type::MAX)
            m_algo = std::bind(&NMinMaxCapacityExtractor::getMax, this, std::placeholders::_1);

        else
            m_algo = std::bind(&NMinMaxCapacityExtractor::getMin, this, std::placeholders::_1);
    }

    UnitSPtrVector operator()(Unit& unit, Army& allies, Army& opponent)
    {
        return m_algo((*m_setExtractor)(unit, allies, opponent));
    }

    unsigned int getN() const
    {
        return m_n;
    }

    unsigned int getCapacityIndex() const
    {
        return m_capacityIndex;
    }

    void setType(Type type)
    {
        m_type = type;

        if (type == Type::MAX)
            m_algo = std::bind(&NMinMaxCapacityExtractor::getMax, this, std::placeholders::_1);

        else
            m_algo = std::bind(&NMinMaxCapacityExtractor::getMin, this, std::placeholders::_1);
    }

    void setN(unsigned int N)
    {
        m_n = N;
    }

    void setCapacityIndex(unsigned int capacityIndex)
    {
        m_capacityIndex = capacityIndex;
    }

    std::string getCode() const
    {
        std::string code("N");

        code += (m_type == MIN ? "L" : " H");
        code += '0' + static_cast<char>(m_capacityIndex);
        code += std::to_string(m_n);
        code += m_setExtractor->getCode();

        return code;
    }

protected:
    Type m_type;
    unsigned int m_n;
    unsigned int m_capacityIndex;
    
    SetExtractorSPtr m_setExtractor;
    std::function<UnitSPtrVector(UnitSPtrVector)> m_algo;
    

    UnitSPtrVector getMin(UnitSPtrVector set)
    {
        if (set.size() == 0 || set.size() < m_n)
            return set;

        /* nth is for not sorting all the vector */
        std::nth_element(set.begin(), set.begin() + m_n, set.end(), [&](const UnitSPtr& a, const UnitSPtr& b)
        {
            return a->getCapacity(m_capacityIndex)->getValue() < b->getCapacity(m_capacityIndex)->getValue();
        });

        set.erase(set.begin() + m_n, set.end());

        return set;
    }

    UnitSPtrVector getMax(UnitSPtrVector set)
    {
        if (set.size() == 0 || set.size() < m_n)
            return set;

        /* nth is for not sorting all the vector */
        std::nth_element(set.begin(), set.begin() + m_n, set.end(), [&](const UnitSPtr& a, const UnitSPtr& b)
        {
            return  b->getCapacity(m_capacityIndex)->getValue() < a->getCapacity(m_capacityIndex)->getValue();
        });

        set.erase(set.begin() + m_n, set.end());

        return set;
    }
};

/**
 * The same as NMinMaxCapacity but with distance
 * Take a PointExtractor instead of a capacityIndex
 */
class NFarNearExtractor : public SetExtractor
{
public:
    NFarNearExtractor(bool isFar, unsigned int N, SetExtractorUPtr& sex, PointExtractorUPtr& pex)
        : SetExtractor(),
        m_isFar(isFar),
        m_n(N),
        m_setExtractor(std::move(sex)),
        m_pointExtractor(std::move(pex))
    {
        if (isFar)
        {
            m_algo = std::bind(&NFarNearExtractor::getFar, this, std::placeholders::_1, std::placeholders::_2);
        }
        else
        {
            m_algo = std::bind(&NFarNearExtractor::getNear, this, std::placeholders::_1, std::placeholders::_2);
        }
    }

    UnitSPtrVector operator()(Unit& unit, Army& allies, Army& opponent)
    {
        return m_algo((*m_setExtractor)(unit, allies, opponent), (*m_pointExtractor)(unit, allies, opponent));
    }

    unsigned int getN() const
    {
        return m_n;
    }

    void setIsFar(bool isFar)
    {
        m_isFar = isFar;

        if (isFar)
        {
            m_algo = std::bind(&NFarNearExtractor::getFar, this, std::placeholders::_1, std::placeholders::_2);
        }
        else
        {
            m_algo = std::bind(&NFarNearExtractor::getNear, this, std::placeholders::_1, std::placeholders::_2);
        }
    }

    void setN(unsigned int N)
    {
        m_n = N;
    }

    std::string getCode() const
    {
        std::string code("N");

        code += (m_isFar ? "HD" : "LD");
        code += std::to_string(m_n);
        code += m_setExtractor->getCode();
        code += m_pointExtractor->getCode();

        return code;
    }

protected:
    bool m_isFar;
    unsigned int m_n;
    SetExtractorUPtr m_setExtractor;
    PointExtractorUPtr m_pointExtractor;

    std::function<UnitSPtrVector(UnitSPtrVector, const Point&)> m_algo;

    UnitSPtrVector getFar(UnitSPtrVector set, const Point& p)
    {
        if (set.size() == 0 || set.size() < m_n)
            return set;

        /* nth is for not sorting all the vector */
        std::nth_element(set.begin(), set.begin() + m_n, set.end(), [&](const UnitSPtr& a, const UnitSPtr& b)
        {
            return a->getPosition().distance(p) > b->getPosition().distance(p);
        });

        set.erase(set.begin() + m_n, set.end());

        return set;
    }

    UnitSPtrVector getNear(UnitSPtrVector set, const Point& p)
    {

        if (set.size() == 0 || set.size() < m_n)
            return set;

        /* nth is for not sorting all the vector */
        std::nth_element(set.begin(), set.begin() + m_n, set.end(), [&](const UnitSPtr& a, const UnitSPtr& b)
        {
            return b->getPosition().distance(p) > a->getPosition().distance(p);

        });

        set.erase(set.begin() + m_n, set.end());

        return set;
    }
};

/**
 * Make an array of all the unit which have a greater capacity value or lower than the treshold's value
 */
class ThresholdCapacityExtractor : public SetExtractor
{
public:
    ThresholdCapacityExtractor(bool isMin, float threshold, unsigned int capacityIndex, SetExtractorUPtr& sex)
        : SetExtractor(),
        m_isMin(isMin),
        m_threshold(threshold),
        m_capacityIndex(capacityIndex),
        m_setExtractor(std::move(sex))
    {
        if (isMin)
        {
            m_algo = std::bind(&ThresholdCapacityExtractor::getMin, this, std::placeholders::_1);
        }
        else
        {
            m_algo = std::bind(&ThresholdCapacityExtractor::getMax, this, std::placeholders::_1);
        }
    }

    UnitSPtrVector operator()(Unit& unit, Army& allies, Army& opponent)
    {
        
        return m_algo((*m_setExtractor)(unit, allies, opponent));
    }

    float getTreshold() const
    {
        return m_threshold;
    }

    unsigned int getCapacityIndex() const
    {
        return m_capacityIndex;
    }

    void setIsMin(bool isMin)
    {
        m_isMin = isMin;

        if (isMin)
        {
            m_algo = std::bind(&ThresholdCapacityExtractor::getMin, this, std::placeholders::_1);
        }
        else
        {
            m_algo = std::bind(&ThresholdCapacityExtractor::getMax, this, std::placeholders::_1);
        }
    }

    void setThreshold(float threshold)
    {
        m_threshold = threshold;
    }

    void setCapacityIndex(unsigned int cI)
    {
        m_capacityIndex = cI;
    }

    std::string getCode() const
    {
        std::stringstream ss;

        ss << "T";
        ss << (m_isMin ? "L" : "H");
        ss << m_capacityIndex;
        ss << std::setprecision(4) << std::fixed;
        ss << m_threshold;
        ss << m_setExtractor->getCode();

        return ss.str();
    }

protected:
    bool m_isMin;
    float m_threshold;
    unsigned int m_capacityIndex;
    SetExtractorUPtr m_setExtractor;

    std::function<UnitSPtrVector(UnitSPtrVector)> m_algo;

    UnitSPtrVector getMin(UnitSPtrVector set)
    {
        if (set.size() == 0)
            return set;

        set.erase(
            std::remove_if(set.begin(), set.end(), [&](const UnitSPtr& u)
        {
            return u->getCapacity(m_capacityIndex)->getValue() > m_threshold;
        }), set.end());

        return set;
    }

    UnitSPtrVector getMax(UnitSPtrVector set)
    {
        if (set.size() == 0)
            return set;

        set.erase(
            std::remove_if(set.begin(), set.end(), [&](const UnitSPtr& u)
        {
            return u->getCapacity(m_capacityIndex)->getValue() < m_threshold;
        }), set.end());

        return set;
    }
};

/**
 * It's the same of Threshold for capacity value but with the distance
 */

class ThresholdDistanceExtractor : public SetExtractor
{
public:
    ThresholdDistanceExtractor(bool isMin, float threshold, SetExtractorUPtr& sex, PointExtractorUPtr& pex)
        : SetExtractor(),
        m_isMin(isMin),
        m_threshold(threshold),
        m_setExtractor(std::move(sex)),
        m_pointExtractor(std::move(pex))
    {
        if (isMin)
        {
            m_algo = std::bind(&ThresholdDistanceExtractor::getNear, this, std::placeholders::_1, std::placeholders::_2);
        }
        else
        {
            m_algo = std::bind(&ThresholdDistanceExtractor::getFar, this, std::placeholders::_1, std::placeholders::_2);
        }
    }

    UnitSPtrVector operator()(Unit& unit, Army& allies, Army& opponent)
    {
        return m_algo((*m_setExtractor)(unit, allies, opponent), (*m_pointExtractor)(unit, allies, opponent));
    }

    float getTreshold() const
    {
        return m_threshold;
    }
    
    void setThreshold(float threshold)
    {
        m_threshold = threshold;
    }

    void setMin(bool isMin)
    {
        m_isMin = isMin;
        if (isMin)
        {
            m_algo = std::bind(&ThresholdDistanceExtractor::getNear, this, std::placeholders::_1, std::placeholders::_2);
        }
        else
        {
            m_algo = std::bind(&ThresholdDistanceExtractor::getFar, this, std::placeholders::_1, std::placeholders::_2);
        }
    }

    std::string getCode() const
    {

        std::stringstream ss;

        ss << "T";
        ss << (m_isMin ? "LD" : "HD");
        ss << std::setprecision(4) << std::fixed;
        ss << m_threshold;
        ss << m_setExtractor->getCode();
        ss << m_pointExtractor->getCode();

        return ss.str();
    }

protected:
    bool m_isMin;
    float m_threshold;

    SetExtractorUPtr m_setExtractor;
    PointExtractorUPtr m_pointExtractor;

    std::function<UnitSPtrVector(UnitSPtrVector, const Point&)> m_algo;

    UnitSPtrVector getNear(UnitSPtrVector set, const Point& p)
    {
        if (set.size() == 0)
            return set;

        set.erase(
            std::remove_if(set.begin(), set.end(), [&](const UnitSPtr& u)
        {
            return u->getPosition().distance(p) > m_threshold;
        }), set.end());

        return set;
    }

    UnitSPtrVector getFar(UnitSPtrVector set, const Point& p)
    {
        if (set.size() == 0)
            return set;

        set.erase(
            std::remove_if(set.begin(), set.end(), [&](const UnitSPtr& u)
        {
            return u->getPosition().distance(p) < m_threshold;
        }), set.end());

        return set;
    }
};

#endif //_SETEXTRACTOR_H_