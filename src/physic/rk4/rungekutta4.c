#include "../../includes/particles.h"
#include "../../includes/gravity.h"


void rk4Step(ParticleSystem *sys, double deltaT)
{
    DerivSystem k[4];
    ParticleSystem tmp;
    tmp.n = sys->n;

    creParticleSystem(&tmp);
    cpyParticleSystem(sys, &tmp);

    creDerivSystem(&k[0], tmp.n);
    creDerivSystem(&k[1], tmp.n);
    creDerivSystem(&k[2], tmp.n);
    creDerivSystem(&k[3], tmp.n);

    computeDerivative(&tmp, &k[0]);
    addParticleDeriv(&tmp, &k[0], deltaT / 2);

    computeDerivative(&tmp, &k[1]);
    cpyParticleSystem(sys, &tmp);

    addParticleDeriv(&tmp, &k[1], deltaT / 2);
    computeDerivative(&tmp, &k[2]);
    cpyParticleSystem(sys, &tmp);

    addParticleDeriv(&tmp, &k[2], deltaT);
    computeDerivative(&tmp, &k[3]);

    systemNewState(sys, k, deltaT / 6);

    remParticleSystem(&tmp);
    
    remDerivSystem(&k[0]);
    remDerivSystem(&k[1]);
    remDerivSystem(&k[2]);
    remDerivSystem(&k[3]);
    sys->t += deltaT;
}