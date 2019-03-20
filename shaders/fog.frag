#version 440  

in vec3 Position;  //position in camera coordinate
in vec3 Normal;	   //normals

uniform vec3 Kd;
uniform vec3 Ka;
uniform vec3 Ks;
uniform float shininess;

uniform float resolution;
uniform int nbLight;

struct LightInfo {
	vec4 Position;
	vec4 Direction;
	vec3 Intensity; //color
	float exponent;
	float cutOff;
	float innerCutOff;
};

uniform LightInfo Light[5];

layout (location = 0) out vec4 fragColor;  //r,g,b,a

vec3 calculatePhong()
{
	vec3 L = vec3(0, 0, 0);
	vec3 N = normalize(Normal);
	vec3 V = normalize(-Position);
	vec3 R = vec3(0, 0, 0);

	vec3 color = vec3(0, 0, 0);
	
	vec3 ambient = Light[0].Intensity * Ka;
	
	vec3 diffuse = vec3(0, 0, 0);
	vec3 specular = vec3(0, 0, 0);

	for (int i = 0; i<5; i++) {
		L = normalize(Light[i].Position.xyz - Position);
		R = normalize(reflect(-L, Normal));
		diffuse = Kd * Light[i].Intensity * max(dot(L,N), 0.0);
		specular = Ks * Light[i].Intensity * pow(max(dot(R,V), 0.0), shininess);
		color += diffuse + specular;
	}
	
	color += ambient;


	return color;
}

vec3 calculateToon()
{
	const int levels = 4;
	const float scaleFactor = 1.0 / levels;
	vec3 color;

	for (int i = 0; i<5; i++) {

		vec3 L = Light[i].Position.xyz - Position;
	
		L = normalize(L);	
		float cos = max(dot(Normal, L), 0);
		float value = floor(cos * levels) * scaleFactor;
		vec3 diffuse = Kd * Light[i].Intensity * value;
		color += diffuse;
	}

	vec3 ambient = Ka * Light[0].Intensity;
	color += ambient;

	return color;
}

void main()
{
	vec3 color;
	vec3 fogColor = vec3(0.5, 0.5, 0.5);
	float maxDist = 50;
	float minDist = 0.5;

	color = calculatePhong();

	float f = (maxDist - abs(Position.z)) / (maxDist - minDist);

	f = clamp(f, 0.0, 1.0);

	fragColor = vec4(mix(fogColor, color,f), 1.0f);
}