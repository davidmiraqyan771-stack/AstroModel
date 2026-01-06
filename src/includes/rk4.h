#ifndef RK4_H
#define RK4_H

#include "./particles.h"

void rk4Step(ParticleSystem *sys, double deltaT);

#endif