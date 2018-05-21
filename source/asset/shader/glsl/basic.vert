#version 430

layout (location = 0) uniform mat4 modelview;	//modelview matrix
layout (location = 1) uniform mat4 projection;	//projection matrix
layout (location = 2) uniform vec4 lightpos;	//light position
layout (location = 3) uniform mat4 lightmatrix;	//light matrix
layout (location = 4) uniform mat4 lightproj;	//light projection matrix

layout (location = 0) in vec4 position;	//vertex attribute: position
layout (location = 1) in vec4 inNormal;	//vertex attribute: normal
layout (location = 2) in vec2 inUV;		//vertex attribute: texcoords
layout (location = 3) in vec4 m_position;		//vertex attribute: camera


out vec4 outNormal;		//output: normal
out vec4 lightvec;		//output: light vector
out vec4 viewvec;		//output: view vector
out vec2 outUV;			//output: texcoords
out vec4 outPosInLight;	//output: vertex position in light space

void main()
{	
	gl_Position = projection*modelview*position;
		
	lightvec = lightpos - position;
	lightvec = normalize ( lightvec ) ;
	
	viewvec = m_position - position;
	viewvec = normalize ( viewvec ) ;
	outNormal = -inNormal;
	
	outUV = inUV;
}
