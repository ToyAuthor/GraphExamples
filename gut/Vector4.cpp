#include <stdio.h>
#include <stdlib.h>

#include "Vector4.h"

Vector4 g_VectorCPU_Zero = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
Vector4 g_VectorCPU_One = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
Vector4 g_VectorCPU_MinusOne = Vector4(-1.0f, -1.0f, -1.0f, -1.0f);
Vector4 g_VectorCPU_AbsMask = Vector4( (unsigned int)0x7fffffff );
Vector4 g_VectorCPU_SignMask = Vector4( (unsigned int)0x80000000 );

void Vector4::ConsoleOutput(void)
{
	printf("Vector: (%5.2f,%5.2f,%5.2f,%5.2f)\n", x, y, z, w);
}
