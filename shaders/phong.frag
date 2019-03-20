#version 440

in vec3 Position; 
in vec3 Normal;

uniform vec3 Kd;
uniform vec3 Ka;
uniform vec3 Ks;
uniform float shininess;

struct LightInfo {
	vec4 Position;
	vec3 Intensity;
};

uniform LightInfo Light[5];

//we need to create at least one "out vec4" for settting fragment colors
layout (location = 0) out vec4 fragColor;  //r,g,b,a

void main()
{

	vec3 L = vec3(0, 0, 0);
	vec3 N = normalize(Normal);
	vec3 V = normalize(-Position);
	vec3 R = vec3(0, 0, 0);

	vec3 finalColor = vec3(0, 0, 0);
	
	vec3 ambient = Light[0].Intensity * Ka;
	
	vec3 diffuse = vec3(0, 0, 0);
	vec3 specular = vec3(0, 0, 0);

	for (int i = 0; i<5; i++) {
		L = normalize(Light[i].Position.xyz - Position);
		R = normalize(reflect(-L, Normal));
		diffuse = Kd * Light[i].Intensity * max(dot(L,N), 0.0);
		specular = Ks * Light[i].Intensity * pow(max(dot(R,V), 0.0), shininess);
		finalColor += diffuse + specular;
	}
	
	finalColor += ambient;
	
	fragColor = vec4(finalColor, 1.0);
}