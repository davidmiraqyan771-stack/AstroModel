#include "./physic/particles/particles.h"
#include "./physic/rk4/rk4.h"
#include <math.h>
#include <stdio.h>
int main(void)
{
    ParticleSystem sys = {0};

    sys.n = 2;
    creParticleSystem(&sys);
    double dt = 3600;
    sys.p[0].m = 1.98847e30;
    sys.p[0].r = 6.9634e8;
    sys.p[0].x.x = 0.0;
    sys.p[0].x.y = 0.0;
    sys.p[0].v.x = 0.0;
    sys.p[0].v.y = -0.008934;
    sys.p[1].m = 5.9722e24;
    sys.p[1].r = 6.371e6;
    sys.p[1].x.x = 1.495978707e11;
    sys.p[1].x.y = 0.0;
    sys.p[1].v.x = 0.0;
    sys.p[1].v.y = 29784.691;
    sys.t = 0.0;

    for (long step = 0; step < 8766 * 1; ++step)
    { 
        rk4Step(&sys, dt);

        if (step % 24 == 0)
        {
            double r = sqrt(sys.p[1].x.x*sys.p[1].x.x + sys.p[1].x.y * sys.p[1].x.y);
            printf(
                "Day: %ld\nt: %lf x: %lf y: %lf dist: %lf vx: %lf vy: %lf\n", 
                step / 24, sys.t, sys.p[1].x.x, sys.p[1].x.y, r, sys.p[1].v.x, sys.p[1].v.y
            );
        }
    }

    remParticleSystem(&sys);
}
