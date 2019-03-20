#version 440 

in vec3 Position;  //position in camera coordinate
in vec3 Normal;	   //normals

uniform vec3 Kd;
uniform vec3 Ka;
uniform vec3 Ks;
uniform float shininess;

struct LightInfo {
	vec4 Position;
	vec4 Direction;
	vec3 Intensity; //color
	float exponent;
	float cutOff;
	float innerCutOff;
};

uniform LightInfo Light[5];

//we need to create at least one "out vec4" for settting fragment colors
layout (location = 0) out vec4 fragColor;  //r,g,b,a

const float a = 1.0f;
const float b = 0.01f;
const float c = 0.001f;

vec3 calculateSpot()
{
	vec3 color;

	for (int i = 0; i<5; i++) {
		vec3 L = Light[i].Position.xyz - Position;
		float d = length(L);
		float attenuation = 1.0f / (a + b*d + c*d*d);
	
		L = normalize(L);
		float spotDir = dot(-L, normalize(Light[i].Direction.xyz));
		float angle = acos(spotDir); //radian
		float spotAttenuation;

		if (angle < radians(Light[i].innerCutOff)) {
			 // position is inside the inner cone 
			 spotAttenuation = 1.0f;

		} else {
			float spotValue = smoothstep(cos(radians(Light[i].cutOff)), cos(radians(Light[i].innerCutOff)), spotDir);
			spotAttenuation = pow(spotValue, Light[i].exponent);
		}

		attenuation *= spotAttenuation;

		// blinn phong specular
		vec3 V = normalize(-Position);
		vec3 H = normalize(V+L);
		//phong
		float dotNL = max(dot(Normal, L), 0);
		float dotHN = max(dot(H, Normal), 0);
		float pf = pow(dotHN, shininess); //specular term

		vec3 diffuse = Kd * Light[i].Intensity * dotNL * attenuation;
		vec3 specular = Ks * Light[i].Intensity * pf * attenuation;
		color += diffuse + specular;
	}

	vec3 ambient = Ka * Light[0].Intensity;
	color += ambient;

	return color;
}

void main()
{	
	fragColor = vec4(calculateSpot(), 1.0f);
}