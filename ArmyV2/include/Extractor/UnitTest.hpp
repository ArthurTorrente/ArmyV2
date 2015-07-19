#ifndef _UNITTESTEXTRACTOR_H_
#define _UNITTESTEXTRACTOR_H_

#include "ValueExtractor.hpp"
#include "UnitExtractor.hpp"
#include "PointExtractor.hpp"
#include "SetExtractor.hpp"

/**
 * All unit Test for Extractor are here
 */

/** ===================================================================================================== *
 *  ===================================================================================================== *
 *  ====================================== VALUE EXTRACTOR ============================================== *
 *  ===================================================================================================== *
 *  ===================================================================================================== */

bool unitTest_predefineValueExtractor()
{
    float predefine = 10.0f;

    PreDefineValueExtractor e(predefine);

    Army a(10, 200), b(10, 200);
    Unit& u(*(a.getUnitsList().front()));

    auto result = e(u, a, b);

    if (result != predefine)
        return false;

    return true;
}

bool unitTest_CapacityValueExtractor()
{
    Army a(10, 200), b(10, 200);
    Unit& u(*a.getUnitsList().front());

    auto capacityValue = u.getCapacity(0)->getValue();
    UnitExtractorUPtr unit(new IAUnitExtractor());
    CapacityValueExtractor ce(0, unit);

    auto result = ce(u, a, b);

    if (result != capacityValue)
        return false;

    return true;
}

bool unitTest_DistanceExtractor()
{
    Army a(10, 200), b(10, 200);
    Unit& u(*a.getUnitsList().front());

    Point ori(0.0f, 0.0f), uP(10.0f, 10.0f);
    float distance = ori.distance(uP);
    u.setPosition(uP);

    UnitExtractorUPtr unit(new IAUnitExtractor);
    PointExtractorUPtr point(new PredefinePoint(ori));

    DistanceExtractor d(point, unit);

    float result = d(u, a, b);

    if (result != distance)
        return false;

    return true;
}

bool unitTest_MinMaxAverageCapacityValueExtracteur()
{
    Army a(3, 0), b(0, 0);
    Unit& u(*a.getUnitsList().front()), &u2(*a.getUnitsList()[1]), &u3(*a.getUnitsList()[2]);

    u3.getCapacity(0)->upgrade(5);

    SetExtractorUPtr army(new AlliesArmyExtractor());

    /* MAX */
    auto capacityMax = u3.getCapacity(0)->getValue();
    MinMaxAverageCapacityValueExtractor mCMax(MinMaxAverageCapacityValueExtractor::MAX, 0, army);

    auto result = mCMax(u, a, b);

    if (result != capacityMax)
        return false;

    /* MIN */
    army.reset(new AlliesArmyExtractor());
    u2.getCapacity(0)->upgrade(5);
    auto capacityMin = u.getCapacity(0)->getValue();
    MinMaxAverageCapacityValueExtractor mCMin(MinMaxAverageCapacityValueExtractor::MIN, 0, army);

    result = mCMin(u, a, b);

    if (result != capacityMin)
        return false;

    /* AVERAGE */
    auto capacityAverage = u.getCapacity(0)->getValue() + u2.getCapacity(0)->getValue() + u3.getCapacity(0)->getValue();
    capacityAverage /= 3.0f;
    army.reset(new AlliesArmyExtractor());
    MinMaxAverageCapacityValueExtractor mCAverage(MinMaxAverageCapacityValueExtractor::AVERAGE, 0, army);
    
    result = mCAverage(u, a, b);

    if (result != capacityAverage)
        return false;

    return true;
}

bool unitTest_MinMaxAverageDistanceValueExtractor()
{
    Army a(3, 200), b(10, 200);
    Unit& u(*a.getUnitsList().front()), &u2(*a.getUnitsList()[1]), &u3(*a.getUnitsList()[2]);

    Point ori(0.0f, 0.0f), uP(1.0f, 1.0f), u2P(2.0f, 2.0f), u3P(3.0f, 3.0f);

    u.setPosition(uP);
    u2.setPosition(u2P);
    u3.setPosition(u3P);

    PointExtractorUPtr point(new PredefinePoint(ori));
    SetExtractorUPtr army(new AlliesArmyExtractor);

    /* MIN */
    auto minDist = ori.distance(uP);
    MinMaxAverageDistanceValueExtractor mDMin(MinMaxAverageDistanceValueExtractor::MIN, army, point);

    auto result = mDMin(u, a, b);

    if (result != minDist)
        return false;

    /* MAX */
    auto maxDist = ori.distance(u3P);
    point.reset(new PredefinePoint(ori));
    army.reset(new AlliesArmyExtractor);

    MinMaxAverageDistanceValueExtractor mDMax(MinMaxAverageDistanceValueExtractor::MAX, army, point);

    result = mDMax(u, a, b);

    if (result != maxDist)
        return false;

    /* AVERAGE */
    auto distAverage = ori.distance(uP) + ori.distance(u2P) + ori.distance(u3P);
    distAverage /= 3.0f;

    point.reset(new PredefinePoint(ori));
    army.reset(new AlliesArmyExtractor);

    MinMaxAverageDistanceValueExtractor mCAverage(MinMaxAverageDistanceValueExtractor::AVERAGE, army, point);
    result = mCAverage(u, a, b);

    if (result != distAverage)
        return false;

    return true;
}


bool unitTest_ValueExtractor()
{
    if (!unitTest_predefineValueExtractor() ||
        !unitTest_CapacityValueExtractor() ||
        !unitTest_DistanceExtractor() ||
        !unitTest_MinMaxAverageCapacityValueExtracteur() ||
        !unitTest_MinMaxAverageDistanceValueExtractor())
        return false;

    return true;
}


/** ===================================================================================================== *
 *  ===================================================================================================== *
 *  ====================================== POINT EXTRACTOR ============================================== *
 *  ===================================================================================================== *
 *  ===================================================================================================== */
bool unitTest_BaryCenterExtractor()
{
    Army a(3, 200), b(10, 200);
    Unit& u(*a.getUnitsList().front()), &u2(*a.getUnitsList()[1]), &u3(*a.getUnitsList()[2]);

    Point ori(0.0f, 0.0f), uP(1.0f, 1.0f), uP2(2.0f, 2.0f), uP3(3.0f, 3.0f);
    u.setPosition(uP);
    u2.setPosition(uP2);
    u3.setPosition(uP3);

    auto baryCenter = (uP + uP2 + uP3) / 3.0f;
    SetExtractorUPtr set(new AlliesArmyExtractor);

    BaryCenterExtractor bcE(set);

    if (baryCenter != bcE(u, a, b))
        return false;

    return true;
}

bool unitTest_UnitPositionExtractor()
{
    Army a(3, 200), b(10, 200);
    Unit& u(*a.getUnitsList().front()), &u2(*a.getUnitsList()[1]), &u3(*a.getUnitsList()[2]);

    Point ori(0.0f, 0.0f), uP(1.0f, 1.0f), uP2(2.0f, 2.0f), uP3(3.0f, 3.0f);
    u.setPosition(uP);
    u2.setPosition(uP2);
    u3.setPosition(uP3);

    UnitExtractorUPtr uEx(new IAUnitExtractor);

    UnitPositionExtractor uPE(uEx);

    if (uP != uPE(u, a, b))
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


/** ===================================================================================================== *
 *  ===================================================================================================== *
 *  ======================================= UNIT EXTRACTOR ============================================== *
 *  ===================================================================================================== *
 *  ===================================================================================================== */

bool unitTest_IAUnitExtractor()
{
    Army a(3, 200), b(10, 200);
    Unit& u(*a.getUnitsList().front());

    IAUnitExtractor iaE;

    if (iaE(u, a, b).getId() != u.getId())
        return false;

    return true;
}

bool unitTest_MinMaxCapacityExtractor()
{
    Army a(3, 200), b(10, 200);
    Unit& u(*a.getUnitsList().front()), &u2(*a.getUnitsList()[1]), &u3(*a.getUnitsList()[2]);

    SetExtractorUPtr set(new AlliesArmyExtractor);

    /* MAX*/
    MinMaxCapacityUnitExtractor mmCMax(false, 0, set);
    u3.getCapacity(0)->upgrade(10);

    if (mmCMax(u, a, b).getId() != u3.getId())
        return false;

    /* MIN */
    set.reset(new AlliesArmyExtractor);
    MinMaxCapacityUnitExtractor mmCMin(true, 0, set);
    u2.getCapacity(0)->upgrade(10);

    if (mmCMin(u, a, b).getId() != u.getId())
        return false;

    return true;
}

bool unitTest_FarNearExtractor()
{
    Army a(3, 200), b(10, 200);
    Unit& u(*a.getUnitsList().front()), &u2(*a.getUnitsList()[1]), &u3(*a.getUnitsList()[2]);

    Point ori(0.0f, 0.0f), uP(1.0f, 1.0f), uP2(2.0f, 2.0f), uP3(3.0f, 3.0f);
    u.setPosition(uP);
    u2.setPosition(uP2);
    u3.setPosition(uP3);

    SetExtractorUPtr set(new AlliesArmyExtractor);
    PointExtractorUPtr point(new PredefinePoint(ori));

    /* FAR */
    FarNearExtractor fnFar(true, set, point);

    if (fnFar(u, a, b).getId() != u3.getId())
        return false;

    /* NEAR */
    set.reset(new AlliesArmyExtractor);
    point.reset(new PredefinePoint(ori));

    FarNearExtractor fnNear(false, set, point);

    if (fnNear(u, a, b).getId() != u.getId())
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

/** ===================================================================================================== *
 *  ===================================================================================================== *
 *  ======================================== SET EXTRACTOR ============================================== *
 *  ===================================================================================================== *
 *  ===================================================================================================== */

bool unitTest_AlliesArmyExtractor()
{
    Unit u(0);
    Army a(10, 200), b(20, 200);

    AlliesArmyExtractor aE;

    if (aE(u, a, b).size() != 10)
        return false;

    return true;
}

bool unitTest_OpponentArmyExtractor()
{
    Unit u(0);
    Army a(10, 200), b(20, 200);

    OpponentArmyExtractor oE;

    if (oE(u, a, b).size() != 20)
        return false;

    return true;
}

bool unitTest_NMinMaxCapacityExtractor()
{
    Army a(3, 0), b(3, 0);
    Unit& u(*a.getUnitsList().front()), &u2(*a.getUnitsList()[1]), &u3(*a.getUnitsList()[2]);

    u.getCapacity(0)->upgrade(5);

    SetExtractorUPtr alliesExtractor(new AlliesArmyExtractor());
    NMinMaxCapacityExtractor nmmcEMax(NMinMaxCapacityExtractor::MAX, 1, 0, alliesExtractor);

    auto result = nmmcEMax(u, a, b);

    if (result.size() != 1 || *result.front() != u)
        return false;

    alliesExtractor.reset(new AlliesArmyExtractor());

    u2.getCapacity(0)->upgrade(5);
    NMinMaxCapacityExtractor nmmcEMin(NMinMaxCapacityExtractor::MIN, 1, 0, alliesExtractor);

    result = nmmcEMin(u, a, b);

    if (result.size() != 1 || *result.front() != u3)
        return false;

    return true;
}

bool unitTest_NFarNearExtractor()
{
    Army a(3, 0), b(3, 0);
    Unit& u(*a.getUnitsList().front()), &u2(*a.getUnitsList()[1]), &u3(*a.getUnitsList()[2]);

    Point ori(0.0f, 0.0f), uP(1.0f, 1.0f), uP2(2.0f, 2.0f), uP3(3.0f, 3.0f);
    u.setPosition(uP);
    u2.setPosition(uP2);
    u3.setPosition(uP3);

    PointExtractorUPtr point(new PredefinePoint(ori));
    SetExtractorUPtr set(new AlliesArmyExtractor);

    NFarNearExtractor nfnEFar(true, 1, set, point);

    auto result = nfnEFar(u, a, b);

    if (result.size() != 1 || *result.front() != u3)
        return false;

    point.reset(new PredefinePoint(ori));
    set.reset(new AlliesArmyExtractor);

    NFarNearExtractor nfnENear(false, 1, set, point);

    result = nfnENear(u, a, b);

    if (result.size() != 1 || *result.front() != u)
        return false;

    return true;
}

bool unitTest_ThresholdCapacityExtractor()
{
    Army a(3, 0), b(3, 0);
    Unit& u(*a.getUnitsList().front()), &u2(*a.getUnitsList()[1]), &u3(*a.getUnitsList()[2]);

    SetExtractorUPtr set(new AlliesArmyExtractor);

    u.getCapacity(0)->upgrade(10);

    ThresholdCapacityExtractor thresholdCapMax(false, 2.0f, 0, set);

    auto result = thresholdCapMax(u, a, b);

    if (result.size() != 1 || *result.front() != u)
        return false;

    set.reset(new AlliesArmyExtractor);
    u3.getCapacity(0)->upgrade(10);

    ThresholdCapacityExtractor thresholdCapMin(true, 2.0f, 0, set);

    result = thresholdCapMin(u, a, b);

    if (result.size() != 1 || *result.front() != u2)
        return false;

    return true;
}

bool unitTest_ThresholdDistanceExtractor()
{
    Army a(3, 0), b(3, 0);
    Unit& u(*a.getUnitsList().front()), &u2(*a.getUnitsList()[1]), &u3(*a.getUnitsList()[2]);

    Point ori, uP1(1.0f, 1.0f), uP2(2.0f, 2.0f), uP3(3.0f, 3.0f);

    SetExtractorUPtr set(new AlliesArmyExtractor);
    PointExtractorUPtr point(new PredefinePoint(ori));

    u.setPosition(uP1);
    u2.setPosition(uP2);
    u3.setPosition(uP3);

    float distU1 = ori.distance(uP1);
    float distU2 = ori.distance(uP2);
    float distU3 = ori.distance(uP3);

    distU3 -= (distU3 - distU2) * 0.5f;

    ThresholdDistanceExtractor thresholdDisMax(false, distU3, set, point);

    auto result = thresholdDisMax(u, a, b);

    if (result.size() != 1 || *result.front() != u3)
        return false;

    set.reset(new AlliesArmyExtractor);
    point.reset(new PredefinePoint(ori));

    distU1 += (distU2 - distU1) * 0.5f;

    ThresholdDistanceExtractor thresholdDisMin(true, distU1, set, point);

    result = thresholdDisMin(u, a, b);

    if (result.size() != 1 || *result.front() != u)
        return false;

    return true;
}

bool unitTest_SetExtractor()
{
    if (!unitTest_AlliesArmyExtractor() ||
        !unitTest_OpponentArmyExtractor() ||
        !unitTest_NMinMaxCapacityExtractor() ||
        !unitTest_NFarNearExtractor() ||
        !unitTest_ThresholdCapacityExtractor() || 
        !unitTest_ThresholdDistanceExtractor())
        return false;

    return true;
}

#endif //_UNITTESTEXTRACTOR_H_