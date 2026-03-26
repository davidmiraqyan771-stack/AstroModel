#include "./includes/buffer.h"
#include "./includes/callbacks.h"
#include "./includes/camera.h"
#include "./includes/config.h"
#include "./includes/shader.h"
#include "./includes/sleep.h"
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <cglm/types.h>
#include <cglm/vec3.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define FRAME_TIME 1.0 / 60.0
#define PI 3.14159265359
    
RenderParticle *buffers[BUF_COUNT];
atomic_int guardIndex;
char appRunning = 1;

float lastX = START_WIDTH;
float lastY = START_HEIGHT;

int main(void) {

  GLFWwindow *window = initOpenGL();

  initPhysicsData();

  pthread_t physThreadId;

  if (pthread_create(&physThreadId, NULL, physicCaluclate, NULL) != 0) {
    perror("Thread failed");

    exit(EXIT_FAILURE);
  }
  float vertices[] = {-0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.5f, -0.5f,
                      1.0f,  1.0f,  1.0f, 0.5f, 0.5f, 1.0f, 1.0f,
                      1.0f,  -0.5f, 0.5f, 1.0f, 1.0f, 1.0f};

  GLuint indices[] = {0, 1, 2, 2, 3, 0};

  shaderProgram =
      shaderProgramCreate("../render/shader/GLSLcodes/vertexShader.glsl",
                          "../render/shader/GLSLcodes/fragmentShader.glsl");

  GLuint VBO, VAO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);
  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(2 * sizeof(float)));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glViewport(0, 0, 800, 600);

  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  glm_mat4_identity(projectionMatrix);

  float aspect = START_WIDTH / (float)START_HEIGHT;
  glm_perspective(glm_rad(45.0f), aspect, 0.1f, 100.0f, projectionMatrix);

  glm_mat4_identity(viewMatrix);

  vec3 spaceCenter;
  glm_vec3_add(cameraPos, cameraFront, spaceCenter);

  glm_lookat(cameraPos, spaceCenter, cameraUp, viewMatrix);

  int readIndex = 1;

  programUse(&shaderProgram);
  setMat4Uniform(&shaderProgram, "projection", projectionMatrix);
  setMat4Uniform(&shaderProgram, "view", viewMatrix);
  setFloatUniform(&shaderProgram, "aspect", aspect);

  double lastTime = glfwGetTime();

  while (!glfwWindowShouldClose(window)) {
    double startTime = glfwGetTime();
    char cameraChanged = processInput(window);

    readIndex = atomic_exchange(&guardIndex, readIndex);
    RenderParticle *drawData = buffers[readIndex];

    if (drawData == NULL) {
      glfwPollEvents();
      glfwSwapBuffers(window);
      continue;
    }

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    programUse(&shaderProgram);

    if (cameraChanged) {
      vec3 center;
      glm_vec3_add(cameraPos, cameraFront, center);
      glm_lookat(cameraPos, center, cameraUp, viewMatrix);
      setMat4Uniform(&shaderProgram, "view", viewMatrix);
    }

    for (int i = 0; i < PARTICLE_COUNT; ++i) {
      mat4 model;
      glm_mat4_identity(model);
      glm_translate(model,
                    (float[]){drawData[i].x, drawData[i].y, drawData[i].z});
      float scale = drawData[i].r;
      glm_scale(model, (vec3){scale, scale, scale});
      setMat4Uniform(&shaderProgram, "model", model);
      glBindVertexArray(VAO);
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
    glfwPollEvents();
    glfwSwapBuffers(window);

    double endTime = glfwGetTime();
    double frameDuration = endTime - startTime;

    if (frameDuration < FRAME_TIME) {
      double sleepSeconds = FRAME_TIME - frameDuration;

      UsSleep((long)(sleepSeconds * 1000000.0));
    }
  }

  appRunning = 0;
  pthread_join(physThreadId, NULL);
  destroyPhysicsData();
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(shaderProgram.id);

  glfwTerminate();
}
