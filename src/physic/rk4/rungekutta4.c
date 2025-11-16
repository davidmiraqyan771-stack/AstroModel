#include "../particles/particles.h"
#include "../Gravity/gravity.h"


void rk4Step(ParticleSystem *sys, double deltaT) {
    DerivSystem k1, k2, k3, k4;
    creDerivSystem(&k1, sys->n);
    computeDerivative(sys, &k1);
    
}   