#ifndef _UNITEXTRACTOR_H_
#define _UNITEXTRACTOR_H_

#include "IExtractor.hpp"

class IAUnitExtractor : public UnitExtractor
{
public:
    virtual UnitPtr operator()(const UnitPtr& unit, const ArmyPtr& allies, const ArmyPtr& opponent)
    {
        tools::unusedArg(allies, opponent);

        return unit;
    }
};

class MinMaxCapacityExtractor : public UnitExtractor
{
public:
    MinMaxCapacityExtractor(bool minAlgorithm, unsigned int capacityIndex, const UnitVector& set)
        : UnitExtractor(),
        m_isMin(minAlgorithm),
        m_capacityIndex(capacityIndex),
        m_set(set)
    {}

    virtual UnitPtr operator()(const UnitPtr& unit, const ArmyPtr& allies, const ArmyPtr& opponent)
    {
        tools::unusedArg(unit, allies, opponent);

        if (m_isMin)
        {
            return getMin();
        }

        return getMax();
    }

    bool isMin() const
    {
        return m_isMin;
    }

    unsigned int getCapacityIndex() const
    {
        return m_capacityIndex;
    }

    const UnitVector& getSet() const
    {
        return m_set;
    }

    void setAlgorithm(bool isMin)
    {
        m_isMin = isMin;
    }

    void setCapacityIndex(unsigned int capacityIndex)
    {
        m_capacityIndex = capacityIndex;
    }

    void setSet(const UnitVector& set)
    {
        m_set = set;
    }

protected:
    bool m_isMin;
    unsigned int m_capacityIndex;
    UnitVector m_set;

    UnitPtr getMin()
    {
        return *std::min_element(m_set.begin(), m_set.end(), [&](const UnitPtr& a, const UnitPtr& b)
        {
            return a->getCapacity(m_capacityIndex)->getValue() < b->getCapacity(m_capacityIndex)->getValue();
        });
    }

    UnitPtr getMax()
    {
        return *std::max_element(m_set.begin(), m_set.end(), [&](const UnitPtr& a, const UnitPtr& b)
        {
            return a->getCapacity(m_capacityIndex)->getValue() < b->getCapacity(m_capacityIndex)->getValue();
        });
    }
};

class FarNearExtractor : public UnitExtractor
{
public:

    FarNearExtractor(bool isFar, const Point& p, const UnitVector& set)
        : UnitExtractor(),
        m_isFar(isFar),
        m_point(p),
        m_set(set)
    {}

    virtual UnitPtr operator()(const UnitPtr& unit, const ArmyPtr& allies, const ArmyPtr& opponent)
    {
        tools::unusedArg(unit, allies, opponent);

        if (m_isFar)
        {
            return getFar();
        }

        return getNear();
    }

protected:
    bool m_isFar;
    Point m_point;
    UnitVector m_set;

    UnitPtr getFar()
    {
        return *std::max_element(m_set.begin(), m_set.end(), [&](const UnitPtr& a, const UnitPtr& b)
        {
            return a->getPosition().distance(m_point) < b->getPosition().distance(m_point);
        });
    }

    UnitPtr getNear()
    {
        return *std::min_element(m_set.begin(), m_set.end(), [&](const UnitPtr& a, const UnitPtr& b)
        {
            return a->getPosition().distance(m_point) < b->getPosition().distance(m_point);
        });
    }
};

bool unitTest_IAUnitExtractor()
{
    UnitPtr u(new Unit(0));
    ArmyPtr a(new Army(10, 200)), b(new Army(10, 200));
    
    IAUnitExtractor iaE;

    if (iaE(u, a, b)->getId() != u->getId())
        return false;

    return true;
}

bool unitTest_MinMaxCapacityExtractor()
{
    UnitPtr u(new Unit(0)), u2(new Unit(0)), u3(new Unit(0));
    ArmyPtr a(new Army(10, 200)), b(new Army(10, 200));

    UnitVector set;
    set.push_back(u);
    set.push_back(u2);
    set.push_back(u3);

    /* MAX*/
    MinMaxCapacityExtractor mmCMax(false, 0, set);
    u3->getCapacity(0)->upgrade();

    if (mmCMax(u, a, b)->getId() != u3->getId())
        return false;

    /* MIN */
    MinMaxCapacityExtractor mmCMin(true, 0, set);
    u2->getCapacity(0)->upgrade();

    if (mmCMin(u, a, b)->getId() != u->getId())
        return false;

    return true;
}

bool unitTest_FarNearExtractor()
{
    UnitPtr u(new Unit(0)), u2(new Unit(0)), u3(new Unit(0));
    ArmyPtr a(new Army(10, 200)), b(new Army(10, 200));

    UnitVector set;
    set.push_back(u);
    set.push_back(u2);
    set.push_back(u3);

    Point ori(0.0f, 0.0f), uP(1.0f, 1.0f), uP2(2.0f, 2.0f), uP3(3.0f, 3.0f);
    u->setPosition(uP);
    u2->setPosition(uP2);
    u3->setPosition(uP3);

    /* FAR */
    FarNearExtractor fnFar(true, ori, set);

    if (fnFar(u, a, b)->getId() != u3->getId())
        return false;

    /* NEAR */
    FarNearExtractor fnNear(false, ori, set);

    if (fnNear(u, a, b)->getId() != u->getId())
        return false;

    return true;
}

bool unitTest_UnitExtractor()
{
    if (!unitTest_IAUnitExtractor() ||
        !unitTest_MinMaxCapacityExtractor() ||
        !unitTest_FarNearExtractor())
        return false;

    return true;
}

#endif //_UNITEXTRACTOR_H_