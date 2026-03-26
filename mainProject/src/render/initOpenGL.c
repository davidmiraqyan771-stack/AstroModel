#include "../includes/callbacks.h"
#include "../includes/config.h"
#include <stdio.h>
#include <stdlib.h>

mat4 viewMatrix;
mat4 projectionMatrix;
Shader shaderProgram;

GLFWwindow *initOpenGL() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window =
      glfwCreateWindow(START_WIDTH, START_HEIGHT, "AstroMod", NULL, NULL);
  if (window == NULL) {
    perror("window");
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    perror("GLAD");
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glEnable(GL_DEPTH_TEST);

  return window;
}