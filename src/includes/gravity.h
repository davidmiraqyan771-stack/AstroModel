#ifndef GRAVITY_H
#define GRAVITY_H

#include "./particles.h"

#define G_CONSTANT 39.4784176

void computeDerivative(const ParticleSystem *src, DerivSystem *dest);


#endif