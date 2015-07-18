#ifndef _POINTEXTRACTOR_H_
#define _POINTEXTRACTOR_H_

#pragma warning(push, 0)
#include <numeric>
#pragma warning(pop)

#include "IExtractor.hpp"

/**
 * This file contain all Extrator which return a Point value
 */


/**
 * Barycenter takes a set extrator and returns the average of positions of all unit contained into the set
 */
class BaryCenterExtractor : public PointExtractor
{
public:
    BaryCenterExtractor(SetExtractorUPtr& extractor)
        : PointExtractor(),
        m_getter(std::move(extractor))
    {}

    Point operator()(const UnitSPtr& unit, const ArmySPtr& allies, const ArmySPtr& opponent)
    {
        const UnitVector& set((*m_getter)(unit, allies, opponent));

        if (set.size() == 0)
            return Point(0.0f, 0.0f);

        else if (set.size() == 1)
            return set.front()->getPosition();

        auto sum = std::accumulate(set.begin(), set.end(), Point(0.0f, 0.0f), [](const Point& p, const UnitSPtr& u)
        {
            return p + u->getPosition();
        });

        return sum / static_cast<float>(set.size());
    }

    std::string getCode() const
    {
        return std::string("B") + m_getter->getCode();
    }

protected:
    SetExtractorUPtr m_getter;
};

/**
 * UnitPosition returns the position of the unit contained into the UnitExtractor
 */

class UnitPositionExtractor : public PointExtractor
{
public:
    UnitPositionExtractor(UnitExtractorUPtr& extractor)
        : PointExtractor(),
        m_getter(std::move(extractor))
    {}

    Point operator()(const UnitSPtr& unit, const ArmySPtr& allies, const ArmySPtr& opponent)
    {
        const UnitSPtr& u((*m_getter)(unit, allies, opponent));
        
        if (u)
            return u->getPosition();

        return Point(0.0f, 0.0f);
    }

    std::string getCode() const
    {
        return std::string("P") + m_getter->getCode();
    }

protected:
    UnitExtractorUPtr m_getter;
};

/**
 * Returns the predefine position
 */
class PredefinePoint : public PointExtractor
{
public:
    PredefinePoint(const Point& p)
        : PointExtractor(),
        m_p(p)
    {}

    Point operator()(const UnitSPtr& unit, const ArmySPtr& allies, const ArmySPtr& opponent)
    {
        tools::unusedArg(unit, allies, opponent);

        return m_p;
    }

    std::string getCode() const
    {
        return std::string("p");

    }

protected:
    Point m_p;
};

#endif //_POINTEXTRACTOR_H_