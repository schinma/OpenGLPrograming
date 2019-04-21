#version 430

in vec2 uv;

out vec4 final_color;

uniform sampler2D tex;
uniform bool depth;

subroutine vec4 shading_t(in vec2 uv);
subroutine uniform shading_t  Shading;


float LinearizeDepth(in vec2 uv)
{
    float zNear = 0.1;    
    float zFar  = 500.0; 
    float depth = texture(tex, uv).x;
    return (2.0 * zNear) / (zFar + zNear - depth * (zFar - zNear));
}

const float f = 1.0 / 300;

subroutine (shading_t)
vec4 Blurring(in vec2 uv)
{
	vec4  color;

	vec2 offsets[9] = vec2[](
	
		vec2(-f, f),
		vec2(0.0, f),
		vec2(f, f),	
		vec2(-f, 0.0),
		vec2(0.0, 0.0),
		vec2(f, 0),
		vec2(-f, -f),
		vec2(0.0, -f),
		vec2(f, -f)
		);
		
	float filter_kernel[9] = {	
		-1, -1, -1,	
		-1, 9, -1,
		-1, -1, -1
	};

	 for (int i = 0; i < 9; i++) {
			color += texture(tex, (uv + offsets[i])).rgba * filter_kernel[i];
	 }

	 return color;
}

subroutine (shading_t)
vec4 GreyScale(in vec2 uv)
{
	vec4 color = texture(tex, uv);
	float value = 0.2126*color.r + 0.7152 * color.g + 0.0722 * color.b;
	return vec4(value, value, value, color.a);
}

subroutine (shading_t)
vec4 SepiaTone(in vec2 uv)
{
	vec4 color = texture(tex, uv);
	float outputRed = (color.r * .393) + (color.g *.769) + (color.b * .189);
	float outputGreen = (color.r * .349) + (color.g *.686) + (color.b * .168);	
	float outputBlue = (color.r * .272) + (color.g *.534) + (color.b * .131);

	return vec4(outputRed, outputGreen, outputBlue, color.a);
}

subroutine (shading_t)
vec4 SobelFilter(in vec2 uv)
{
	vec2 offsets[9] = vec2[](
	
		vec2(-f, f),
		vec2(0.0, f),
		vec2(f, f),	
		vec2(-f, 0.0),
		vec2(0.0, 0.0),
		vec2(f, 0),
		vec2(-f, -f),
		vec2(0.0, -f),
		vec2(f, -f)
	);

	float filter_Sx[9] = {	
		-1, 0, 1,	
		-2, 0, 2,
		-1, 0, 1
	};

	float filter_Sy[9] = {	
		-1, -2, -1,	
		0, 0, 0,
		1, 2, -1
	};

	vec4 Y;
	vec4 X;
	vec4 G;

	for (int i=0; i <9 ; i++) {
		Y += GreyScale(uv + offsets[i]) * filter_Sy[i];
		X += GreyScale(uv + offsets[i]) * filter_Sx[i];
	}
	
	G = sqrt(X*X + Y*Y);

	return vec4(vec3(G), 1.0);
}

void main()
{
	
	//rrra because depth textures are not usual textures, they have only one channel
	final_color = (depth) ? texture(tex, uv).rrra : texture(tex, uv).rgba;

	float d;

	final_color = Shading(uv);


	if (depth) {
	   d = LinearizeDepth(uv);
	   final_color = vec4(d,d,d,1.0);

	}

    //final_color = texture(tex, uv).rrra;
	//final_color = vec4(1.0,0.0,0.0,1.0);
}