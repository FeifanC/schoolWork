#version 330 core

in vec3 frag_pos; // World space position from vertex shader
in vec3 fragment_normal;

out vec4 frag_color;

uniform vec3 eye_pos;
uniform vec4 light_pos; // Light position or direction (homogeneous coordinates)
uniform float ambient_intensity; // Ambient light intensity
uniform vec3 material_color; // Object's material color
uniform vec3 specular_color; // Specular reflection color
uniform float shininess; // Shininess of the object
uniform float K_s; // Specular reflection coefficient

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

    // Calculate the half vector between light and view direction
    vec3 view_dir = normalize(eye_pos - frag_pos);
    vec3 half_vector = normalize(light_dir + view_dir);

    // Calculate diffuse reflection
    vec3 diffuse_reflection = material_color * clamp(dot(normal, light_dir), 0, 1);

    // Calculate specular reflection using Blinn-Phong model
    vec3 specular_reflection = specular_color * pow(clamp(dot(normal, half_vector), 0, 1), shininess);

    // Calculate ambient light component
    vec3 ambient_light = ambient_intensity * material_color;

    // Compute the final color using the combination of components
    vec3 final_color = ambient_light + diffuse_reflection + K_s * specular_reflection;


    frag_color = vec4(final_color, 1.0);
}
