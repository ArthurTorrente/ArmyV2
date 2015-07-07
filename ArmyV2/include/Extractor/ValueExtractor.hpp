#ifndef _VALUEEXTRACTOR_H
#define _VALUEEXTRACTOR_H

#include <algorithm>
#include <numeric>

#include "IExtractor.hpp"

/**
 * Predefine value extractor
 *
 * Return the predefine value
 */
class PreDefineValueExtractor : public FloatExtractor
{
public:
    PreDefineValueExtractor(float value = 0.0f)
        : FloatExtractor(),
        m_value(value)
    {}

    virtual float operator()(const UnitPtr& unit, const ArmyPtr& allies, const ArmyPtr& opponent)
    {
        tools::unusedArg(unit, allies, opponent);

        return m_value;
    }

    float getValue() const
    {
        return m_value;
    }

    void setValue(float value)
    {
        m_value = value;
    }

protected:
    float m_value;
};

class CapacityValueExtractor : public FloatExtractor
{
public:
    CapacityValueExtractor(unsigned int capacityIndex = 0)
        : FloatExtractor(),
        m_capacityIndex(capacityIndex)
    {}

    virtual float operator()(const UnitPtr& unit, const ArmyPtr& allies, const ArmyPtr& opponent)
    {
        tools::unusedArg(allies, opponent);

        return unit->getCapacity(m_capacityIndex)->getValue();
    }

    unsigned int getCapacityIndex() const
    {
        return m_capacityIndex;
    }

    void setCapacityIndex(unsigned int c)
    {
        m_capacityIndex = c;
    }

protected:
    unsigned int m_capacityIndex;
};

class DistanceExtractor : public FloatExtractor
{
public:
    DistanceExtractor(const Point& p)
        : FloatExtractor(),
        m_point(p)
    {}

    virtual float operator()(const UnitPtr& unit, const ArmyPtr& allies, const ArmyPtr& opponent)
    {
        tools::unusedArg(allies, opponent);

        return unit->getPosition().distance(m_point);
    }

    const Point& getPoint() const
    {
        return m_point;
    }

    void setPoint(const Point& p)
    {
        m_point = p;
    }

protected:
    Point m_point;
};

class MinMaxAverageCapacityValueExtractor : public FloatExtractor
{
public:
    enum Type
    {
        MIN,
        MAX,
        AVERAGE
    };

    MinMaxAverageCapacityValueExtractor(Type type, unsigned int capacityIndex = 0, const UnitVector& set = UnitVector())
        : FloatExtractor(),
        m_type(type),
        m_capacityIndex(capacityIndex),
        m_set(set)
    {}

    virtual float operator()(const UnitPtr& unit, const ArmyPtr& allies, const ArmyPtr& opponent)
    {
        tools::unusedArg(unit, allies, opponent);

        float value = 0.0f;

        switch (m_type)
        {
        case MIN:
            value = getMinValue();
            break;

        case MAX:
            value = getMaxValue();
            break;

        case AVERAGE:
            value = getAverageValue();
            break;
        }

        return value;
    }

    Type getType() const
    {
        return m_type;
    }

    unsigned int getCapacityValue() const
    {
        return m_capacityIndex;
    }

    const UnitVector& getSet() const
    {
        return m_set;
    }

    void setType(Type type)
    {
        m_type = type;
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
    Type m_type;
    unsigned int m_capacityIndex;
    UnitVector m_set;

    float getMinValue()
    {
        return (*std::min_element(m_set.begin(), m_set.end(), [&](const UnitPtr& a, const UnitPtr& b)
        {
            return a->getCapacity(m_capacityIndex)->getValue() < b->getCapacity(m_capacityIndex)->getValue();
        }))->getCapacity(m_capacityIndex)->getValue();
    }

    float getMaxValue()
    {
        return (*std::max_element(m_set.begin(), m_set.end(), [&](const UnitPtr& a, const UnitPtr& b)
        {
            return a->getCapacity(m_capacityIndex)->getValue() < b->getCapacity(m_capacityIndex)->getValue();
        }))->getCapacity(m_capacityIndex)->getValue();
    }

    float getAverageValue()
    {
        if (m_set.size() == 0)
            return 0.0f;

        return std::accumulate(m_set.begin(), m_set.end(), 0.0f, [&](const float& v, const UnitPtr& unit)
        {
            return v + unit->getCapacity(m_capacityIndex)->getValue();
        }) / static_cast<float>(m_set.size());
    }
};

class MinMaxAverageDistanceValueExtractor : public FloatExtractor
{
public:
    enum Type
    {
        MIN,
        MAX,
        AVERAGE
    };

    MinMaxAverageDistanceValueExtractor(Type type, const Point& p, const UnitVector& set = UnitVector())
        : FloatExtractor(),
        m_type(type),
        m_point(p),
        m_set(set)
    {}

    virtual float operator()(const UnitPtr& unit, const ArmyPtr& allies, const ArmyPtr& opponent)
    {
        tools::unusedArg(unit, allies, opponent);

        float value = 0.0f;

        switch (m_type)
        {
        case MIN:
            value = getMinValue();
            break;

        case MAX:
            value = getMaxValue();
            break;

        case AVERAGE:
            value = getAverageValue();
            break;
        }

        return value;
    }

    Type getType() const
    {
        return m_type;
    }

    const Point& getPoint() const
    {
        return m_point;
    }

    const UnitVector& getSet() const
    {
        return m_set;
    }

    void setType(Type type)
    {
        m_type = type;
    }

    void setPoint(const Point& p)
    {
        m_point = p;
    }

    void setSet(const UnitVector& set)
    {
        m_set = set;
    }

protected:
    Type m_type;
    Point m_point;
    UnitVector m_set;

    float getMinValue()
    {
        return (*std::min_element(m_set.begin(), m_set.end(), [&](const UnitPtr& a, const UnitPtr& b)
        {
            return a->getPosition().distance(m_point) < b->getPosition().distance(m_point);
        }))->getPosition().distance(m_point);
    }

    float getMaxValue()
    {
        return (*std::max_element(m_set.begin(), m_set.end(), [&](const UnitPtr& a, const UnitPtr& b)
        {
            return a->getPosition().distance(m_point) < b->getPosition().distance(m_point);
        }))->getPosition().distance(m_point);
    }

    float getAverageValue()
    {
        if (m_set.size() == 0)
            return 0.0f;

        return std::accumulate(m_set.begin(), m_set.end(), 0.0f, [&](const float& v, const UnitPtr& unit)
        {
            return v + unit->getPosition().distance(m_point);
        }) / static_cast<float>(m_set.size());
    }
};

bool unitTest_predefineExtractor()
{
    float predefine = 10.0f;

    PreDefineValueExtractor e(predefine);
    UnitPtr u(new Unit(100));
    ArmyPtr a(new Army(10, 200)), b(new Army(10, 200));
    auto result = e(u, a, b);

    if (result != predefine)
        return false;

    return true;
}

bool unitTest_CapacityValueExtractor()
{
    UnitPtr u(new Unit(100));
    ArmyPtr a(new Army(10, 200)), b(new Army(10, 200));

    auto capacityValue = u->getCapacity(0)->getValue();

    CapacityValueExtractor ce(0);

    auto result = ce(u, a, b);

    if (result != capacityValue)
        return false;

    return true;
}

bool unitTest_DistanceExtractor()
{
    UnitPtr u(new Unit(100));
    ArmyPtr a(new Army(10, 200)), b(new Army(10, 200));

    Point ori(0.0f, 0.0f), uP(10.0f, 10.0f);
    float distance = ori.distance(uP);
    u->setPosition(uP);

    DistanceExtractor d(ori);

    float result = d(u, a, b);

    if (result != distance)
        return false;

    return true;
}

bool unitTest_MinMaxAverageCapacityValueExtracteur()
{
    UnitPtr u(new Unit(0)), u2(new Unit(0)), u3(new Unit(100));
    ArmyPtr a(new Army(10, 200)), b(new Army(10, 200));

    UnitVector set;

    set.push_back(u);
    set.push_back(u2);
    set.push_back(u3);

    /* MAX */
    auto capacityMax = u3->getCapacity(0)->getValue();
    MinMaxAverageCapacityValueExtractor mCMax(MinMaxAverageCapacityValueExtractor::MAX, 0, set);

    auto result = mCMax(u, a, b);

    if (result != capacityMax)
        return false;

    /* MIN */
    u2->getCapacity(0)->upgrade();
    u2->getCapacity(0)->upgrade();
    u3->getCapacity(0)->upgrade();
    u3->getCapacity(0)->upgrade();
    auto capacityMin = u->getCapacity(0)->getValue();
    MinMaxAverageCapacityValueExtractor mCMin(MinMaxAverageCapacityValueExtractor::MIN, 0, set);

    result = mCMin(u, a, b);

    if (result != capacityMin)
        return false;

    /* AVERAGE */
    auto capacityAverage = u->getCapacity(0)->getValue() + u2->getCapacity(0)->getValue() + u3->getCapacity(0)->getValue();
    capacityAverage /= 3.0f;

    MinMaxAverageCapacityValueExtractor mCAverage(MinMaxAverageCapacityValueExtractor::AVERAGE, 0, set);
    result = mCAverage(u, a, b);

    if (result != capacityAverage)
        return false;

    return true;
}

bool unitTest_MinMaxAverageDistanceValueExtractor()
{
    UnitPtr u(new Unit(0)), u2(new Unit(0)), u3(new Unit(100));
    ArmyPtr a(new Army(10, 200)), b(new Army(10, 200));

    Point ori(0.0f, 0.0f), uP(1.0f, 1.0f), u2P(2.0f, 2.0f), u3P(3.0f, 3.0f);
    
    u->setPosition(uP);
    u2->setPosition(u2P);
    u3->setPosition(u3P);

    UnitVector set;

    set.push_back(u);
    set.push_back(u2);
    set.push_back(u3);

    /* MIN */
    auto minDist = ori.distance(uP);
    MinMaxAverageDistanceValueExtractor mDMin(MinMaxAverageDistanceValueExtractor::MIN, ori, set);

    auto result = mDMin(u, a, b);

    if (result != minDist)
        return false;

    /* MAX */
    auto maxDist = ori.distance(u3P);
    MinMaxAverageDistanceValueExtractor mDMax(MinMaxAverageDistanceValueExtractor::MAX, ori, set);

    result = mDMax(u, a, b);

    if (result != maxDist)
        return false;

    /* AVERAGE */
    auto distAverage = ori.distance(uP) + ori.distance(u2P) + ori.distance(u3P);
    distAverage /= 3.0f;

    MinMaxAverageDistanceValueExtractor mCAverage(MinMaxAverageDistanceValueExtractor::AVERAGE, ori, set);
    result = mCAverage(u, a, b);

    if (result != distAverage)
        return false;

    return true;
}


bool unitTest_ValueExtractor()
{

    if (!unitTest_predefineExtractor() ||
        !unitTest_CapacityValueExtractor() ||
        !unitTest_DistanceExtractor() ||
        !unitTest_MinMaxAverageCapacityValueExtracteur() ||
        !unitTest_MinMaxAverageDistanceValueExtractor())
        return false;

    return true;
}

#endif //_VALUEEXTRACTOR_H