#ifndef GRAVITY_H
#define GRAVITY_H

#include "./particles.h"

#define G_CONSTANT 4*(3.14159265359 * 3.14159265359)

void computeDerivative(const ParticleSystem *src, DerivSystem *dest);


#endif