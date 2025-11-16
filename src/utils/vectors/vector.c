#include "vector.h"

Vec2 vecAdd(Vec2 aVector, Vec2 bVector) { return (Vec2){aVector.x + bVector.x, aVector.y + bVector.y}; }
Vec2 vecSub(Vec2 aVector, Vec2 bVector) { return (Vec2){aVector.x - bVector.x, aVector.y - bVector.y}; }
Vec2 vecScale(Vec2 aVector, double factor) { return (Vec2){aVector.x * factor, aVector.y * factor}; }
double vecDistSq(Vec2 aVector, Vec2 bVector)
{
    Vec2 subRes = vecSub(aVector, bVector);

    double dist2 = subRes.x * subRes.x + subRes.y * subRes.y + EPSILON * EPSILON;

    return dist2;
}
