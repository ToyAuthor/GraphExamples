#ifndef _VECTOR4_
#define _VECTOR4_

//// GENERAL CPU Version, reference code.
//#include "Vector4_Reference.h"
//// Intel SSE instruction Version
//#include "Vector4_SSE.h"
//
//#ifdef _GENERAL_CPU_
//typedef Vector4CPU Vector4;
//#else
//typedef Vector4SSE Vector4;
//#endif

#include "Vector4_Reference.h"
typedef Vector4CPU Vector4;

#endif // _VECTOR4_