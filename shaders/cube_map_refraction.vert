	#version 440

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;

out vec3 ReflectDir;
out vec3 RefractDirR;
out vec3 RefractDirG;
out vec3 RefractDirB;
out float RefractFactor;

uniform vec3 WorldCameraPosition;
uniform mat4 mvp;
uniform bool DrawSkyBox;
uniform mat4 ModelMatrix;

uniform float EtaR;
uniform float EtaG;
uniform float EtaB;

void main()
{
	if (DrawSkyBox) {
		ReflectDir = vertexPosition;
	} else {
	
		vec3 P = vec3(ModelMatrix * vec4(vertexPosition, 1.0));
		vec3 N = normalize(vec3(ModelMatrix * vec4(vertexNormal, 0.0)));
		vec3 V = normalize(WorldCameraPosition - P);
		
		ReflectDir = reflect(-V, N);
		RefractDirR = refract(-V, N, EtaR);
		RefractDirG = refract(-V, N, EtaG);
		RefractDirB = refract(-V, N, EtaB);

		float n1 = 1.0;
		float n2 = 1.52;

		float r0 = pow((n1-n2), 2) / pow((n1+n2), 2);

		RefractFactor = r0 + (1-r0) * pow((1 - dot(V,N)), 5);	
	}
	
	gl_Position = mvp * vec4(vertexPosition, 1.0);
}

