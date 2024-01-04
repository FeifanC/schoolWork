#version 330 core

in vec3 position;// Attribute
in vec3 normal;// Attribute

uniform mat4 model_mat;
uniform mat4 view_mat;
uniform mat4 projection_mat;

out vec3 fragNormal;

void main(){
    //    vec3 scaledPos = (position - center) * scale;
    //    scaledPos.x /= aspect;
    //    scaledPos.z = -scaledPos.z;
    //
    //    gl_Position = vec4(scaledPos, 1.0);
    //
    //    fragNormal = normalize(normal);


//    // Transform the position from object space (a.k.a model space) to clip space. The range of clip space is [-1,1] in all 3 dimensions.
//    vec4 pos = model_matrix * vec4(position, 1.0);
//    pos.x = pos.x / aspect;// Correction for aspect ratio (optional)
//    gl_Position = pos;
//    // Transform the normal from object (or model) space to world space
//    mat4 normal_matrix = transpose(inverse(model_matrix));
//    vec3 new_normal = (normal_matrix*vec4(normal, 0)).xyz;
//    fragNormal = normalize(new_normal);

    mat4 normal_matrix = transpose(inverse(model_mat));
    vec3 new_normal = (normal_matrix*vec4(normal,0)).xyz;
    fragNormal = normalize(new_normal);

    gl_Position = projection_mat * view_mat * model_mat * vec4(position, 1.0);

}
