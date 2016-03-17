#include "Gut.h"
#include "GutUserControl.h"
#include "GutInput.h"

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

void CGutUserControl::SetCamera(Vector4 vEye, float heading, float pitch)
{
	/*
	m_vEye = vEye;
	m_fCameraHeading = heading;
	m_fCameraPitch = pitch;
	
	FPSCamera(0.0f);
	*/
}

void CGutUserControl::SetCamera(Vector4 vEye, Vector4 vLookAt, Vector4 vUp)
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

void CGutUserControl::RotateObject(float time_advance)
{
	static float rx=0, ry=0;

	// Ū���ƹ�
	GutMouseInfo mouse;
	if ( GutReadMouse(&mouse) )
	{
		float rotation_speed = m_fRotateSpeed * time_advance;

		m_eUP=UP_CUSTOMIZED;

		// �p�G���U�ƹ�����A�N�������Y
		if ( mouse.button[0] ) 
		{
			Matrix4x4 rotate_matrix;

			rotate_matrix.Identity();
			rotate_matrix.Rotate(m_CameraMatrix[1], mouse.x * rotation_speed);
			rotate_matrix.Rotate(m_CameraMatrix[0], mouse.y * rotation_speed);

			Vector4 vPos = m_ObjectMatrix[3];

			m_ObjectMatrix.NoTranslate();
			m_ObjectMatrix = m_ObjectMatrix * rotate_matrix;
			m_ObjectMatrix[3] = vPos;
		}

		// �κu�����Y�񪫥�
		if ( mouse.z )
		{
			float scale = 1.0f + mouse.z / 1000.0f;
			m_ObjectMatrix.Scale(scale, scale, scale);
		}
	}
}

void CGutUserControl::SetObjectPosition(Vector4 &vPosition)
{
	m_ObjectMatrix[3] = vPosition;
	m_ObjectMatrix[3][3] = 1.0f;
}

void CGutUserControl::FPSCamera(float time_advance)
{
	char keyboard_state[256];
	float moving_speed = m_fMoveSpeed * time_advance;
	float rotation_speed = m_fRotateSpeed * time_advance;

	if ( time_advance )
	{
		// Ū���ƹ�
		GutMouseInfo mouse;
		GutReadMouse(&mouse);
		// Ū����L
		GutReadKeyboard(keyboard_state);
		// �t��
		// �p�G���U�ƹ�����A�N�������Y
		if ( mouse.button[0] ) 
		{
			m_fCameraHeading -= mouse.x * rotation_speed;
			m_fCameraPitch -= mouse.y * rotation_speed;
		}
	}

	/*
	// ���y�Шt��
	float heading_sin, heading_cos;
	float pitch_sin, pitch_cos;
	float up_sin, up_cos;

	FastMath::SinCos(m_fCameraHeading, heading_sin, heading_cos);
	FastMath::SinCos(m_fCameraPitch, pitch_sin, pitch_cos);
	FastMath::SinCos(m_fCameraPitch + MATH_PI*0.5f, up_sin, up_cos);

	Vector4 camera_facing, camera_up;

	switch(m_eUP)
	{
	case UP_X:
		break;
	case UP_Y:
		// �p�����Y�����V
		camera_facing[0] = pitch_cos * heading_cos;
		camera_facing[1] = pitch_sin;
		camera_facing[2] = pitch_cos * heading_sin;
		// �p�����Y���W�誺�b�V
		camera_up[0] = up_cos * heading_cos;
		camera_up[1] = up_sin;
		camera_up[2] = up_cos * heading_sin;
		break;
	case UP_Z:
		// �p�����Y�����V
		camera_facing[0] = pitch_cos * heading_cos;
		camera_facing[2] = pitch_sin;
		camera_facing[1] = -pitch_cos * heading_sin;
		// �p�����Y���W�誺�b�V
		camera_up[0] = up_cos * heading_cos;
		camera_up[2] = up_sin;
		camera_up[1] = -up_cos * heading_sin;
		break;
	}

	// ���o�譱�k�誺��V
	Vector4 camera_right = Vector3CrossProduct(camera_up, camera_facing);
	*/

	Matrix4x4 rot_matrix; 
	rot_matrix.Identity();
	rot_matrix.RotateY_Replace(m_fCameraHeading);
	rot_matrix.RotateX(m_fCameraPitch);

	Matrix4x4 CameraOrient = rot_matrix * m_RefCameraMatrix;

	Vector4 camera_right = CameraOrient[0];
	Vector4 camera_up = CameraOrient[1];
	Vector4 camera_facing = -CameraOrient[2];

	if ( time_advance )
	{
		// ���UW�Τ�V��V�W
		if ( keyboard_state[GUTKEY_W] || keyboard_state[GUTKEY_UP] )
		{
			m_vEye += camera_facing * moving_speed;
		}
		// ���US�Τ�V��V�U
		if ( keyboard_state[GUTKEY_S] || keyboard_state[GUTKEY_DOWN] )
		{
			m_vEye -= camera_facing * moving_speed;
		}
		// ���UA�Τ�V��V��
		if ( keyboard_state[GUTKEY_A] || keyboard_state[GUTKEY_LEFT] )
		{
			m_vEye -= camera_right * moving_speed;
		}
		// ���UD�Τ�V��V�k
		if ( keyboard_state[GUTKEY_D] || keyboard_state[GUTKEY_RIGHT] )
		{
			m_vEye += camera_right * moving_speed;
		}
	}

	// �p��X���Y��Ǫ��I, �������Y�ഫ�x�}�ɷ|�Ψ�.
	m_vLookAt = m_vEye + camera_facing;
	// �]���O��2�Ӷb���, �ݭn��s���Y�¤W���b
	m_vUp = camera_up;
	// �T�{w = 1
	m_vEye[3] = m_vUp[3] = m_vLookAt[3] = 1.0f;

	UpdateViewMatrix();
}

