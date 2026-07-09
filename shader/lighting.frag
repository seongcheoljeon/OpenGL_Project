#version 330 core

in vec3 normal;
in vec3 texcoord;
in vec3 position;
out vec4 frag_color;

uniform vec3 view_pos;

struct Light
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Material material;


void main() {
    vec3 ambient = material.ambient * light.ambient;

    vec3 light_dir = normalize(light.position - position);
    vec3 pixel_normal = normalize(normal);
    float diff = max(dot(pixel_normal, light_dir), 0.0);
    vec3 diffuse = diff * material.diffuse * light.diffuse;

    vec3 view_dir = normalize(view_pos - position);
    vec3 reflect_dir = reflect(-light_dir, pixel_normal);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
    vec3 specular = spec * material.specular * light.specular;

    vec3 result = ambient + diffuse + specular;

    frag_color = vec4(result, 1.0);
}