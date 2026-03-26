#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <cglm/cglm.h>

typedef struct {
    GLuint id;
} Shader;

Shader shaderProgramCreate(const char* vertexPath, const char* fragmentPath);
void programUse(const Shader* shaderProg);
void setFloatUniform(const Shader* shaderProg, const char* name, float value);
void setMat4Uniform(const Shader *prog, const char *name, mat4 value);

#endif