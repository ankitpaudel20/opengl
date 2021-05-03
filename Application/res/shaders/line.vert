#version 440 core
layout(location = 0) in vec4 v_position;
layout(location = 1) in vec4 v_color;
layout(location = 2) in vec4 v_normal;

out vec4 f_color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
    float a = 1.0;
    gl_Position = proj * view * model * v_position;    
    f_color=v_color;  
}