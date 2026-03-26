#include "../../includes/vector.h"

Vec3 vecAdd(Vec3 aVector, Vec3 bVector) { return (Vec3){aVector.x + bVector.x, aVector.y + bVector.y, aVector.z + bVector.z}; }
Vec3 vecSub(Vec3 aVector, Vec3 bVector) { return (Vec3){aVector.x - bVector.x, aVector.y - bVector.y, aVector.z - bVector.z}; }
Vec3 vecScale(Vec3 aVector, double factor) { return (Vec3){aVector.x * factor, aVector.y * factor, aVector.z * factor}; }
double vecDistSq(Vec3 aVector, Vec3 bVector)
{
    Vec3 subRes = vecSub(aVector, bVector);

    double dist2 = subRes.x * subRes.x + subRes.y * subRes.y + subRes.z * subRes.z + EPSILON * EPSILON;

    return dist2;
}
