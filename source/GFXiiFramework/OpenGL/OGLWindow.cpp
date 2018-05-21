#include "OGLWindow.h"
#include "Resource.h"
#include "Matrix4x4.h"
#include "Vector4.h"
#include "camera.h"
#include "GLEW/include/glew.h"
#include "OGLCube.h"
#include "OGLTexture.h"

OGLWindow::OGLWindow()
{
	m_euler[0] = m_euler[1] = m_euler[2] = 0.0f;
}

OGLWindow::~OGLWindow()
{
	//Clean up the renderable
	delete m_mesh;
	delete m_shader;
	delete m_skyshader;

	DestroyOGLContext();
}

OGLWindow::OGLWindow(HINSTANCE hInstance, int width, int height)
{
	//InitWindow(hInstance, width, height);
	m_euler[0] = m_euler[1] = m_euler[2] = 0.0f;
}

HGLRC OGLWindow::CreateOGLContext(HDC hdc)
{
	unsigned int pixelformat;
	HGLRC hglrc;

	static PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Support Window
		PFD_SUPPORT_OPENGL |						// Format Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request RGBA Format
		32,											// Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits mask
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// Z-Buffer depth
		0,											// Stencil Buffer
		0,											// Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};

	if (!(pixelformat = ChoosePixelFormat(hdc, &pfd)))
	{
		return 0;
	}

	if (!SetPixelFormat(hdc, pixelformat, &pfd))
	{
		return 0;
	}

	if (!(hglrc = wglCreateContext(hdc)))
	{
		return 0;
	}

	if (!wglMakeCurrent(hdc, hglrc))
	{
		return 0;
	}

	return hglrc;
}

void OGLWindow::DestroyRenderWindow()
{
	DestroyWindow(m_hwnd);

	m_hwnd = NULL;
	m_hdc = NULL;
}

BOOL OGLWindow::DestroyOGLContext()
{

	glDeleteSamplers(1, (GLuint*)(&m_texDefaultSampler));

	if (m_hglrc)
	{
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(m_hglrc);
		m_hglrc = NULL;
	}

	if (!ReleaseDC(m_hwnd, m_hdc))
		return FALSE;

	return TRUE;
}


BOOL OGLWindow::InitWindow(HINSTANCE hInstance, int width, int height)
{
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
		L"RenderWindow", L"OGLWindow", WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		0, 0, width, height, NULL, NULL, hInstance, NULL);

	if (!m_hwnd)
		return FALSE;

	m_hdc = GetDC(m_hwnd);

	if (!(m_hglrc = CreateOGLContext(m_hdc)))
		return FALSE;

	glewInit();

	InitOGLState();

	m_width = width;
	m_height = height;

	m_camera = new Camera;  // add camera
	m_camera->InitCamera(); // initialize camera

	m_skybox = new OGLCube();          // add skybox
	m_skytexture = new OGLTexture();   // texture for it
	m_skytexture->LoadSkyBox();        // load skybox
	m_skybox->SetTexture(m_skytexture);

	// Storing each mesh into the array and creating positions for each one

	// For House
	m_mesh = new OGLMesh(L"../asset/models/house.obj");
	ArrayMesh[1] = m_mesh;

	m_texture = new OGLTexture();
	m_texture->CreateTextureFromFile("../asset/texture/house_diffuse.tga");
	ArrayMesh[1]->SetTexture(m_texture);

	xPos[1] = -50.0;
	yPos[1] = -10.0;
	zPos[1] = -50.0;

	// For Space Ship: we can move space ship beacuse it is the first object from the scene
	m_mesh = new OGLMesh(L"../asset/models/space_frigate_6.obj");
	ArrayMesh[0] = m_mesh;

	m_texture = new OGLTexture();
	m_texture->CreateTextureFromFile("../asset/texture/space_frigate_6_color.tga");
	ArrayMesh[0]->SetTexture(m_texture);

	xPos[0] = 0.0;
	yPos[0] = 0.0;
	zPos[0] = -50.0;

	//// for canyon
	//m_mesh = new OGLMesh(L"../asset/models/canyon.obj");
	//ArrayMesh[2] = m_mesh;

	//m_texture = new OGLTexture();
	//m_texture->CreateTextureFromFile("../asset/texture/canyon_diffuse.tga");
	//ArrayMesh[2]->SetTexture(m_texture);

	//xPos[2] = -50.0;
	//yPos[2] = -10.0;
	//zPos[2] = -50.0;

	// For Terrain
	m_mesh = new OGLMesh(L"../asset/models/MountnN14_obj.obj");
	ArrayMesh[2] = m_mesh;

	m_texture = new OGLTexture();
	m_texture->CreateTextureFromFile("../asset/texture/ColorTex.tga");
	ArrayMesh[2]->SetTexture(m_texture);

	xPos[2] = -50.0;
	yPos[2] = -36.0;
	zPos[2] = -50.0;

	return TRUE;
}

void OGLWindow::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Set up model, view and projection matrix using our own Matrix4x4 class
	Matrix4x4 modelview;
	Matrix4x4 translation;
	Matrix4x4 roty;
	Matrix4x4 rotx;
	Matrix4x4 projection;
	Matrix4x4 viewMatrix;
	
	Vector3 m_PositionC = *m_camera->GetCameraPosition();
	int a = 0; //intermediat value

	while ( a != 3) // loop to go trough each mesh
	{ 
		//m_euler[1] = 15;// m_euler[0] > 360.0 ? 0 : m_euler[0] + 1.0;
		//m_euler[1] =  m_euler[1] > 360.0 ? 0 : m_euler[1] + .01;
		//m_euler[2] = m_euler[2] > 360.0 ? 0 : m_euler[2] + .01;

		Renderable* prenderable = static_cast<Renderable*>(ArrayMesh[a]); // changed for arrays
																		  
		rotx.SetRotationXAxis(0);//(m_euler[0]);
		roty.SetRotationYAxis(-90);//(m_euler[1]);

		translation.SetTranslate(Vector4(xPos[a], yPos[a], zPos[a])); // changed for arrays
			   
		// if the modeview is first and viewmatrix second, the movement it s gonna be attached on the spaceship
		// if the viewmatrix is first and modeview second, the movement it s gonna be attached on the camera only
		
		//viewMatrix = *m_camera->GetViewMatrix();
		modelview = translation*viewMatrix *rotx*roty;
		viewMatrix = *m_camera->GetViewMatrix();

		float aspect_ratio = (float)m_width / (float)m_height;
		m_camera->SetCameraAspectRatio(aspect_ratio);

		m_camera->SetProjectionMatrix(m_camera->GetCameraFOV(), m_camera->GetCameraAspectRatio(), 1.0, 1000.0f);

		projection = *m_camera->GetProjectionMatrix();

		glUniformMatrix4fv(m_uniform_modelview, 1, GL_TRUE, modelview.ToPtr());
		glUniformMatrix4fv(m_uniform_projection, 1, GL_FALSE, projection.ToPtr());
		glUniform4f(m_cameraPosition, m_PositionC[0], m_PositionC[1], m_PositionC[2], 1.0);
		glBindSampler(0, m_texDefaultSampler);

		prenderable->Render();

		glCullFace(GL_FRONT);
		
		// Skybox implementation
		if (m_skyshader != NULL)
		{
			Matrix4x4 sky_modelview;
			Matrix4x4 sky_translation;
			Matrix4x4 sky_projection;
			
			sky_translation.SetScale(100.0f, 100.0f, 100.0f); //set scale

			sky_translation.SetTranslate(Vector4(0.0, 0.0, 0.0, 0)); //set the position

			sky_modelview = sky_translation;

			sky_projection.SetPerspective(60.0f, aspect_ratio, 1.0f, 1000.0f);

			glUniformMatrix4fv(0, 1, GL_TRUE, sky_modelview.ToPtr());
			glUniformMatrix4fv(1, 1, GL_FALSE, sky_projection.ToPtr());

			prenderable->Render();
		}

		glCullFace(GL_BACK);
		a++; 
	}

	//Mapping the terrain for a large area ( prefer small area so far ) 
	//for (int i = 0; i > -1600; i -= 200) 
	//	{
	//		for (int y = 0; y > -1600; y -= 200)
	//		{
	//			Renderable* prenderable = static_cast<Renderable*>(ArrayMesh[2]);

	//			translation.SetTranslate(Vector4(y, 100, i));

	//			modelview = translation*viewMatrix;

	//			glUniformMatrix4fv(m_uniform_modelview, 1, GL_TRUE, modelview.ToPtr());
	//			glUniformMatrix4fv(m_uniform_projection, 1, GL_FALSE, projection.ToPtr());
	//			glUniform4f(m_cameraPosition, m_PositionC[0], m_PositionC[1], m_PositionC[2], 0);
	//			prenderable->Render();
	//		}
	//}

	// Camera keyboard controls
	if ((unsigned short)GetAsyncKeyState(0x57)) // W ( Forward )
	{
		m_camera->DollyCamera(-0.5);
	}
	if ((unsigned short)GetAsyncKeyState(0x53)) // S ( Backword )
	{
		m_camera->DollyCamera(0.5);
	}
	if ((unsigned short)GetAsyncKeyState(0x44)) // D (	Right )
	{
		m_camera->StrafeCamera(0.5);
	}
	if ((unsigned short)GetAsyncKeyState(0x41)) // A ( Left )
	{
		m_camera->StrafeCamera(-0.5);
	}
	if ((unsigned short)GetAsyncKeyState(0x5A)) // Z ( Up )
	{
		m_camera->PedCamera(0.5);
	}
	if ((unsigned short)GetAsyncKeyState(0x58)) // X ( Down )
	{
		m_camera->PedCamera(-0.5);
	}

	//	ROTATION BY KEYBOARd: used for spaceship movement
	if ((unsigned short)GetAsyncKeyState(0x51)) // R 
	{
		m_camera->RotateCamera(0.25f, 0.0f, 0.0f);
	}
	if ((unsigned short)GetAsyncKeyState(0x45)) // F 
	{
		m_camera->RotateCamera(-0.25f, 0.0f, 0.0f);
	}
	if ((unsigned short)GetAsyncKeyState(0x52)) // Q 
	{
		m_camera->RotateCamera(0.0f, 0.25f, 0.0f);
	}
	if ((unsigned short)GetAsyncKeyState(0x46)) // E 
	{
		m_camera->RotateCamera(0.0f, -0.25f, 0.0f);
	}

	// Roll not working 
	if ((unsigned short)GetAsyncKeyState(0x4F)) // o
	{
		m_camera->RotateCamera(0.0f, 0.0f, 0.25f);
	}
	if ((unsigned short)GetAsyncKeyState(0x50)) //p
	{
		m_camera->RotateCamera(0.0f, -0.0f, -0.25f);
	}

	SwapBuffers(m_hdc);

	return;
}

void OGLWindow::Resize(int width, int height)
{
	m_width = width;
	m_height = height;

	glViewport(0, 0, width, height);

	return;
}

void OGLWindow::InitOGLState()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// New Shader added for SkyBox
	m_skyshader = new OGLShaderProgram();

	m_skyshader->CreateShaderProgram();
	m_skyshader->AttachAndCompileShaderFromFile(L"../asset/shader/glsl/sky.vert", SKYBOX_SHADER_VERTEX);
	m_skyshader->AttachAndCompileShaderFromFile(L"../asset/shader/glsl/sky.frag", SKYBOX_SHADER_FRAGMEMT);
	m_skyshader->BindAttributeLocation(0, "Sky_position");

	glBindFragDataLocation(m_skyshader->GetProgramHandle(), 0, "outcolor");
	m_skyshader->BuildShaderProgram();

	//Initialise OGL shader
	m_shader = new OGLShaderProgram();

	m_shader->CreateShaderProgram();
	m_shader->AttachAndCompileShaderFromFile(L"../asset/shader/glsl/basic.vert", SHADER_VERTEX);
	m_shader->AttachAndCompileShaderFromFile(L"../asset/shader/glsl/basic.frag", SHADER_FRAGMENT);

	m_shader->BindAttributeLocation(0, "position");
	m_shader->BindAttributeLocation(1, "inNormal");
	m_shader->BindAttributeLocation(2, "inUV");
	///////

	glBindFragDataLocation(m_shader->GetProgramHandle(), 0, "outFrag");

	m_shader->BuildShaderProgram();
	m_shader->ActivateShaderProgram();

	m_uniform_modelview = glGetUniformLocation(m_shader->GetProgramHandle(), "modelview");
	m_uniform_projection = glGetUniformLocation(m_shader->GetProgramHandle(), "projection");
	m_uniform_texture = glGetUniformLocation(m_shader->GetProgramHandle(), "texColour");
	m_cameraPosition = glGetUniformLocation(m_shader->GetProgramHandle(), "m_PositionC");

	glUniform1i(m_uniform_texture, 0);

	//Create a texture sampler
	glGenSamplers(1, (GLuint*)(&m_texDefaultSampler));

	glSamplerParameteri(m_texDefaultSampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glSamplerParameteri(m_texDefaultSampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glSamplerParameteri(m_texDefaultSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glSamplerParameteri(m_texDefaultSampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

BOOL OGLWindow::MouseLBDown(int x, int y)
{			 
	return TRUE;
}

BOOL OGLWindow::MouseLBUp(int x, int y)
{
	return TRUE;
}

// Mouse control for rotation
BOOL OGLWindow::MouseMove(int x, int y)
{
	if (x > MouseX)
	{
		m_camera->RotateCamera(0.1f, 0.0f, 0.0f);
	}
	if (x < MouseX)
	{
		m_camera->RotateCamera(-0.1f, 0.0f, 0.0f);
	}
	if (y > MouseY)
	{
		m_camera->RotateCamera(0.0f, 0.1f, 0.0f);
	}
	if (y < MouseY)
	{
		m_camera->RotateCamera(0.0f, -0.1f, 0.0f);
	}

	// Track the location of mouse
	MouseY = y; 
	MouseX = x;

	return TRUE;
}
