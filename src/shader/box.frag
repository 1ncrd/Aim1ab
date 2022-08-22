#version 330 core

struct Material {
    sampler2D diffuseTex;
    sampler2D specularTex;
    float shininess;
};

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    // attenuation
    float constant;
    float linear;
    float quadratic;
};

struct DirectLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 fragPos;
in vec3 normal;
in vec2 TexCoords;

uniform vec3 cameraPos;

uniform Material material;
uniform PointLight pointLight[4];
uniform DirectLight directLight;

out vec4 FragColor;

vec3 calcDirectLight(DirectLight light, vec3 normal, vec3 viewDir);
vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{   
    vec3 normal_n = normalize(normal);
    vec3 viewDir = normalize(fragPos - cameraPos);
    vec3 result = calcDirectLight(directLight, normal_n, viewDir);
    for (int i = 0; i < 4; i++)
    {
        result += calcPointLight(pointLight[i], normal_n, fragPos, viewDir);
    }
    FragColor = vec4(result, 1.0);
}

vec3 calcDirectLight(DirectLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir_n = normalize(light.direction);
    float diff = max(dot(-lightDir_n, normal), 0.0f);
    vec3 reflectDir = normalize(reflect(lightDir_n, normal));
    float spec = pow(max(dot(-viewDir, reflectDir), 0.0f), material.shininess);
  
    vec3 ambient = light.ambient * vec3(texture(material.diffuseTex, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuseTex, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specularTex, TexCoords));
    
    return (ambient + diffuse + specular);
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir_n = normalize(fragPos - light.position);
    float diff = max(dot(-lightDir_n, normal), 0.0f);
    vec3 reflectDir = normalize(reflect(lightDir_n, normal));
    float spec = pow(max(dot(-viewDir, reflectDir), 0.0f), material.shininess);

    float d = distance(light.position, fragPos);
    float attenuation = 1 / (light.constant + d * light.linear + d * d * light.quadratic);
    
    vec3 ambient = light.ambient * vec3(texture(material.diffuseTex, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuseTex, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specularTex, TexCoords));
    
    return (ambient + diffuse + specular) * attenuation;
}