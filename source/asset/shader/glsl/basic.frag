#version 430

layout (binding = 0) uniform sampler2D texColour;
layout (binding = 1) uniform sampler2D texNormal;
layout (binding = 2) uniform sampler2D texSpec;
layout (binding = 3) uniform sampler2D texEnv;
layout (binding = 4) uniform sampler2D shadowmap;

in vec4 outNormal;	//input: normal
in vec4 lightvec;	//input: light vector
in vec4 viewvec;	//input: view vector
in vec2 outUV;		//input: texcoords
in vec4 outPosInLight;	//input: position in light space

layout (location = 0) out vec4 outFrag;

void main()
{
	vec4 white = vec4(1.0,1.0, 1.0, 1.0);
	
	vec4 DiffuseC;
	vec4 SpecularC=white;
	
	vec4 halfvec = (lightvec +viewvec);
	halfvec = normalize (halfvec);
	
	float cos = dot(outNormal,lightvec );
	float cos2 = dot(outNormal,halfvec );
	cos2 = pow(cos2, 10);
		
	DiffuseC = white * cos; // Calculate Diffuse colour		
	DiffuseC = clamp (DiffuseC, 0.0f,1.0f);
	
	SpecularC= white * cos2; //Calculate Specular colour
	SpecularC = clamp (SpecularC, 0.0f,1.0f);
		
	outFrag = (DiffuseC+SpecularC) *texture(texColour, outUV);
}
