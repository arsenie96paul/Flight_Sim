#include "Skybox.h"
#include <windows.h>
#include "Resource.h"
#include "Matrix4x4.h"
#include "Vector4.h"
#include "camera.h"
#include "GLEW/include/glew.h"
#include "OpenGL\OGLShader.h"

void Skybox::Init()
{

	// taking textures into the arrey
	m_texture = new OGLTexture();

	m_texture->CreateTextureFromFile("../asset/texture/skybox/sky_ft.tga");
	m_textures[0] = m_texture;

	m_texture->CreateTextureFromFile("../asset/texture/skybox/sky_bk.tga");
	m_textures[1] = m_texture;

	m_texture->CreateTextureFromFile("../asset/texture/skybox/sky_lt.tga");
	m_textures[2] = m_texture;

	m_texture->CreateTextureFromFile("../asset/texture/skybox/sky_rt.tga");
	m_textures[3] = m_texture;

	m_texture->CreateTextureFromFile("../asset/texture/skybox/sky_tp.tga");
	m_textures[4] = m_texture;

	m_texture->CreateTextureFromFile("../asset/texture/skybox/sky_bt.tga");
	m_textures[5] = m_texture;
	



	//glActiveTexture(GL_TEXTURE0);
	//glEnable(GL_TEXTURE_CUBE_MAP);
	//glGenTextures(1, m_textures);
	//glBindTexture(GL_TEXTURE_CUBE_MAP, m_textures);
	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	
}

void Skybox::Render()
{
	//create a new shader for skybox
	m_shader = new OGLShaderProgram();

	m_shader->CreateShaderProgram();
	m_shader->AttachAndCompileShaderFromFile(L"../asset/shader/glsl/sky.vert", SHADER_VERTEX);
	m_shader->AttachAndCompileShaderFromFile(L"../asset/shader/glsl/sky.frag", SHADER_FRAGMENT);
}
