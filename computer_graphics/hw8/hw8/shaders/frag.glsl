#version 330 core

in vec3 frag_pos; // World space position from vertex shader
in vec3 fragment_normal;

out vec4 frag_color;

uniform vec4 light_pos; // Light position or direction (homogeneous coordinates)
uniform vec3 material_color; // Object's material color
uniform vec3 eye_pos;
uniform bool silhouette_edge_enabled;
uniform float toon_shading_enabled;

void main() {
    // Calculate light direction based on light type (point or directional)
    vec3 light_dir;

    if (light_pos.w == 1.0) {
        light_dir = normalize(light_pos.xyz - frag_pos);
    } else {
        light_dir = normalize(light_pos.xyz);
    }

    // Calculate the normalized fragment normal (you need to pass this as an attribute)
    vec3 normal = normalize(fragment_normal);

    // Calculate the diffuse intensity
    float intensity = dot(light_dir, normal);

    // Define intensity levels
    float level1 = 0.85; // High intensity
    float level2 = 0.5;
    float level3 = 0.25;
    float level4 = 0.1;
    float level5 = 0.0; // Low intensity

    // Map the intensity to discrete levels
    if (intensity > level1) {
        intensity = 1.0;
    } else if (intensity > level2) {
        intensity = 0.7;
    } else if (intensity > level3) {
        intensity = 0.5;
    } else if (intensity > level4) {
        intensity = 0.3;
    } else {
        intensity = 0.1;
    }

    vec3 final_color;

    if(toon_shading_enabled == 0)
        final_color = intensity * material_color;
    else
        final_color = material_color * clamp(dot(normal, light_dir), 0, 1);

    if(silhouette_edge_enabled && dot(normal, normalize(eye_pos - frag_pos)) < 0.2)
        final_color = vec3(0,0,0);

    frag_color = vec4(final_color, 1.0);
}
