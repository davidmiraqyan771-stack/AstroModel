#ifndef PARTICLE_H
#define PARTICLE_H

#include "../../utils/vectors/vector.h"

typedef unsigned int pCount;

typedef struct
{
    double m;
    double r;
    Vec2 x;
    Vec2 v;

} Particle;

typedef struct
{

    Particle *p;
    double t;
    pCount n;

} ParticleSystem;


typedef struct
{
    Vec2 dx;
    Vec2 dv;

} DerivParticle;

typedef struct 
{

    DerivParticle *dp;
    pCount n;

} DerivSystem;

void creDerivSystem(DerivSystem* deriv, pCount n);
void remDerivSystem(DerivSystem* deriv);

#endif