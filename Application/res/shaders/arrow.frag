#version 440 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 
  

in vec4 f_color;
in vec3 f_normal;
in vec3 f_pos;

out vec4 color;

uniform vec3 lightPos;  
uniform vec3 lightColor;
uniform vec3 viewPos;
uniform float ambientStrength;
uniform float specularStrength;
uniform float shineness;

uniform Material material;


void main()
{
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(f_normal);
    vec3 lightDir = normalize(lightPos - f_pos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor.xyz;

    vec3 viewDir = normalize(viewPos - f_pos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shineness);
    vec3 specular = specularStrength * spec * lightColor;  

    vec3 objectColor= f_color.xyz;
    vec3 result = (ambient + diffuse + specular) * objectColor;
	color = vec4(result,f_color.w);
}
