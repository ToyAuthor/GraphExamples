// 改成不需要組合語言

#ifndef _FASTMATH_WIN32_
#define _FASTMATH_WIN32_

#define MATH_PI 3.141592653f

#include <math.h>
#include <float.h>

namespace FastMath
{

inline const char *CPUType(void)
{
	return "Intel_X86";
}

template<class T> inline void Swap(T &a, T &b)
{
	T c = b;
	b = a;
	a = c;
}

inline float DegreeToRadian(float r)
{
	float radian = r * (MATH_PI / 180.0f);
	return radian;
}

inline float DegToRad(float r)
{
	return DegreeToRadian(r);
}

inline float RadianToDegree(float r)
{
	float degree = r * (180.0f / MATH_PI);
	return degree;
}

inline float RadToDeg(float r)
{
	return RadianToDegree(r);
}

inline float Abs(float v)
{
	return v<0 ? v*-1:v;
}

inline float Sqrt(float v)
{
	return sqrt(v);
}

inline float ReciprocalSqrt(float v)
{
	return 1.0f/sqrt(v);
}

inline float Sin(float radian)
{
	return sin(radian);
}

// -pi < p_Angle < pi
inline float FastSin(float p_Angle)
{
	const float Factor1 = 4.0f / MATH_PI;
	const float Factor2 = 4.0f / (MATH_PI * MATH_PI);
	return (Factor1 * p_Angle) - (Factor2 * p_Angle * fabs(p_Angle));
}

inline float ASin(float v)
{
	float result = (float) asin(v);
	return result;
}

inline float Cos(float radian)
{
	return cos(radian);
}

inline float ACos(float v)
{
	float result = (float) acos(v);
	return result;
}

inline void SinCos(float radian, float &sinvalue, float &cosvalue)
{
	sinvalue=sin(radian);
	cosvalue=cos(radian);
}

inline float Tan(float radian)
{
	return tan(radian);
}

inline float Cot(float radian)
{
	return 1/tan(radian);
}

inline float ATan(float radian)
{
	return atan(radian);
}

inline float Lerp(float a, float b, float t)
{
	return a + (b-a) * t;
}

inline float frac(float a)
{
	return a - (int)a;
}

};
#endif // _FASTMATH_WIN32_
