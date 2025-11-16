#ifndef GRAVITY_H
#define GRAVITY_H

#include "../particles/particles.h"

#define G_CONSTANT (6.67430e-11)

void computeDerivative(const ParticleSystem *src, DerivSystem *dest);


#endif