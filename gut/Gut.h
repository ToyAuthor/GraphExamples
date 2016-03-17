#ifndef _GUTIL_
#define _GUTIL_

#include <string>

#ifdef _WIN32

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include "GL/glew.h"
#include "GL/wglew.h"

#else

#include <X11/X.h>
#include "GL/glew.h"
#include "GL/glxew.h"
#include <GL/glx.h>

struct HWND
{
	HWND(const void *ptr)
	{
		if(ptr==NULL)
		{
			//做些類似初值的事情吧
		}
	}

	Window      window;
	GLXContext  context;
};

typedef Display* HINSTANCE;

#ifndef TURE
#define TURE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#endif//_WIN32


#include <GL/gl.h>
#include "GutDefs.h"
#include "Vector4.h"
#include "Matrix4x4.h"

Matrix4x4 GutMatrixLookAtRH(Vector4 &eye, Vector4 &lookat, Vector4 &up);
Matrix4x4 GutMatrixPerspectiveRH_OpenGL(float fovy, float aspect, float z_near, float z_far);
void GutSetShaderPath(const char *path);
const char *GutGetShaderPath();


#endif // _GUTIL_
