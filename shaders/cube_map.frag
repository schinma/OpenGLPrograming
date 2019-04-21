#version 440  

in vec3 ReflectDir;

uniform bool DrawSkyBox;
uniform float ReflectFactor;
uniform vec3 MaterialColor;

uniform samplerCube CubeMapTex;

layout (location = 0) out vec4 fragColor;  //r,g,b,a

void main()
{
	vec4 cubeMapColor = texture(CubeMapTex, ReflectDir);
	
	if (DrawSkyBox) {
		fragColor = cubeMapColor;
	} else {		
		fragColor = mix(vec4(MaterialColor, 1.0), cubeMapColor, ReflectFactor);
	}
}
	