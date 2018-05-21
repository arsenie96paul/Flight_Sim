#pragma once

#include "RenderWindow.h"
#include "OGLCube.h"
#include "OGLMesh.h"
#include "OGLShader.h"
#include "OGLTexture.h"
#include "camera.h"

class OGLWindow : public RenderWindow
{
	private:
		float	    m_euler[3];
		HDC			m_hdc;				//handle to a device context
		HGLRC		m_hglrc;			//handle to a gl rendering context

		int			m_width;
		int			m_height;
		
		Renderable		*m_mesh;
		OGLTexture		*m_texture;
		OGLTexture		*m_skytexture;		// sky texture
		Camera			*m_camera;			// Camera 
		Renderable		*ArrayMesh[10];     // array for meshes
		OGLCube			*m_skybox;

		double xPos[10];			//
		double yPos[10];			// COORDINATES FOR OBJ
		double zPos[10];			//

		double MouseX;				// MOUSE COORDINATES X
		double MouseY;				// MOUSE COORDINATES Y

		//Declear an OGL shader program
		OGLShaderProgram		*m_shader;
		OGLShaderProgram		*m_skyshader;
		int						m_uniform_modelview;
		int						m_uniform_projection;
		int                     m_uniform_texture;
		int						m_texDefaultSampler;
		int						m_cameraPosition;

protected:

		HGLRC CreateOGLContext (HDC hdc);
		BOOL DestroyOGLContext();
		void InitOGLState();

	public:
					OGLWindow();
					OGLWindow(HINSTANCE hInstance, int width, int height);
					~OGLWindow();
		
		BOOL		InitWindow(HINSTANCE hInstance, int width, int height);

		void		Render();
		void		Resize( int width, int height );
		void		DestroyRenderWindow();

		BOOL		MouseLBDown ( int x, int y );
		BOOL		MouseLBUp ( int x, int y );
		BOOL		MouseMove ( int x, int y );
};
