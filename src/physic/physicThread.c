#include "../includes/buffer.h"
#include "../includes/dynamicMem.h"
#include "../includes/rk4.h"
#include "../includes/sleep.h"
#include <unistd.h>
#include <stdlib.h>

#define PI 3.14159265359

ParticleSystem sys = {0};

static void floatConverter(int writeIndex)
{
    RenderParticle *target = buffers[writeIndex];
    for (int i = 0; i < PARTICLE_COUNT; ++i)
    {
        target[i].x = (float)sys.p[i].x.x;
        target[i].y = (float)sys.p[i].x.y;
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

    sys.p[0].m = 1;
    sys.p[0].r = 0.5;
    sys.p[0].x.x = -0.0047238;
    sys.p[0].x.y = 0.0;
    sys.p[0].v.x = 0.0;
    sys.p[0].v.y = -0.002758;
    sys.p[1].m = 0.000954;
    sys.p[1].r = 0.3;
    sys.p[1].x.x = 4.95;
    sys.p[1].x.y = 0.0;
    sys.p[1].v.x = 0.0;
    sys.p[1].v.y = 2.89;
    sys.p[2].m = 1.3e-10;
    sys.p[2].r = 0.22;
    sys.p[2].x.x = 2.15;
    sys.p[2].x.y = 0.0;
    sys.p[2].v.x = 0.0;
    sys.p[2].v.y = 4.47;
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
    double dt = 3600.0 / 31557600.0;
    int writeIndex = 0;

    while (appRunning)
    {
        rk4Step(&sys, dt);

        floatConverter(writeIndex);

        writeIndex = atomic_exchange(&guardIndex, writeIndex);

        UsSleep(500);
    }
}