#pragma once

#include "Renderable.h"
#include "Listener.h"
#include "OGLTexture.h"

class OGLCube : public Renderable
{
	private:
		struct SimpleVertex
		{
				float position[3];
				float colour[3];
				float uv[2];
		};

		void			InitUnitCube();

	protected:
		unsigned int	m_vao;
		unsigned int	m_vbo_verts;
		unsigned int	m_vbo_colours;
		unsigned int	m_vbo_indices;
				
		
	public:
						OGLCube();
						~OGLCube();
		

		void			Render();
};