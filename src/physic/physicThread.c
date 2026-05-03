#include "../includes/buffer.h"
#include "../includes/dynamicMem.h"
#include "../includes/rk4.h"
#include "../includes/sleep.h"
#include <unistd.h>
#include <stdlib.h>
#include <math.h>

#define PI 3.14159265359

ParticleSystem sys = {0};

static void floatConverter(int writeIndex)
{
    RenderParticle *target = buffers[writeIndex];
    for (int i = 0; i < PARTICLE_COUNT; ++i)
    {
        target[i].x = (float)sys.p[i].pos.x;
        target[i].y = (float)sys.p[i].pos.y;
        target[i].r = (float)sys.p[i].r;
    }
}

void initPhysicsData(void)
{

    sys.n = PARTICLE_COUNT;
    creParticleSystem(&sys);

    for (int i = 0; i < BUF_COUNT; ++i)
    {
        buffers[i] = creDy(BUF_COUNT, sizeof(RenderParticle));
    }

    double mass = 1.0;

    double x1 = 0.97000436;
    double y1 = -0.24308753;

    double vx1 = 0.4662036850 * 6.283185307; 
    double vy1 = 0.4323657300 * 6.283185307;

    sys.p[0].m = mass;
    sys.p[0].r = 0.1; 
    sys.p[0].pos.x = 0.0;
    sys.p[0].pos.y = 0.0;
    sys.p[0].vel.x = -2.0 * vx1;
    sys.p[0].vel.y = -2.0 * vy1;

    sys.p[1].m = mass;
    sys.p[1].r = 0.1;
    sys.p[1].pos.x = x1;
    sys.p[1].pos.y = y1;
    sys.p[1].vel.x = vx1;
    sys.p[1].vel.y = vy1;

    sys.p[2].m = mass;
    sys.p[2].r = 0.1;
    sys.p[2].pos.x = -x1;
    sys.p[2].pos.y = -y1;
    sys.p[2].vel.x = vx1;
    sys.p[2].vel.y = vy1;

    sys.t = 0.0;

    atomic_init(&guardIndex, 2);

    printf("Physics initialized.\n");
}

void destroyPhysicsData(void)
{
    remParticleSystem(&sys);
    for (int i = 0; i < BUF_COUNT; ++i)
    {
        free(buffers[i]);
    }
}

void *physicCaluclate(void *args)
{
    double dt = 0.00002;
    int writeIndex = 1;
    double lasty = 0.0;
    while (appRunning)
    {
        lasty = sys.p[1].pos.y;
        rk4Step(&sys, dt);
        // if (sys.p[1].pos.y - lasty > 0 && -lasty > 0)
        // {
            double r = sqrt(sys.p[1].pos.x * sys.p[1].pos.x + sys.p[1].pos.y * sys.p[1].pos.y);

            printf("lasty: %lf, y: %lf, y-lasty: %lf r: %lf, T: %lf | %lf time: %lf\n", lasty, sys.p[1].pos.y, sys.p[1].pos.y-lasty, r, (sys.t * 365.256363004) - 365.256363004, sys.t * 365.256363004, sys.t);
        // }
 
        floatConverter(writeIndex);

        writeIndex = atomic_exchange(&guardIndex, writeIndex);

        UsSleep(500);
    }
}