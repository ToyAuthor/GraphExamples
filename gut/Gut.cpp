#include <string.h>
#include "Gut.h"

static char g_szShaderPath[128] = "";

void GutSetShaderPath(const char *path)
{
	strcpy(g_szShaderPath, path);
}

const char *GutGetShaderPath(void)
{
	return g_szShaderPath;
}

Matrix4x4 GutMatrixLookAtRH(Vector4 &eye, Vector4 &lookat, Vector4 &up)
{
	Vector4 up_normalized = VectorNormalize(up);
	Vector4 zaxis = (eye - lookat); zaxis.Normalize();
	Vector4 xaxis = Vector3CrossProduct(up_normalized, zaxis);
	Vector4 yaxis = Vector3CrossProduct(zaxis, xaxis);

	Matrix4x4 matrix;
	matrix.Identity();

	matrix.SetColumn(0, xaxis);
	matrix.SetColumn(1, yaxis);
	matrix.SetColumn(2, zaxis);
	matrix[3][0] = -Vector3Dot(xaxis, eye)[0];
	matrix[3][1] = -Vector3Dot(yaxis, eye)[0];
	matrix[3][2] = -Vector3Dot(zaxis, eye)[0];

	return matrix;
}

// OpenGL native right hand system
// fovy = `垂直方向的視角`
// aspect = `水平方向視角對重直方向視角的比值`
// z_hear = `鏡頭可以看到的最近距離`
// z_far = `鏡頭可以看到的最遠距離`
Matrix4x4 GutMatrixPerspectiveRH_OpenGL(float fovy, float aspect,
										float z_near, float z_far)
{
	Matrix4x4 matrix;
	matrix.Identity();

	float fovy_radian = FastMath::DegreeToRadian(fovy);
	float yscale =  FastMath::Cot(fovy_radian/2.0f);
	float xscale = yscale * aspect;

	matrix[0][0] = xscale;
	matrix[1][1] = yscale;
	matrix[2][2] = (z_far + z_near)/(z_near - z_far);
	matrix[2][3] = -1.0f;
	matrix[3][2] = 2.0f * z_far * z_near / (z_near - z_far);
	matrix[3][3] = 0.0f;

	return matrix;
}
