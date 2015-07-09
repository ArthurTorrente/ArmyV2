#ifndef _VALUEEXTRACTOR_H
#define _VALUEEXTRACTOR_H

#include <algorithm>
#include <numeric>
#include <functional>

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

    virtual float operator()(const UnitSPtr& unit, const ArmyPtr& allies, const ArmyPtr& opponent)
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
    CapacityValueExtractor(unsigned int capacityIndex, UnitExtractorUPtr& uex)
        : FloatExtractor(),
        m_capacityIndex(capacityIndex),
        m_unitExtractor(std::move(uex))
    {}

    virtual float operator()(const UnitSPtr& unit, const ArmyPtr& allies, const ArmyPtr& opponent)
    {
        const UnitSPtr& u = (*m_unitExtractor)(unit, allies, opponent);

        if (u)
            return u->getCapacity(m_capacityIndex)->getValue();

        return 0.0f;
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
    UnitExtractorUPtr m_unitExtractor;
};

class DistanceExtractor : public FloatExtractor
{
public:
    DistanceExtractor(PointExtractorUPtr& pex, UnitExtractorUPtr& uex)
        : FloatExtractor(),
        m_pointExtractor(std::move(pex)),
        m_unitExtractor(std::move(uex))
    {}

    virtual float operator()(const UnitSPtr& unit, const ArmyPtr& allies, const ArmyPtr& opponent)
    {
        const Point& point = (*m_pointExtractor)(unit, allies, opponent);
        const UnitSPtr& u = (*m_unitExtractor)(unit, allies, opponent);

        if (u)
            return u->getPosition().distance(point);

        return 0.0f;
    }

protected:
    PointExtractorUPtr m_pointExtractor;
    UnitExtractorUPtr m_unitExtractor;
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

    MinMaxAverageCapacityValueExtractor(Type type, unsigned int capacityIndex, SetExtractorUPtr& sex)
        : FloatExtractor(),
        m_capacityIndex(capacityIndex),
        m_setExtractor(std::move(sex))
    {
        if (type == Type::MIN)
            m_algo = std::bind(&MinMaxAverageCapacityValueExtractor::getMinValue, this, std::placeholders::_1);
        
        else if (type == Type::MAX)
            m_algo = std::bind(&MinMaxAverageCapacityValueExtractor::getMaxValue, this, std::placeholders::_1);

        else
            m_algo = std::bind(&MinMaxAverageCapacityValueExtractor::getAverageValue, this, std::placeholders::_1);
    }

    virtual float operator()(const UnitSPtr& unit, const ArmyPtr& allies, const ArmyPtr& opponent)
    {
        return m_algo((*m_setExtractor)(unit, allies, opponent));
    }

    unsigned int getCapacityValue() const
    {
        return m_capacityIndex;
    }

    void setType(Type type)
    {
        if (type == Type::MIN)
            m_algo = std::bind(&MinMaxAverageCapacityValueExtractor::getMinValue, this, std::placeholders::_1);

        else if (type == Type::MAX)
            m_algo = std::bind(&MinMaxAverageCapacityValueExtractor::getMaxValue, this, std::placeholders::_1);

        else
            m_algo = std::bind(&MinMaxAverageCapacityValueExtractor::getAverageValue, this, std::placeholders::_1);
    }

    void setCapacityIndex(unsigned int capacityIndex)
    {
        m_capacityIndex = capacityIndex;
    }

protected:
    unsigned int m_capacityIndex;
    SetExtractorUPtr m_setExtractor;

    std::function<float(const UnitVector&)> m_algo;
    
    float getMinValue(const UnitVector& set)
    {
        if (set.size() == 0)
            return 0.0f;

        return (*std::min_element(set.begin(), set.end(), [&](const UnitSPtr& a, const UnitSPtr& b)
        {
            return a->getCapacity(m_capacityIndex)->getValue() < b->getCapacity(m_capacityIndex)->getValue();
        }))->getCapacity(m_capacityIndex)->getValue();
    }

    float getMaxValue(const UnitVector& set)
    {
        if (set.size() == 0)
            return 0.0f;

        return (*std::max_element(set.begin(), set.end(), [&](const UnitSPtr& a, const UnitSPtr& b)
        {
            return a->getCapacity(m_capacityIndex)->getValue() < b->getCapacity(m_capacityIndex)->getValue();
        }))->getCapacity(m_capacityIndex)->getValue();
    }

    float getAverageValue(const UnitVector& set)
    {
        if (set.size() == 0)
            return 0.0f;

        return std::accumulate(set.begin(), set.end(), 0.0f, [&](const float& v, const UnitSPtr& unit)
        {
            return v + unit->getCapacity(m_capacityIndex)->getValue();
        }) / static_cast<float>(set.size());
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

    MinMaxAverageDistanceValueExtractor(Type type, SetExtractorUPtr& sex, PointExtractorUPtr& pex)
        : FloatExtractor(),
        m_setExtractor(std::move(sex)),
        m_pointExtractor(std::move(pex))
    {
        switch (type)
        {
        case MIN:
            m_algo = std::bind(&MinMaxAverageDistanceValueExtractor::getMinValue, this, std::placeholders::_1, std::placeholders::_2);
            break;

        case MAX:
            m_algo = std::bind(&MinMaxAverageDistanceValueExtractor::getMaxValue, this, std::placeholders::_1, std::placeholders::_2);
            break;

        case AVERAGE:
            m_algo = std::bind(&MinMaxAverageDistanceValueExtractor::getAverageValue, this, std::placeholders::_1, std::placeholders::_2);
            break;
        }
    }

    virtual float operator()(const UnitSPtr& unit, const ArmyPtr& allies, const ArmyPtr& opponent)
    {
        return m_algo(
            (*m_setExtractor)(unit, allies, opponent),
            (*m_pointExtractor)(unit, allies, opponent)
            );
    }

    void setType(Type type)
    {
        switch (type)
        {
        case MIN:
            m_algo = std::bind(&MinMaxAverageDistanceValueExtractor::getMinValue, this, std::placeholders::_1, std::placeholders::_2);
            break;

        case MAX:
            m_algo = std::bind(&MinMaxAverageDistanceValueExtractor::getMaxValue, this, std::placeholders::_1, std::placeholders::_2);
            break;

        case AVERAGE:
            m_algo = std::bind(&MinMaxAverageDistanceValueExtractor::getAverageValue, this, std::placeholders::_1, std::placeholders::_2);
            break;
        }
    }

protected:
    PointExtractorUPtr m_pointExtractor;
    SetExtractorUPtr m_setExtractor;

    std::function<float(const UnitVector&, const Point&)> m_algo;

    float getMinValue(const UnitVector& set, const Point& p)
    {
        if (set.size() == 0)
            return 0.0f;

        return (*std::min_element(set.begin(), set.end(), [&](const UnitSPtr& a, const UnitSPtr& b)
        {
            return a->getPosition().distance(p) < b->getPosition().distance(p);
        }))->getPosition().distance(p);
    }

    float getMaxValue(const UnitVector& set, const Point& p)
    {
        if (set.size() == 0)
            return 0.0f;

        return (*std::max_element(set.begin(), set.end(), [&](const UnitSPtr& a, const UnitSPtr& b)
        {
            return a->getPosition().distance(p) < b->getPosition().distance(p);
        }))->getPosition().distance(p);
    }

    float getAverageValue(const UnitVector& set, const Point& p)
    {
        if (set.size() == 0)
            return 0.0f;

        return std::accumulate(set.begin(), set.end(), 0.0f, [&](const float& v, const UnitSPtr& unit)
        {
            return v + unit->getPosition().distance(p);
        }) / static_cast<float>(set.size());
    }
};

#endif //_VALUEEXTRACTOR_H