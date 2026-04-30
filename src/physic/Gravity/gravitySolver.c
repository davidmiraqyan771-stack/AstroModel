#include "../../includes/gravity.h"
#include <math.h>

void computeDerivative(const ParticleSystem *src, DerivSystem *dest)
{

    for (int i = 0; i < src->n; ++i)
    {
        dest->dp[i].dpos = src->p[i].vel;
        dest->dp[i].dvel.x = 0;
        dest->dp[i].dvel.y = 0;
        for (int j = 0; j < src->n; ++j)
        {
            if (i == j)
            {
                continue;
            }

            Vec2 distVec = vecSub(src->p[j].pos, src->p[i].pos);
            double distanceSq = vecDistSq(src->p[j].pos, src->p[i].pos);
            dest->dp[i].dvel.x += G_CONSTANT * src->p[j].m * (distVec.x / (distanceSq * sqrt(distanceSq)));
            dest->dp[i].dvel.y += G_CONSTANT * src->p[j].m * (distVec.y / (distanceSq * sqrt(distanceSq)));
        }
    }
}