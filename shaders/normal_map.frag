#version 440  

in vec3 L;
in vec3 V;
in vec2 TexCoord;  //coordinate for texture

uniform vec3 Ka;
uniform vec3 Ks;
uniform float shininess;

struct LightInfo {
	vec4 Position;
	vec4 Direction;
	vec3 Intensity; //color
};

uniform sampler2D ColorTex;
uniform sampler2D NormalMapTex;

uniform LightInfo Light[1];

layout (location = 0) out vec4 fragColor;  //r,g,b,a

void main()
{
	vec4 N = texture(NormalMapTex, TexCoord);
	vec3 R = normalize(reflect(-L, N.xyz));
	vec4 Kd = texture(ColorTex, TexCoord);

	vec3 diffuseColor = Light[0].Intensity * Kd.xyz * max(dot(L,N.xyz), 0.0);
	vec3 specularColor = Light[0].Intensity * Ks * pow(max(dot(R, V), 0.0), shininess);
	vec3 ambientColor = Light[0].Intensity * Ka;

	fragColor = vec4(diffuseColor + ambientColor + specularColor, 1.0);
}