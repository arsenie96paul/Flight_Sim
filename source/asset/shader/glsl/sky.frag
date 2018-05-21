#version 330

in vec3 TexCoord0;

out vec4 outcolor;

uniform samplerCube SkyTexture;

void main()
{
    outcolor = texture(SkyTexture, TexCoord0);
}