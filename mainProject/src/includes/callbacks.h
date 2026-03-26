#ifndef CALLBACKS_H
#define CALLBACKS_H

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
char processInput(GLFWwindow *window);

#endif