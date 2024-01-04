#version 330 core

in vec3 position;// Attribute
in vec3 normal;// Attribute

uniform mat4 model_mat;
uniform mat4 view_mat;
uniform mat4 projection_mat;

out vec3 fragNormal;

void main(){

    mat4 normal_matrix = transpose(inverse(model_mat));
    vec3 new_normal = (normal_matrix*vec4(normal,0)).xyz;
    fragNormal = normalize(new_normal);

    gl_Position = projection_mat * view_mat * model_mat * vec4(position, 1.0);

}
