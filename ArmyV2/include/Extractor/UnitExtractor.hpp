#ifndef _UNITEXTRACTOR_H_
#define _UNITEXTRACTOR_H_

#include "IExtractor.hpp"

class IAUnitExtractor : public UnitExtractor
{
public:
    virtual UnitSPtr operator()(const UnitSPtr& unit, const ArmyPtr& allies, const ArmyPtr& opponent)
    {
        tools::unusedArg(allies, opponent);

        return unit;
    }
};

class MinMaxCapacityExtractor : public UnitExtractor
{
public:
    MinMaxCapacityExtractor(bool isMin, unsigned int capacityIndex, SetExtractorUPtr& sex)
        : UnitExtractor(),
        m_capacityIndex(capacityIndex),
        m_setGetter(std::move(sex))
    {
        if (isMin)
            m_algo = std::bind(&MinMaxCapacityExtractor::getMin, this, std::placeholders::_1);

        else
            m_algo = std::bind(&MinMaxCapacityExtractor::getMax, this, std::placeholders::_1);
    }

    virtual UnitSPtr operator()(const UnitSPtr& unit, const ArmyPtr& allies, const ArmyPtr& opponent)
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
        if (isMin)
            m_algo = std::bind(&MinMaxCapacityExtractor::getMin, this, std::placeholders::_1);
        
        else
            m_algo = std::bind(&MinMaxCapacityExtractor::getMax, this, std::placeholders::_1);
    }

    void setCapacityIndex(unsigned int capacityIndex)
    {
        m_capacityIndex = capacityIndex;
    }

protected:
    unsigned int m_capacityIndex;
    SetExtractorUPtr m_setGetter;

    std::function<UnitSPtr(const UnitVector&)> m_algo;

    UnitSPtr getMin(const UnitVector& set)
    {
        if (set.size() == 0)
            return UnitSPtr(nullptr);

        return *std::min_element(set.begin(), set.end(), [&](const UnitSPtr& a, const UnitSPtr& b)
        {
            return a->getCapacity(m_capacityIndex)->getValue() < b->getCapacity(m_capacityIndex)->getValue();
        });
    }

    UnitSPtr getMax(const UnitVector& set)
    {
        if (set.size() == 0)
            return UnitSPtr(nullptr);

        return *std::max_element(set.begin(), set.end(), [&](const UnitSPtr& a, const UnitSPtr& b)
        {
            return a->getCapacity(m_capacityIndex)->getValue() < b->getCapacity(m_capacityIndex)->getValue();
        });
    }
};

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

    virtual UnitSPtr operator()(const UnitSPtr& unit, const ArmyPtr& allies, const ArmyPtr& opponent)
    {
        return m_algo(
            (*m_setExtractor)(unit, allies, opponent),
            (*m_pointExtractor)(unit, allies, opponent)
            );
    }

protected:
    bool m_isFar;
    SetExtractorUPtr m_setExtractor;
    PointExtractorUPtr m_pointExtractor;

    std::function<UnitSPtr(const UnitVector&, const Point&)> m_algo;

    UnitSPtr getFar(const UnitVector& set, const Point& p)
    {
        if (set.size() == 0)
            return UnitSPtr(nullptr);

        return *std::max_element(set.begin(), set.end(), [&](const UnitSPtr& a, const UnitSPtr& b)
        {
            return a->getPosition().distance(p) < b->getPosition().distance(p);
        });
    }

    UnitSPtr getNear(const UnitVector& set, const Point& p)
    {
        if (set.size() == 0)
            return UnitSPtr(nullptr);

        return *std::min_element(set.begin(), set.end(), [&](const UnitSPtr& a, const UnitSPtr& b)
        {
            return a->getPosition().distance(p) < b->getPosition().distance(p);
        });
    }
};

#endif //_UNITEXTRACTOR_H_