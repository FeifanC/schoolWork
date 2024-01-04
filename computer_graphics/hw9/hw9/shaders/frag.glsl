#version 420 core

in vec3 frag_pos; // World space position from vertex shader
in vec3 fragment_normal;
in vec3 fragColor;
in vec2 fragUV;

out vec4 outColor;

uniform vec3 eye_pos;
layout( binding = 1 ) uniform samplerCube cubeMapTex;
layout( binding = 0 ) uniform sampler2D tex;
uniform float texType;

void main(){
    // environment texture
    vec3 N = normalize(fragment_normal);
    vec3 V = normalize(eye_pos - frag_pos);
    vec3 R = reflect(-V, N);
    //vec3 envColor = texture(cubeMapTex, R).rgb;

     //2d tex
    //vec3 materialColor = texture(tex, fragUV).rgb;

    if(texType == 1.0){
        vec3 envColor = texture(cubeMapTex, R).rgb;
        outColor = vec4(envColor, 1.0);
    }
    else if(texType == 0.0)
    {
        vec3 materialColor = texture(tex, fragUV).rgb;
        outColor = vec4(materialColor, 1.0 );
    }
    else{
        vec3 materialColor = texture(tex, fragUV).rgb;
        vec3 envColor = texture(cubeMapTex, R).rgb;
        vec3 color_mix = mix(materialColor, envColor, 0.2);
        outColor = vec4(color_mix, 1.0);
    }
}
