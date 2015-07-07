#ifndef _SETEXTRACTOR_H_
#define _SETEXTRACTOR_H_

#include <numeric>
#include <algorithm>

#include "IExtractor.hpp"

class AlliesArmyExtractor : public SetExtractor
{
public:
    AlliesArmyExtractor()
        : SetExtractor()
    {}

    virtual UnitVector operator()(const UnitPtr& unit, const ArmyPtr& allies, const ArmyPtr& opponent)
    {
        tools::unusedArg(unit, opponent);

        return allies->getUnitsList();
    }
};

class OpponentArmyExtractor : public SetExtractor
{
public:
    OpponentArmyExtractor()
        : SetExtractor()
    {}

    virtual UnitVector operator()(const UnitPtr& unit, const ArmyPtr& allies, const ArmyPtr& opponent)
    {
        tools::unusedArg(unit, allies);

        return opponent->getUnitsList();
    }
};

class NMinMaxCapacityExtractor : public SetExtractor
{
public:
    enum Type
    {
        MIN,
        MAX
    };

    NMinMaxCapacityExtractor(Type type, unsigned int capacityIndex, unsigned int N, const UnitVector& set = UnitVector())
        : SetExtractor(),
        m_type(type),
        m_capacityIndex(capacityIndex),
        m_n(N),
        m_set(set)
    {}

    virtual UnitVector operator()(const UnitPtr& unit, const ArmyPtr& allies, const ArmyPtr& opponent)
    {
        tools::unusedArg(unit, allies, opponent);

        if (m_type == Type::MIN)
        {
            return getMin();
        }

        return getMax();
    }

    Type getType() const
    {
        return m_type;
    }

    unsigned int getCapacityIndex() const
    {
        return m_capacityIndex;
    }

    unsigned int getN() const
    {
        return m_n;
    }

    const UnitVector& getSet() const
    {
        return m_set;
    }

    void setType(Type type)
    {
        m_type = type;
    }

    void setCapacityIndex(unsigned int cI)
    {
        m_capacityIndex = cI;
    }

    void setN(unsigned int N)
    {
        m_n = N;
    }

    void setSet(const UnitVector& set)
    {
        m_set = set;
    }
protected:
    Type m_type;
    unsigned int m_capacityIndex;
    unsigned int m_n;
    UnitVector m_set;

    UnitVector getMin()
    {
        UnitVector set(m_set);

        if (set.size() == 0)
            return set;

        std::sort(set.begin(), set.end(), [&](const UnitPtr& a, const UnitPtr& b)
        {
            return a->getCapacity(m_capacityIndex)->getValue() < b->getCapacity(m_capacityIndex)->getValue();
        });

        return UnitVector(set.begin(), set.begin() + (set.size() > m_n ? m_n : set.size()));
    }

    UnitVector getMax()
    {
        UnitVector set(m_set);

        if (set.size() == 0)
            return set;

        std::sort(set.begin(), set.end(), [&](const UnitPtr& a, const UnitPtr& b)
        {
            return a->getCapacity(m_capacityIndex)->getValue() > b->getCapacity(m_capacityIndex)->getValue();
        });

        return UnitVector(set.begin(), set.begin() + (set.size() > m_n ? m_n : set.size()));
    }
};

class NFarNearExtractor : public SetExtractor
{
public:
    NFarNearExtractor(bool isFar, const Point& p, unsigned int N, const UnitVector& set = UnitVector())
        : SetExtractor(),
        m_isFar(isFar),
        m_p(p),
        m_n(N),
        m_set(set)
    {}

    virtual UnitVector operator()(const UnitPtr& unit, const ArmyPtr& allies, const ArmyPtr& opponent)
    {
        tools::unusedArg(unit, allies, opponent);

        if (m_isFar)
        {
            return getFar();
        }

        return getNear();
    }

    bool isFar() const
    {
        return m_isFar;
    }

    const Point& getPoint() const
    {
        return m_p;
    }

    unsigned int getN() const
    {
        return m_n;
    }

    const UnitVector& getSet() const
    {
        return m_set;
    }

    void setIsFar(bool isFar)
    {
        m_isFar = isFar;
    }

    void setPoint(const Point& p)
    {
        m_p = p;
    }

    void setN(unsigned int N)
    {
        m_n = N;
    }

    void setSet(const UnitVector& set)
    {
        m_set = set;
    }

protected:
    bool m_isFar;
    Point m_p;
    unsigned int m_n;
    UnitVector m_set;

    UnitVector getFar()
    {
        UnitVector set(m_set);

        if (set.size() == 0)
            return set;

        std::sort(set.begin(), set.end(), [&](const UnitPtr& a, const UnitPtr& b)
        {
            return a->getPosition().distance(m_p) > b->getPosition().distance(m_p);
        });

        return UnitVector(set.begin(), set.begin() + (set.size() > m_n ? m_n : set.size()));
    }

    UnitVector getNear()
    {
        UnitVector set(m_set);

        if (set.size() == 0)
            return set;

        std::sort(set.begin(), set.end(), [&](const UnitPtr& a, const UnitPtr& b)
        {
            return b->getPosition().distance(m_p) > a->getPosition().distance(m_p);
        });

        return UnitVector(set.begin(), set.begin() + (set.size() > m_n ? m_n : set.size()));
    }
};

class ThresholdCapacityDistanceExtractor : public SetExtractor
{
public:
    ThresholdCapacityDistanceExtractor(const Point& p, bool isMin, float threshold, unsigned int N, const UnitVector& set = UnitVector())
        : SetExtractor(),
        m_isDistance(true),
        m_isMin(isMin),
        m_threshold(threshold),
        m_n(N),
        m_set(set),
        m_p(p),
        m_capacityIndex(0)
    {}

    ThresholdCapacityDistanceExtractor(unsigned int capacityIndex, bool isMin, float threshold, unsigned int N, const UnitVector& set = UnitVector())
        : SetExtractor(),
        m_isDistance(false),
        m_isMin(isMin),
        m_threshold(threshold),
        m_n(N),
        m_set(set),
        m_capacityIndex(capacityIndex)
    {}

    virtual UnitVector operator()(const UnitPtr& unit, const ArmyPtr& allies, const ArmyPtr& opponent)
    {
        tools::unusedArg(unit, allies, opponent);

        if (m_isMin)
        {
            if (m_isDistance)
                return getNear();

            return getMin();
        }

        if (m_isDistance)
            return getFar();

        return getMax();
    }

    bool isDistance() const
    {
        return m_isDistance;
    }

    bool isMin() const
    {
        return m_isMin;
    }

    float getTreshold() const
    {
        return m_threshold;
    }

    const UnitVector& getSet() const
    {
        return m_set;
    }

    const Point& getPoint() const
    {
        return m_p;
    }

    unsigned int getCapacityIndex() const
    {
        return m_capacityIndex;
    }

    void setIsDitance(bool isDistance)
    {
        m_isDistance = isDistance;
    }

    void setIsMin(bool isMin)
    {
        m_isMin = isMin;
    }
    
    void setThreshold(float threshold)
    {
        m_threshold = threshold;
    }

    void setSet(const UnitVector& set)
    {
        m_set = set;
    }

    void setPoint(const Point& p)
    {
        m_p = p;
    }

    void setCapacityIndex(unsigned int cI)
    {
        m_capacityIndex = cI;
    }

protected:
    bool m_isDistance;
    bool m_isMin;
    float m_threshold;
    unsigned int m_n;
    UnitVector m_set;
    unsigned int m_capacityIndex;
    Point m_p;

    UnitVector getMin()
    {
        UnitVector set(m_set);

        if (m_set.size() == 0)
            return set;

        set.erase(
            std::remove_if(set.begin(), set.end(), [&](const UnitPtr& u)
        {
            return u->getCapacity(m_capacityIndex)->getValue() > m_threshold;
        }), set.end());

        if (set.size() > m_n)
        {
            set.erase(set.begin() + m_n, set.end());
        }

        return set;
    }

    UnitVector getMax()
    {
        UnitVector set(m_set);

        if (m_set.size() == 0)
            return set;

        set.erase(
            std::remove_if(set.begin(), set.end(), [&](const UnitPtr& u)
        {
            return u->getCapacity(m_capacityIndex)->getValue() < m_threshold;
        }), set.end());

        if (set.size() > m_n)
        {
            set.erase(set.begin() + m_n, set.end());
        }

        return set;
    }

    UnitVector getNear()
    {
        UnitVector set(m_set);

        if (m_set.size() == 0)
            return set;

        set.erase(
            std::remove_if(set.begin(), set.end(), [&](const UnitPtr& u)
        {
            return u->getPosition().distance(m_p) > m_threshold;
        }), set.end());

        if (set.size() > m_n)
        {
            set.erase(set.begin() + m_n, set.end());
        }

        return set;
    }

    UnitVector getFar()
    {
        UnitVector set(m_set);

        if (m_set.size() == 0)
            return set;

        set.erase(
            std::remove_if(set.begin(), set.end(), [&](const UnitPtr& u)
        {
            return u->getPosition().distance(m_p) < m_threshold;
        }), set.end());

        if (set.size() > m_n)
        {
            set.erase(set.begin() + m_n, set.end());
        }

        return set;
    }
};

bool unitTest_AlliesArmyExtractor()
{
    UnitPtr u(new Unit(0));
    ArmyPtr a(new Army(10, 200)), b(new Army(20, 200));

    AlliesArmyExtractor aE;

    if (aE(u, a, b).size() != 10)
        return false;

    return true;
}

bool unitTest_OpponentArmyExtractor()
{
    UnitPtr u(new Unit(0));
    ArmyPtr a(new Army(10, 200)), b(new Army(20, 200));

    OpponentArmyExtractor oE;

    if (oE(u, a, b).size() != 20)
        return false;
    
    return true;
}

bool unitTest_NMinMaxCapacityExtractor()
{
    UnitPtr u(new Unit(0)), u2(new Unit(0)), u3(new Unit(0));
    ArmyPtr a(new Army(10, 200)), b(new Army(10, 200));

    UnitVector set;
    set.push_back(u);
    set.push_back(u2);
    set.push_back(u3);

    u->getCapacity(0)->upgrade();

    NMinMaxCapacityExtractor nmmcEMax(NMinMaxCapacityExtractor::MAX, 0, 1, set);

    auto result = nmmcEMax(u, a, b);

    if (result.size() != 1 || result.front() != u)
        return false;

    NMinMaxCapacityExtractor nmmcEMaxOut(NMinMaxCapacityExtractor::MAX, 0, 4, set);

    result = nmmcEMaxOut(u, a, b);

    if (result.size() != 3)
        return false;

    u2->getCapacity(0)->upgrade();
    NMinMaxCapacityExtractor nmmcEMin(NMinMaxCapacityExtractor::MIN, 0, 1, set);
    
    result = nmmcEMin(u, a, b);

    if (result.size() != 1 || result.front() != u3)
        return false;

    NMinMaxCapacityExtractor nmmcEMinOut(NMinMaxCapacityExtractor::MIN, 0, 4, set);
    result = nmmcEMinOut(u, a, b);

    if (result.size() != 3)
        return false;

    return true;
}

bool unitTest_NFarNearExtractor()
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

    NFarNearExtractor nfnEFar(true, ori, 1, set);

    auto result = nfnEFar(u, a, b);

    if (result.size() != 1 || result.front() != u3)
        return false;

    NFarNearExtractor nfnEFarOut(true, ori, 4, set);

    result = nfnEFarOut(u, a, b);

    if (result.size() != 3)
        return false;

    NFarNearExtractor nfnENear(false, ori, 1, set);

    result = nfnENear(u, a, b);

    if (result.size() != 1 || result.front() != u)
        return false;

    NFarNearExtractor nfnENearOut(false, ori, 4, set);
    result = nfnENearOut(u, a, b);

    if (result.size() != 3)
        return false;

    return true;
}

bool unitTest_ThresholdCapacityDistanceExtractor()
{
    float distThreshold = 2.0f;
    float capacityThreshold = 5.0f;

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

    ThresholdCapacityDistanceExtractor tcECapMin(0, true, capacityThreshold, 1, set);
    u2->getCapacity(0)->upgrade(6);
    
    auto result = tcECapMin(u, a, b);

    if (result.front() != u)
        return false;

    ThresholdCapacityDistanceExtractor tcECapMinOut(0, true, capacityThreshold, 4, set);

    result = tcECapMinOut(u, a, b);

    if (result.size() > 3)
        return false;


    ThresholdCapacityDistanceExtractor tcECapMax(0, false, capacityThreshold, 1, set);

    result = tcECapMax(u, a, b);

    if (result.front() != u2)
        return false;


    ThresholdCapacityDistanceExtractor tcECapMaxOut(0, false, capacityThreshold, 4, set);

    result = tcECapMaxOut(u, a, b);

    if (result.size() > 3)
        return false;

    ThresholdCapacityDistanceExtractor tcEDistMin(ori, true, distThreshold, 1, set);

    result = tcEDistMin(u, a, b);

    if (result.front() != u)
        return false;


    ThresholdCapacityDistanceExtractor tcEDistMinOut(ori, true, distThreshold, 4, set);

    result = tcEDistMinOut(u, a, b);

    if (result.size() > 3)
        return false;


    ThresholdCapacityDistanceExtractor tcEDistMax(ori, false, distThreshold, 1, set);

    result = tcEDistMax(u, a, b);

    if (result.front() != u2)
        return false;


    ThresholdCapacityDistanceExtractor tcEDistMaxOut(ori, false, distThreshold, 4, set);

    result = tcEDistMaxOut(u, a, b);

    if (result.size() > 3)
        return false;

    return true;
}

/*
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
*/

bool unitTest_SetExtractor()
{
    if (!unitTest_AlliesArmyExtractor() ||
        !unitTest_OpponentArmyExtractor() ||
        !unitTest_NMinMaxCapacityExtractor() ||
        !unitTest_NFarNearExtractor() ||
        !unitTest_ThresholdCapacityDistanceExtractor())
        return false;
    
    return true;
}

#endif //_SETEXTRACTOR_H_