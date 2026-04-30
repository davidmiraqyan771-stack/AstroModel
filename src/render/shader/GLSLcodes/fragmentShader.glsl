#version 330 core
out vec4 fragColor;
in vec3 ourColor;
in vec2 fragPos;

void main()
{
    float dist = length(fragPos);
    float radius = 0.5;
    
    if (dist > radius) {
        discard;
    }

    fragColor = vec4(ourColor, 1.0);
}