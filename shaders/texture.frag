#version 440  

in vec3 Position;  //position in camera coordinate
in vec3 Normal;	   //normals
in vec2 TexCoord;  //coordinate for texture 

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

uniform sampler2D Tex1;
uniform sampler2D Tex2;

uniform LightInfo Light[5];

layout (location = 0) out vec4 fragColor;  //r,g,b,a

void main()
{

	vec3 L = vec3(0, 0, 0);
	vec3 N = normalize(Normal);
	vec3 V = normalize(-Position);
	vec3 R = vec3(0, 0, 0);

	vec3 color = vec3(0, 0, 0);
	
	vec3 ambient = Light[0].Intensity * Ka;
	
	vec3 diffuse = vec3(0, 0, 0);
	vec3 specular = vec3(0, 0, 0);
	vec3 totalSpec;

	for (int i = 0; i<5; i++) {
		L = normalize(Light[i].Position.xyz - Position);
		R = normalize(reflect(-L, Normal));
		diffuse = Kd * Light[i].Intensity * max(dot(L,N), 0.0);
		specular = Ks * Light[i].Intensity * pow(max(dot(R,V), 0.0), shininess);
		color += diffuse;
		totalSpec += specular; 
	}
	
	color += ambient;

	vec4 texColor = texture(Tex1, TexCoord);
	vec4 texColor2 = texture(Tex2, TexCoord);
	vec4 texColor3 = mix(texColor2, texColor, texColor2.a);

	fragColor = (vec4(color, 1.0) * texColor3) + vec4(totalSpec, 1.0);
}