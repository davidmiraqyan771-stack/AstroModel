#include "../includes/buffer.h"
#include "../includes/dynamicMem.h"
#include "../includes/rk4.h"
#include "../includes/sleep.h"
#include <math.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>

#define PI 3.14159265359

ParticleSystem sys = {0};

static void floatConverter(int writeIndex) {
  RenderParticle *target = buffers[writeIndex];
  for (int i = 0; i < PARTICLE_COUNT; ++i) {
    target[i].x = (float)sys.p[i].x.x;
    target[i].y = (float)sys.p[i].x.y;
    target[i].z = (float)sys.p[i].x.z;
    target[i].r = (float)sys.p[i].r;
  }
}

void initPhysicsData(void) {

  sys.n = PARTICLE_COUNT;
  creParticleSystem(&sys);

  for (int i = 0; i < BUF_COUNT; ++i) {
    buffers[i] = creDy(BUF_COUNT, sizeof(RenderParticle));
  }

  const double mass = 1.0;

  const double x = 0.97000436;
  const double y = -0.24308753;

  const double vx = 0.4662036850 * (2 * PI);
  const double vy = 0.4323657300 * (2 * PI);

  const double z_offset = 0.05;
  const double vz_offset = 0.05;

  sys.p[0].m = mass;
  sys.p[0].r = 0.1;
  sys.p[0].x.x = 0.0;
  sys.p[0].x.y = 0.0;
  sys.p[0].x.z = 0.0;
  // sys.p[0].v.x = 0.0;
  // sys.p[0].v.y = 0.0;
  sys.p[0].v.x = -2.0 * vx;
  sys.p[0].v.y = -2.0 * vy;
  sys.p[0].v.z = vz_offset;

  sys.p[1].m = mass;
  sys.p[1].r = 0.1;
  sys.p[1].x.x = x;
  sys.p[1].x.y = y;
  sys.p[1].x.z = z_offset;
  sys.p[1].v.x = vx;
  sys.p[1].v.y = vy;
  sys.p[1].v.z = -vz_offset;

  sys.p[2].m = mass;
  sys.p[2].r = 0.1;
  sys.p[2].x.x = -x;
  sys.p[2].x.y = -y;
  sys.p[2].x.z = -z_offset;
  sys.p[2].v.x = vx;
  sys.p[2].v.y = vy;
  sys.p[2].v.z = -vz_offset;

  sys.t = 0.0;

  atomic_init(&guardIndex, 2);

  printf("Physics initialized.\n");
}

void destroyPhysicsData(void) {
  remParticleSystem(&sys);
  for (int i = 0; i < BUF_COUNT; ++i) {
    free(buffers[i]);
  }
}

void *physicCaluclate(void *args) {
  double dt = 0.00002;
  int writeIndex = 0;
  double lasty = 0.0;
  while (appRunning) {
    lasty = sys.p[1].x.y;
    rk4Step(&sys, dt);
    // if (sys.p[1].x.y - lasty > 0 && -lasty > 0)
    // {
    double r = sqrt(sys.p[1].x.x * sys.p[1].x.x + sys.p[1].x.y * sys.p[1].x.y);

    // printf("z: %lf, lasty: %lf, y: %lf, y-lasty: %lf r: %lf, T: %lf | %lf "
    //        "time: %lf\n",
    //        sys.p[1].x.z, lasty, sys.p[1].x.y, sys.p[1].x.y - lasty, r,
    //        (sys.t * 365.256363004) - 365.256363004, sys.t * 365.256363004,
    //        sys.t);
    // }

    floatConverter(writeIndex);

    writeIndex = atomic_exchange(&guardIndex, writeIndex);

    UsSleep(500);
  }
  return NULL;
}