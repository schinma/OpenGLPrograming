#version 440  

//we need to create at least one "out vec4" for settting fragment colors
layout (location = 0) out vec4 fragColor;  //r,g,b,a

uniform vec3 u_color;

void main()
{
	fragColor = vec4(u_color, 1.0f);
}