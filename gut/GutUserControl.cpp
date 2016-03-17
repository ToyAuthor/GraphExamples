#include "Gut.h"
#include "GutUserControl.h"
//#include "GutInput.h"

CGutUserControl::CGutUserControl(void)
{
	m_ObjectMatrix.Identity();
	m_ViewMatrix.Identity();
	m_CameraMatrix.Identity();
	m_RefCameraMatrix.Identity();

	m_eUP = UP_Y;
	m_eControllerType = CONTROLLER_FPSCAMERA;

	m_fMoveSpeed = 2.0f;
	m_fRotateSpeed = 1.0f;

	m_fCameraHeading = 0.0f; //-MATH_PI * 0.5f;
	m_fCameraPitch = 0.0f;
}

CGutUserControl::~CGutUserControl()
{
}

void CGutUserControl::SetCamera(Vector4 &vEye, float heading, float pitch)
{
	/*
	m_vEye = vEye;
	m_fCameraHeading = heading;
	m_fCameraPitch = pitch;

	FPSCamera(0.0f);
	*/
}

void CGutUserControl::SetCamera(Vector4 &vEye, Vector4 &vLookAt, Vector4 &vUp)
{
	m_vEye = vEye;
	m_vLookAt = vLookAt;
	m_vUp = vUp;

	m_eUP = UP_CUSTOMIZED;

	m_ViewMatrix = GutMatrixLookAtRH(m_vEye, m_vLookAt, m_vUp);

	m_CameraMatrix = m_ViewMatrix;
	m_CameraMatrix.FastInvert();

	m_RefCameraMatrix = m_CameraMatrix;
	m_RefCameraMatrix[3].Set(0.0f, 0.0f, 0.0f, 1.0f);
}

void CGutUserControl::UpdateViewMatrix(void)
{
	m_ViewMatrix = GutMatrixLookAtRH(m_vEye, m_vLookAt, m_vUp);
	m_CameraMatrix = m_ViewMatrix;
	m_CameraMatrix.FastInvert();
}
/*
void CGutUserControl::Update(float time_advance, CONTROLLER_TYPE type)
{
	m_eControllerType = type;

	switch(m_eControllerType)
	{
	case CONTROLLER_ROTATEOBJECT:
		RotateObject(time_advance);
		break;
	case CONTROLLER_FPSCAMERA:
		FPSCamera(time_advance);
		break;
	default:
		printf("Unsupported type!!\n");
		break;
	}
}
*/

void CGutUserControl::SetObjectPosition(Vector4 &vPosition)
{
	m_ObjectMatrix[3] = vPosition;
	m_ObjectMatrix[3][3] = 1.0f;
}
