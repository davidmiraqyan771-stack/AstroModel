#ifndef VEC_H
#define VEC_H

// #define EPSILON 1e-6
#define EPSILON 1e-3

typedef struct {
    double x,y,z;
} Vec3;

Vec3 vecAdd(Vec3 aVector, Vec3 bVector);
Vec3 vecSub(Vec3 aVector, Vec3 bVector);
Vec3 vecScale(Vec3 aVector, double factor);
double vecDistSq(Vec3 aVector, Vec3 bVector);


#endif
