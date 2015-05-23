#ifndef _MATRIX4x4_GENERAL_
#define _MATRIX4x4_GENERAL_

#if !defined(_ENABLE_GENERALCPU_)
	#define _ENABLE_GENERALCPU_
#endif

#include "Vector4.h"

class Matrix4x4;

extern Matrix4x4 g_MatrixCPU_Identity;

inline Vector4 operator*(Vector4 v, Matrix4x4 matrix);
inline Matrix4x4 operator*(Matrix4x4 a, Matrix4x4 b);

class Matrix4x4
{
public:
//	union
//	{
//		struct
//		{
//			float m_00, m_01, m_02, m_03;
//			float m_10, m_11, m_12, m_13;
//			float m_20, m_21, m_22, m_23;
//			float m_30, m_31, m_32, m_33;
//		};
//
//		struct
//		{
//			Vector4 m_Vec0;
//			Vector4 m_Vec1;
//			Vector4 m_Vec2;
//			Vector4 m_Vec3;
//		};
//	};
	Vector4 m_Vec0;
	Vector4 m_Vec1;
	Vector4 m_Vec2;
	Vector4 m_Vec3;

//	typedef m_Vec0.x m_00;
//	typedef m_Vec0.y m_01;
//	typedef m_Vec0.z m_02;
//	typedef m_Vec0.w m_03;
//
//	typedef m_Vec1.x m_10;
//	typedef m_Vec1.y m_11;
//	typedef m_Vec1.z m_12;
//	typedef m_Vec1.w m_13;
//
//	typedef m_Vec2.x m_20;
//	typedef m_Vec2.y m_21;
//	typedef m_Vec2.z m_22;
//	typedef m_Vec2.w m_23;
//
//	typedef m_Vec3.x m_30;
//	typedef m_Vec3.y m_31;
//	typedef m_Vec3.z m_32;
//	typedef m_Vec3.w m_33;

public:
	inline Matrix4x4()
	{

	}

	inline Matrix4x4(const Matrix4x4 &bro)
	{
		m_Vec0=bro.m_Vec0;
		m_Vec1=bro.m_Vec1;
		m_Vec2=bro.m_Vec2;
		m_Vec3=bro.m_Vec3;
	}

	inline Matrix4x4(
		float f00, float f01, float f02, float f03,
		float f10, float f11, float f12, float f13,
		float f20, float f21, float f22, float f23,
		float f30, float f31, float f32, float f33
	)
	{
//		m_00 = f00; m_01 = f01; m_02 = f02; m_03 = f03;
//		m_10 = f10; m_11 = f11; m_12 = f12; m_13 = f13;
//		m_20 = f20; m_21 = f21; m_22 = f22; m_23 = f23;
//		m_30 = f30; m_31 = f31; m_32 = f32; m_33 = f33;
		m_Vec0.Set(f00, f01, f02, f03);
		m_Vec1.Set(f10, f11, f12, f13);
		m_Vec2.Set(f20, f21, f22, f23);
		m_Vec3.Set(f30, f31, f32, f33);
	}

	inline void Identity(void)
	{
		/*
		m_00 = 1.0f; m_01 = 0.0f; m_02 = 0.0f; m_03 = 0.0f;
		m_10 = 0.0f; m_11 = 1.0f; m_12 = 0.0f; m_13 = 0.0f;
		m_20 = 0.0f; m_21 = 0.0f; m_22 = 1.0f; m_23 = 0.0f;
		m_30 = 0.0f; m_31 = 0.0f; m_32 = 0.0f; m_33 = 1.0f;
		*/
		*this = g_MatrixCPU_Identity;
	}

	static Matrix4x4 &IdentityMatrix(void)
	{
		return g_MatrixCPU_Identity;
	}

	inline void SetRow(int row, Vector4 &vec)
	{
		assert(row>=0 && row<4);
		Vector4 *rows = &m_Vec0;
		rows[row] = vec;
	}

	inline void SetColumn(int column, Vector4 &vec)
	{
		assert(column>=0 && column<4);
		float *p = &m_Vec0.x + column;
		p[0] = vec.GetX();
		p[4] = vec.GetY();
		p[8] = vec.GetZ();
		p[12] = vec.GetW();
	}

	inline Vector4 &GetRow(int row)
	{
		assert(row>=0 && row<4);
		Vector4 *rows = &m_Vec0;
		return rows[row];
	}

	inline Vector4 GetColumn(int column)
	{
		assert(column>=0 && column<4);
		Vector4 vec;
		float *p = &m_Vec0.x + column;

		vec.SetX(p[0]);
		vec.SetY(p[4]);
		vec.SetZ(p[8]);
		vec.SetW(p[12]);

		return vec;
	}

	inline Matrix4x4 &Transpose(void)
	{
		FastMath::Swap(m_Vec0.y,m_Vec1.x);
		FastMath::Swap(m_Vec0.z,m_Vec2.x);
		FastMath::Swap(m_Vec0.w,m_Vec3.x);
		FastMath::Swap(m_Vec1.z,m_Vec2.y);
		FastMath::Swap(m_Vec1.w,m_Vec3.y);
		FastMath::Swap(m_Vec2.w,m_Vec3.z);

		return *this;
	}

	// assume input vec.w equals 1
	// result.w is garbage
	inline Vector4 TransformVec3(Vector4 &vec)
	{
		Vector4 result = vec * (*this);
		return result;
	}

	void Rotate( Vector4 &v, float radian)
	{
		float x = v.x;
		float y = v.y;
		float z = v.z;
		float w = v.w;

		float fSin, fCos;
		FastMath::SinCos(radian, fSin, fCos);

		Matrix4x4  local, final;

		local.m_Vec0.x = ( x * x ) * ( 1.0f - fCos ) + fCos;
		local.m_Vec0.y = ( x * y ) * ( 1.0f - fCos ) + (z * fSin);
		local.m_Vec0.z = ( x * z ) * ( 1.0f - fCos ) - (y * fSin);
		local.m_Vec0.w = 0.0f;

		local.m_Vec1.x = ( y * x ) * ( 1.0f - fCos ) - (z * fSin);
		local.m_Vec1.y = ( y * y ) * ( 1.0f - fCos ) + fCos ;
		local.m_Vec1.z = ( y * z ) * ( 1.0f - fCos ) + (x * fSin);
		local.m_Vec1.w = 0.0f;

		local.m_Vec2.x = ( z * x ) * ( 1.0f - fCos ) + (y * fSin);
		local.m_Vec2.y = ( z * y ) * ( 1.0f - fCos ) - (x * fSin);
		local.m_Vec2.z = ( z * z ) * ( 1.0f - fCos ) + fCos;
		local.m_Vec2.w = 0.0f;

		local.m_Vec3.x = 0.0f;
		local.m_Vec3.y = 0.0f;
		local.m_Vec3.w = 0.0f;
		local.m_Vec3.w = 1.0f;

		final = local * (*this);
		*this = final;
	}

	void Rotate_Replace( Vector4 &v, float radian)
	{
		float x = v.x;
		float y = v.y;
		float z = v.z;
		float w = v.w;

		float fSin, fCos;
		FastMath::SinCos(radian, fSin, fCos);

		m_Vec0.x = ( x * x ) * ( 1.0f - fCos ) + fCos;
		m_Vec0.y = ( x * y ) * ( 1.0f - fCos ) + (z * fSin);
		m_Vec0.z = ( x * z ) * ( 1.0f - fCos ) - (y * fSin);
		m_Vec0.w = 0.0f;

		m_Vec1.x = ( y * x ) * ( 1.0f - fCos ) - (z * fSin);
		m_Vec1.y = ( y * y ) * ( 1.0f - fCos ) + fCos ;
		m_Vec1.z = ( y * z ) * ( 1.0f - fCos ) + (x * fSin);
		m_Vec1.w = 0.0f;

		m_Vec2.x = ( z * x ) * ( 1.0f - fCos ) + (y * fSin);
		m_Vec2.y = ( z * y ) * ( 1.0f - fCos ) - (x * fSin);
		m_Vec2.z = ( z * z ) * ( 1.0f - fCos ) + fCos;
		m_Vec2.w = 0.0f;

		m_Vec3.x = 0.0f;
		m_Vec3.y = 0.0f;
		m_Vec3.z = 0.0f;
		m_Vec3.w = 1.0f;
	}

	void Rotate( float x, float y, float z, float radian )
	{
		Vector4 v(x, y, z, 0.0f);
		Rotate(v, radian);
	}

	void Rotate_Replace(float x, float y, float z, float radian)
	{
		Vector4 v(x, y, z, 0.0f);
		Rotate_Replace(v, radian);
	}

	// this = Rx * this
	void RotateX( const float radian )
	{
		float Sin, Cos;
		FastMath::SinCos(radian, Sin, Cos);

		float Temp10, Temp11, Temp12, Temp13;
		float Temp20, Temp21, Temp22, Temp23;

		Temp10 = m_Vec1.x * Cos + m_Vec2.x * Sin;
		Temp11 = m_Vec1.y * Cos + m_Vec2.y * Sin;
		Temp12 = m_Vec1.z * Cos + m_Vec2.z * Sin;
		Temp13 = m_Vec1.w * Cos + m_Vec2.w * Sin;

		Temp20 = m_Vec1.x *-Sin + m_Vec2.x * Cos;
		Temp21 = m_Vec1.y *-Sin + m_Vec2.y * Cos;
		Temp22 = m_Vec1.z *-Sin + m_Vec2.z * Cos;
		Temp23 = m_Vec1.w *-Sin + m_Vec2.w * Cos;

		m_Vec1.x = Temp10;
		m_Vec1.y = Temp11;
		m_Vec1.z = Temp12;
		m_Vec1.w = Temp13;
		m_Vec2.x = Temp20;
		m_Vec2.y = Temp21;
		m_Vec2.z = Temp22;
		m_Vec2.w = Temp23;
	}

	void RotateX_Replace( const float radian )
	{
		float fSin, fCos;
		FastMath::SinCos(radian, fSin, fCos);

		m_Vec0.Set(1.0f,  0.0f,  0.0f, 0.0f);
		m_Vec1.Set(0.0f,  fCos,  fSin, 0.0f);
		m_Vec2.Set(0.0f, -fSin,  fCos, 0.0f);
		m_Vec3.Set(0.0f,  0.0f,  0.0f, 1.0f);
	}

	// this = Ry * this
	void RotateY( const float radian )
	{
		float Temp00, Temp01, Temp02, Temp03;
		float Temp20, Temp21, Temp22, Temp23;
		float Sin, Cos;
		FastMath::SinCos(radian, Sin, Cos);

		Temp00 = m_Vec0.x * Cos - m_Vec2.x * Sin;
		Temp01 = m_Vec0.y * Cos - m_Vec2.y * Sin;
		Temp02 = m_Vec0.z * Cos - m_Vec2.z * Sin;
		Temp03 = m_Vec0.w * Cos - m_Vec2.w * Sin;

		Temp20 = m_Vec0.x * Sin + m_Vec2.x * Cos;
		Temp21 = m_Vec0.y * Sin + m_Vec2.y * Cos;
		Temp22 = m_Vec0.z * Sin + m_Vec2.z * Cos;
		Temp23 = m_Vec0.w * Sin + m_Vec2.w * Cos;

		m_Vec0.x = Temp00;
		m_Vec0.y = Temp01;
		m_Vec0.z = Temp02;
		m_Vec0.w = Temp03;
		m_Vec2.x = Temp20;
		m_Vec2.y = Temp21;
		m_Vec2.z = Temp22;
		m_Vec2.w = Temp23;
	}

	void RotateY_Replace( const float radian )
	{
		float fSin, fCos;
		FastMath::SinCos(radian, fSin, fCos);

		m_Vec0.Set( fCos,  0.0f, -fSin, 0.0f);
		m_Vec1.Set( 0.0f,  1.0f,  0.0f, 0.0f);
		m_Vec2.Set(-fSin,  0.0f,  fCos, 0.0f);
		m_Vec3.Set( 0.0f,  0.0f,  0.0f, 1.0f);
	}

	// this = Rx * this
	void RotateZ( const float radian )
	{
		float Temp00, Temp01, Temp02, Temp03;
		float Temp10, Temp11, Temp12, Temp13;
		float Sin, Cos;
		FastMath::SinCos(radian, Sin, Cos);

		Temp00 = m_Vec0.x * Cos + m_Vec1.x * Sin;
		Temp01 = m_Vec0.y * Cos + m_Vec1.y * Sin;
		Temp02 = m_Vec0.z * Cos + m_Vec1.z * Sin;
		Temp03 = m_Vec0.w * Cos + m_Vec1.w * Sin;

		Temp10 = m_Vec0.x *-Sin + m_Vec1.x * Cos;
		Temp11 = m_Vec0.y *-Sin + m_Vec1.y * Cos;
		Temp12 = m_Vec0.z *-Sin + m_Vec1.z * Cos;
		Temp13 = m_Vec0.w *-Sin + m_Vec1.w * Cos;

		m_Vec0.x = Temp00;
		m_Vec0.y = Temp01;
		m_Vec0.z = Temp02;
		m_Vec0.w = Temp03;
		m_Vec1.x = Temp10;
		m_Vec1.y = Temp11;
		m_Vec1.z = Temp12;
		m_Vec1.w = Temp13;
	}

	void RotateZ_Replace( const float radian )
	{
		float fSin, fCos;
		FastMath::SinCos(radian, fSin, fCos);

		m_Vec0.Set( fCos,  fSin, 0.0f, 0.0f);
		m_Vec1.Set(-fSin,  fCos, 0.0f, 0.0f);
		m_Vec2.Set( 0.0f,  0.0f, 1.0f, 0.0f);
		m_Vec3.Set( 0.0f,  0.0f, 0.0f, 1.0f);
	}

	void Scale(Vector4 &scale)
	{
		Scale(scale[0], scale[1], scale[2]);
	}

	void Scale(float x, float y, float z)
	{
		m_Vec0 *= x;
		m_Vec1 *= y;
		m_Vec2 *= z;
	}

	void Scale_Replace(Vector4 &scale)
	{
		Scale_Replace(scale[0], scale[1], scale[2]);
	}

	void Scale_Replace(float x, float y, float z)
	{
		m_Vec0.Set(x, 0, 0, 0);
		m_Vec1.Set(0, y, 0, 0);
		m_Vec2.Set(0, 0, z, 0);
		m_Vec3.Set(0, 0, 0, 1);
	}

	inline Matrix4x4 FastInvert(void)
	{
		Vector4 pos = VectorInvertSign(m_Vec3);
		m_Vec3 = g_MatrixCPU_Identity.m_Vec3;

		Transpose();
		Translate(pos);

		return *this;
	}

	Matrix4x4 Invert(void);

	// this = T * this
	void Translate(float x, float y, float z)
	{
		m_Vec3 += m_Vec0 * x + m_Vec1 * y + m_Vec2 * z;
	}

	void Translate(Vector4 &v)
	{
		m_Vec3 += m_Vec0 * v.GetX() + m_Vec1 * v.GetY() + m_Vec2 * v.GetZ();
	}

	void Translate_Replace(float x, float y, float z)
	{
		Identity();
		m_Vec3[0] = x;
		m_Vec3[1] = y;
		m_Vec3[2] = z;
	}

	void Translate_Replace(Vector4 &v)
	{
		Identity();
		m_Vec3 = v;
	}

	void TranslateX(float d)
	{
		Vector4 dddd(d);
		m_Vec3 += m_Vec0 * dddd;
	}

	void TrsnalteY(float d)
	{
		Vector4 dddd(d);
		m_Vec3 += m_Vec1 * dddd;
	}

	void TranslateZ(float d)
	{
		Vector4 dddd(d);
		m_Vec3 += m_Vec2 * dddd;
	}

	Vector4 RotateVector(Vector4 v)
	{
		Vector4 result;

		result.x = v.x*m_Vec0.x + v.y*m_Vec1.x + v.z*m_Vec2.x;
		result.y = v.x*m_Vec0.y + v.y*m_Vec1.y + v.z*m_Vec2.y;
		result.z = v.x*m_Vec0.z + v.y*m_Vec1.z + v.z*m_Vec2.z;
		result.w = 1.0f;

		return result;
	}

	void NoTranslate(void)
	{
		m_Vec3 = IdentityMatrix()[3];
	}

	void NoRotate(void)
	{
		m_Vec0 = IdentityMatrix()[0];
		m_Vec1 = IdentityMatrix()[1];
		m_Vec2 = IdentityMatrix()[2];
	}

	void SetMatrix3x4(Matrix4x4 mat)
	{
		m_Vec0 = mat[0];
		m_Vec1 = mat[1];
		m_Vec2 = mat[2];
	}

	void ConsoleOutput(void);

	// operator
	inline float &operator() (int i, int j)
	{
		assert(i>=0 && i<4);
		assert(j>=0 && j<4);
		return GetRow(i)[j];
	}

	inline Vector4 &operator[](int row)
	{
		return GetRow(row);
	}

	inline Matrix4x4 operator=(Matrix4x4 rhs)
	{
		m_Vec0 = rhs.m_Vec0;
		m_Vec1 = rhs.m_Vec1;
		m_Vec2 = rhs.m_Vec2;
		m_Vec3 = rhs.m_Vec3;

		return *this;
	}

	inline Matrix4x4 operator=(float f)
	{
		Vector4 ffff(f);

		m_Vec0 = ffff;
		m_Vec1 = ffff;
		m_Vec2 = ffff;
		m_Vec3 = ffff;

		return *this;
	}

	inline Matrix4x4 operator+=(float f)
	{
		Vector4 ffff(f);

		m_Vec0 += ffff;
		m_Vec1 += ffff;
		m_Vec2 += ffff;
		m_Vec3 += ffff;

		return *this;
	}

	inline Matrix4x4 operator+=(Matrix4x4 rhs)
	{
		m_Vec0 += rhs.m_Vec0;
		m_Vec1 += rhs.m_Vec1;
		m_Vec2 += rhs.m_Vec2;
		m_Vec3 += rhs.m_Vec3;

		return *this;
	}

	inline Matrix4x4 operator-=(float f)
	{
		Vector4 ffff(f);

		m_Vec0 -= f;
		m_Vec1 -= f;
		m_Vec2 -= f;
		m_Vec3 -= f;

		return *this;
	}

	inline Matrix4x4 operator-=(Matrix4x4 &rhs)
	{
		m_Vec0 -= rhs.m_Vec0;
		m_Vec1 -= rhs.m_Vec1;
		m_Vec2 -= rhs.m_Vec2;
		m_Vec3 -= rhs.m_Vec3;

		return *this;
	}

	inline Matrix4x4 operator*=(float f)
	{
		m_Vec0 *= f;
		m_Vec1 *= f;
		m_Vec2 *= f;
		m_Vec3 *= f;

		return *this;
	}

	inline Matrix4x4 operator*=(Matrix4x4 rhs)
	{
		m_Vec0 = m_Vec0 * rhs;
		m_Vec1 = m_Vec1 * rhs;
		m_Vec2 = m_Vec2 * rhs;
		m_Vec3 = m_Vec3 * rhs;

		return *this;
	}

	inline Matrix4x4 operator/=(float f)
	{
		m_Vec0 /= f;
		m_Vec1 /= f;
		m_Vec2 /= f;
		m_Vec3 /= f;

		return *this;
	}

};

inline bool operator==(Matrix4x4 a, Matrix4x4 b)
{
	// true if all vectors equal to each other
	bool result = a.m_Vec0==b.m_Vec0 && a.m_Vec1==b.m_Vec1 && a.m_Vec2==b.m_Vec2 && a.m_Vec3==b.m_Vec3;
	return result;
}

inline bool operator!=(Matrix4x4 a, Matrix4x4 b)
{
	// true if any one vector not-equal
	bool result = a.m_Vec0!=b.m_Vec0 || a.m_Vec1!=b.m_Vec1 || a.m_Vec2!=b.m_Vec2 || a.m_Vec3!=b.m_Vec3;
	return result;
}

inline Matrix4x4 operator+(Matrix4x4 a, float f)
{
	Matrix4x4 result = a;
	result += f;
	return result;
}

inline Matrix4x4 operator+(float f, Matrix4x4 a)
{
	Matrix4x4 result = a;
	result += f;
	return result;
}

inline Matrix4x4 operator+(Matrix4x4 a, Matrix4x4 b)
{
	Matrix4x4 result;

	result.m_Vec0 = a.m_Vec0 + b.m_Vec0;
	result.m_Vec1 = a.m_Vec1 + b.m_Vec1;
	result.m_Vec2 = a.m_Vec2 + b.m_Vec2;
	result.m_Vec3 = a.m_Vec3 + b.m_Vec3;

	return result;
}

inline Matrix4x4 operator-(Matrix4x4 a, float f)
{
	Matrix4x4 result = a;
	result -= f;
	return result;
}

inline Matrix4x4 operator-(float f, Matrix4x4 a)
{
	Matrix4x4 result = a;
	result -= f;
	return result;
}

inline Matrix4x4 operator-(Matrix4x4 a, Matrix4x4 b)
{
	Matrix4x4 result;

	result.m_Vec0 = a.m_Vec0 - b.m_Vec0;
	result.m_Vec1 = a.m_Vec1 - b.m_Vec1;
	result.m_Vec2 = a.m_Vec2 - b.m_Vec2;
	result.m_Vec3 = a.m_Vec3 - b.m_Vec3;

	return result;
}

inline Vector4 operator*(Vector4 v, Matrix4x4 m)
{
	Vector4 result;

	result.x = v.x*m.m_Vec0.x + v.y*m.m_Vec1.x + v.z*m.m_Vec2.x + v.w*m.m_Vec3.x;
	result.y = v.x*m.m_Vec0.y + v.y*m.m_Vec1.y + v.z*m.m_Vec2.y + v.w*m.m_Vec3.y;
	result.z = v.x*m.m_Vec0.z + v.y*m.m_Vec1.z + v.z*m.m_Vec2.z + v.w*m.m_Vec3.z;
	result.w = v.x*m.m_Vec0.w + v.y*m.m_Vec1.w + v.z*m.m_Vec2.w + v.w*m.m_Vec3.w;

	return result;
}

inline Vector4 operator*(Matrix4x4 matrix, Vector4 vec)
{
	Vector4 result;

	result[0] = VectorDot(vec, matrix[0]).GetX();
	result[1] = VectorDot(vec, matrix[1]).GetX();
	result[2] = VectorDot(vec, matrix[2]).GetX();
	result[3] = VectorDot(vec, matrix[3]).GetX();

	return result;
}

inline Matrix4x4 operator*(Matrix4x4 a, Matrix4x4 b)
{
	Matrix4x4 result;

	result.m_Vec0 = a.m_Vec0 * b;
	result.m_Vec1 = a.m_Vec1 * b;
	result.m_Vec2 = a.m_Vec2 * b;
	result.m_Vec3 = a.m_Vec3 * b;

	return result;
}

inline Matrix4x4 operator*(Matrix4x4 a, float f)
{
	Matrix4x4 result;

	result.m_Vec0 = a.m_Vec0 * f;
	result.m_Vec1 = a.m_Vec1 * f;
	result.m_Vec2 = a.m_Vec2 * f;
	result.m_Vec3 = a.m_Vec3 * f;

	return result;
}

inline Matrix4x4 operator*(float f, Matrix4x4 a)
{
	Matrix4x4 result;

	result.m_Vec0 = a.m_Vec0 * f;
	result.m_Vec1 = a.m_Vec1 * f;
	result.m_Vec2 = a.m_Vec2 * f;
	result.m_Vec3 = a.m_Vec3 * f;

	return result;
}

inline Matrix4x4 operator/(Matrix4x4 a, float f)
{
	Matrix4x4 result;

	result.m_Vec0 = a.m_Vec0 / f;
	result.m_Vec1 = a.m_Vec1 / f;
	result.m_Vec2 = a.m_Vec2 / f;
	result.m_Vec3 = a.m_Vec3 / f;

	return result;

	return result;
}

inline Matrix4x4 Matrix4x4Transpose(Matrix4x4 matrix)
{
	Matrix4x4 result = matrix;
	result.Transpose();
	return result;
}

void Matrix4x4FromQuaternion(Matrix4x4 &matrix, Vector4 &v);
void Matrix4x4FromQuaternionAndTransform(Matrix4x4 &matrix, Vector4 &q, Vector4 &t = g_MatrixCPU_Identity.m_Vec3 );
void QuaternionFromMatrix4x4(Matrix4x4 &matrix, Vector4 &quaternion);

#endif // _MATRIX4x4_GENERAL_
