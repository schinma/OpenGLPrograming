	#version 440

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;

out vec3 ReflectDir;

uniform vec3 WorldCameraPosition;
uniform mat4 mvp;
uniform bool DrawSkyBox;
uniform mat4 ModelMatrix;

void main()
{
	if (DrawSkyBox) {
		ReflectDir = vertexPosition;
	} else {
	
		vec3 P = vec3(ModelMatrix * vec4(vertexPosition, 1.0));
		vec3 N = normalize(vec3(ModelMatrix * vec4(vertexNormal, 0.0)));
		vec3 V = normalize(WorldCameraPosition - P);
		
		ReflectDir = reflect(-V, N);
	}
	
	gl_Position = mvp * vec4(vertexPosition, 1.0);
}

