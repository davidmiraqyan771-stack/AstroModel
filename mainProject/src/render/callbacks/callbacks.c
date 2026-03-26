#include "../../includes/callbacks.h"
#include "../../includes/camera.h"
#include "../../includes/shader.h"
#include "../../includes/config.h"
#include <cglm/cglm.h>

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);

  float aspect = width / (float)height;

  glm_mat4_identity(projectionMatrix);
  glm_perspective(glm_rad(45.0f), aspect, 0.1f, 100.0f, projectionMatrix);

  programUse(&shaderProgram);
  setMat4Uniform(&shaderProgram, "projection", projectionMatrix);
}

char processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }

  char changed = 0;
  const float cameraSpeed = 0.05f;
  vec3 positionChange;
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    glm_vec3_scale(cameraFront, cameraSpeed, positionChange);
    glm_vec3_add(cameraPos, positionChange, cameraPos);
    changed = 1;
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    glm_vec3_scale(cameraFront, cameraSpeed, positionChange);
    glm_vec3_sub(cameraPos, positionChange, cameraPos);
    changed = 1;
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    glm_cross(cameraFront, cameraUp, positionChange);
    glm_vec3_normalize(positionChange);
    glm_vec3_scale(positionChange, cameraSpeed, positionChange);
    glm_vec3_sub(cameraPos, positionChange, cameraPos);
    changed = 1;
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    glm_cross(cameraFront, cameraUp, positionChange);
    glm_vec3_normalize(positionChange);
    glm_vec3_scale(positionChange, cameraSpeed, positionChange);
    glm_vec3_add(cameraPos, positionChange, cameraPos);
    changed = 1;
  }
  return changed;
}