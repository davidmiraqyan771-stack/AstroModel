#ifndef CONFIG_H
#define CONFIG_H

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <cglm/types.h>
#include "shader.h"
#define START_WIDTH 800
#define START_HEIGHT 600
GLFWwindow *initOpenGL();

extern mat4 viewMatrix;
extern mat4 projectionMatrix;
extern Shader shaderProgram;

#endif