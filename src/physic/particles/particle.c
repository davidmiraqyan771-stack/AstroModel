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
        sys->p[i].x = vecAdd(sys->p[i].x, vecScale(deriv->dp[i].dx, factor));
        sys->p[i].v = vecAdd(sys->p[i].v, vecScale(deriv->dp[i].dv, factor));
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
                        deriv[0].dp[i].dx,
                        vecScale(deriv[1].dp[i].dx, 2)),
                    vecScale(deriv[2].dp[i].dx, 2)),
                deriv[3].dp[i].dx);

        Vec2 tmpdv =
            vecAdd(
                vecAdd(
                    vecAdd(
                        deriv[0].dp[i].dv,
                        vecScale(deriv[1].dp[i].dv, 2)),
                    vecScale(deriv[2].dp[i].dv, 2)),
                deriv[3].dp[i].dv);
        sys->p[i].x = vecAdd(sys->p[i].x, vecScale(tmpdx, factor));
        sys->p[i].v = vecAdd(sys->p[i].v, vecScale(tmpdv, factor));
    }
}
