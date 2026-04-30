#include "../../includes/particles.h"
#include "../../includes/dynamicMem.h"
#include <stdlib.h>

void creParticleSystem(ParticleSystem *sys) {
    sys->p = creDy(sys->n, sizeof(Particle));
}

void cpyParticleSystem(ParticleSystem *sys, ParticleSystem *tmp) {
    for(int i = 0; i < tmp->n; ++i) {
        tmp->p[i] = sys->p[i];
    }
}

void remParticleSystem(ParticleSystem *sys) {
    free(sys->p);
    sys->n = 0;
}


void creDerivSystem(DerivSystem *deriv, pCount n)
{

    deriv->n = n;
    deriv->dp = creDy(n, sizeof(DerivParticle));
}

void remDerivSystem(DerivSystem *deriv)
{
    free(deriv->dp);
    deriv->n = 0;
}

void addParticleDeriv(ParticleSystem *sys, const DerivSystem *deriv, double factor)
{
    for (int i = 0; i < sys->n; ++i)
    {
        sys->p[i].pos = vecAdd(sys->p[i].pos, vecScale(deriv->dp[i].dpos, factor));
        sys->p[i].vel = vecAdd(sys->p[i].vel, vecScale(deriv->dp[i].dvel, factor));
    }
}

void systemNewState(ParticleSystem *sys, DerivSystem *deriv, double factor)
{
    for (int i = 0; i < sys->n; ++i)
    {
        Vec2 tmpdx =
            vecAdd(
                vecAdd(
                    vecAdd(
                        deriv[0].dp[i].dpos,
                        vecScale(deriv[1].dp[i].dpos, 2)),
                    vecScale(deriv[2].dp[i].dpos, 2)),
                deriv[3].dp[i].dpos);

        Vec2 tmpdv =
            vecAdd(
                vecAdd(
                    vecAdd(
                        deriv[0].dp[i].dvel,
                        vecScale(deriv[1].dp[i].dvel, 2)),
                    vecScale(deriv[2].dp[i].dvel, 2)),
                deriv[3].dp[i].dvel);
        sys->p[i].pos = vecAdd(sys->p[i].pos, vecScale(tmpdx, factor));
        sys->p[i].vel = vecAdd(sys->p[i].vel, vecScale(tmpdv, factor));
    }
}
