#version 330 core
layout(location = 0) in vec4 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texCoord;
layout(location = 3) in vec3 v_tangent;
layout(location = 4) in vec3 v_bitangent;


out vec3 f_position;
out vec3 f_normal;
out vec2 f_texCoord;
out vec3  f_tangent;
out vec3 f_bitangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
    gl_Position = proj * view * model * v_position;

    f_position = (model * v_position).xyz;
    f_normal = mat3(transpose(inverse(model)))*v_normal;
    f_texCoord = v_texCoord;
     f_tangent=v_tangent;
    f_bitangent=v_bitangent;
}
