#version 430 core
in vec2 fragPos;
in vec3 color;
in vec4 viewSpacePos;
in float sphereRadius;

out vec4 FragColor;

uniform mat4 projection;

void main() {
    float distSq = dot(fragPos, fragPos);
    if (distSq > 0.25) discard;

    float z = sqrt(0.25 - distSq);

    vec3 normal = normalize(vec3(fragPos, z));

    vec3 lightDir = normalize(vec3(1.0, 0.0, 1.0));
    float diff = max(dot(normal, lightDir), 0.0);
    float ambient = 0.1;

    vec3 finalColor = color * (ambient + diff);
    FragColor = vec4(finalColor, 1.0);

    
    vec4 pixelViewPos = viewSpacePos;
    pixelViewPos.z += z * 2.0 * sphereRadius;
    
    vec4 clipPos = projection * pixelViewPos;
    
    float ndcDepth = clipPos.z / clipPos.w;
    
    gl_FragDepth = ((gl_DepthRange.diff * ndcDepth) + gl_DepthRange.near + gl_DepthRange.far) / 2.0;
}