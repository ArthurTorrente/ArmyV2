#ifndef _UNITEXTRACTOR_H_
#define _UNITEXTRACTOR_H_

#pragma warning(push, 0)
#include <functional>
#pragma warning(pop)

#include "IExtractor.hpp"

/**
 * Return the Unit of current IA
 */
class IAUnitExtractor : public UnitExtractor
{
public:
    Unit& operator()(Unit& unit, Army& allies, Army& opponent)
    {
        tools::unusedArg(allies, opponent);

        return unit;
    }

    std::string getCode() const
    {
        return std::string("U");
    }
};

/**
 * Returns the unit which has the best capacity value or the worst.
 */

class MinMaxCapacityUnitExtractor : public UnitExtractor
{
public:
    MinMaxCapacityUnitExtractor(bool isMin, unsigned int capacityIndex, SetExtractorUPtr& sex)
        : UnitExtractor(),
        m_isMin(isMin),
        m_capacityIndex(capacityIndex),
        m_setGetter(std::move(sex))
    {
        if (isMin)
            m_algo = std::bind(&MinMaxCapacityUnitExtractor::getMin, this, std::placeholders::_1);

        else
            m_algo = std::bind(&MinMaxCapacityUnitExtractor::getMax, this, std::placeholders::_1);
    }

    Unit& operator()(Unit& unit, Army& allies, Army& opponent)
    {
        return m_algo(
            (*m_setGetter)(unit, allies, opponent)
            );
    }

    unsigned int getCapacityIndex() const
    {
        return m_capacityIndex;
    }

    void setAlgorithm(bool isMin)
    {
        m_isMin = isMin;
        if (isMin)
            m_algo = std::bind(&MinMaxCapacityUnitExtractor::getMin, this, std::placeholders::_1);
        
        else
            m_algo = std::bind(&MinMaxCapacityUnitExtractor::getMax, this, std::placeholders::_1);
    }

    void setCapacityIndex(unsigned int capacityIndex)
    {
        m_capacityIndex = capacityIndex;
    }

    std::string getCode() const
    {
        std::string code(m_isMin ? "L" : "H");

        code += std::to_string(m_capacityIndex);
        code += m_setGetter->getCode();

        return code;
    }

protected:
    bool m_isMin;
    unsigned int m_capacityIndex;
    SetExtractorUPtr m_setGetter;

    std::function<Unit&(const UnitSPtrVector&)> m_algo;

    Unit& getMin(const UnitSPtrVector& set)
    {
        if (set.size() == 0)
            throw std::invalid_argument("Unit not found");

        return **std::min_element(set.begin(), set.end(), [&](const UnitSPtr& a, const UnitSPtr& b)
        {
            return a->getCapacity(m_capacityIndex)->getValue() < b->getCapacity(m_capacityIndex)->getValue();
        });
    }

    Unit& getMax(const UnitSPtrVector& set)
    {
        if (set.size() == 0)
            throw std::invalid_argument("Unit not found");

        return **std::max_element(set.begin(), set.end(), [&](const UnitSPtr& a, const UnitSPtr& b)
        {
            return a->getCapacity(m_capacityIndex)->getValue() < b->getCapacity(m_capacityIndex)->getValue();
        });
    }
};

/**
 * It's the same of Capacity value but it use the distance between a point and the unit position
 */

class FarNearExtractor : public UnitExtractor
{
public:

    FarNearExtractor(bool isFar, SetExtractorUPtr& sex, PointExtractorUPtr& pex)
        : UnitExtractor(),
        m_setExtractor(std::move(sex)),
        m_pointExtractor(std::move(pex))
    {
        if (isFar)
            m_algo = std::bind(&FarNearExtractor::getFar, this, std::placeholders::_1, std::placeholders::_2);

        else
            m_algo = std::bind(&FarNearExtractor::getNear, this, std::placeholders::_1, std::placeholders::_2);
    }

    Unit& operator()(Unit& unit, Army& allies, Army& opponent)
    {
        return m_algo(
            (*m_setExtractor)(unit, allies, opponent),
            (*m_pointExtractor)(unit, allies, opponent)
            );
    }

    std::string getCode() const
    {
        std::string code(m_isFar ? "HD" : "LD");

        code += m_setExtractor->getCode();
        code += m_pointExtractor->getCode();

        return code;
    }

protected:
    bool m_isFar;
    SetExtractorUPtr m_setExtractor;
    PointExtractorUPtr m_pointExtractor;

    std::function<Unit&(const UnitSPtrVector&, const Point&)> m_algo;

    Unit& getFar(const UnitSPtrVector& set, const Point& p)
    {
        if (set.size() == 0)
            throw std::invalid_argument("Unit not found");

        return **std::max_element(set.begin(), set.end(), [&](const UnitSPtr& a, const UnitSPtr& b)
        {
            return a->getPosition().distance(p) < b->getPosition().distance(p);
        });
    }

    Unit& getNear(const UnitSPtrVector& set, const Point& p)
    {
        if (set.size() == 0)
            throw std::invalid_argument("Unit not found");

        return **std::min_element(set.begin(), set.end(), [&](const UnitSPtr& a, const UnitSPtr& b)
        {
            return a->getPosition().distance(p) < b->getPosition().distance(p);
        });
    }
};

#endif //_UNITEXTRACTOR_H_