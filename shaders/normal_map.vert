#version 440

layout(location = 0) in vec3 vertexPosition;  //set the attr number set in the glVertexAttribPointer function
layout(location = 1) in vec3 vertexNormal;	  // same
layout(location = 2) in vec2 vertexTexCoord;
layout(location = 3) in vec3 vertexTangent;
layout(location = 4) in vec3 vertexBitTangent;


out vec3 L;
out vec3 V;
out vec2 TexCoord;

struct LightInfo {
	vec4 Position;
	vec4 Direction;
	vec3 Intensity; //color
};

uniform LightInfo Light[1];

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 mvp;

void main()
{
	TexCoord = vertexTexCoord;

	vec3 norm = normalize(NormalMatrix * vertexNormal);
	vec3 tang = normalize(NormalMatrix * vertexTangent);
	vec3 bittang = normalize(NormalMatrix * vertexBitTangent);

	mat3 tangentMatrix = mat3(tang.x, norm.x, bittang.x,
                          tang.y, norm.y, bittang.y,
                          tang.z, norm.z, bittang.z);
	
	vec3 Position = vec3(ModelViewMatrix * vec4(vertexPosition, 1.0));
	L = normalize(tangentMatrix * (Light[0].Position.xyz - Position));
	V = normalize(tangentMatrix * (-Position));

	//we need to put NDC coordinates int gl_position
	gl_Position = mvp * vec4(vertexPosition, 1.0);
}