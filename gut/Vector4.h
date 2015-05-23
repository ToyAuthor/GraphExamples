#ifndef _VECTOR4_GENERAL_
#define _VECTOR4_GENERAL_

#if !defined(_ENABLE_GENERALCPU_)
	#define _ENABLE_GENERALCPU_
#endif


#include <assert.h>
#include <stdio.h>

#include "FastMath.h"

class Vector4;

extern Vector4 g_VectorCPU_Zero;
extern Vector4 g_VectorCPU_One;

class Vector4
{
public:
	union{
		struct
		{
			float x,y,z,w;
		};
		float m_elem[4];
		unsigned int m_ielem[4];
	};

public:
	Vector4()
	{
	}
//	inline Vector4():x(0.0f),y(0.0f),z(0.0f),w(0.0f)
//	{
//	}

	inline Vector4(const Vector4 &bro)
	{
		x = bro.x;
		y = bro.y;
		z = bro.z;
		w = bro.w;
	}

	inline Vector4(float x, float y, float z, float w=1.0f)
	{
		Set(x, y, z, w);
	}

	inline Vector4(float value)
	{
		Set(value);
	}

	inline Vector4(unsigned int x, unsigned int y, unsigned int z, unsigned int w)
	{
		m_ielem[0] = x;
		m_ielem[0] = y;
		m_ielem[0] = z;
		m_ielem[0] = w;
	}

	inline Vector4(unsigned int x)
	{
		m_ielem[0] = x;
		m_ielem[0] = x;
		m_ielem[0] = x;
		m_ielem[0] = x;
	}

	void ConsoleOutput(void);

	inline void Set(float x, float y, float z, float w=0.0f)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	inline void Set(float value)
	{
		this->x = value;
		this->y = value;
		this->z = value;
		this->w = value;
	}

	inline void Set(float *p)
	{
		this->x = p[0];
		this->y = p[1];
		this->z = p[2];
		this->w = p[3];
	}

	inline void SetX(float x)
	{
		this->x = x;
	}

	inline void SetY(float y)
	{
		this->y = y;
	}

	inline void SetZ(float z)
	{
		this->z = z;
	}

	inline void SetW(float w)
	{
		this->w = w;
	}

	inline void SetXYZ(float *p)
	{
		x = p[0];
		y = p[1];
		z = p[2];
	}

	inline float GetX(void)
	{
		return x;
	}

	inline Vector4 GetXXXX(void)
	{
		Vector4 result(x);
		return result;
	}

	inline float GetY(void)
	{
		return y;
	}

	inline Vector4 GetYYYY(void)
	{
		Vector4 result(y);
		return result;
	}

	inline float GetZ(void)
	{
		return z;
	}

	inline Vector4 GetZZZZ(void)
	{
		Vector4 result(z);
		return result;
	}

	inline float GetW(void)
	{
		return w;
	}

	inline Vector4 GetWWWW(void)
	{
		Vector4 result(w);
		return result;
	}

	inline float StoreAligned(float *p)
	{
		p[0] = x;
		p[1] = y;
		p[2] = z;
		p[3] = w;
	}

	inline void Store(float *p)
	{
		p[0] = x;
		p[1] = y;
		p[2] = z;
		p[3] = w;
	}

	inline void StoreXYZ(float *p)
	{
		p[0] = x;
		p[1] = y;
		p[2] = z;
	}

	inline void Reciprocal(void)
	{
		x = 1.0f/x;
		y = 1.0f/y;
		z = 1.0f/z;
		w = 1.0f/w;
	}

	inline void Sqrt(void)
	{
		x = FastMath::Sqrt(x);
		y = FastMath::Sqrt(y);
		z = FastMath::Sqrt(z);
		w = FastMath::Sqrt(w);
	}

	inline void ReciprocalSqrt(void)
	{
		x = FastMath::ReciprocalSqrt(x);
		y = FastMath::ReciprocalSqrt(y);
		z = FastMath::ReciprocalSqrt(z);
		w = FastMath::ReciprocalSqrt(w);
	}

	inline float Length(void)
	{
		float vectorlength;

		float sum = x*x + y*y + z*z;
		vectorlength = FastMath::Sqrt(sum);

		return vectorlength;
	}

	inline void Normalize(void)
	{
		float len = Length();
		x/=len;
		y/=len;
		z/=len;
	}

	inline float NormalizeAndGetLength(void)
	{
		float len = Length();
		x/=len;
		y/=len;
		z/=len;
		return len;
	}

	inline Vector4 &Abs(void)
	{
		x = FastMath::Abs(x);
		y = FastMath::Abs(y);
		z = FastMath::Abs(z);
		w = FastMath::Abs(w);

		return *this;
	}

	inline Vector4 InvertSign(void)
	{
		x = -x;
		y = -y;
		z = -z;
		w = -w;

		return *this;
	}

	inline Vector4 &Saturate(void)
	{
		if ( x > 1.0f ) x = 1.0f;
		if ( y > 1.0f ) y = 1.0f;
		if ( z > 1.0f ) z = 1.0f;
		if ( w > 1.0f ) w = 1.0f;

		if ( x < 0.0f ) x = 0.0f;
		if ( y < 0.0f ) y = 0.0f;
		if ( z < 0.0f ) z = 0.0f;
		if ( w < 0.0f ) w = 0.0f;

		return *this;
	}

	inline Vector4 &Clamp_to_0(void)
	{
		if ( x < 0.0f ) x = 0.0f;
		if ( y < 0.0f ) y = 0.0f;
		if ( z < 0.0f ) z = 0.0f;
		if ( w < 0.0f ) w = 0.0f;

		return *this;
	}

	inline Vector4 &Clamp_to_1(void)
	{
		if ( x > 1.0f ) x = 1.0f;
		if ( y > 1.0f ) y = 1.0f;
		if ( z > 1.0f ) z = 1.0f;
		if ( w > 1.0f ) w = 1.0f;

		return *this;
	}

	inline Vector4 &Floor(void)
	{
		x = floor(x);
		y = floor(y);
		z = floor(z);
		w = floor(w);
		return *this;
	}

	inline static Vector4 &GetZero(void)
	{
		return g_VectorCPU_Zero;
	}

	inline static Vector4 &GetOne(void)
	{
		return g_VectorCPU_One;
	}

	// operators
	inline Vector4 operator=(Vector4 rhs)
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		w = rhs.w;
		return *this;
	}

	inline Vector4 operator=(float value)
	{
		Set(value);
		return *this;
	}

	inline Vector4 operator+=(Vector4 rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		w += rhs.w;
		return *this;
	}

	inline Vector4 operator+=(float value)
	{
		x += value;
		y += value;
		z += value;
		w += value;
		return *this;
	}

	inline Vector4 operator-=(Vector4 rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		w -= rhs.w;
		return *this;
	}

	inline Vector4 operator-=(float value)
	{
		x -= value;
		y -= value;
		z -= value;
		w -= value;
		return *this;
	}

	inline Vector4 operator*=(Vector4 rhs)
	{
		x *= rhs.x;
		y *= rhs.y;
		z *= rhs.z;
		w *= rhs.w;
		return *this;
	}

	inline Vector4 operator*=(float value)
	{
		x *= value;
		y *= value;
		z *= value;
		w *= value;
		return *this;
	}

	inline Vector4 operator/=(Vector4 rhs)
	{
		x /= rhs.x;
		y /= rhs.y;
		z /= rhs.z;
		w /= rhs.w;
		return *this;
	}

	inline Vector4 operator/=(float value)
	{
		x /= value;
		y /= value;
		z /= value;
		w /= value;
		return *this;
	}

	inline Vector4 operator-()
	{
		return this->InvertSign();
	}

	inline float &operator[](int index)
	{
		assert(index>=0 && index<4);
		return m_elem[index];
	}

	static const char *InstructionType(void)
	{
		return "General_CPU";
	}
};

inline bool operator==(Vector4 &a, Vector4 &b)
{
	bool result = (a.x==b.x && a.y==b.y && a.z==b.z && a.w==b.w);
	return result;
}

inline bool operator!=(Vector4 &a, Vector4 &b)
{
	bool result = (a.x==b.x && a.y==b.y && a.z==b.z && a.w==b.w);
	return !result;
}

inline Vector4 operator+(Vector4 a, Vector4 b)
{
	Vector4 c;

	c.x = a.x + b.x;
	c.y = a.y + b.y;
	c.z = a.z + b.z;
	c.w = a.w + b.w;

	return c;
}

inline Vector4 operator-(Vector4 a, Vector4 b)
{
	Vector4 c;

	c.x = a.x - b.x;
	c.y = a.y - b.y;
	c.z = a.z - b.z;
	c.w = a.w - b.w;

	return c;
}

inline Vector4 operator*(Vector4 a, Vector4 b)
{
	Vector4 c;

	c.x = a.x * b.x;
	c.y = a.y * b.y;
	c.z = a.z * b.z;
	c.w = a.w * b.w;

	return c;
}

inline Vector4 operator*(float f, Vector4 v)
{
	Vector4 result;

	result.x = f * v.x;
	result.y = f * v.y;
	result.z = f * v.z;
	result.w = f * v.w;

	return result;
}

inline Vector4 operator*(Vector4 v, float f)
{
	Vector4 result;

	result.x = f * v.x;
	result.y = f * v.y;
	result.z = f * v.z;
	result.w = f * v.w;

	return result;
}

inline Vector4 operator/(Vector4 a, Vector4 b)
{
	Vector4 c;

	c.x = a.x / b.x;
	c.y = a.y / b.y;
	c.z = a.z / b.z;
	c.w = a.w / b.w;

	return c;
}

inline Vector4 operator/(Vector4 a, float f)
{
	Vector4 c;

	c.x = a.x / f;
	c.y = a.y / f;
	c.z = a.z / f;
	c.w = a.w / f;

	return c;
}

inline Vector4 operator/(float f, Vector4 a)
{
	Vector4 c;

	c.x = f / a.x;
	c.y = f / a.y;
	c.z = f / a.z;
	c.w = f / a.w;

	return c;
}

inline float Vector3Dotf(Vector4 &a, Vector4 &b)
{
	float result = a.x * b.x + a.y * b.y + a.z * b.z;
	return result;
}

inline float VectorDotf(Vector4 &a, Vector4 &b)
{
	float result = a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
	return result;
}

inline Vector4 Vector3Dot(Vector4 &a, Vector4 &b)
{
	Vector4 c = Vector3Dotf(a, b);
	return c;
}

inline Vector4 VectorDot(Vector4 &a, Vector4 &b)
{
	Vector4 c = VectorDotf(a,b);
	return c;
}

inline Vector4 Vector3CrossProduct(Vector4 &a, Vector4 &b)
{
	Vector4 c;

	c.x = a.y * b.z - a.z * b.y;
	c.y = a.z * b.x - a.x * b.z;
	c.z = a.x * b.y - a.y * b.x;
	c.w = 0.0f;

	return c;
}
inline Vector4 VectorMin(Vector4 a, Vector4 b)
{
	Vector4 c;

	if(a.Length()<b.Length())
		c=a;
	else
		c=b;

	return c;
}
inline Vector4 VectorMax(Vector4 a, Vector4 b)
{
	Vector4 c;

	if(a.Length()>b.Length())
		c=a;
	else
		c=b;

	return c;
}
inline Vector4 VectorLerp(Vector4 &a, Vector4 &b, float t)
{
	Vector4 c = a + (b - a) * t;
	return c;
}

inline Vector4 VectorGetMax(Vector4 &a, Vector4 &b)
{
	Vector4 c;

	c.x = a.x > b.x ? a.x : b.x;
	c.y = a.y > b.y ? a.y : b.y;
	c.z = a.z > b.z ? a.z : b.z;
	c.w = a.w > b.w ? a.w : b.w;

	return c;
}

inline Vector4 VectorGetMin(Vector4 &a, Vector4 &b)
{
	Vector4 c;

	c.x = a.x < b.x ? a.x : b.x;
	c.y = a.y < b.y ? a.y : b.y;
	c.z = a.z < b.z ? a.z : b.z;
	c.w = a.w < b.w ? a.w : b.w;

	return c;
}

inline Vector4 VectorReciprocal(Vector4 &a)
{
	Vector4 result = a;
	result.Reciprocal();
	return result;
}

inline Vector4 VectorSqrt(Vector4 &a)
{
	Vector4 result = a;
	result.Sqrt();
	return result;
}

inline Vector4 VectorReciprocalSqrt(Vector4 &a)
{
	Vector4 result = a;
	result.ReciprocalSqrt();
	return result;
}

inline Vector4 VectorNormalize(Vector4 &a)
{
	Vector4 result = a;
	result.Normalize();
	return result;
}

inline float VectorNormalizeAndGetLength(Vector4 &a, Vector4 &result)
{
	result = a;
	float vectorlength = result.NormalizeAndGetLength();
	return vectorlength;
}

inline Vector4 VectorAbs(Vector4 &a)
{
	Vector4 result = a;
	result.Abs();
	return result;
}

inline Vector4 VectorInvertSign(Vector4 &a)
{
	Vector4 result = a;
	result.InvertSign();

	return result;
}

inline Vector4 VectorSaturate(Vector4 &a)
{
	Vector4 result = a;
	result.Saturate();
	return result;
}

inline Vector4 VectorQuaternionToRotation(Vector4 &v)
{
	Vector4 result;

	float cosvalue = FastMath::ACos(v.w);
	float s = FastMath::ASin(cosvalue);

	if ( s==0.0f )
	{
		result.Set(1.0f, 0.0f, 0.0f, 0.0f);
	}
	else
	{
		result = v / s;
		result.w = cosvalue * 2.0f;
	}

	return result;
}

inline Vector4 VectorRotationToQuaternion(Vector4 &v)
{
	Vector4 result;

	float radius_half = v.w * 0.5f;
	float sinvalue, cosvalue;
	FastMath::SinCos(radius_half, sinvalue, cosvalue);

	result.x = v.x * sinvalue;
	result.y = v.y * sinvalue;
	result.z = v.z * sinvalue;
	result.w = cosvalue;

	return result;
}

inline Vector4 PointToPlaneDistance(Vector4 &point, Vector4 &plane)
{
	return VectorDot(point, plane);
}

inline Vector4 MirrorPoint(Vector4 &point, Vector4 &plane)
{
	Vector4 mirrored_point;

	float d = PointToPlaneDistance(point, plane)[0];
	mirrored_point = point - (2.0f * d ) * plane;

	return mirrored_point;
}

// 計算vector對plane平面鏡射後的3D方向
inline Vector4 MirrorVector(Vector4 &vector, Vector4 &plane)
{
	Vector4 mirrored_a, mirrored_b, mirrored_vector;
	Vector4 vZero = Vector4::GetZero();

	float d = PointToPlaneDistance(vector, plane)[0];
	mirrored_a = vector - (2.0f * d ) * plane;

	d = plane[3];
	mirrored_b = vZero - (2.0f * d ) * plane;

	mirrored_vector = mirrored_a - mirrored_b;
	mirrored_vector[3] = 1.0f;

	return mirrored_vector;
}

#endif // _VECTOR4_SSE_
