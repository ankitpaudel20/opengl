#version 330 core
layout(location = 0) in vec4 v_position;
layout(location = 1) in vec4 v_color;
layout(location = 2) in vec3 v_normal;
layout(location = 3) in vec2 v_TexCoord;
layout(location = 4) in float v_TexID;

out vec3 f_position;
out vec4 f_color;
out vec3 f_normal;
out vec2 f_TexCoord;
flat out float f_TexID;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
    gl_Position = proj * view * model * v_position;

    f_position = (model * v_position).xyz;
    f_color=v_color;
    f_normal = mat3(transpose(inverse(model)))*v_normal;
    f_TexCoord = v_TexCoord;
    f_TexID=v_TexID;
}
