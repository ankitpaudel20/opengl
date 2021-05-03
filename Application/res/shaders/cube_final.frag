#version 330 core

struct Material {
    sampler2D texture_diffuse;
    sampler2D texture_specular;
    vec3 specularColor;
    float shininess;
    float ambientStrength;
    float diffuseStrength;
    float specularStrength;
};

struct pointLight {
    vec3 position;
    float intensity;
    vec3 diffusecolor;
    vec3 specularcolor;

    float constant;
    float linear;
    float quadratic;
};

struct DirLight {
    vec3 direction;
    float intensity;

    vec3 diffusecolor;
    vec3 specularcolor;
};


in vec3 f_position;
in vec4 f_color;
in vec3 f_normal;
in vec2 f_TexCoord;
flat in float f_TexID;

out vec4 final_color;



#define NR_POINT_LIGHTS 1
uniform vec3 camPos;
uniform Material material;
uniform DirLight dirLight;
uniform pointLight pointLights[NR_POINT_LIGHTS];
uniform vec3 ambientLight;


vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(pointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    int index=int(f_TexID-1);

    vec3 norm = normalize(f_normal);
    vec3 viewDir = normalize(camPos - f_position);

    vec3 result=vec3(0);

    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], norm, f_position, viewDir);

    //result+=CalcDirLight(dirLight,norm,viewDir);

    final_color = vec4(result, 1.0);
}

vec3 CalcPointLight(pointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = -normalize(fragPos-light.position );

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    //vec3 ambient =vec3(texture(material.texture_diffuse, f_TexCoord));
    vec3 ambient =attenuation * light.intensity * ambientLight * material.ambientStrength * vec3(texture(material.texture_diffuse, f_TexCoord));
    //vec3 ambient =attenuation * light.intensity * ambientLight * material.ambientStrength *  vec3(1.0,0.0,0.0);
    //vec3 diffuse = vec3(texture(material.texture_diffuse, f_TexCoord));
    vec3 diffuse =attenuation * light.intensity * light.diffusecolor * material.diffuseStrength * diff * vec3(texture(material.texture_diffuse, f_TexCoord));
    //vec3 diffuse =attenuation * light.intensity * light.diffusecolor * material.diffuseStrength * diff * vec3(1.0,0.0,0.0);
    //vec3 specular =  vec3(texture(material.texture_specular, f_TexCoord));
    vec3 specular = attenuation * light.intensity * light.specularcolor * material.specularStrength * spec * material.specularColor * vec3(texture(material.texture_specular, f_TexCoord));
    //vec3 specular = attenuation * light.intensity * light.specularcolor * material.specularStrength * spec * material.specularColor * vec3(1.0,1.0,1.0);
    return (ambient + diffuse + specular);
    //return vec3(f_TexCoord,0.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 ambient = light.intensity * ambientLight * material.ambientStrength * vec3(texture(material.texture_diffuse, f_TexCoord));
    vec3 diffuse =light.intensity * light.diffusecolor * material.diffuseStrength * diff * vec3(texture(material.texture_diffuse, f_TexCoord));
    vec3 specular = light.intensity * light.specularcolor * material.specularStrength * spec * material.specularColor * vec3(texture(material.texture_specular, f_TexCoord));
    return (ambient + diffuse + specular);
}