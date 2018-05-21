//Created for Graphics I and II
//Author: Minsi Chen

#include "camera.h"
#include "Vector4.h"
#include "quaternion.h"
#include <math.h>

//GLM


Camera::Camera()
{
	InitCamera();
}

Camera::~Camera()
{
}

void Camera::InitCamera()
{
	//Initialise the camera that sits at the origin looking down the negative z-axis.
	m_position.SetVector(0.0f, 0.0f, 0.0f);
	m_upVector.SetVector(0.0f, 1.0f, 0.0f);
	m_direction.SetVector(0.0f, 0.0, -1.0f);
	m_rightVector = m_direction.CrossProduct(m_upVector);
	m_upVector = m_rightVector.CrossProduct(m_direction);
	UpdateViewMatrix();
}

void Camera::SetCameraPosition(const Vector3* position)
{
	m_position = (*position);
	UpdateViewMatrix();
}

void Camera::SetLookAtPoint(const Vector3* lookAt)
{

	m_direction = (*lookAt) - m_position;
	m_direction.Normalise();

	//Recompute right vector and reinforce orthonormality of this three vectors.
	//Recompute up vector, from new direction and right vector;
	//TODO: Gram-Schmidt orthogonalisation algorithm can be used for this
	m_rightVector = m_direction.CrossProduct(m_upVector);
	m_upVector = m_rightVector.CrossProduct(m_direction);

	m_rightVector.Normalise();
	m_upVector.Normalise();

	UpdateViewMatrix();
}

void Camera::SetUpVector(const Vector3* up)
{
	m_upVector = (*up);

	m_rightVector = m_direction.CrossProduct(m_upVector);
	m_upVector = m_rightVector.CrossProduct(m_direction);

	m_rightVector.Normalise();
	m_upVector.Normalise();

	UpdateViewMatrix();
}

void Camera::UpdateViewMatrix()
{
	float mat[16];

	mat[0] = m_rightVector[0];
	mat[4] = m_rightVector[1];
	mat[8] = m_rightVector[2];

	mat[1] = m_upVector[0];
	mat[5] = m_upVector[1];
	mat[9] = m_upVector[2];

	mat[2] = -m_direction[0];
	mat[6] = -m_direction[1];
	mat[10] = -m_direction[2];

	mat[3] = mat[7] = mat[11] = 0.0f;

	mat[12] = -m_position[0];
	mat[13] = -m_position[1];
	mat[14] = -m_position[2];
	mat[15] = 1.0f;

	Vector4 camera_p = Vector4(mat[12], mat[13], mat[14]);
	Vector4 viewVector = Vector4(mat[2], mat[6], mat[10]);
	Vector4 upVector = Vector4(mat[1], mat[5], mat[9]);

	//TODO: call the SetViewMatrix method in Matrix4x4 to set up m_viewMatrix as a view matrix
	m_viewMatrix.SetViewMatrix(camera_p, viewVector, upVector);
}

void Camera::SetProjectionMatrix(float fovy, float aspect, float zNear, float zFar)
{
	m_projectionMatrix.SetPerspective(fovy, aspect, zNear, zFar);
}

void Camera::DollyCamera(float amount)
{
	m_position[2] += amount;
	m_lookAt[2] += amount;
	UpdateViewMatrix();
}

void Camera::StrafeCamera(float amount)
{
	m_position[0] += amount;
	m_lookAt[0] += amount;
	UpdateViewMatrix();
}

void Camera::PedCamera(float amount)
{
	m_position[1] += amount;
	m_lookAt[1] += amount;
	UpdateViewMatrix();
}

void Camera::RotateCamera(float yaw, float pitch, float roll)
{
	Matrix4x4 rotationX, rotationY, rotationZ, rotation;

	rotationX.SetRotationXAxis(pitch);
	rotationY.SetRotationYAxis(yaw);
	rotationZ.SetRotationZAxis(roll);

	rotation = rotationX* rotationY* rotationZ;

	Vector4 lookat = Vector4(m_lookAt[0], m_lookAt[1], m_lookAt[2]);
	lookat = rotation * lookat;

	m_lookAt = Vector3(lookat[0], lookat[1], lookat[2]);
	SetLookAtPoint(&m_lookAt);
}
				   
void  Camera::ZoomCamera(float amount)
{
	m_fov = m_fov + amount;
	UpdateViewMatrix();
}