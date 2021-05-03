#version 440 core

struct Material {
    sampler2D diffuse_map;
    sampler2D specular_map;    
    float shininess;
}; 

struct Light {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    vec3 ambient;
    float ambientStrength;
    vec3 diffuse;
    float diffuseStrength;
    vec3 specular;
    float specularStrength;
    
    float constant;
    float linear;
    float quadratic;
};
  

in vec3 f_pos;
in vec4 f_color;
in vec3 f_normal;
in vec2 f_TexCoord;
flat in float f_TexID;

out vec4 color;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;  



void main()
{
    vec3 lightDir = normalize(light.position - f_pos);
  
        // ambient
    vec3 ambient = light.ambient*light.ambientStrength * vec3(texture(material.diffuse_map, f_TexCoord));
        
        // diffuse 
    vec3 norm = normalize(f_normal);
        float diff = max(dot(norm, lightDir), 0.0);
            vec3 diffuse =light.diffuse*light.diffuseStrength * diff * vec3(texture(material.diffuse_map, f_TexCoord)); 

        // specular
        vec3 viewDir = normalize(viewPos - f_pos);
        vec3 reflectDir = reflect(-lightDir, norm);  
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular *light.specularStrength* spec * vec3(texture(material.specular_map, f_TexCoord));  
        
    // spotlight (soft edges)
    float theta     = dot(lightDir, normalize(-light.direction));
    float epsilon   = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
      diffuse  *= intensity;
    specular *= intensity;

        // attenuation
        float distance = length(light.position - f_pos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

        ambient  *= attenuation; 
        diffuse   *= attenuation;
        specular *= attenuation;   

         color = vec4((ambient + diffuse + specular),1.0);
    
}
  