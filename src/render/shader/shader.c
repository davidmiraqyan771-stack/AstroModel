#include "../../includes/shader.h"
#include <stdio.h>
#include <stdlib.h>
#include "../../includes/dynamicMem.h"

static char *readFile(const char *path)
{
    FILE *file = fopen(path, "r");
    if (file == NULL)
    {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    char *data;

    fseek(file, 0, SEEK_END);
    long len = ftell(file);
    rewind(file);

    data = (char *)creDy(len + 1, sizeof(char));
    fread(data, sizeof(char), len, file);
    data[len] = '\0';
    fclose(file);
    return data;
}

Shader shaderProgramCreate(const char *vertexPath, const char *fragmentPath)
{
    Shader prog;
    char *vertexSource = readFile(vertexPath);
    char *fragmentSource = readFile(fragmentPath);
    const char *vsrc[] = {vertexSource};
    const char *fsrc[] = {fragmentSource};

    GLuint vertexShader, fragmentShader;
    int success;
    char infoLog[512];

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertexShader, 1, vsrc, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(vertexShader, sizeof(infoLog), NULL, infoLog);
        printf("Vertex shader compilation error:\n%s", infoLog);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        free(vertexSource);
        free(fragmentSource);
        exit(EXIT_FAILURE);
    }

    glShaderSource(fragmentShader, 1, fsrc, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, sizeof(infoLog), NULL, infoLog);
        printf("Fragment shader compilation error:\n%s", infoLog);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        free(vertexSource);
        free(fragmentSource);
        exit(EXIT_FAILURE);
    }

    prog.id = glCreateProgram();
    glAttachShader(prog.id, vertexShader);
    glAttachShader(prog.id, fragmentShader);
    glLinkProgram(prog.id);
    glGetProgramiv(prog.id, GL_LINK_STATUS, &success);

    if (!success)
    {
        glGetProgramInfoLog(prog.id, sizeof(infoLog), NULL, infoLog);
        printf("Shader program link error:\n%s", infoLog);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        free(vertexSource);
        free(fragmentSource);
        glDeleteProgram(prog.id);
        exit(EXIT_FAILURE);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    free(vertexSource);
    free(fragmentSource);
    return prog;
}

void programUse(const Shader *prog)
{
    static GLuint currentId = 0;
    if (prog->id != currentId)
    {
        glUseProgram(prog->id);
        currentId = prog->id;
    }
}

void setFloatUniform(const Shader *prog, const char *name, float value)
{
    glUniform1f(glGetUniformLocation(prog->id, name), value);
}

// glUniformMatrix4fv

void setMat4Uniform(const Shader *prog, const char *name, mat4 value)
{
    GLuint loc = glGetUniformLocation(prog->id, name);
    if (loc == -1)
    {
        printf("Uniform '%s' not found!\n", name);
    }
    glUniformMatrix4fv(loc, 1, GL_FALSE, (float *)value);
}