#ifndef _POINTEXTRACTOR_H_
#define _POINTEXTRACTOR_H_

#include <numeric>

#include "IExtractor.hpp"

class BaryCenterExtractor : public PointExtractor
{
public:
    BaryCenterExtractor(const UnitVector& set)
        : PointExtractor(),
        m_set(set)
    {}

    virtual Point operator()(const UnitPtr& unit, const ArmyPtr& allies, const ArmyPtr& opponent)
    {
        tools::unusedArg(unit, allies, opponent);

        if (m_set.size() == 0)
            return Point(0.0f, 0.0f);

        auto sum = std::accumulate(m_set.begin(), m_set.end(), Point(0.0f, 0.0f), [](const Point& p, const UnitPtr& u)
        {
            return p + u->getPosition();
        });

        return sum / static_cast<float>(m_set.size());
    }

protected:
    UnitVector m_set;
};

class UnitPositionExtractor : public PointExtractor
{
public:
    UnitPositionExtractor(const UnitPtr& u)
        : PointExtractor(),
        m_unit(u)
    {}

    virtual Point operator()(const UnitPtr& unit, const ArmyPtr& allies, const ArmyPtr& opponent)
    {
        tools::unusedArg(unit, allies, opponent);

        return m_unit->getPosition();
    }

protected:
    UnitPtr m_unit;
};


bool unitTest_BaryCenterExtractor()
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

    auto baryCenter = (uP + uP2 + uP3) / 3.0f;

    BaryCenterExtractor bcE(set);

    if (baryCenter != bcE(u, a, b))
        return false;

    return true;
}

bool unitTest_UnitPositionExtractor()
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

    UnitPositionExtractor uPE(u2);

    if (uP2 != uPE(u, a, b))
        return false;

    return true;
}

bool unitTest_PointExtractor()
{
    if (!unitTest_BaryCenterExtractor() ||
        !unitTest_UnitPositionExtractor())
        return false;

    return true;
}

#endif //_POINTEXTRACTOR_H_