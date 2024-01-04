#version 330 core

in vec3 position; // Attribute
in vec3 normal; // Attribute

uniform float scale;
uniform vec3 center;
uniform float aspect;

out vec3 fragNormal;

void main(){
    vec3 scaledPos = (position - center) * scale;
    scaledPos.x /= aspect;

    gl_Position = vec4(scaledPos, 1.0);

    fragNormal = normalize(normal);
}
