#version 420 core

// Attributes
layout (location = 0) in vec3 position;    // we can also use layout to specify the location of the attribute
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 normal;


// todo: define all the out variables
out vec3 fragment_normal;
out vec3 frag_pos;
out vec4 fragPosLightSpace;


// todo: define all the uniforms
uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

uniform mat4 lightProjectionMatrix;
uniform mat4 lightViewMatrix;



void main(){
    // todo: fill in vertex shader
     // Transform the position from object (or model) space to clip space. Range [-1,1] in all 3 dimensions
    vec4 pos =  model_matrix * vec4(position, 1.0);
    frag_pos = pos.xyz;                                 // Position in world space. Used for lighting calculations in fragment shader
    gl_Position = projection_matrix * view_matrix * pos;

    // Transform the normal from object (or model) space to world space
    mat4 normal_matrix = transpose(inverse(model_matrix));
    vec3 new_normal = (normal_matrix*vec4(normal,0)).xyz;
    fragment_normal = normalize(new_normal);

    fragPosLightSpace = lightProjectionMatrix * lightViewMatrix * model_matrix * vec4(position, 1.0);
}