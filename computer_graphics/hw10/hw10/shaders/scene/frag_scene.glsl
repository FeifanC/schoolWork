#version 420 core

// todo: define all the input variables to the fragment shader
in vec3 frag_pos; // World space position from vertex shader
in vec3 fragment_normal;
in vec4 fragPosLightSpace;

// todo: define all the uniforms
uniform vec3 eye_pos;
uniform vec3 light_pos;
uniform vec3 material_color;


layout (binding=0) uniform sampler2D depthTex;  // depth texture bound to texture unit 0
out vec4 outColor;

vec3 computeDiffuse(vec3 N, vec3 L){
      return material_color * clamp(dot(L,N), 0.,1.);
}

void main(){
//    // todo: fill in the fragment shader


    vec3 N = normalize(fragment_normal);

    vec3 L = normalize(light_pos-frag_pos);

    vec3 difuse = computeDiffuse(N, L);

    float bias = max(0.0005f * (1.0 - dot(fragment_normal, L)), 0.0001f);

//    outColor = vec4(difuse, 1.0);

    // Convert homogeneous coordinates to 3D coordinates
    vec3 fragPos3D = fragPosLightSpace.xyz / fragPosLightSpace.w;
    fragPos3D = (fragPos3D + 1.0) / 2.0;


    // Get the depth of the current fragment from the depth texture
    float z_current = fragPos3D.z;
    float z_depthTex = texture(depthTex, fragPos3D.xy).r;



    // Compare the depths to determine if the fragment is in shadow or not
    if (z_current - bias > z_depthTex)
    {
        // Fragment is in shadow, apply shadow color (e.g., black)
        outColor = vec4(0.0, 0.0, 0.0, 1.0);
    }
    else
    {
        // Fragment is not in shadow, apply diffuse color
        vec3 diffuseColor = vec3(difuse); // Replace with your diffuse color
        outColor = vec4(diffuseColor, 1.0);
    }
}

