#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;
out vec3 ourColor;
out vec2 fragPos;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    fragPos = aPos;
    gl_Position = projection * view * model * vec4(aPos, 0.0, 1.0);
    ourColor = aColor;
}