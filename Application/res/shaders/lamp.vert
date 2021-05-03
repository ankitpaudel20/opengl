#version 330 core
layout(location = 0) in vec4 v_position;
layout(location = 1) in vec4 v_color;
layout(location = 2) in vec3 v_normal;
layout(location = 3) in vec2 v_TexCoord;
layout(location = 4) in float v_TexID;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform vec3 color;

out vec3 f_color;

void main()
{
	gl_Position = proj * view * model * v_position;
	if((color.x==color.y) && (color.y==color.z))
		f_color=vec3(1);
	else
		f_color=color;
}