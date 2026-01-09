#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include "./includes/shader.h"
#include "./includes/dynamicMem.h"
#include "./includes/buffer.h"
#include "./includes/sleep.h"
#define START_WIDTH 800
#define START_HEIGHT 600
#define FRAME_TIME 1.0 / 60.0
#define PI 3.14159265359

mat4 viewMatrix;
mat4 projectionMatrix;
float viewSize = 20.0f;
Shader shaderProgram;

RenderParticle *buffers[BUF_COUNT];
atomic_int guardIndex;
char appRunning = 1;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

int main(void)
{

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(START_WIDTH, START_HEIGHT, "AstroMod", NULL, NULL);
    if (window == NULL)
    {
        perror("window");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        perror("GLAD");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    initPhysicsData();

    pthread_t physThreadId;

    if (pthread_create(&physThreadId, NULL, physicCaluclate, NULL) != 0)
    {
        perror("Thread filed");

        exit(EXIT_FAILURE);
    }

    float vertices[] = {
        -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,
        0.5f, -0.5f, 1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, 1.0f, 1.0f, 1.0f};

    GLuint indices[] = {
        0, 1, 2,
        2, 3, 0};

    shaderProgram = shaderProgramCreate("../render/shader/GLSLcodes/vertexShader.glsl", "../render/shader/GLSLcodes/fragmentShader.glsl");

    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glViewport(0, 0, 800, 600);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // vec4 vector = {1.0f, 0.0f, 0.0f, 1.0f};
    glm_mat4_identity(projectionMatrix);
    float aspect = START_WIDTH / (float)START_HEIGHT;
    if (aspect >= 1.0f)
    {
        glm_ortho(
            -viewSize * aspect / 2.0f, viewSize * aspect / 2.0f,
            -viewSize / 2.0f, viewSize / 2.0f,
            -1.0f, 1.0f,
            projectionMatrix);
    }
    else
    {
        glm_ortho(
            -viewSize / 2.0f, viewSize / 2.0f,
            -viewSize / aspect / 2.0f, viewSize / aspect / 2.0f,
            -1.0f, 1.0f,
            projectionMatrix);
    }

    glm_mat4_identity(viewMatrix);

    int readIndex = 1;

    programUse(&shaderProgram);
    setMat4Uniform(&shaderProgram, "projection", projectionMatrix);
    setMat4Uniform(&shaderProgram, "view", viewMatrix);
    setFloatUniform(&shaderProgram, "aspect", aspect);

    double lastTime = glfwGetTime();


    while (!glfwWindowShouldClose(window))
    {
        double startTime = glfwGetTime();

        processInput(window);

        readIndex = atomic_exchange(&guardIndex, readIndex);
        RenderParticle *drawData = buffers[readIndex];

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        programUse(&shaderProgram);

        for (int i = 0; i < PARTICLE_COUNT; ++i)
        {
            mat4 model;
            glm_mat4_identity(model);
            glm_translate(model, (float[]){drawData[i].x, drawData[i].y, 0.0f});
            float scale = drawData[i].r;
            glm_scale(model, (vec3){scale, scale, 1.0f});
            setMat4Uniform(&shaderProgram, "model", model);
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }
        float r = sqrt(drawData[2].x*drawData[2].x + drawData[2].y * drawData[2].y);
        printf("x: %f, y: %f r: %f\n",drawData[2].x, drawData[2].y, r);
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

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);

    float aspect = width / (float)height;

    glm_mat4_identity(projectionMatrix);

    if (aspect >= 1.0f)
    {
        glm_ortho(
            -viewSize * aspect / 2.0f, viewSize * aspect / 2.0f,
            -viewSize / 2.0f, viewSize / 2.0f,
            -1.0f, 1.0f,
            projectionMatrix);
    }
    else
    {
        glm_ortho(
            -viewSize / 2.0f, viewSize / 2.0f,
            -viewSize / aspect / 2.0f, viewSize / aspect / 2.0f,
            -1.0f, 1.0f,
            projectionMatrix);
    }

    programUse(&shaderProgram);
    setMat4Uniform(&shaderProgram, "projection", projectionMatrix);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}