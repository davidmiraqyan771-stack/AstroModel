#ifndef VEC_H
#define VEC_H

#define EPSILON 1e6

typedef struct {

    double x;
    double y;

} Vec2;

Vec2 vecAdd(Vec2 aVector, Vec2 bVector);
Vec2 vecSub(Vec2 aVector, Vec2 bVector);
Vec2 vecScale(Vec2 aVector, double factor);
double vecDistSq(Vec2 aVector, Vec2 bVector);


#endif