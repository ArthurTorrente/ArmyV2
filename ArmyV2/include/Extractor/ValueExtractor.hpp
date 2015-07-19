#ifndef _VALUEEXTRACTOR_H
#define _VALUEEXTRACTOR_H

#pragma warning(push, 0)
#include <algorithm>
#include <numeric>
#include <functional>
#include <sstream>
#pragma warning(pop)

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

    float operator()(Unit& unit, Army& allies, Army& opponent)
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

    std::string getCode() const
    {
        std::stringstream ss;

        ss << "V";
        ss << std::setprecision(4) << std::fixed;
        ss << m_value;

        return ss.str();
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

    float operator()(Unit& unit, Army& allies, Army& opponent)
    {
        Unit& u = (*m_unitExtractor)(unit, allies, opponent);

        return u.getCapacity(m_capacityIndex)->getValue();
    }

    unsigned int getCapacityIndex() const
    {
        return m_capacityIndex;
    }

    void setCapacityIndex(unsigned int c)
    {
        m_capacityIndex = c;
    }

    std::string getCode() const
    {
        std::string code("C");

        code += '0' + static_cast<char>(m_capacityIndex);
        code += m_unitExtractor->getCode();

        return code;
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

    float operator()(Unit& unit, Army& allies, Army& opponent)
    {
        const Point& point = (*m_pointExtractor)(unit, allies, opponent);
        Unit& u = (*m_unitExtractor)(unit, allies, opponent);

        return u.getPosition().distance(point);
    }

    std::string getCode() const
    {
        std::string code("D");

        code += m_unitExtractor->getCode();
        code += m_pointExtractor->getCode();

        return code;
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
        m_type(type),
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

    float operator()(Unit& unit, Army& allies, Army& opponent)
    {
        return m_algo((*m_setExtractor)(unit, allies, opponent));
    }

    unsigned int getCapacityValue() const
    {
        return m_capacityIndex;
    }

    void setType(Type type)
    {
        m_type = type;

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

    std::string getCode() const
    {
        std::string code(
            m_type == MAX ? "M" :
            m_type == MIN ? "m" : "a"
            );

        code += '0' + static_cast<char>(m_capacityIndex);
        code += m_setExtractor->getCode();

        return code;
    }

protected:
    Type m_type;
    unsigned int m_capacityIndex;
    SetExtractorUPtr m_setExtractor;

    std::function<float(const UnitSPtrVector&)> m_algo;

    float getMinValue(const UnitSPtrVector& set)
    {
        if (set.size() == 0)
            throw std::invalid_argument("Unit vector is empty");

        return (*std::min_element(set.begin(), set.end(), [&](const UnitSPtr& a, const UnitSPtr& b)
        {
            return a->getCapacity(m_capacityIndex)->getValue() < b->getCapacity(m_capacityIndex)->getValue();
        }))->getCapacity(m_capacityIndex)->getValue();
    }

    float getMaxValue(const UnitSPtrVector& set)
    {
        if (set.size() == 0)
            throw std::invalid_argument("Unit vector is empty");

        return (*std::max_element(set.begin(), set.end(), [&](const UnitSPtr& a, const UnitSPtr& b)
        {
            return a->getCapacity(m_capacityIndex)->getValue() < b->getCapacity(m_capacityIndex)->getValue();
        }))->getCapacity(m_capacityIndex)->getValue();
    }

    float getAverageValue(const UnitSPtrVector& set)
    {
        if (set.size() == 0)
            throw std::invalid_argument("Unit vector is empty");

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
        m_type(type),
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

    float operator()(Unit& unit, Army& allies, Army& opponent)
    {
        return m_algo(
            (*m_setExtractor)(unit, allies, opponent),
            (*m_pointExtractor)(unit, allies, opponent)
            );
    }

    void setType(Type type)
    {
        m_type = type;

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

    std::string getCode() const
    {
        std::string code(
            m_type == MAX ? "MD" :
            m_type == MIN ? "mD" : "aD"
            );

        code += m_setExtractor->getCode();
        code += m_pointExtractor->getCode();
        return code;
    }

protected:
    Type m_type;
    PointExtractorUPtr m_pointExtractor;
    SetExtractorUPtr m_setExtractor;

    std::function<float(const UnitSPtrVector&, const Point&)> m_algo;

    float getMinValue(const UnitSPtrVector& set, const Point& p)
    {
        if (set.size() == 0)
            throw std::invalid_argument("Unit vector is empty");

        return (*std::min_element(set.begin(), set.end(), [&](const UnitSPtr& a, const UnitSPtr& b)
        {
            return a->getPosition().distance(p) < b->getPosition().distance(p);
        }))->getPosition().distance(p);
    }

    float getMaxValue(const UnitSPtrVector& set, const Point& p)
    {
        if (set.size() == 0)
            throw std::invalid_argument("Unit vector is empty");

        return (*std::max_element(set.begin(), set.end(), [&](const UnitSPtr& a, const UnitSPtr& b)
        {
            return a->getPosition().distance(p) < b->getPosition().distance(p);
        }))->getPosition().distance(p);
    }

    float getAverageValue(const UnitSPtrVector& set, const Point& p)
    {
        if (set.size() == 0)
            throw std::invalid_argument("Unit vector is empty");

        return std::accumulate(set.begin(), set.end(), 0.0f, [&](const float& v, const UnitSPtr& unit)
        {
            return v + unit->getPosition().distance(p);
        }) / static_cast<float>(set.size());
    }
};

#endif //_VALUEEXTRACTOR_H