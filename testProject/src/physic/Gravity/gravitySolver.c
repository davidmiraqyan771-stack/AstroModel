#include "../../includes/gravity.h"
#include <math.h>

void computeDerivative(const ParticleSystem *src, DerivSystem *dest)
{

    for (int i = 0; i < src->n; ++i)
    {
        dest->dp[i].dx = src->p[i].v;
        dest->dp[i].dv.x = 0;
        dest->dp[i].dv.y = 0;
        for (int j = 0; j < src->n; ++j)
        {
            if (i == j)
            {
                continue;
            }

            Vec2 subtract = vecSub(src->p[j].x, src->p[i].x);
            double distancesq = vecDistSq(src->p[j].x, src->p[i].x);
            dest->dp[i].dv.x += G_CONSTANT * src->p[j].m * (subtract.x * (1.0 / (distancesq * sqrt(distancesq))));
            dest->dp[i].dv.y += G_CONSTANT * src->p[j].m * (subtract.y * (1.0 / (distancesq * sqrt(distancesq))));
        }
    }
}