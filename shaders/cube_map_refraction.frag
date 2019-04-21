#version 440  

in vec3 ReflectDir;
in vec3 RefractDirR;
in vec3 RefractDirG;
in vec3 RefractDirB;
in float RefractFactor;

uniform bool DrawSkyBox;

uniform samplerCube CubeMapTex;

layout (location = 0) out vec4 fragColor;  //r,g,b,a

void main()
{
	vec4 refractColor;
	vec4 reflectColor = texture(CubeMapTex, ReflectDir);

	refractColor.r = texture(CubeMapTex, RefractDirR).r;
	refractColor.g = texture(CubeMapTex, RefractDirG).g;
	refractColor.b = texture(CubeMapTex, RefractDirB).b;
	refractColor.a = 1.0;

	if (DrawSkyBox) {
		fragColor = reflectColor;
	} else {		
		fragColor = mix(refractColor, reflectColor, RefractFactor);
	}
}
	