#version 330 core

in vec3 fragNormal;

out vec4 outColor;

void main(){
    vec3 normalizedNormal = normalize(fragNormal);

    vec3 color = abs(normalizedNormal);

    outColor = vec4(color, 1.0);
}