#version 430 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;

out vec2 fragPos;
out vec3 color;
out vec4 viewSpacePos;
out float sphereRadius;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    fragPos = aPos; 
    color = aColor;
    
    vec4 viewCenter = view * model * vec4(0.0, 0.0, 0.0, 1.0);
    float scale = length(model[0]);
    sphereRadius = scale / 2.0;

    viewSpacePos = viewCenter + vec4(aPos * scale, 0.0, 0.0);
    
    gl_Position = projection * viewSpacePos;
}