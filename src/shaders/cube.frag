#version 440

in vec3 LightIntensity; //getting color data from vertex shader

//we need to create at least one "out vec4" for settting fragment colors
layout (location = 0) out vec4 fragColor;  //r,g,b,a

void main()
{
		fragColor = vec4(LightIntensity, 1.0);
}