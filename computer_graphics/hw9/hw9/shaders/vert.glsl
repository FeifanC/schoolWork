#version 330 core

// Attributes
layout (location = 0) in vec3 position;    // we can also use layout to specify the location of the attribute
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 color;
layout (location = 3) in vec3 normal;

out vec3 frag_pos; // Pass world space position to fragment shader
out vec3 fragment_normal;
out vec3 fragColor;
out vec2 fragUV;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

void main() {
    // Transform position from object space to world space
    vec4 world_pos = model_matrix * vec4(position, 1.0);

    // Pass world space position to fragment shader
    frag_pos = world_pos.xyz;

    // Transform position to clip coordinates
    gl_Position = projection_matrix * view_matrix * world_pos;


    // Transform the normal from object (or model) space to world space
    mat4 normal_matrix = transpose(inverse(model_matrix));
    vec3 new_normal = (normal_matrix*vec4(normal,0)).xyz;
    fragment_normal = normalize(new_normal);

    fragUV = uv;
    fragColor = color;
}