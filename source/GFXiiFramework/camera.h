//Created for Graphics I and II
//Author: Minsi Chen

#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "vector3.h"
#include "matrix4x4.h"
#include "quaternion.h"

class Camera
{
	public:
		enum ECameraType
		{
			CAM_ORTHOGRAPHIC,
			CAM_PERSPECTIVE
		};

	private:
		Matrix4x4						m_viewMatrix;				//View Matrix
		Matrix4x4						m_projectionMatrix;			//Projection Matrix
		Vector3							m_position;					//Position
		Vector3							m_upVector;					//up vector
		Vector3							m_rightVector;				//right vector
		Vector3							m_viewVector;				//view vector
		Vector3							m_lookAt;					//look at position
		Vector3							m_direction;
		ECameraType						m_cameratype;				//Projection time
		float							m_fov;						//vertical field of view in degree
		float							m_aspectRatio;

	public:
	
										Camera();
		virtual							~Camera();

		void							InitCamera();
		void							SetCameraPosition(const Vector3* position);
		inline const Vector3*			GetCameraPosition() const 
		{
			return &m_position;
		}

		void							SetLookAtPoint(const Vector3* lookAt);
		inline const Vector3*			GetCameraDirection() const
		{
			return &m_direction;
		}
		void							SetUpVector(const Vector3* up);
		inline const Vector3*			GetCameraUpVector() const
		{
			return &m_upVector;
		}

		inline ECameraType				GetCameraType()
		{
			return m_cameratype;
		}

		inline void						SetCameraFOV(float fov)
		{
			m_fov = fov;
		}

		inline float					GetCameraFOV()
		{
			return m_fov;
		}

		inline void						SetCameraAspectRatio(float ar)
		{
			m_aspectRatio = ar;
		}

		inline float					GetCameraAspectRatio()
		{
			return m_aspectRatio;
		}

		void							UpdateViewMatrix();

		inline const Matrix4x4*			GetViewMatrix() const
		{
			return &m_viewMatrix;
		}

		// Added to pass the projection matrix
		inline const Matrix4x4*			GetProjectionMatrix() const
		{
			return &m_projectionMatrix;
		}

		//TODO: Setup the projection matrix, hint the Matrix4x4 already has a method for setting up a perspective projection matrix
		void							SetProjectionMatrix(float fovy, float aspect, float zNear, float zFar);

		//TODO: Implement the following camera movement
		void							StrafeCamera(float amount);							// DONE
		void							DollyCamera(float amount);							// DONE
		void							PedCamera(float amount);							// DONE
		void							RotateCamera(float yaw, float pitch, float roll);   // DONE
		void							ZoomCamera(float amount);						    // DONE	
};

#endif
