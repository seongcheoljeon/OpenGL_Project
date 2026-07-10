#version 460 core

in vec3 normal;
in vec2 texcoord;
in vec3 position;
out vec4 frag_color;

uniform vec3 view_pos;

struct Light
{
    vec3 position;
    vec3 direction;
    vec2 cutoff;
    vec3 attenuation;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

uniform Material material;


void main() {
    vec3 tex_color = texture(material.diffuse, texcoord).xyz;
    vec3 ambient = tex_color * light.ambient;

    float dist = length(light.position - position);
    vec3 dist_poly = vec3(1.0, dist, dist * dist);
    // atten = (1 * Kc) + (d * Kl) + (d * d * Kq)
    float attenuation = 1.0 / dot(dist_poly, light.attenuation);
    vec3 light_dir = (light.position - position) / dist;

    vec3 result = ambient;
    float theta = dot(light_dir, normalize(-light.direction));
    float intensity = clamp((theta - light.cutoff[1]) / (light.cutoff[0] - light.cutoff[1]), 0.0f, 1.0f);

    if(intensity > 0.0f)
    {
        vec3 pixel_normal = normalize(normal);
        float diff = max(dot(pixel_normal, light_dir), 0.0);
        vec3 diffuse = diff * tex_color * light.diffuse;

        vec3 spec_color = texture(material.specular, texcoord).xyz;
        vec3 view_dir = normalize(view_pos - position);
        vec3 reflect_dir = reflect(-light_dir, pixel_normal);
        float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
        vec3 specular = spec * spec_color * light.specular;

        result += (diffuse + specular) * intensity;
    }

    result *= attenuation;

    frag_color = vec4(result, 1.0);
}