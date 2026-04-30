#ifndef GRAVITY_H
#define GRAVITY_H

#include "./particles.h"

#define G_CONSTANT 39.47841760435743

void computeDerivative(const ParticleSystem *src, DerivSystem *dest);

#endif