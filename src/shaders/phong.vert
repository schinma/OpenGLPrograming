#version 440

layout(location = 0) in vec3 vertexPosition;  //set the attr number set in the glVertexAttribPointer function
layout(location = 1) in vec3 vertexNormal;	  // same

out vec3 Position;
out vec3 Normal;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 mvp;

void main()
{
	
	Normal = normalize(NormalMatrix * vertexNormal);
	Position = vec3(ModelViewMatrix * vec4(vertexPosition, 1.0));

	//we need to put NDC coordinates int gl_position
	gl_Position = mvp * vec4(vertexPosition, 1.0);
}