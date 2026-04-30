#include "../../includes/vector.h"

Vec2 vecAdd(Vec2 aVector, Vec2 bVector) { return (Vec2){aVector.x + bVector.x, aVector.y + bVector.y}; }
Vec2 vecSub(Vec2 aVector, Vec2 bVector) { return (Vec2){aVector.x - bVector.x, aVector.y - bVector.y}; }
Vec2 vecScale(Vec2 aVector, double factor) { return (Vec2){aVector.x * factor, aVector.y * factor}; }
double vecDistSq(Vec2 aVector, Vec2 bVector)
{
    Vec2 distVec = vecSub(aVector, bVector);

    return distVec.x * distVec.x + distVec.y * distVec.y + EPSILON * EPSILON;
}
