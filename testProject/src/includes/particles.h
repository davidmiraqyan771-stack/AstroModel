#ifndef PARTICLE_H
#define PARTICLE_H

#include "./vector.h"

typedef unsigned int pCount;

typedef struct
{
    Vec2 x;
    Vec2 v;
    double m;
    double r;
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

typedef struct {
    float x, y, r;
} RenderParticle;

void creDerivSystem(DerivSystem* deriv, pCount n);
void remDerivSystem(DerivSystem* deriv);
void addParticleDeriv(ParticleSystem *sys, const DerivSystem *deriv, double factor);
void systemNewState(ParticleSystem *sys, DerivSystem* derives, double factor);
void creParticleSystem(ParticleSystem *sys);
void remParticleSystem(ParticleSystem *sys);
void cpyParticleSystem(ParticleSystem *sys, ParticleSystem *tmp);

#endif
