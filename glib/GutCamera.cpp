
void GutUserControl_RotateObject(Matrix4x4 &object_matrix, float time_advance)
{
	// Ū���ƹ�
	GutMouseInfo mouse;
	if ( GutReadMouse(&mouse) )
	{
		float rotation_speed = 1.0 * time_advance;

		// �p�G���U�ƹ�����A�N�������Y
		if ( mouse.button[0] ) 
		{
			Matrix4x4 rotate_matrix;
			
			rotate_matrix.RotateY_Replace(mouse.x * rotation_speed);
			rotate_matrix.RotateX(mouse.y * rotation_speed);

			object_matrix = object_matrix * rotate_matrix;
		}

		// �κu�����Y�񪫥�
		if ( mouse.z )
		{
			float scale = 1.0f + mouse.z / 1000.0f;
			object_matrix.Scale(scale, scale, scale);
		}
	}
}

void GetUserControl_FPSCamera(Vector4 &vEye, Vector4 &vUp, Vector4 &vLookAt, float time_advance)
{
	// Ū���ƹ�
	GutMouseInfo mouse;
	GutReadMouse(&mouse);
	// Ū����L
	char keyboard_state[256];
	GutReadKeyboard(keyboard_state);

	float moving_speed = 2.0f * time_advance;
	float rotation_speed = 1.0f * time_advance;

	// ���y�Шt��
	static float theta = -MATH_PI * 0.5f;
	static float phi = 0.0f;

	// �p�G���U�ƹ�����A�N�������Y
	if ( mouse.button[0] ) 
	{
		theta += mouse.x * rotation_speed;
		phi -= mouse.y * rotation_speed;
	}

	float sin_phi, cos_phi;
	float sin_theta, cos_theta;
	
	FastMath::SinCos(phi, sin_phi, cos_phi);
	FastMath::SinCos(theta, sin_theta, cos_theta);

	// �p�����Y�����V
	Vector4 camera_facing;

	camera_facing[0] = cos_phi * cos_theta;
	camera_facing[1] = sin_phi;
	camera_facing[2] = cos_phi * sin_theta;

	// �p�����Y���W�誺�b�V
	Vector4 camera_up;
	FastMath::SinCos(phi + MATH_PI*0.5f, sin_phi, cos_phi);

	camera_up[0] = cos_phi * cos_theta;
	camera_up[1] = sin_phi;
	camera_up[2] = cos_phi * sin_theta;

	// ���o�譱�k�誺��V
	Vector4 camera_right = Vector3CrossProduct(camera_up, camera_facing);

	// ���UW�Τ�V��V�W
	if ( keyboard_state[GUTKEY_W] || keyboard_state[GUTKEY_UP] )
	{
		vEye += camera_facing * moving_speed;
	}
	// ���US�Τ�V��V�U
	if ( keyboard_state[GUTKEY_S] || keyboard_state[GUTKEY_DOWN] )
	{
		vEye -= camera_facing * moving_speed;
	}
	// ���UA�Τ�V��V��
	if ( keyboard_state[GUTKEY_A] || keyboard_state[GUTKEY_LEFT] )
	{
		vEye += camera_right * moving_speed;
	}
	// ���UD�Τ�V��V�k
	if ( keyboard_state[GUTKEY_D] || keyboard_state[GUTKEY_RIGHT] )
	{
		vEye -= camera_right * moving_speed;
	}

	// �p��X���Y��Ǫ��I, �������Y�ഫ�x�}�ɷ|�Ψ�.
	vLookat = vEye + camera_facing;
	// �]���O��2�Ӷb���, �ݭn��s���Y�¤W���b
	vUp = camera_up;

	vEye[3] = vUp[3] = vLookAt[3] = 1.0f;
}
