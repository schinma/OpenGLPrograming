#version 440

layout(location = 0) in vec3 vertexPosition;  //set the attr number set in the glVertexAttribPointer function
layout(location = 1) in vec3 vertexNormal;	  // same

uniform mat4 mvp;
uniform float u_offset;

void main()
{
	//we need to put NDC coordinates int gl_position

	gl_Position = mvp * vec4(vertexPosition + vertexNormal * u_offset, 1.0);
}