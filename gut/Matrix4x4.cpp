#include <stdio.h>
#include <stdlib.h>

#include "Matrix4x4.h"

Matrix4x4 g_MatrixCPU_Identity = Matrix4x4
(
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f
);

#define IEEE_SP_ZERO ((unsigned int) 0)
#define IEEE_SP_SIGN ((unsigned int) ( 1 << 31 ))

void Matrix4x4::ConsoleOutput(void)
{
	printf("Matrix:");
	printf("\t(%5.2f,%5.2f,%5.2f,%5.2f)\n", m_Vec0.x, m_Vec0.y, m_Vec0.z, m_Vec0.w);
	printf("\t(%5.2f,%5.2f,%5.2f,%5.2f)\n", m_Vec1.x, m_Vec1.y, m_Vec1.z, m_Vec1.w);
	printf("\t(%5.2f,%5.2f,%5.2f,%5.2f)\n", m_Vec2.x, m_Vec2.y, m_Vec2.z, m_Vec2.w);
	printf("\t(%5.2f,%5.2f,%5.2f,%5.2f)\n", m_Vec3.x, m_Vec3.y, m_Vec3.z, m_Vec3.w);
}

inline float MINOR(Matrix4x4 &m, const int r0, const int r1, const int r2, const int c0, const int c1, const int c2)
{
	return
		m(r0,c0) * (m(r1,c1) * m(r2,c2) - m(r2,c1) * m(r1,c2)) -
		m(r0,c1) * (m(r1,c0) * m(r2,c2) - m(r2,c0) * m(r1,c2)) +
		m(r0,c2) * (m(r1,c0) * m(r2,c1) - m(r2,c0) * m(r1,c1));
}


Matrix4x4 Matrix4x4_Adjoint(Matrix4x4 &m)
{
	Matrix4x4 ma
	(
		 MINOR(m, 1, 2, 3, 1, 2, 3),
		-MINOR(m, 0, 2, 3, 1, 2, 3),
		 MINOR(m, 0, 1, 3, 1, 2, 3),
		-MINOR(m, 0, 1, 2, 1, 2, 3),

		-MINOR(m, 1, 2, 3, 0, 2, 3),
		 MINOR(m, 0, 2, 3, 0, 2, 3),
		-MINOR(m, 0, 1, 3, 0, 2, 3),
		 MINOR(m, 0, 1, 2, 0, 2, 3),

		 MINOR(m, 1, 2, 3, 0, 1, 3),
		-MINOR(m, 0, 2, 3, 0, 1, 3),
		 MINOR(m, 0, 1, 3, 0, 1, 3),
		-MINOR(m, 0, 1, 2, 0, 1, 3),

		-MINOR(m, 1, 2, 3, 0, 1, 2),
		 MINOR(m, 0, 2, 3, 0, 1, 2),
		-MINOR(m, 0, 1, 3, 0, 1, 2),
		 MINOR(m, 0, 1, 2, 0, 1, 2)
	);

	return ma;
}

float Matrix4x4_Determinant(Matrix4x4 &m)
{
	return
		m.m_Vec0.x * MINOR(m, 1, 2, 3, 1, 2, 3) -
		m.m_Vec0.y * MINOR(m, 1, 2, 3, 0, 2, 3) +
		m.m_Vec0.z * MINOR(m, 1, 2, 3, 0, 1, 3) -
		m.m_Vec0.w * MINOR(m, 1, 2, 3, 0, 1, 2);
}

Matrix4x4 Matrix4x4::Invert()
{
	Matrix4x4 m = (1.0f / Matrix4x4_Determinant(*this)) * Matrix4x4_Adjoint(*this);
	*this = m;
	return *this;
}

void Matrix4x4Rotate(Matrix4x4 &matrix, Vector4 &v, float radian)
{
	float x = v.x;
	float y = v.y;
	float z = v.z;

	float fSin, fCos;
	FastMath::SinCos(radian, fSin, fCos);
	float fOneMinusCos = 1.0f - fCos;

	matrix.m_Vec0.x = ( x * x ) * fOneMinusCos + fCos;
	matrix.m_Vec0.y = ( x * y ) * fOneMinusCos + (z * fSin);
	matrix.m_Vec0.z = ( x * z ) * fOneMinusCos - (y * fSin);
	matrix.m_Vec0.w = 0.0f;

	matrix.m_Vec1.x = ( y * x ) * fOneMinusCos - (z * fSin);
	matrix.m_Vec1.y = ( y * y ) * fOneMinusCos + fCos;
	matrix.m_Vec1.z = ( y * z ) * fOneMinusCos + (x * fSin);
	matrix.m_Vec1.w = 0.0f;

	matrix.m_Vec2.x = ( z * x ) * fOneMinusCos + (y * fSin);
	matrix.m_Vec2.y = ( z * y ) * fOneMinusCos - (x * fSin);
	matrix.m_Vec2.z = ( z * z ) * fOneMinusCos + fCos;
	matrix.m_Vec2.w = 0.0f;

	matrix.m_Vec3 = g_MatrixCPU_Identity.m_Vec3;
}

void Matrix4x4FromQuaternionAndTransform(Matrix4x4 &mat, Vector4 &v, Vector4 &t)
{
	float wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;

	x2 = v.x + v.x;
	y2 = v.y + v.y;
	z2 = v.z + v.z;
	xx = v.x * x2;
	xy = v.x * y2;
	xz = v.x * z2;
	yy = v.y * y2;
	yz = v.y * z2;
	zz = v.z * z2;
	wx = v.w * x2;
	wy = v.w * y2;
	wz = v.w * z2;

	mat.m_Vec0.x = 1.0f - (yy + zz);
	mat.m_Vec1.x = xy - wz;
	mat.m_Vec2.x = xz + wy;
	mat.m_Vec3.x = 0.0f;

	mat.m_Vec0.y = xy + wz;
	mat.m_Vec1.y = 1.0f - (xx + zz);
	mat.m_Vec2.y = yz - wx;
	mat.m_Vec3.y = 0.0f;

	mat.m_Vec0.z = xz - wy;
	mat.m_Vec1.z = yz + wx;
	mat.m_Vec2.z = 1.0f - (xx + yy);
	mat.m_Vec3.z = 0.0f;

	mat.m_Vec3 = t;
}

void QuaternionFromMatrix4x4(Matrix4x4 &matrix, Vector4 &quaternion)
{
	// not optimized yet
	float *q = &quaternion.x;
	const float *m = &matrix.m_Vec0.x;

	if ( m[0 * 4 + 0] + m[1 * 4 + 1] + m[2 * 4 + 2] > 0.0f )
	{
		float t = + m[0 * 4 + 0] + m[1 * 4 + 1] + m[2 * 4 + 2] + 1.0f;
		float s = FastMath::ReciprocalSqrt( t ) * 0.5f;
		q[3] = s * t;
		q[2] = ( m[0 * 4 + 1] - m[1 * 4 + 0] ) * s;
		q[1] = ( m[2 * 4 + 0] - m[0 * 4 + 2] ) * s;
		q[0] = ( m[1 * 4 + 2] - m[2 * 4 + 1] ) * s;
	}
	else if ( m[0 * 4 + 0] > m[1 * 4 + 1] && m[0 * 4 + 0] > m[2 * 4 + 2] )
	{
		float t = + m[0 * 4 + 0] - m[1 * 4 + 1] - m[2 * 4 + 2] + 1.0f;
		float s = FastMath::ReciprocalSqrt( t ) * 0.5f;
		q[0] = s * t;
		q[1] = ( m[0 * 4 + 1] + m[1 * 4 + 0] ) * s;

		q[2] = ( m[2 * 4 + 0] + m[0 * 4 + 2] ) * s;
		q[3] = ( m[1 * 4 + 2] - m[2 * 4 + 1] ) * s;
	}
	else if ( m[1 * 4 + 1] > m[2 * 4 + 2] )
	{
		float t = - m[0 * 4 + 0] + m[1 * 4 + 1] - m[2 * 4 + 2] + 1.0f;
		float s = FastMath::ReciprocalSqrt( t ) * 0.5f;
		q[1] = s * t;
		q[0] = ( m[0 * 4 + 1] + m[1 * 4 + 0] ) * s;
		q[3] = ( m[2 * 4 + 0] - m[0 * 4 + 2] ) * s;
		q[2] = ( m[1 * 4 + 2] + m[2 * 4 + 1] ) * s;
	}
	else
	{
		float t = - m[0 * 4 + 0] - m[1 * 4 + 1] + m[2 * 4 + 2] + 1.0f;
		float s = FastMath::ReciprocalSqrt( t ) * 0.5f;
		q[2] = s * t;
		q[3] = ( m[0 * 4 + 1] - m[1 * 4 + 0] ) * s;
		q[0] = ( m[2 * 4 + 0] + m[0 * 4 + 2] ) * s;
		q[1] = ( m[1 * 4 + 2] + m[2 * 4 + 1] ) * s;
	}
}

void Matrix4x4FromQuaternion(Matrix4x4 &mat, Vector4 &v)
{
	float wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;

	x2 = v.x + v.x;
	y2 = v.y + v.y;
	z2 = v.z + v.z;
	xx = v.x * x2;
	xy = v.x * y2;
	xz = v.x * z2;
	yy = v.y * y2;
	yz = v.y * z2;
	zz = v.z * z2;
	wx = v.w * x2;
	wy = v.w * y2;
	wz = v.w * z2;

	mat.m_Vec0.x = 1.0f - (yy + zz);
	mat.m_Vec1 = xy - wz;
	mat.m_Vec2.x = xz + wy;
	mat.m_Vec3.x = 0.0f;

	mat.m_Vec0.y = xy + wz;
	mat.m_Vec1.y = 1.0f - (xx + zz);
	mat.m_Vec2.y = yz - wx;
	mat.m_Vec3.y = 0.0f;

	mat.m_Vec0.z = xz - wy;
	mat.m_Vec1.z = yz + wx;
	mat.m_Vec2.z = 1.0f - (xx + yy);
	mat.m_Vec3.z = 0.0f;

	mat.m_Vec0.w = 0.0f;
	mat.m_Vec1.w = 0.0f;
	mat.m_Vec2.w = 0.0f;
	mat.m_Vec3.w = 1.0f;
}
