#ifndef BUFF_H
#define BUFF_H
#include "particles.h"
#include <stdatomic.h>
#define BUF_COUNT 3
#define PARTICLE_COUNT 3

extern RenderParticle* buffers[BUF_COUNT]; 
extern atomic_int guardIndex;                   
extern char appRunning;

void initPhysicsData(void);
void destroyPhysicsData(void);
void* physicCaluclate(void* args);

#endif