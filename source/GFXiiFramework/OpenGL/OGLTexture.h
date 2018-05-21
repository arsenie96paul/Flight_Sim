#ifndef __TEXTUREGL_H__
#define __TEXTUREGL_H__

#include "BaseTexture.h"
#include "GLEW/include/glew.h"
#include <GL/gl.h>

#include <array>

class OGLTexture : public BaseTexture
{
	private:

	public:
		unsigned int					m_syshandle;
		unsigned int					m_apphandle;

	public:

		// assign the textures for the skybox
		const char* m_skytext[6] = { "../asset/texture/skybox/sky_bk.tga",
			"../asset/texture/skybox/sky_bt.tga",
			"../asset/texture/skybox/sky_ft.tga",
			"../asset/texture/skybox/sky_lt.tga",
			"../asset/texture/skybox/sky_rt.tga",
			"../asset/texture/skybox/sky_tp.tga" };

		GLenum types[6] = { GL_TEXTURE_CUBE_MAP_POSITIVE_X,
			GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
			GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
			GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
			GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
			GL_TEXTURE_CUBE_MAP_NEGATIVE_Z };

										OGLTexture();
		virtual							~OGLTexture();

		virtual void					CreateTextureFromFile(const char* filename);
		virtual void					CreateTextureFromData(void* data, int type, int width, int height, int bits);

		virtual void					CreateTextureAsRenderTarget(int width, int height);

		virtual void					CreateTextureAsDepthTarget(int width, int height);
		virtual void					FreeTexture();
		virtual void					LoadSkyBox();
};

#endif
