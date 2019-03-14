#version 440

layout(location = 0) in vec3 vertexPosition;  //set the attr number set in the glVertexAttribPointer function
layout(location = 1) in vec3 vertexNormal;	  // same

out vec3 LightIntensity; //Vertex Color

uniform vec4 LightLocation;
uniform vec3 Kd;
uniform vec3 Ld;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 mvp;

void main()
{
	vec3 N = normalize(NormalMatrix * vertexNormal);
	vec4 P = ModelViewMatrix * vec4(vertexPosition,  1.0);
	vec3 L = normalize(LightLocation - P).xyz;

	LightIntensity = Kd * Ld * max(dot(L,N), 0.0);
	
	//we need to put NDC coordinates int gl_position
	gl_Position = mvp * vec4(vertexPosition, 1.0);
}